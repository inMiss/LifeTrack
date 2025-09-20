#pragma once

#include <QWidget>

class CCustomWidget : public QWidget
{
public:
    CCustomWidget(QWidget *parent = nullptr);
    ~CCustomWidget();

    virtual void updateLanguageShowText();
};
