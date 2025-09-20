#pragma once

#include <QWidget>
#include <QObject>
#include <QTableView>
#include <qdate>
#include <QAbstractTableModel>
#include <QStandardItemModel>

#include "../management/structCommon.h"
//所有任务
class CBaseTableView : public QTableView
{
public:
    CBaseTableView(QWidget *parent = nullptr);
    ~CBaseTableView();

};


//task list
enum ETaskTableColumn
{
    E_TASKETTABLE_CHECK = 0,
    E_TASKETTABLE_ID,
    E_TASKETTABLE_NAME,
    E_TASKETTABLE_STARTTIME,
    E_TASKETTABLE_ENDTIME,
    E_TASKETTABLE_DURINGTIME,
    E_TASKETTABLE_CREATETIME,
    E_TASKETTABLE_IMPORTLEVEL,
    E_TASKETTABLE_VALUE,
    E_TASKETTABLE_REPEATTYPE,
    E_TASKETTABLE_FINISHTIMES,
    E_TASKETTABLE_SUMPOINTS,
    E_TASKETTABLE_BELONGTYPE,
    E_TASKETTABLE_DEDUCTION,
    E_TASKETTABLE_COUNTDOWN,
    E_TASKETTABLE_CANPAUSE,
    E_TASKETTABLE_TASLOPERATOR,
    E_TASKETTABLE_TASKSTATUS,
    E_TASKETTABLE_FINISHTIME,
    E_TASKETTABLE_PAUSETIME,
    E_TASKETTABLE_PARENTTASK,
    E_TASKETTABLE_REMARK
};

class CBaseTableModel : public QStandardItemModel
{
public:
    CBaseTableModel(bool bFilter = false,QWidget* parent = nullptr);
    CBaseTableModel(int rows, int columns, bool bFilter = false,QWidget* parent = nullptr);
    ~CBaseTableModel();

    void addDatas(const QMap<QString, STaskListPtr>& datas);
    void addData(const STaskListPtr& data);
    void setCurrentDate(const QDate& date);
    void clearAllData();
private:
    void initHorizontalHeaderLabels();   
private:
    QDate m_filterDate;
    bool m_bFilter;
};

