// ============================================================
//  announcements.h - MODULE 5: Announcements  (Max-Heap / Priority Queue)
// ============================================================
#ifndef ANNOUNCEMENTS_H
#define ANNOUNCEMENTS_H
#include <string>
using namespace std;

struct Ann { int pr, id; string title, msg; };   // pr: 1 = High, 2 = Medium, 3 = Low
extern Ann heap[100];
extern int hn, aid;
void hpush(Ann a);
Ann hpop(Ann *h, int &n);
#endif
