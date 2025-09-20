#include "addTaskDialog.h"

#include <QString>
#include <QLabel>
#include <QMap>
#include <QVariant>
#include <QDateTime>

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QMessageBox>

CAddTaskDialog::CAddTaskDialog(const STaskListPtr& task /* = nullptr */,QWidget *parent /* = nullptr */)
    : QDialog(parent)
{
    m_mapTasks.clear();
    createUi(task);
}

CAddTaskDialog::~CAddTaskDialog()
{

}

void CAddTaskDialog::createUi(const STaskListPtr& task /* = nullptr */)
{
    //QString table_taskList_id = taskOrder
    QVBoxLayout* pMainLayout = new QVBoxLayout(this);

    {
        QHBoxLayout* pNameLayout = new QHBoxLayout();
        
        QLabel* pNameLabel = new QLabel("taskName:");
        pNameLayout->addWidget(pNameLabel);

        m_pTaskNmaeEdit = new QLineEdit();
        m_pTaskNmaeEdit->setText((task)?(task->name):"");
        pNameLayout->addWidget(m_pTaskNmaeEdit);

        pMainLayout->addLayout(pNameLayout);
    }

    {
        QLabel* pTimeLabel = new QLabel("Task Time Setting: begin -> end:");        

        QHBoxLayout* pTimeSettingLayout = new QHBoxLayout();
        if(task)
        {
            m_pBeignDate = new QDateTimeEdit(textToDateTime(task->start_time));
            m_pEndData = new QDateTimeEdit(textToDateTime(task->end_time).addSecs(10));
        }
        else
        {
            m_pBeignDate = new QDateTimeEdit(QDateTime::currentDateTime());
            m_pEndData = new QDateTimeEdit(QDateTime::currentDateTime().addSecs(10));
        }
        QLabel* pTimeDurLabel = new QLabel("   ->   ");
        pTimeSettingLayout->addWidget(m_pBeignDate);
        pTimeSettingLayout->addWidget(pTimeDurLabel);
        pTimeSettingLayout->addWidget(m_pEndData);

        pMainLayout->addWidget(pTimeLabel);
        pMainLayout->addLayout(pTimeSettingLayout);
        //table_taskList_during_time = during //持续时间(结束时间,单位:s)
    }

    {
        QHBoxLayout* pImportLayout = new QHBoxLayout();

        QLabel* pTimeLabel = new QLabel("ImportLevel:");
        pImportLayout->addWidget(pTimeLabel);

        //m_mapImportType = importMap;
        //QMap放在这里会在下面的循环中产生崩溃，因此qm的=操作可能是一种自我删除的数据拷贝
        QMap<QString, SImportLevelDictionary> importMap = CDataCenter::GetInstance()->getImportLevelDictionar();
        QMap<QString, SImportLevelDictionary>::const_iterator iter = importMap.begin();
        
        m_pImprotLevelCom = new QComboBox();
        for (iter; iter != importMap.end(); ++iter)
        {
            m_pImprotLevelCom->addItem(iter->shouw_text, QVariant(iter->level_id));
        }
        QString importLevel = (task) ? task->import_level : m_pImprotLevelCom->currentData().toString();
        m_pImprotLevelCom->setCurrentText(CDataCenter::GetInstance()->getImportLevelShowContent(importLevel));
        pImportLayout->addWidget(m_pImprotLevelCom);        
        connect(m_pImprotLevelCom, SIGNAL(currentIndexChanged(int)), this, SLOT(slot_importTypeChange(int)));
        pMainLayout->addLayout(pImportLayout);

        //
        QHBoxLayout* pTaskValueLayout = new QHBoxLayout();

        QLabel* pTaskValueLabel = new QLabel("TaskValue:");
        pTaskValueLayout->addWidget(pTaskValueLabel);

        m_pTaskValueEdit = new QLineEdit();
        m_pTaskValueEdit->setEnabled(false);
        QString levelText = m_pImprotLevelCom->currentData().toString();
        m_pTaskValueEdit->setText(QString::number(importMap[levelText].default_value));
        pTaskValueLayout->addWidget(m_pTaskValueEdit);

        m_mapImportType = importMap;
        pMainLayout->addLayout(pTaskValueLayout);
    }
    //
    {
        QHBoxLayout* pRepeatLayout = new QHBoxLayout();

        QLabel* pRepeatLabel = new QLabel("Repeat Type:");
        pRepeatLayout->addWidget(pRepeatLabel);

        QMap<QString, SRepeatTypeDictionary> repeatTypeMap = CDataCenter::GetInstance()->getRepeatTypeDictionar();
        QMap<QString, SRepeatTypeDictionary>::const_iterator iter = repeatTypeMap.begin();
        m_pRepeatTypeCom = new QComboBox();
        for (iter; iter != repeatTypeMap.end(); ++iter)
        {
            m_pRepeatTypeCom->addItem(iter->show_content, QVariant(iter->repeat_type));
        }
        QString repeatType = (task) ? task->repeat_type : m_pRepeatTypeCom->currentData().toString();
        m_pRepeatTypeCom->setCurrentText(CDataCenter::GetInstance()->getRepeatTypeShowContent(repeatType));
        pRepeatLayout->addWidget(m_pRepeatTypeCom);
        connect(m_pRepeatTypeCom, SIGNAL(currentIndexChanged(int)), this, SLOT(slot_repeatTypeChange(int)));
        pMainLayout->addLayout(pRepeatLayout);
    }

    {
        QHBoxLayout* pBelongLayout = new QHBoxLayout();

        QLabel* pBelongLabel = new QLabel("Belong Type:");
        pBelongLayout->addWidget(pBelongLabel);

        QMap<QString, SBelongTypeDictionary> belongTypeMap = CDataCenter::GetInstance()->getBelongTypeDictionary();
        QMap<QString, SBelongTypeDictionary>::const_iterator iter = belongTypeMap.begin();
        m_pBelongTypeCom = new QComboBox();
        for (iter; iter != belongTypeMap.end(); ++iter)
        {
            m_pBelongTypeCom->addItem(iter->show_content, QVariant(iter->belong_id));
        }
        QString belongType = (task) ? task->belong_type : m_pBelongTypeCom->currentData().toString();
        m_pBelongTypeCom->setCurrentText(CDataCenter::GetInstance()->getBelongTypeShowContent(belongType));
        pBelongLayout->addWidget(m_pBelongTypeCom);       
        pMainLayout->addLayout(pBelongLayout);
    }

    {
        QHBoxLayout* pCheckLayout = new QHBoxLayout();
        //是否进行分值的减去，任务失败扣除累计分值
        m_pDeductionCheck = new QCheckBox("deduction");
        m_pDeductionCheck->setChecked((task) ? (task->is_deduction) : false);
        pCheckLayout->addWidget(m_pDeductionCheck);
        //是否进行倒计时提醒，
        m_pCountdownCheck = new QCheckBox("countdown");
        m_pCountdownCheck->setChecked((task) ? (task->is_countdown) : false);
        pCheckLayout->addWidget(m_pCountdownCheck);
        //是否能够进行暂停，如果false，则断掉后，直接判断为失败
        m_pCanPauseCheck = new QCheckBox("couldPause");
        m_pCanPauseCheck->setChecked((task) ? (task->is_can_pause) : false);
        pCheckLayout->addWidget(m_pCanPauseCheck);

        pMainLayout->addLayout(pCheckLayout);
    }

    {
        QHBoxLayout* pStatusLayout = new QHBoxLayout();

        QLabel* pStatusLabel = new QLabel("Status Type:");
        pStatusLayout->addWidget(pStatusLabel);

        QMap<QString, SStatusTypeDictionary> statusTypeMap = CDataCenter::GetInstance()->getStatusLevelDictionar();
        QMap<QString, SStatusTypeDictionary>::const_iterator iter = statusTypeMap.begin();
        m_pStatusTypeCom = new QComboBox();
        for (iter; iter != statusTypeMap.end(); ++iter)
        {
            m_pStatusTypeCom->addItem(iter->show_content, QVariant(iter->task_status));
//             if(iter->task_status == QString("task_status_undetermined") || iter->task_status == QString("task_status_inProgress"))
//             {
//                 
//             }
        }
        //m_pStatusTypeCom->setCurrentIndex(statusTypeMap.size()-1);
        QString taskStatus = (task) ? task->task_status : TASK_STATUS_UNDETERMINED;
        m_pStatusTypeCom->setCurrentText(CDataCenter::GetInstance()->getStatusTypeShowContent(taskStatus));
        pStatusLayout->addWidget(m_pStatusTypeCom);

        pMainLayout->addLayout(pStatusLayout);
    }
    //table_taskList_parent_task = parentTask  目前根据重复类型不断的产生相关子任务，先不提供自选功能

    {
        QHBoxLayout* pRemarkLayout = new QHBoxLayout();

        QLabel* pRemarkLabel = new QLabel("Remark:");
        pRemarkLayout->addWidget(pRemarkLabel);

        m_pRemarkEdit = new QLineEdit();
        m_pRemarkEdit->setFixedHeight(100);
        m_pRemarkEdit->setText((task) ? (task->remark) : "");
        pRemarkLayout->addWidget(m_pRemarkEdit);

        pMainLayout->addLayout(pRemarkLayout);
    }

    {
        m_pTableModel = new CBaseTableModel(false,this);
        m_pTableView = new QTableView();
        m_pTableView->setModel(m_pTableModel);
        m_pTableView->resizeColumnsToContents();
        m_pTableView->hideColumn(E_TASKETTABLE_CHECK);
        m_pTableView->hideColumn(E_TASKETTABLE_NAME);
        m_pTableView->hideColumn(E_TASKETTABLE_ID);
        m_pTableView->hideColumn(E_TASKETTABLE_IMPORTLEVEL);
        m_pTableView->hideColumn(E_TASKETTABLE_BELONGTYPE);
        m_pTableView->hideColumn(E_TASKETTABLE_DURINGTIME);
        m_pTableView->hideColumn(E_TASKETTABLE_VALUE);
        m_pTableView->hideColumn(E_TASKETTABLE_FINISHTIMES);
        m_pTableView->hideColumn(E_TASKETTABLE_SUMPOINTS);
        m_pTableView->hideColumn(E_TASKETTABLE_FINISHTIME);
        m_pTableView->hideColumn(E_TASKETTABLE_PAUSETIME);
        m_pTableView->hideColumn(E_TASKETTABLE_PARENTTASK);
        m_pTableView->hideColumn(E_TASKETTABLE_TASLOPERATOR);
        m_pTableView->hideColumn(E_TASKETTABLE_DEDUCTION);
        m_pTableView->hideColumn(E_TASKETTABLE_COUNTDOWN);
        m_pTableView->hideColumn(E_TASKETTABLE_CANPAUSE);
        m_pTableView->hideColumn(E_TASKETTABLE_TASKSTATUS);
        m_pTableView->hideColumn(E_TASKETTABLE_REMARK);
        m_pTableView->setModel(m_pTableModel);
        pMainLayout->addWidget(m_pTableView);
    }

    {
        QHBoxLayout* pOperBtnLayout = new QHBoxLayout();
        QPushButton* pConfirmBtn = new QPushButton("Confirm");
        connect(pConfirmBtn, &QPushButton::clicked, this, &CAddTaskDialog::on_confirmBen_onClick);

        QPushButton* pCancelBtn = new QPushButton("Cancel");
        connect(pCancelBtn, &QPushButton::clicked, this, &CAddTaskDialog::on_cancelBen_onClick);
        pOperBtnLayout->addWidget(pConfirmBtn);
        pOperBtnLayout->addWidget(pCancelBtn);

        pMainLayout->addLayout(pOperBtnLayout);
    }
}

void CAddTaskDialog::on_confirmBen_onClick()
{
    if (!isContentAllRight())
    {
        return;
    }
    accept();
}

void CAddTaskDialog::on_cancelBen_onClick()
{
    reject();
}

void CAddTaskDialog::slot_importTypeChange(int index)
{
    QString importId = m_pImprotLevelCom->currentData().toString();
    m_pTaskValueEdit->setText(QString::number(m_mapImportType[importId].default_value));
}

void CAddTaskDialog::slot_repeatTypeChange(int index)
{
    QString repeatId = m_pRepeatTypeCom->currentData().toString();
    m_pTableModel->clearAllData();
    m_mapTasks.clear();
    if (repeatId == REPEAT_TYPE_ALWAYS/*"repeat_type_always"*/)
    {
        m_pEndData->setDateTime(DATETIME_MAX);
        m_pEndData->setEnabled(false);        
        updateTableData(REPEAT_TYPE_ALWAYS);
    }
    else if(repeatId == REPEAT_TYPE_FORGETTING_CURVE/*"repeat_type_forgetting_curve"*/)
    {
        m_pEndData->setDateTime(m_pBeignDate->dateTime().addDays(15));
        m_pEndData->setEnabled(false);

        QDate date = m_pBeignDate->date();
        QDateTime dateTime = QDateTime(date, QTime(23, 59, 59));
        m_pEndData->setDateTime(dateTime);
        updateTableData(REPEAT_TYPE_FORGETTING_CURVE);
    }
//     else if (repeatId == REPEAT_TYPE_EVERYDAY/*"repeat_type_everyday"*/)
//     {
//         m_pEndData->setDateTime(m_pBeignDate->dateTime().addSecs(10));
//         m_pEndData->setEnabled(true);
//     }
    else//repeat_type_once REPEAT_TYPE_ONCE
    {
        m_pEndData->setDateTime(m_pBeignDate->dateTime().addSecs(10));
        m_pEndData->setEnabled(true);
    }
}

void CAddTaskDialog::updateTableData(const QString& repeatType)
{
    if (repeatType == REPEAT_TYPE_FORGETTING_CURVE)
    {
        QDateTime begin = m_pBeignDate->dateTime();
        QDateTime end = m_pEndData->dateTime();
        if (begin > end)
        {
            return;
        }
        //开始时间 
        STaskListPtr task = STaskListPtr(new STaskList());
        makeTaskInformation(task);
        //mapData[QString("1")] = task;
        m_mapTasks[task->id] = task;
        //30分钟-由于操作的不确定性，暂时不提供30分钟、12小时的间隔学习
//         STaskListPtr task30 = task;
//         QDateTime nextStartTime = end.addSecs(30 * 60);
//         task30->start_time = dateTimeToShowText(nextStartTime);
//         end = nextStartTime.addSecs(task->during_time);
//         task30->end_time = dateTimeToShowText(end);
//         mapData[QString("2")] = task30;
        //12小时
//         STaskListPtr task12 = task30;
//         nextStartTime = end.addSecs((12*24*60));
//         task30->start_time = dateTimeToShowText(nextStartTime);
//         end = nextTime.addSecs(30 * 60);
//         task30->end_time = dateTimeToShowText(nextTime);
//         mapData[QString("2")] = task30;
 
        //1天
        STaskListPtr task1 = STaskListPtr(new STaskList());
        makeTaskInformation(task1,task->id);
        //task1 = task;
        QDateTime nextStartTime = begin.addDays(1);
        task1->start_time = dateTimeToShowText(nextStartTime);
        end = nextStartTime.addSecs(task->during_time);
        task1->end_time = dateTimeToShowText(end);
        //mapData[QString("2")] = task1;
        m_mapTasks[task1->id] = task1;
        //2天
        STaskListPtr task2 = STaskListPtr(new STaskList());
        makeTaskInformation(task2, task->id);
        nextStartTime = begin.addDays(2);
        task2->start_time = dateTimeToShowText(nextStartTime);
        end = nextStartTime.addSecs(task->during_time);
        task2->end_time = dateTimeToShowText(end);
        //mapData[QString("3")] = task2;
        m_mapTasks[task2->id] = task2;
        //4天
        STaskListPtr task4 = STaskListPtr(new STaskList());
//         task4 = task;
        makeTaskInformation(task4, task->id);
        nextStartTime = begin.addDays(4);
        task4->start_time = dateTimeToShowText(nextStartTime);
        end = nextStartTime.addSecs(task->during_time);
        task4->end_time = dateTimeToShowText(end);
        //mapData[QString("4")] = task4;
        m_mapTasks[task4->id] = task4;
        //7天
        STaskListPtr task7 = STaskListPtr(new STaskList());
//         task7 = task;
        makeTaskInformation(task7, task->id);
        nextStartTime = begin.addDays(7);
        task7->start_time = dateTimeToShowText(nextStartTime);
        end = nextStartTime.addSecs(task->during_time);
        task7->end_time = dateTimeToShowText(end);
        //mapData[QString("5")] = task7;
        m_mapTasks[task7->id] = task7;
        //15天
        STaskListPtr task15 = STaskListPtr(new STaskList());
//         task15 = task;
        makeTaskInformation(task15, task->id);
        nextStartTime = begin.addDays(15);
        task15->start_time = dateTimeToShowText(nextStartTime);
        end = nextStartTime.addSecs(task->during_time);
        task15->end_time = dateTimeToShowText(end);
       // mapData[QString("6")] = task15;
        m_mapTasks[task15->id] = task15;

        m_pTableModel->addDatas(m_mapTasks);
        m_pTableView->resizeColumnsToContents();
    }
}

void CAddTaskDialog::makeTaskInformation(STaskListPtr& task,const QString& parent /* = "" */)
{
    if (m_pTaskNmaeEdit->text().isEmpty())
    {
        return ;
    }
    task->id = getNewTaskId(parent,m_mapTasks.size());
    task->name = m_pTaskNmaeEdit->text();
    task->start_time = dateTimeToShowText(m_pBeignDate->dateTime());
    task->end_time = dateTimeToShowText(m_pEndData->dateTime());
    task->create_time = task->start_time;
    task->during_time = get2DateTimeTextDiff(task->start_time, task->end_time);//task->start_time.secsTo(task->end_time);//task->end_time.secsTo(task->start_time);
    task->import_level = m_pImprotLevelCom->currentData().toString();
    task->value = m_pTaskValueEdit->text().toInt();
    task->repeat_type = m_pRepeatTypeCom->currentData().toString();
    task->belong_type = m_pBelongTypeCom->currentData().toString();
    task->is_deduction = m_pDeductionCheck->isChecked();
    task->is_countdown = m_pCountdownCheck->isChecked();
    task->is_can_pause = m_pCanPauseCheck->isChecked();
    task->task_status = m_pStatusTypeCom->currentData().toString();
    task->remark = m_pRemarkEdit->text();
    task->parent_task = parent;
    //QString     parent_task;         //父任务id
}

QMap<QString, STaskListPtr> CAddTaskDialog::getTaskInformation()
{
    //makeTaskInformation(task);
    if (m_pRepeatTypeCom->currentData().toString() != REPEAT_TYPE_FORGETTING_CURVE)
    {
        STaskListPtr task = STaskListPtr(new STaskList());
        makeTaskInformation(task);
        m_mapTasks[task->id] = task;
    }
    return m_mapTasks;
}

bool CAddTaskDialog::isContentAllRight()
{
    if (m_pTaskNmaeEdit->text().isEmpty())
    {
        QMessageBox::question(nullptr, "Title", "Name is can not be empty!");
        return false;
    }

    QDateTime begin = m_pBeignDate->dateTime();
    QDateTime end = m_pEndData->dateTime();
    if (begin > end)
    {
        QMessageBox::question(nullptr, "Title", "End time should later then begin time!");
        return false;
    }
    return true;
}

