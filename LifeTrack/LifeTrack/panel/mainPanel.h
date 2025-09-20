#pragma once

#include <QWidget>

#include "todayTaskPage.h"
#include "textInputPage.h"
#include "textShowPage.h"

class CMainPanel : public QWidget
{
    Q_OBJECT

public:
    CMainPanel(QWidget *parent = nullptr);
    ~CMainPanel();

    void createUi();

private:
    CTodayTaskPage* m_pTodayTaskPage;
    CTextInputPage* m_pTextInputPage;
    CTextShowPage*  m_pTextShowPage;
};
