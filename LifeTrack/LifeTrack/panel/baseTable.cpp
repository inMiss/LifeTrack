#include "baseTable.h"
#include <QStandardItem>
#include <QDateTime>
#include <QDateTimeEdit>

#include "../language/language.h"
#include "../common/commonFunc.h"
#include "../management/dataCenter.h"

CBaseTableView::CBaseTableView(QWidget *parent)
    : QTableView(parent)
{
    
}

CBaseTableView::~CBaseTableView()
{}

/// /////////////////////////
/// CBaseTableModel
/// ///////////////////////
CBaseTableModel::CBaseTableModel(bool bFilter /* = false */,QWidget* parent /* = nullptr */)
    : QStandardItemModel(parent)
    , m_bFilter(bFilter)
{
    initHorizontalHeaderLabels();
}

CBaseTableModel::CBaseTableModel(int rows, int columns, bool bFilter /* = false */,QWidget* parent /* = nullptr */)
    : QStandardItemModel(rows,columns,parent)
    , m_bFilter(bFilter)
{
    initHorizontalHeaderLabels();
    
}

CBaseTableModel::~CBaseTableModel()
{

}

void CBaseTableModel::initHorizontalHeaderLabels()
{
    QStringList head;
    //head << "Column-1" << "Column-2";
    head << getLanguegeShowText("table_taskList_check");
    head << getLanguegeShowText("table_taskList_id");
    head << getLanguegeShowText("table_taskList_name");
    head << getLanguegeShowText("table_taskList_start_time");
    head << getLanguegeShowText("table_taskList_end_time");
    head << getLanguegeShowText("table_taskList_during_time");
    head << getLanguegeShowText("table_taskList_create_time");
    head << getLanguegeShowText("table_taskList_import_level");
    head << getLanguegeShowText("table_taskList_value");
    head << getLanguegeShowText("table_taskList_repeat_type");
    head << getLanguegeShowText("table_taskList_finish_times");
    head << getLanguegeShowText("table_taskList_sum_ponits");
    head << getLanguegeShowText("table_taskList_belong_type");
    head << getLanguegeShowText("table_taskList_is_deduction");
    head << getLanguegeShowText("table_taskList_is_countdown");
    head << getLanguegeShowText("table_taskList_is_can_pause");
    head << getLanguegeShowText("table_taskList_task_operator");
    head << getLanguegeShowText("table_taskList_task_status");
    head << getLanguegeShowText("table_taskList_finish_time");
    head << getLanguegeShowText("table_taskList_pause_time");
    head << getLanguegeShowText("table_taskList_parent_task");
    head << getLanguegeShowText("table_taskList_remark");
    this->setHorizontalHeaderLabels(head);

    m_filterDate = QDate::currentDate();
}

void CBaseTableModel::addDatas(const QMap<QString, STaskListPtr>& datas)
{
    QMap<QString, STaskListPtr>::const_iterator iter = datas.begin();
    for (iter; iter != datas.end(); ++iter)
    {
        this->addData(iter.value());
    }
}

void CBaseTableModel::addData(const STaskListPtr& data)
{
    if (m_bFilter && m_filterDate != textToDate(data->start_time)/*iter.value()->start_time.date()*/)
    {
        return;
    }

    int nRowIndex = rowCount();

    bool bFinish = data->task_status == TASK_STATUS_FINISH;
    QStandardItem* itemCheck = new QStandardItem("");
    itemCheck->setEditable(false);
    itemCheck->setCheckable(true);
    itemCheck->setCheckState((bFinish) ? Qt::Checked : Qt::Unchecked);
    if (bFinish)
    {
        itemCheck->setEnabled(false);
    }
    setItem(nRowIndex, E_TASKETTABLE_CHECK, itemCheck);

    QStandardItem* itemID = new QStandardItem(data->id);
    itemID->setEditable(false);
    setItem(nRowIndex, E_TASKETTABLE_ID, itemID);

    QStandardItem* itemName = new QStandardItem(data->name);
    itemName->setEditable(false);
    setItem(nRowIndex, E_TASKETTABLE_NAME, itemName);

    QStandardItem* itemStartTime = new QStandardItem((data->start_time)/*data->start_time.toString("yyyy-MM-dd hh:mm:ss")*/);
    itemStartTime->setEditable(false);
    //QDateTimeEdit* startTime = new QDateTimeEdit(data->start_time);
    //itemStartTime->setData(startTime, Qt::UserRole + 1);
    setItem(nRowIndex, E_TASKETTABLE_STARTTIME, itemStartTime);

    QStandardItem* itemEndTime = new QStandardItem((data->end_time)/*data->end_time.toString("yyyy-MM-dd hh:mm:ss")*/);
    itemEndTime->setEditable(false);
    setItem(nRowIndex, E_TASKETTABLE_ENDTIME, itemEndTime);

    QStandardItem* itemDuringTime = new QStandardItem(QString::number(data->during_time));
    itemDuringTime->setEditable(false);
    setItem(nRowIndex, E_TASKETTABLE_DURINGTIME, itemDuringTime);

    QStandardItem* itemCreateTime = new QStandardItem((data->create_time)/*data->create_time.toString("yyyy-MM-dd hh:mm:ss")*/);
    itemCreateTime->setEditable(false);
    setItem(nRowIndex, E_TASKETTABLE_CREATETIME, itemCreateTime);

    QStandardItem* itemImportLevel = new QStandardItem(CDataCenter::GetInstance()->getImportLevelShowContent(data->import_level));
    itemImportLevel->setEditable(false);
    setItem(nRowIndex, E_TASKETTABLE_IMPORTLEVEL, itemImportLevel);

    QStandardItem* itemValue = new QStandardItem(QString::number(data->value));
    itemValue->setEditable(false);
    setItem(nRowIndex, E_TASKETTABLE_VALUE, itemValue);

    QStandardItem* itemRepeatType = new QStandardItem(CDataCenter::GetInstance()->getRepeatTypeShowContent(data->repeat_type));
    itemRepeatType->setEditable(false);
    setItem(nRowIndex, E_TASKETTABLE_REPEATTYPE, itemRepeatType);

    QStandardItem* itemFinishTimes = new QStandardItem(QString::number(data->finish_times));
    itemFinishTimes->setEditable(false);
    setItem(nRowIndex, E_TASKETTABLE_FINISHTIMES, itemFinishTimes);

    QStandardItem* itemSumPoints = new QStandardItem(QString::number(data->sum_points));
    itemSumPoints->setEditable(false);
    setItem(nRowIndex, E_TASKETTABLE_SUMPOINTS, itemSumPoints);

    QStandardItem* itemBelongType = new QStandardItem(CDataCenter::GetInstance()->getBelongTypeShowContent(data->belong_type));
    itemBelongType->setEditable(false);
    setItem(nRowIndex, E_TASKETTABLE_BELONGTYPE, itemBelongType);

    QStandardItem* itemDeductionCheck = new QStandardItem("Deduction");
    itemDeductionCheck->setEditable(false);
    itemDeductionCheck->setCheckable(true);
    itemDeductionCheck->setEnabled(false);
    itemDeductionCheck->setCheckState((data->is_deduction)?Qt::Checked : Qt::Unchecked);
    setItem(nRowIndex, E_TASKETTABLE_DEDUCTION, itemDeductionCheck);

    QStandardItem* itemCountDownCheck = new QStandardItem("Count Down");
    itemCountDownCheck->setEditable(false);
    itemCountDownCheck->setCheckable(true);
    itemCountDownCheck->setEnabled(false);
    itemCountDownCheck->setCheckState((data->is_countdown) ? Qt::Checked : Qt::Unchecked);
    setItem(nRowIndex, E_TASKETTABLE_COUNTDOWN, itemCountDownCheck);

    QStandardItem* itemCanPauseCheck = new QStandardItem("Could Pause");
    itemCanPauseCheck->setEditable(false);
    itemCanPauseCheck->setCheckable(true);
    itemCanPauseCheck->setEnabled(false);
    itemCanPauseCheck->setCheckState((data->is_can_pause) ? Qt::Checked : Qt::Unchecked);
    setItem(nRowIndex, E_TASKETTABLE_CANPAUSE, itemCanPauseCheck);

    //操作
    QString operText = "完成 暂停 取消";
    QStandardItem* itemOperation = new QStandardItem(operText);
    itemOperation->setEditable(false);
    setItem(nRowIndex, E_TASKETTABLE_TASLOPERATOR, itemOperation);

    QStandardItem* itemTaskStatus = new QStandardItem(CDataCenter::GetInstance()->getStatusTypeShowContent(data->task_status));
    itemTaskStatus->setEditable(false);
    setItem(nRowIndex, E_TASKETTABLE_TASKSTATUS, itemTaskStatus);

    QStandardItem* itemFinishTime = new QStandardItem((data->finish_time)/*data->finish_time.toString("yyyy-MM-dd hh:mm:ss")*/);
    itemFinishTime->setEditable(false);
    setItem(nRowIndex, E_TASKETTABLE_FINISHTIME, itemFinishTime);

    QStandardItem* itemPauseTime = new QStandardItem((data->pause_time)/*data->pause_time.toString("yyyy-MM-dd hh:mm:ss")*/);
    itemPauseTime->setEditable(false);
    setItem(nRowIndex, E_TASKETTABLE_PAUSETIME, itemPauseTime);

    QStandardItem* itemParentId = new QStandardItem(data->parent_task);
    itemParentId->setEditable(false);
    setItem(nRowIndex, E_TASKETTABLE_PARENTTASK, itemParentId);

    QStandardItem* itemRemark = new QStandardItem(data->remark);
    itemRemark->setEditable(false);
    setItem(nRowIndex, E_TASKETTABLE_REMARK, itemRemark);

}

void CBaseTableModel::setCurrentDate(const QDate& date)
{
    m_filterDate = date;
}

void CBaseTableModel::clearAllData()
{
    const int nRow = rowCount();
    this->removeRows(0, nRow);
}
