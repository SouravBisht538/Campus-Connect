// ============================================================
//  auth.h - university registry, accounts, roles and login sessions
//  Roles: 1 = Student, 2 = Faculty, 3 = Admin
//
//  Only people listed in the REGISTRY (registry.txt) can create an account:
//  they must give their ID + the email registered for that ID.
//  The registry also decides the account type, so nobody can pick "Admin" themselves.
// ============================================================
#ifndef AUTH_H
#define AUTH_H
#include <string>
#include <vector>
using namespace std;

// All return "" on success, otherwise the error message to show the user.
string registerUser(const string &id, const string &email, const string &pass);
string loginUser(const string &id, const string &pass, int &role, string &token, string &fullName);
int roleOf(const string &token);      // 0 = not logged in, otherwise 1..3
string userOf(const string &token);   // ID of the logged-in person ("" if none)
void logoutToken(const string &token);
void loadUsers();                     // read registry.txt (creates it with sample people on the first run)

// Admin tools
struct UserInfo { string id, name, email; int role; bool registered; };
vector<UserInfo> allUsers();                                             // sorted by ID
string addPerson(const string &id, const string &name, const string &email, int role);
string setUserRole(const string &id, int role, const string &actingUser);
string deleteUser(const string &id, const string &actingUser);           // removes from registry too
string resetPassword(const string &id, const string &actingUser);        // person can create their account again
#endif
