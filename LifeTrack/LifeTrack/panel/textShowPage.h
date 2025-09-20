#pragma once

#include <QWidget>
#include <QLabel>

#include "customWidget.h"

class CTextShowPage : public CCustomWidget
{
    Q_OBJECT

public:
    CTextShowPage(QWidget *parent = nullptr);
    ~CTextShowPage();

    void createUi();

private:
    QLabel* m_pTextLabel;
    QLabel* m_pAuthorLabel;
};
