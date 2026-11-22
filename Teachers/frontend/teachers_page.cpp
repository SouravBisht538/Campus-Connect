// ============================================================
//  teachers_page.cpp - MODULE 3 screen: Teacher Details
// ============================================================
#include "teachers_page.h"
#include "ui_common.h"

// ---------- Teachers ----------
TeachersPage::TeachersPage() {
    QVBoxLayout *lay = new QVBoxLayout(this);
    header(lay, "Teachers", "Find faculty, departments and cabins.");
    nameEdit = edit("Name"); deptEdit = edit("Department"); cabinEdit = edit("Cabin no."); emailEdit = edit("Email");
    QPushButton *add = new QPushButton("Add teacher");
    QHBoxLayout *form = new QHBoxLayout;
    form->addWidget(nameEdit, 1); form->addWidget(deptEdit, 1); form->addWidget(cabinEdit); form->addWidget(emailEdit, 1); form->addWidget(add);
    lay->addLayout(form);
    searchEdit = edit("Search name / department");
    lay->addWidget(searchEdit);
    tbl = makeTable({"Name", "Department", "Cabin", "Email"});
    lay->addWidget(tbl, 1);
    if (ROLE_LEVEL < 3) {                              // Only Admin accounts can add teachers.
        for (QWidget *w : {(QWidget *)nameEdit, (QWidget *)deptEdit, (QWidget *)cabinEdit, (QWidget *)emailEdit, (QWidget *)add}) w->hide();
        QLabel *note = new QLabel("Only Admin accounts can add teachers."); note->setObjectName("sub");
        lay->insertWidget(lay->count() - 1, note);
    }
    connect(add, &QPushButton::clicked, this, [this] {
        if (nameEdit->text().trimmed().isEmpty()) return;
        apiGet("/teachers/add?name=" + enc(nameEdit->text()) + "&dept=" + enc(deptEdit->text()) +
               "&cabin=" + enc(cabinEdit->text()) + "&email=" + enc(emailEdit->text()));
        nameEdit->clear(); deptEdit->clear(); cabinEdit->clear(); emailEdit->clear();
        refresh();
    });
    connect(searchEdit, &QLineEdit::textChanged, this, [this] { refresh(); });
}
void TeachersPage::refresh() {                        // backend returns teachers already sorted (BST)
    fill(tbl, table("/teachers?q=" + enc(searchEdit->text().toLower())));
}
