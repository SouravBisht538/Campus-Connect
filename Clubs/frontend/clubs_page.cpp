// ============================================================
//  clubs_page.cpp - MODULE 4 screen: Clubs
// ============================================================
#include "clubs_page.h"
#include "ui_common.h"

// ---------- Clubs ----------
ClubsPage::ClubsPage() {
    QVBoxLayout *lay = new QVBoxLayout(this);
    header(lay, "Clubs", "Discover communities and join the ones you like.");
    nameEdit = edit("Club name"); headEdit = edit("Club head"); descEdit = edit("Description");
    QPushButton *add = new QPushButton("Add club");
    QHBoxLayout *form = new QHBoxLayout;
    form->addWidget(nameEdit, 1); form->addWidget(headEdit, 1); form->addWidget(descEdit, 2); form->addWidget(add);
    lay->addLayout(form);
    QPushButton *join = new QPushButton("Join selected club");
    QHBoxLayout *row = new QHBoxLayout;
    row->addWidget(join); row->addStretch();
    lay->addLayout(row);
    tbl = makeTable({"Club", "Head", "Description", "Members"});
    lay->addWidget(tbl, 1);
    if (ROLE_LEVEL < 2) {                              // Only Faculty and Admin accounts can add clubs.
        for (QWidget *w : {(QWidget *)nameEdit, (QWidget *)headEdit, (QWidget *)descEdit, (QWidget *)add}) w->hide();
        QLabel *note = new QLabel("Only Faculty and Admin accounts can add clubs."); note->setObjectName("sub");
        lay->insertWidget(lay->count() - 1, note);
    }
    connect(add, &QPushButton::clicked, this, [this] {
        if (nameEdit->text().trimmed().isEmpty()) return;
        apiGet("/clubs/add?name=" + enc(nameEdit->text()) + "&head=" + enc(headEdit->text()) + "&desc=" + enc(descEdit->text()));
        nameEdit->clear(); headEdit->clear(); descEdit->clear();
        refresh();
    });
    connect(join, &QPushButton::clicked, this, [this] {
        int r = tbl->currentRow();
        if (r < 0) return;
        apiGet("/clubs/join?i=" + QString::number(r));  // row number = club position in the backend list
        refresh();
        tbl->selectRow(r);
    });
}
void ClubsPage::refresh() { fill(tbl, table("/clubs")); }
