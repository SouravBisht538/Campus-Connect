// ============================================================
//  auth.cpp - registry + accounts in a HASH TABLE (separate chaining)
//  * passwords are stored as SHA-256(salt + password), never in plain text
//  * 5 wrong passwords lock that ID for 60 seconds
//  * login sessions are random 128-bit tokens kept in memory
//  Saved to registry.txt:  id|name|email|role|salt|hash   (empty hash = account not created yet)
// ============================================================
#include <fstream>
#include <sstream>
#include <map>
#include <algorithm>
#include <random>
#include <ctime>
#include <cstdint>
#include <cstdio>
#include "auth.h"

struct User { string id, name, email; int role; string salt, hash; User *next; };

static const int BUCKETS = 101;
static User *table[BUCKETS] = {};
static const char *FILE_NAME = "registry.txt";
struct Session { string id, name; int role; };
static map<string, Session> sessions;                       // token -> who is logged in
static map<string, pair<int, time_t>> fails;                // id -> (wrong attempts, locked until)

static string lowerStr(string s) { for (char &c : s) c = (char)tolower((unsigned char)c); return s; }
static string trim(const string &s) {
    size_t a = s.find_first_not_of(" \t\r\n"), b = s.find_last_not_of(" \t\r\n");
    return a == string::npos ? "" : s.substr(a, b - a + 1);
}

// ---------------- SHA-256 ----------------
static string sha256(const string &msg) {
    static const uint32_t K[64] = {
        0x428a2f98,0x71374491,0xb5c0fbcf,0xe9b5dba5,0x3956c25b,0x59f111f1,0x923f82a4,0xab1c5ed5,
        0xd807aa98,0x12835b01,0x243185be,0x550c7dc3,0x72be5d74,0x80deb1fe,0x9bdc06a7,0xc19bf174,
        0xe49b69c1,0xefbe4786,0x0fc19dc6,0x240ca1cc,0x2de92c6f,0x4a7484aa,0x5cb0a9dc,0x76f988da,
        0x983e5152,0xa831c66d,0xb00327c8,0xbf597fc7,0xc6e00bf3,0xd5a79147,0x06ca6351,0x14292967,
        0x27b70a85,0x2e1b2138,0x4d2c6dfc,0x53380d13,0x650a7354,0x766a0abb,0x81c2c92e,0x92722c85,
        0xa2bfe8a1,0xa81a664b,0xc24b8b70,0xc76c51a3,0xd192e819,0xd6990624,0xf40e3585,0x106aa070,
        0x19a4c116,0x1e376c08,0x2748774c,0x34b0bcb5,0x391c0cb3,0x4ed8aa4a,0x5b9cca4f,0x682e6ff3,
        0x748f82ee,0x78a5636f,0x84c87814,0x8cc70208,0x90befffa,0xa4506ceb,0xbef9a3f7,0xc67178f2};
    uint32_t h[8] = {0x6a09e667,0xbb67ae85,0x3c6ef372,0xa54ff53a,0x510e527f,0x9b05688c,0x1f83d9ab,0x5be0cd19};
    string data = msg;
    uint64_t bits = (uint64_t)msg.size() * 8;
    data += (char)0x80;
    while (data.size() % 64 != 56) data += (char)0;
    for (int i = 7; i >= 0; i--) data += (char)((bits >> (i * 8)) & 0xff);
    auto rotr = [](uint32_t x, int n) { return (x >> n) | (x << (32 - n)); };
    for (size_t off = 0; off < data.size(); off += 64) {
        uint32_t w[64];
        for (int i = 0; i < 16; i++)
            w[i] = ((uint32_t)(uint8_t)data[off + i * 4] << 24) | ((uint32_t)(uint8_t)data[off + i * 4 + 1] << 16) |
                   ((uint32_t)(uint8_t)data[off + i * 4 + 2] << 8) | (uint32_t)(uint8_t)data[off + i * 4 + 3];
        for (int i = 16; i < 64; i++) {
            uint32_t s0 = rotr(w[i - 15], 7) ^ rotr(w[i - 15], 18) ^ (w[i - 15] >> 3);
            uint32_t s1 = rotr(w[i - 2], 17) ^ rotr(w[i - 2], 19) ^ (w[i - 2] >> 10);
            w[i] = w[i - 16] + s0 + w[i - 7] + s1;
        }
        uint32_t a = h[0], b = h[1], c = h[2], d = h[3], e = h[4], f = h[5], g = h[6], hh = h[7];
        for (int i = 0; i < 64; i++) {
            uint32_t S1 = rotr(e, 6) ^ rotr(e, 11) ^ rotr(e, 25), ch = (e & f) ^ (~e & g);
            uint32_t t1 = hh + S1 + ch + K[i] + w[i];
            uint32_t S0 = rotr(a, 2) ^ rotr(a, 13) ^ rotr(a, 22), mj = (a & b) ^ (a & c) ^ (b & c);
            uint32_t t2 = S0 + mj;
            hh = g; g = f; f = e; e = d + t1; d = c; c = b; b = a; a = t1 + t2;
        }
        h[0] += a; h[1] += b; h[2] += c; h[3] += d; h[4] += e; h[5] += f; h[6] += g; h[7] += hh;
    }
    char out[65];
    for (int i = 0; i < 8; i++) snprintf(out + i * 8, 9, "%08x", h[i]);
    return out;
}

static string randomHex(int bytes) {
    static mt19937_64 rng{random_device{}()};
    string s; char buf[3];
    for (int i = 0; i < bytes; i++) { snprintf(buf, sizeof buf, "%02x", (unsigned)(rng() & 0xff)); s += buf; }
    return s;
}

// ---------------- hash table of people ----------------
static size_t slot(const string &id) { return hash<string>()(lowerStr(id)) % BUCKETS; }

static User *find(const string &id) {
    string key = lowerStr(id);
    for (User *u = table[slot(id)]; u; u = u->next)
        if (lowerStr(u->id) == key) return u;
    return 0;
}
static void insert(const string &id, const string &name, const string &email, int role, const string &salt, const string &hash) {
    size_t i = slot(id);
    table[i] = new User{id, name, email, role, salt, hash, table[i]};
}
static void save() {
    ofstream f(FILE_NAME);
    for (int i = 0; i < BUCKETS; i++)
        for (User *u = table[i]; u; u = u->next)
            f << u->id << "|" << u->name << "|" << u->email << "|" << u->role << "|" << u->salt << "|" << u->hash << "\n";
}
static vector<string> split(const string &s) {
    vector<string> out; string cur;
    for (char c : s) { if (c == '|') { out.push_back(cur); cur.clear(); } else cur += c; }
    out.push_back(cur);
    return out;
}

void loadUsers() {
    ifstream f(FILE_NAME);
    if (!f.good()) {                         // first run: sample registry (edit registry.txt or use the Admin Panel)
        insert("2510011872", "Ansul Singh Negi", "ansulnegi23@gmail.com", 3, "", "");
        insert("2510380056", "Dev Dhoundiyal", "devdhoundiyal00@gmail.com", 1, "", "");
        insert("26021203", "Sohan Goswami", "sohangoswami4567@gmail.com", 1, "", "");
        insert("2510011386", "Sourav Bisht", "souravbisht538@gmail.com", 1, "", "");
        insert("FAC001", "Dr. Sharma", "sharma@campus.edu", 2, "", "");
        save();
        return;
    }
    string line;
    while (getline(f, line)) {
        vector<string> v = split(line);
        if (v.size() < 6) continue;
        int role = atoi(v[3].c_str());
        if (role < 1 || role > 3) role = 1;
        if (!find(v[0])) insert(v[0], v[1], v[2], role, v[4], v[5]);
    }
}

// ---------------- register / login ----------------
string registerUser(const string &idIn, const string &emailIn, const string &pass) {
    string id = trim(idIn), email = trim(emailIn);
    if (id.empty() || email.empty()) return "Enter your ID and your registered email";
    if (pass.size() < 6) return "Password must be at least 6 characters";
    User *u = find(id);
    if (!u || lowerStr(u->email) != lowerStr(email)) return "ID and email do not match any registered person";
    if (!u->hash.empty()) return "This ID already has an account. Please sign in";
    u->salt = randomHex(8);
    u->hash = sha256(u->salt + ":" + pass);
    save();
    return "";
}

string loginUser(const string &idIn, const string &pass, int &role, string &token, string &fullName) {
    string id = trim(idIn), key = lowerStr(id);
    time_t now = time(0);
    auto &fl = fails[key];
    if (fl.second > now) return "Too many wrong attempts. Try again in " + to_string((long)(fl.second - now)) + " seconds";
    User *u = find(id);
    if (u && u->hash.empty()) return "No account yet for this ID. Use Create account";
    if (!u || sha256(u->salt + ":" + pass) != u->hash) {
        if (++fl.first >= 5) { fl.first = 0; fl.second = now + 60; }          // lock for a minute
        return "Wrong ID or password";
    }
    fails.erase(key);
    role = u->role;
    fullName = u->name;
    token = randomHex(16);
    sessions[token] = Session{u->id, u->name, u->role};
    return "";
}

int roleOf(const string &token) {
    auto it = sessions.find(token);
    return it == sessions.end() ? 0 : it->second.role;
}
string userOf(const string &token) {
    auto it = sessions.find(token);
    return it == sessions.end() ? "" : it->second.id;
}
void logoutToken(const string &token) { sessions.erase(token); }

// ---------------- admin tools ----------------
vector<UserInfo> allUsers() {
    vector<UserInfo> v;
    for (int i = 0; i < BUCKETS; i++)
        for (User *u = table[i]; u; u = u->next) v.push_back({u->id, u->name, u->email, u->role, !u->hash.empty()});
    sort(v.begin(), v.end(), [](const UserInfo &a, const UserInfo &b) { return a.id < b.id; });
    return v;
}

string addPerson(const string &idIn, const string &nameIn, const string &emailIn, int role) {
    string id = trim(idIn), name = trim(nameIn), email = trim(emailIn);
    if (id.size() < 3) return "ID must be at least 3 characters";
    if (id.find_first_of(" |\t") != string::npos) return "ID cannot contain spaces";
    if (name.empty()) return "Enter the person's name";
    if (email.find('@') == string::npos) return "Enter a valid email";
    if (role < 1 || role > 3) return "Invalid account type";
    if (find(id)) return "This ID is already in the registry";
    insert(id, name, email, role, "", "");
    save();
    return "";
}

string setUserRole(const string &id, int role, const string &actingUser) {
    if (role < 1 || role > 3) return "Invalid account type";
    if (lowerStr(id) == lowerStr(actingUser)) return "You cannot change your own account type";
    User *u = find(id);
    if (!u) return "No such person";
    u->role = role;
    for (auto &s : sessions)                          // takes effect for their open session too
        if (lowerStr(s.second.id) == lowerStr(id)) s.second.role = role;
    save();
    return "";
}

string deleteUser(const string &id, const string &actingUser) {
    if (lowerStr(id) == lowerStr(actingUser)) return "You cannot delete your own account";
    size_t i = slot(id);
    User *cur = table[i], *prev = 0;
    string key = lowerStr(id);
    while (cur && lowerStr(cur->id) != key) { prev = cur; cur = cur->next; }
    if (!cur) return "No such person";
    if (prev) prev->next = cur->next; else table[i] = cur->next;
    delete cur;
    for (auto it = sessions.begin(); it != sessions.end();)   // log them out
        it = lowerStr(it->second.id) == key ? sessions.erase(it) : ++it;
    save();
    return "";
}

string resetPassword(const string &id, const string &actingUser) {
    if (lowerStr(id) == lowerStr(actingUser)) return "You cannot reset your own password";
    User *u = find(id);
    if (!u) return "No such person";
    u->salt.clear(); u->hash.clear();                 // they create their account again (ID + email)
    string key = lowerStr(id);
    for (auto it = sessions.begin(); it != sessions.end();)
        it = lowerStr(it->second.id) == key ? sessions.erase(it) : ++it;
    save();
    return "";
}
