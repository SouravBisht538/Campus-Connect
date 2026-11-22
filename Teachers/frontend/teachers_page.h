// ============================================================
//  teachers_page.h - MODULE 3 screen: Teacher Details
// ============================================================
#pragma once
#include "page.h"

class TeachersPage : public Page {
public:
    TeachersPage();
    void refresh() override;
private:
    QLineEdit *nameEdit, *deptEdit, *cabinEdit, *emailEdit, *searchEdit;
    QTableWidget *tbl;
};
