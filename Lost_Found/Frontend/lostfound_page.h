// ============================================================
//  lostfound_page.h - MODULE 2 screen: Lost & Found
// ============================================================
#pragma once
#include "page.h"

class LostFoundPage : public Page {
public:
    LostFoundPage();
    void refresh() override;
private:
    QComboBox *typeBox, *filterBox;
    QLineEdit *nameEdit, *placeEdit, *contactEdit, *searchEdit;
    QTableWidget *tbl;
    QList<int> ids;                   // item id of each table row
};
