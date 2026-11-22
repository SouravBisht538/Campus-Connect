// ============================================================
//  announcements_page.cpp - MODULE 5 screen: Announcements
// ============================================================
#include "announcements_page.h"
#include "ui_common.h"

QString priorityName(int p) {
    if (p == 1) return "High";
    if (p == 2) return "Medium";
    return "Low";
}

// ---------- Announcements ----------
AnnouncementsPage::AnnouncementsPage() {
    QVBoxLayout *lay = new QVBoxLayout(this);
    header(lay, "Announcements", "Latest campus updates, most important first.");
    titleEdit = edit("Title"); msgEdit = edit("Message");
    prBox = new QComboBox; prBox->addItems({"High", "Medium", "Low"}); prBox->setCurrentIndex(1);
    QPushButton *post = new QPushButton("Post");
    QPushButton *pop = new QPushButton("Remove top");
    QHBoxLayout *form = new QHBoxLayout;
    form->addWidget(titleEdit, 1); form->addWidget(msgEdit, 2); form->addWidget(prBox); form->addWidget(post); form->addWidget(pop);
    lay->addLayout(form);
    tbl = makeTable({"Priority", "Title", "Message"});
    lay->addWidget(tbl, 1);
    if (ROLE_LEVEL < 2) {                               // students can only read announcements
        for (QWidget *w : {(QWidget *)titleEdit, (QWidget *)msgEdit, (QWidget *)prBox, (QWidget *)post}) w->hide();
    }
    if (ROLE_LEVEL < 3) pop->hide();                    // only Admin removes announcements
    if (ROLE_LEVEL < 2) {
        QLabel *note = new QLabel("Only Faculty and Admin accounts can post announcements.");
        note->setObjectName("sub");
        lay->insertWidget(lay->count() - 1, note);
    }
    connect(post, &QPushButton::clicked, this, [this] {
        if (titleEdit->text().trimmed().isEmpty()) return;
        apiGet("/news/add?title=" + enc(titleEdit->text()) + "&msg=" + enc(msgEdit->text()) + "&pr=" + QString::number(prBox->currentIndex() + 1));
        titleEdit->clear(); msgEdit->clear();
        refresh();
    });
    connect(pop, &QPushButton::clicked, this, [this] { apiGet("/news/pop"); refresh(); });
}
void AnnouncementsPage::refresh() {                   // backend returns them in priority order (heap)
    QList<QStringList> shown;
    for (const QStringList &r : table("/news")) {     // r = priority | id | title | message
        if (r.size() < 4) continue;
        QStringList row; row << priorityName(r[0].toInt()) << r[2] << r[3];
        shown.append(row);
    }
    fill(tbl, shown);
    for (int r = 0; r < tbl->rowCount(); r++)           // High = red, Medium = orange, Low = green
        if (QTableWidgetItem *it = tbl->item(r, 0)) {
            QString pr = it->text();
            bool high = pr == "High", med = pr == "Medium";
            it->setForeground(QColor(high ? "#dc2626" : med ? "#d97706" : "#059669"));
            QFont f = it->font(); f.setBold(true); it->setFont(f);
            it->setText("\u25CF  " + pr);
        }
}
