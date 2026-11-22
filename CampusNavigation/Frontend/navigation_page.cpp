// ============================================================
//  navigation_page.cpp - MODULE 1 screen: Campus Navigation
//  No algorithm here: the backend runs Dijkstra, this file only draws the result.
// ============================================================
#include "navigation_page.h"
#include "ui_common.h"
#include <cmath>

// ---------- Real campus map ----------
// The campus picture is compiled into the app (resources.qrc). Every graph node has a fixed
// pixel position on that picture, so the nodes sit exactly on the real buildings and roads.
static const QSizeF MAP_SIZE(2298, 1410);                // size of campus_map_new.jpeg in pixels
static const QPointF NODE_POS[128] = {
    {177, 300},
    {49, 5},
    {121, 549},
    {1083, 1099},
    {1561, 899},
    {1766, 982},
    {1612, 473},
    {1211, 1307},
    {1629, 729},
    {1304, 912},
    {1438, 1006},
    {1297, 1086},
    {867, 990},
    {1014, 438},
    {883, 186},
    {1467, 395},
    {421, 883},
    {541, 966},
    {659, 1055},
    {766, 715},
    {860, 1163},
    {1004, 1262},
    {1397, 1198},
    {1869, 544},
    {903, 450},
    {2170, 439},
    {297, 537},
    {1085, 923},
    {899, 432},
    {458, 477},
    {952, 480},
    {287, 503},
    {434, 758},
    {1320, 830},
    {1379, 787},
    {1381, 1098},
    {1226, 913},
    {1157, 918},
    {1457, 793},
    {1423, 830},
    {1020, 803},
    {965, 843},
    {911, 885},
    {1744, 816},
    {1657, 834},
    {1565, 830},
    {1039, 830},
    {316, 527},
    {448, 424},
    {397, 363},
    {341, 438},
    {1203, 1019},
    {1293, 968},
    {1033, 954},
    {981, 983},
    {846, 927},
    {778, 971},
    {946, 934},
    {1381, 1060},
    {1341, 974},
    {909, 380},
    {1339, 1152},
    {1686, 755},
    {1809, 208},
    {1727, 286},
    {1647, 364},
    {1568, 440},
    {1490, 518},
    {1410, 594},
    {1524, 838},
    {1485, 861},
    {2153, 433},
    {2068, 510},
    {1987, 588},
    {1906, 665},
    {1825, 742},
    {251, 528},
    {475, 670},
    {517, 597},
    {442, 518},
    {585, 556},
    {655, 514},
    {711, 502},
    {766, 490},
    {833, 459},
    {389, 545},
    {1615, 966},
    {1414, 1128},
    {1432, 1131},
    {1481, 1093},
    {1530, 1056},
    {1572, 1012},
    {1082, 1295},
    {377, 312},
    {317, 235},
    {254, 177},
    {193, 118},
    {131, 59},
    {1413, 653},
    {1418, 711},
    {1408, 756},
    {1449, 871},
    {1398, 919},
    {579, 826},
    {543, 831},
    {490, 796},
    {1253, 1199},
    {1168, 1246},
    {992, 520},
    {1032, 559},
    {1037, 658},
    {1028, 729},
    {1266, 788},
    {1199, 764},
    {1125, 773},
    {535, 768},
    {492, 708},
    {432, 650},
    {373, 592},
    {645, 890},
    {708, 952},
    {744, 973},
    {946, 1060},
    {951, 1125},
    {994, 1181},
    {1036, 1237},
    {929, 262},
    {1270, 872}
};
static const int NODE_COUNT = 128;
static const int PLACE_COUNT = 27;                       // ids 0-26 = places, the rest = road points
// junctions / walkways: small dots without a name label, not selectable in the drop-downs
static bool isRoadNode(int i) { return i >= PLACE_COUNT; }

// ---------- MapWidget (QPainter) ----------
// Draws the real campus map and puts the graph (places + roads) on top of it.
static QPixmap &campusPicture() { static QPixmap pic(":/campus_map_new.jpeg"); return pic; }

static QRectF pictureRect(const QWidget *w) {            // picture keeps its shape and is centred
    double scale = qMin(w->width() / MAP_SIZE.width(), w->height() / MAP_SIZE.height());
    double pw = MAP_SIZE.width() * scale, ph = MAP_SIZE.height() * scale;
    return QRectF((w->width() - pw) / 2, (w->height() - ph) / 2, pw, ph);
}
static QPointF nodePoint(const QWidget *w, int i) {      // picture pixel -> widget pixel
    QRectF r = pictureRect(w);
    return QPointF(r.x() + NODE_POS[i].x() / MAP_SIZE.width() * r.width(),
                   r.y() + NODE_POS[i].y() / MAP_SIZE.height() * r.height());
}

void MapWidget::paintEvent(QPaintEvent *) {
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing);
    p.setRenderHint(QPainter::SmoothPixmapTransform);
    p.fillRect(rect(), QColor("#fafafa"));

    QRectF pic = pictureRect(this);
    p.drawPixmap(pic, campusPicture(), QRectF(campusPicture().rect()));

    int n = qMin<int>(names.size(), NODE_COUNT);

    // campus road network (black) and each building's short link to its road (green)
    double lw = qBound(2.0, pic.width() / MAP_SIZE.width() * 8.0, 5.0);
    for (const QList<int> &e : edges) {
        if (e[0] >= n || e[1] >= n) continue;
        bool link = !isRoadNode(e[0]) || !isRoadNode(e[1]);
        p.setPen(QPen(link ? QColor("#16a34a") : QColor("#111827"), link ? lw * 0.6 : lw, Qt::SolidLine, Qt::RoundCap));
        p.drawLine(nodePoint(this, e[0]), nodePoint(this, e[1]));
    }

    // the route chosen by the backend (white outline + orange line)
    for (int pass = 0; pass < 2; pass++) {
        QPen pen(pass == 0 ? QColor("#ffffff") : QColor("#f59e0b"), pass == 0 ? 10 : 6);
        pen.setCapStyle(Qt::RoundCap); pen.setJoinStyle(Qt::RoundJoin);
        p.setPen(pen);
        for (int k = 0; k + 1 < path.size(); k++)
            if (path[k] < n && path[k + 1] < n) p.drawLine(nodePoint(this, path[k]), nodePoint(this, path[k + 1]));
    }

    // places
    QFont labelFont("Arial");
    labelFont.setBold(true);
    labelFont.setPointSizeF(qBound(7.0, pic.width() / 95.0, 11.0));
    p.setFont(labelFont);
    for (int i = 0; i < n; i++) {
        QPointF c = nodePoint(this, i);
        bool road = isRoadNode(i), onRoute = path.contains(i);
        if (road) continue;                                   // road points are not drawn
        QColor color = road ? QColor("#64748b") : QColor("#2563eb");
        if (onRoute) color = QColor("#f59e0b");
        if (!path.isEmpty() && i == from) color = QColor("#059669");
        if (!path.isEmpty() && i == to) color = QColor("#dc2626");
        double r = (road && !onRoute) ? 4 : 8;
        p.setPen(QPen(Qt::white, 2));
        p.setBrush(color);
        p.drawEllipse(c, r, r);
        if (road) continue;                                   // junctions have no label
        QRectF box(c.x() - 70, c.y() + 10, 140, 34);
        int flags = Qt::AlignHCenter | Qt::AlignTop | Qt::TextWordWrap;
        p.setPen(Qt::white);                                  // white outline keeps the text readable
        for (int dx = -1; dx <= 1; dx++)
            for (int dy = -1; dy <= 1; dy++)
                if (dx || dy) p.drawText(box.translated(dx, dy), flags, names[i]);
        p.setPen(QColor("#1e293b"));
        p.drawText(box, flags, names[i]);
    }
}

void MapWidget::mousePressEvent(QMouseEvent *ev) {       // click = choose that place as destination
    int best = -1; double bestDist = 16;
    for (int i = 0; i < names.size() && i < NODE_COUNT; i++) {
        double d = QLineF(QPointF(ev->pos()), nodePoint(this, i)).length();
        if (d < bestDist) { bestDist = d; best = i; }
    }
    if (best >= 0 && !isRoadNode(best) && onClick) onClick(best);
}

// ---------- Navigation ----------
NavigationPage::NavigationPage() {
    QVBoxLayout *lay = new QVBoxLayout(this);
    header(lay, "Campus Navigation", "Find the shortest way between two places. You can also click a place on the map.");
    fromBox = new QComboBox; toBox = new QComboBox;
    QPushButton *btn = new QPushButton("Find shortest path");
    QHBoxLayout *row = new QHBoxLayout;
    row->addWidget(new QLabel("From")); row->addWidget(fromBox);
    row->addWidget(new QLabel("To"));   row->addWidget(toBox);
    row->addWidget(btn); row->addStretch();
    lay->addLayout(row);
    result = new QLabel; result->setObjectName("banner"); result->setWordWrap(true); result->hide();
    lay->addWidget(result);
    mapw = new MapWidget;
    mapw->setMinimumHeight(520);
    lay->addWidget(mapw, 1);
    connect(btn, &QPushButton::clicked, this, [this] { showRoute(); });
    mapw->onClick = [this](int i) { int k = toBox->findData(i); if (k >= 0) toBox->setCurrentIndex(k); showRoute(); };
    loadGraph();
}
void NavigationPage::loadGraph() {                    // places + roads come from the backend
    mapw->names.clear(); mapw->edges.clear(); fromBox->clear(); toBox->clear();
    for (const QStringList &r : table("/graph")) {
        if (r[0] == "L" && r.size() >= 2) {
            int id = mapw->names.size();                     // node id = position in the backend list
            mapw->names << r[1];
            if (!isRoadNode(id)) { fromBox->addItem(r[1], id); toBox->addItem(r[1], id); }   // only real places in the lists
        }
        else if (r[0] == "E" && r.size() >= 4) {
            QList<int> e; e << r[1].toInt() << r[2].toInt() << r[3].toInt();
            mapw->edges.append(e);
        }
    }
    mapw->update();
}
void NavigationPage::refresh() { if (mapw->names.isEmpty()) loadGraph(); }
void NavigationPage::showRoute() {                    // backend runs Dijkstra, we only display the answer
    if (fromBox->currentIndex() < 0 || toBox->currentIndex() < 0) return;
    int s = fromBox->currentData().toInt(), t = toBox->currentData().toInt();
    QStringList ln = apiGet("/route?from=" + QString::number(s) + "&to=" + QString::number(t)).split('\n', Qt::SkipEmptyParts);
    int d = ln.value(0, "-1").toInt();
    QList<int> path;
    if (d >= 0 && ln.size() > 1)
        for (const QString &x : ln[1].split(' ', Qt::SkipEmptyParts)) path << x.toInt();
    QStringList placeNames;
    for (int i : path) if (!isRoadNode(i)) placeNames << mapw->names.value(i);   // list only real places
    mapw->path = path; mapw->from = s; mapw->to = t; mapw->update();
    if (d < 0) result->setText("No path found");
    else result->setText("Route: " + placeNames.join("  ->  ") + "   (" + QString::number(d) + " m)");
    result->show();
}
