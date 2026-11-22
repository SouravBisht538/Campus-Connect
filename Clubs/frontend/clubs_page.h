// ============================================================
//  clubs_page.h - MODULE 4 screen: Clubs
// ============================================================
#pragma once
#include "page.h"

class ClubsPage : public Page {
public:
    ClubsPage();
    void refresh() override;
private:
    QLineEdit *nameEdit, *headEdit, *descEdit;
    QTableWidget *tbl;
};
