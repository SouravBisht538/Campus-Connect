// ============================================================
//  teachers.h - MODULE 3: Teacher Details  (Binary Search Tree)
// ============================================================
#ifndef TEACHERS_H
#define TEACHERS_H
#include <string>
#include <vector>
using namespace std;

struct Teacher { string name, dept, cabin, email; Teacher *l, *r; };
extern Teacher *root;
Teacher *ins(Teacher *n, Teacher *t);                                          // BST insert (by name)
void teachersInOrder(Teacher *n, const string &q, vector<Teacher *> &out);     // in-order = sorted by name
int countTeachers(Teacher *n);
#endif
