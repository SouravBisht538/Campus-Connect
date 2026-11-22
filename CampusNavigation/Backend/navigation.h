// ============================================================
//  navigation.h - MODULE 1: Campus Navigation  (Graph + Dijkstra's algorithm)
// ============================================================
#ifndef NAVIGATION_H
#define NAVIGATION_H
#include <string>
using namespace std;

// 27 places (buildings, grounds, gates) + 101 road points that trace the real campus roads.
// Ids 0-26 are places; ids 27 and above are road points.
const int V = 128;

extern const char *LOC[V];
extern int G[V][V];          // adjacency matrix, weights in metres (0 = no road)
extern int PATH[V], PN;      // last computed route (PN stops)

void initGraph();
int shortestPath(int s, int t);   // Dijkstra: distance in metres, or -1 if unreachable
#endif
