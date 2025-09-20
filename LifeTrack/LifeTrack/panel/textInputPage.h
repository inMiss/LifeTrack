#pragma once

#include <QWidget>
#include <QLineEdit>
#include <QLabel>
#include <QPushButton>

#include "customWidget.h"

class CTextInputPage : public CCustomWidget
{
    Q_OBJECT

public:
    CTextInputPage(QWidget *parent = nullptr);
    ~CTextInputPage();

    void createUi();

private:
    QLabel* m_pShowTextLabel;
    QLineEdit* m_pTextEdit;
    QPushButton* m_pSaveBtn;
};
