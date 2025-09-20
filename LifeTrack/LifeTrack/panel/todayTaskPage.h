#pragma once

#include <QWidget>
#include <QStandardItemModel>
#include <QTableView>

#include <QLabel>
#include <QPushButton>
#include <QDateEdit>
#include <QEvent>
#include <QMap>

#include "baseTable.h"
#include "customWidget.h"
#include "../management/dataCenter.h"
class CTodayTaskPage : public CCustomWidget
{
    Q_OBJECT

public:
    CTodayTaskPage(QWidget *parent = nullptr);
    ~CTodayTaskPage();

    void createUi();
protected:
    //bool event(QEvent* even) override;
    //void mousePressEvent(QMouseEvent* event) override;
private:
    
    void initTableData();
    void updateTableData(const QMap<QString,STaskListPtr>& data);
private:
    void addTaskBtn_click();
    void onDateEditChange();
private:
    CBaseTableModel* m_pTableModel;
    QTableView* m_pTableView;

    QLabel* m_pShowLabel;
    QPushButton* m_pAddTaskBtn;
    QDateEdit* m_pDateEdit;

    QMap<QString,STaskListPtr> m_tempAddDiaTaskPtr;
};
