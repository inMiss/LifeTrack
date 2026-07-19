#pragma once

#include <QWidget>
#include "../common/customMessage.h"

class CCustomWidget : public QWidget
{
public:
    CCustomWidget(QWidget *parent = nullptr);
    ~CCustomWidget();

    virtual void updateLanguageShowText();

};
