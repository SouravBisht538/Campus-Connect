// ============================================================
//  lostfound.h - MODULE 2: Lost & Found  (Singly Linked List)
// ============================================================
#ifndef LOSTFOUND_H
#define LOSTFOUND_H
#include <string>
using namespace std;

struct Item { int id; string type, name, place, contact; Item *next; };
extern Item *lhead;
void addItem(string type, string name, string place, string contact);
void claimItem(int id);   // removes the item with this id from the list
#endif
