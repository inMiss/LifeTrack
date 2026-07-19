#pragma once

#include <QWidget>

#include <QLabel>
#include <QPushButton>
#include <QEvent>
#include <QMap>
#include <QObject>

#include "activeTaskPage.h"
#include "unActiveTaskPage.h"

class CMainPanel : public QWidget
{
    Q_OBJECT
signals:   
    void sigUpdateCompleteTaskTable();//在分析界面
private slots:
    void slotTaskOperation(const QString& task_id, const int nRow, const QString& taskActiveType, const QString& taskOperType);
public:
    CMainPanel(QWidget *parent = nullptr);
    ~CMainPanel();

    void createUi();
    void slotTaskOperation_outerUse(const QString& task_id, const int nRow, const QString& taskActiveType, const QString& taskOperType);
private:
    void addTaskBtn_click();
private:
    CActiveTaskPage* m_pActivePage;
    CUnActiveTaskPage* m_pUnActivePage;

    QPushButton* m_pAddTaskBtn;
    STaskListPtr m_tempAddDiaTaskPtr;
};
