// ============================================================
//  lostfound_page.cpp - MODULE 2 screen: Lost & Found
// ============================================================
#include "lostfound_page.h"
#include "ui_common.h"

// ---------- Lost & Found ----------
LostFoundPage::LostFoundPage() {
    QVBoxLayout *lay = new QVBoxLayout(this);
    header(lay, "Lost & Found", "Report something missing or help return what you found.");
    typeBox = new QComboBox; typeBox->addItems({"Lost", "Found"});
    nameEdit = edit("Item name"); placeEdit = edit("Place"); contactEdit = edit("Contact");
    QPushButton *add = new QPushButton("Report");
    QHBoxLayout *form = new QHBoxLayout;
    form->addWidget(typeBox); form->addWidget(nameEdit, 1); form->addWidget(placeEdit, 1); form->addWidget(contactEdit, 1); form->addWidget(add);
    lay->addLayout(form);
    searchEdit = edit("Search item...");
    filterBox = new QComboBox; filterBox->addItems({"All", "Lost", "Found"});
    QPushButton *claim = new QPushButton("Mark selected as claimed");
    QHBoxLayout *row = new QHBoxLayout;
    row->addWidget(searchEdit, 1); row->addWidget(filterBox); row->addWidget(claim);
    lay->addLayout(row);
    tbl = makeTable({"Type", "Item", "Place", "Contact"});
    lay->addWidget(tbl, 1);
    connect(add, &QPushButton::clicked, this, [this] {
        if (nameEdit->text().trimmed().isEmpty()) return;
        apiGet("/items/add?type=" + enc(typeBox->currentText()) + "&name=" + enc(nameEdit->text()) +
               "&place=" + enc(placeEdit->text()) + "&contact=" + enc(contactEdit->text()));
        nameEdit->clear(); placeEdit->clear(); contactEdit->clear();
        refresh();
    });
    connect(claim, &QPushButton::clicked, this, [this] {
        int r = tbl->currentRow();
        if (r < 0 || r >= ids.size()) return;
        apiGet("/items/claim?id=" + QString::number(ids[r]));
        refresh();
    });
    connect(searchEdit, &QLineEdit::textChanged, this, [this] { refresh(); });
    connect(filterBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, [this] { refresh(); });
}
void LostFoundPage::refresh() {
    QString s = searchEdit->text().toLower(), f = filterBox->currentText();
    QList<QStringList> shown;
    ids.clear();
    for (const QStringList &r : table("/items")) {        // r = id | type | name | place | contact
        if (r.size() < 5) continue;
        if (f != "All" && r[1] != f) continue;
        if (!s.isEmpty() && !r[2].toLower().contains(s)) continue;
        ids << r[0].toInt();
        QStringList row; row << r[1] << r[2] << r[3] << r[4];
        shown.append(row);
    }
    fill(tbl, shown);
    for (int r = 0; r < tbl->rowCount(); r++)           // Lost = red, Found = green
        if (QTableWidgetItem *it = tbl->item(r, 0)) {
            bool lost = it->text() == "Lost";
            it->setForeground(QColor(lost ? "#dc2626" : "#059669"));
            QFont f = it->font(); f.setBold(true); it->setFont(f);
            it->setText("\u25CF  " + it->text());
        }
}
