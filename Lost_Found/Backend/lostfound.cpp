// ============================================================
//  lostfound.cpp - MODULE 2: Lost & Found
//  Data structure: Singly Linked List (add at the end, remove by id)
// ============================================================
#include "lostfound.h"

// ================= 2. LOST & FOUND: Linked List ==============

Item *lhead = 0;
int lid = 1;

void addItem(string type, string name, string place, string contact) {
    Item *n = new Item{lid++, type, name, place, contact, 0};

    if (!lhead)
        lhead = n;
    else {
        Item *t = lhead;
        while (t->next)
            t = t->next;
        t->next = n;
    }
}

void claimItem(int id) {
    Item *c = lhead, *pr = 0;

    while (c && c->id != id) {
        pr = c;
        c = c->next;
    }

    if (!c)
        return;

    if (pr)
        pr->next = c->next;
    else
        lhead = c->next;

    delete c;
}
