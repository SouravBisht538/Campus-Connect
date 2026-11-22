// ============================================================
//  teachers.cpp - MODULE 3: Teacher Details
//  Data structure: Binary Search Tree (sorted by name, in-order traversal)
// ============================================================
#include "teachers.h"
#include "common.h"

// ================= 3. TEACHERS: Binary Search Tree ===========

Teacher *root = 0;

Teacher *ins(Teacher *n, Teacher *t) {
    if (!n)
        return t;

    if (lower(t->name) < lower(n->name))
        n->l = ins(n->l, t);
    else
        n->r = ins(n->r, t);

    return n;
}

void teachersInOrder(Teacher *n, const string &q, vector<Teacher *> &out) {
    if (!n)
        return;

    teachersInOrder(n->l, q, out);

    if (q.empty() || lower(n->name + " " + n->dept).find(q) != string::npos)
        out.push_back(n);

    teachersInOrder(n->r, q, out);
}

int countTeachers(Teacher *n) {
    return n ? 1 + countTeachers(n->l) + countTeachers(n->r) : 0;
}
