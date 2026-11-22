// ============================================================
//  clubs.h - MODULE 4: Clubs  (Singly Linked List)
// ============================================================
#ifndef CLUBS_H
#define CLUBS_H
#include <string>
using namespace std;

struct Club { string name, head, desc; int members; Club *next; };
extern Club *chead;   // new clubs are added at the head of the list
#endif
