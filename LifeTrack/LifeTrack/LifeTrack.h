#pragma once

#include <QWidget>
#include "ui_LifeTrack.h"
#include <QTabWidget>

#include "panel/mainPanel/mainPanel.h"
#include "panel/analysePanel/analysisPanel.h"
#include "panel/systemSettingPanel/systemPanel.h"

class LifeTrack : public QWidget
{
    Q_OBJECT
private slots:
    void slotTaskOperation(const QString& task_id, const int nRow, const QString& taskActiveType, const QString& taskOperType);
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

    CMainPanel* m_pMainPage;
    CAnalysisPanel* m_pAnalysisPage;
    CSystemPanel* m_pSystemPage;
};
