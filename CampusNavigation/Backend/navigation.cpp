// ============================================================
//  navigation.cpp - MODULE 1: Campus Navigation
//  Data structure: Graph (adjacency matrix)   Algorithm: Dijkstra's shortest path
// ============================================================
#include <climits>
#include "navigation.h"

// ================= 1. NAVIGATION: Graph + Dijkstra ===========
// IDs must stay in sync with the frontend (it keeps one map position per ID).
//
// HOW THE GRAPH IS BUILT
//  * Ids 0-26 are PLACES (buildings, sports ground, gates).
//  * Ids 27 and above are ROAD POINTS placed along the real campus roads of the map.
//  * Roads join road points to road points only (ROAD_EDGES).
//  * A place never joins another place directly: each one has a single short link to
//    the road point beside it (PLACE_LINKS), so every route goes along the roads.
//    The three hostels have no road inside them, so each links to the road next to it.
//  * Weights are metres (about 0.55 m per map pixel).

const char *LOC[V] = {
    "CS/IT Block", "CS/IT Block Extension", "New CS/IT Block",
    "Admin Block", "Petroleum Building", "Chanakya Block",
    "Mechanical & Aerospace Block", "Civil Engineering Block", "KP Nautiyal School of Law",
    "Aryabhatt Computer Lab", "Param Computer Lab", "Library",
    "Canteen", "Dispensary", "Silver Jubilee Convention Centre",
    "Priyadarshini Girls Hostel", "Lakshmi Bai Girls Hostel", "Boys Hostel 1",
    "Boys Hostel 2", "Sports Ground", "Cafe & Gym",
    "Indoor Badminton Court", "Basketball Court", "Parking",
    "Gate No. 1", "Gate No. 2", "Gate No. 3",
    "Road Point 1", "Road Point 2", "Road Point 3",
    "Road Point 4", "Road Point 5", "Road Point 6",
    "Road Point 7", "Road Point 8", "Road Point 9",
    "Road Point 10", "Road Point 11", "Road Point 12",
    "Road Point 13", "Road Point 14", "Road Point 15",
    "Road Point 16", "Road Point 17", "Road Point 18",
    "Road Point 19", "Road Point 20", "Road Point 21",
    "Road Point 22", "Road Point 23", "Road Point 24",
    "Road Point 25", "Road Point 26", "Road Point 27",
    "Road Point 28", "Road Point 29", "Road Point 30",
    "Road Point 31", "Road Point 32", "Road Point 33",
    "Road Point 34", "Road Point 35", "Road Point 36",
    "Road Point 37", "Road Point 38", "Road Point 39",
    "Road Point 40", "Road Point 41", "Road Point 42",
    "Road Point 43", "Road Point 44", "Road Point 45",
    "Road Point 46", "Road Point 47", "Road Point 48",
    "Road Point 49", "Road Point 50", "Road Point 51",
    "Road Point 52", "Road Point 53", "Road Point 54",
    "Road Point 55", "Road Point 56", "Road Point 57",
    "Road Point 58", "Road Point 59", "Road Point 60",
    "Road Point 61", "Road Point 62", "Road Point 63",
    "Road Point 64", "Road Point 65", "Road Point 66",
    "Road Point 67", "Road Point 68", "Road Point 69",
    "Road Point 70", "Road Point 71", "Road Point 72",
    "Road Point 73", "Road Point 74", "Road Point 75",
    "Road Point 76", "Road Point 77", "Road Point 78",
    "Road Point 79", "Road Point 80", "Road Point 81",
    "Road Point 82", "Road Point 83", "Road Point 84",
    "Road Point 85", "Road Point 86", "Road Point 87",
    "Road Point 88", "Road Point 89", "Road Point 90",
    "Road Point 91", "Road Point 92", "Road Point 93",
    "Road Point 94", "Road Point 95", "Road Point 96",
    "Road Point 97", "Road Point 98", "Road Point 99",
    "Road Point 100", "Road Point 101"
};

int G[V][V] = {};
int PATH[V], PN = 0;

static void addEdge(int a, int b, int weight)
{
    if (a < 0 || a >= V || b < 0 || b >= V || a == b || weight <= 0)
        return;

    G[a][b] = weight;
    G[b][a] = weight;
}

// each place -> the road point beside it  {place, road point, metres}
static const int PLACE_LINKS[][3] = {
    {0, 95, 40},   // CS/IT Block -> the road beside it
    {1, 97, 25},   // CS/IT Block Extension -> the road beside it
    {2, 76, 40},   // New CS/IT Block -> the road beside it
    {3, 124, 40},   // Admin Block -> the road beside it
    {4, 45, 20},   // Petroleum Building -> the road beside it
    {5, 86, 50},   // Chanakya Block -> the road beside it
    {6, 66, 15},   // Mechanical & Aerospace Block -> the road beside it
    {7, 107, 25},   // Civil Engineering Block -> the road beside it
    {8, 62, 20},   // KP Nautiyal School of Law -> the road beside it
    {9, 127, 15},   // Aryabhatt Computer Lab -> the road beside it
    {10, 58, 25},   // Param Computer Lab -> the road beside it
    {11, 61, 25},   // Library -> the road beside it
    {12, 55, 20},   // Canteen -> the road beside it
    {13, 30, 20},   // Dispensary -> the road beside it
    {14, 126, 25},   // Silver Jubilee Convention Centre -> the road beside it
    {15, 66, 35},   // Priyadarshini Girls Hostel -> the road beside it
    {16, 105, 35},   // Lakshmi Bai Girls Hostel -> the road beside it
    {17, 119, 40},   // Boys Hostel 1 -> the road beside it
    {18, 120, 35},   // Boys Hostel 2 -> the road beside it
    {19, 119, 65},   // Sports Ground -> the road beside it
    {20, 123, 30},   // Cafe & Gym -> the road beside it
    {21, 125, 15},   // Indoor Badminton Court -> the road beside it
    {22, 87, 25},   // Basketball Court -> the road beside it
    {23, 74, 40},   // Parking -> the road beside it
    {24, 28, 5},   // Gate No. 1 -> the road beside it
    {25, 71, 5},   // Gate No. 2 -> the road beside it
    {26, 47, 5},   // Gate No. 3 -> the road beside it
};

// campus roads  {road point, road point, metres}
static const int ROAD_EDGES[][3] = {
    {27, 37, 20}, {27, 46, 35}, {27, 53, 20}, {28, 30, 20}, {28, 60, 15}, {28, 84, 20},
    {29, 48, 15}, {29, 79, 15}, {30, 108, 15}, {31, 50, 25}, {31, 76, 15}, {32, 105, 20},
    {33, 34, 20}, {33, 112, 20}, {33, 127, 20}, {34, 39, 20}, {34, 100, 15}, {35, 58, 10},
    {35, 61, 20}, {35, 87, 15}, {36, 37, 20}, {36, 52, 30}, {36, 127, 20}, {38, 39, 15},
    {39, 101, 15}, {40, 41, 20}, {40, 46, 10}, {40, 111, 25}, {41, 42, 20}, {42, 55, 25},
    {42, 57, 20}, {43, 44, 25}, {43, 62, 25}, {43, 75, 35}, {44, 45, 30}, {45, 69, 15},
    {46, 114, 30}, {47, 85, 20}, {47, 118, 25}, {48, 49, 25}, {49, 50, 25}, {49, 93, 15},
    {51, 52, 30}, {52, 59, 15}, {53, 54, 20}, {54, 57, 20}, {54, 122, 25}, {55, 56, 25},
    {56, 121, 10}, {58, 59, 30}, {59, 102, 25}, {60, 126, 35}, {61, 106, 30}, {63, 64, 30},
    {64, 65, 30}, {65, 66, 30}, {66, 67, 30}, {67, 68, 30}, {68, 98, 20}, {69, 70, 15},
    {70, 101, 10}, {71, 72, 35}, {72, 73, 35}, {73, 74, 35}, {74, 75, 35}, {77, 78, 25},
    {78, 79, 30}, {78, 80, 25}, {79, 85, 20}, {80, 81, 25}, {81, 82, 15}, {82, 83, 15},
    {83, 84, 20}, {86, 91, 20}, {87, 88, 5}, {88, 89, 20}, {89, 90, 20}, {90, 91, 20},
    {92, 107, 30}, {92, 125, 25}, {93, 94, 30}, {94, 95, 25}, {95, 96, 25}, {96, 97, 25},
    {98, 99, 20}, {99, 100, 15}, {101, 102, 20}, {103, 104, 10}, {103, 115, 20}, {103, 119, 30},
    {104, 105, 20}, {106, 107, 30}, {108, 109, 15}, {109, 110, 30}, {110, 111, 20}, {112, 113, 20},
    {113, 114, 25}, {115, 116, 20}, {116, 117, 25}, {117, 118, 25}, {119, 120, 30}, {120, 121, 15},
    {122, 123, 20}, {123, 124, 25}, {124, 125, 25},
};

void initGraph()
{
    for (int i = 0; i < V; i++)
        for (int j = 0; j < V; j++)
            G[i][j] = 0;

    for (const auto &e : ROAD_EDGES)
        addEdge(e[0], e[1], e[2]);

    for (const auto &e : PLACE_LINKS)
        addEdge(e[0], e[1], e[2]);
}


int shortestPath(int s, int t) {
    PN = 0;

    if (s < 0 || s >= V || t < 0 || t >= V)
        return -1;

    int d[V], p[V];
    bool done[V] = {false};

    for (int i = 0; i < V; i++) {
        d[i] = INT_MAX;
        p[i] = -1;
    }

    d[s] = 0;

    for (int k = 0; k < V; k++) {
        int u = -1;

        for (int i = 0; i < V; i++) {
            if (!done[i] && (u == -1 || d[i] < d[u]))
                u = i;
        }

        if (u == -1 || d[u] == INT_MAX)
            break;

        done[u] = true;

        for (int v = 0; v < V; v++) {
            if (G[u][v] > 0 && !done[v] &&
                d[u] + G[u][v] < d[v]) {
                d[v] = d[u] + G[u][v];
                p[v] = u;
            }
        }
    }

    if (d[t] == INT_MAX)
        return -1;

    int reversePath[V], n = 0;

    for (int x = t; x != -1 && n < V; x = p[x])
        reversePath[n++] = x;

    PN = n;

    for (int i = 0; i < n; i++)
        PATH[i] = reversePath[n - 1 - i];

    return d[t];
}
