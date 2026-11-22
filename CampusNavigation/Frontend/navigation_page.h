// ============================================================
//  navigation_page.h - MODULE 1 screen: Campus Navigation (route drawn on the real campus map)
// ============================================================
#pragma once
#include <functional>
#include "page.h"

// Draws the campus graph (circles = places, lines = roads) with QPainter.
class MapWidget : public QWidget {
public:
    QStringList names;
    QList<QList<int>> edges;          // each edge = {from, to, metres}
    QList<int> path;                  // route found by the backend
    int from = -1, to = -1;
    std::function<void(int)> onClick; // called when a place is clicked
    MapWidget() { setMinimumHeight(320); }
protected:
    void paintEvent(QPaintEvent *) override;
    void mousePressEvent(QMouseEvent *ev) override;
};

class NavigationPage : public Page {
public:
    NavigationPage();
    void refresh() override;
private:
    void loadGraph();
    void showRoute();
    QComboBox *fromBox, *toBox;
    QLabel *result;
    MapWidget *mapw;
};
