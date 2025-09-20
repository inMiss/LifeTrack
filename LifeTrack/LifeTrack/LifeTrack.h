#pragma once

#include <QWidget>
#include "ui_LifeTrack.h"
#include <QTabWidget>

class LifeTrack : public QWidget
{
    Q_OBJECT

public:
    LifeTrack(QWidget *parent = nullptr);
    ~LifeTrack();

    void updateLanguageShowText();
    void resetWindowSize();
private:
    void createUi();
private:
    Ui::LifeTrackClass ui;
private:
    QTabWidget* m_pPageManager;
};
