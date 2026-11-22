// ============================================================
//  announcements.cpp - MODULE 5: Announcements
//  Data structure: Max-Heap in an array (push / pop in O(log n))
// ============================================================
#include <algorithm>
#include "announcements.h"

// ================= 5. ANNOUNCEMENTS: Max-Heap ================

Ann heap[100];
int hn = 0, aid = 1;

bool better(const Ann &a, const Ann &b) {
    return a.pr < b.pr || (a.pr == b.pr && a.id > b.id);
}

void hpush(Ann a) {
    if (hn >= 100)
        return;

    int i = hn++;
    heap[i] = a;

    while (i > 0 && better(heap[i], heap[(i - 1) / 2])) {
        swap(heap[i], heap[(i - 1) / 2]);
        i = (i - 1) / 2;
    }
}

Ann hpop(Ann *h, int &n) {
    Ann top = h[0];
    h[0] = h[--n];
    int i = 0;

    while (true) {
        int l = 2 * i + 1, r = l + 1, b = i;

        if (l < n && better(h[l], h[b]))
            b = l;

        if (r < n && better(h[r], h[b]))
            b = r;

        if (b == i)
            break;

        swap(h[i], h[b]);
        i = b;
    }

    return top;
}
