// ============================================================
//  announcements_page.h - MODULE 5 screen: Announcements
// ============================================================
#pragma once
#include "page.h"

QString priorityName(int p);          // 1 = High, 2 = Medium, otherwise Low

class AnnouncementsPage : public Page {
public:
    AnnouncementsPage();
    void refresh() override;
private:
    QLineEdit *titleEdit, *msgEdit;
    QComboBox *prBox;
    QTableWidget *tbl;
};
