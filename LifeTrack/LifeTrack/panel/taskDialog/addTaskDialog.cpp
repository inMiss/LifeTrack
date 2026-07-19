#include "addTaskDialog.h"
#include "../management/structCommon.h"
#include "../language/language.h"
#include "../common/commonFunc.h"

#include <QString>
#include <QMap>
#include <QVariant>
#include <QDateTime>

#include <QVBoxLayout>
#include <QPushButton>
#include <QMessageBox>

CAddTaskDialog::CAddTaskDialog(const STaskListPtr& task /* = nullptr */, const QString& activeType /* = QString(TASK_ACTIVETYPE_NO_ACTIVE) */, const bool bEdit /* = false */, const bool bUnStrat /* = false */, QWidget* parent /* = nullptr */)
    : QDialog(parent)
    , m_pTaskNmaeEdit(nullptr)
    , m_pStartDateLabel(nullptr)
    , m_pStartDate(nullptr)
    , m_pStartDateLayout(nullptr)
    , m_pDeadLineLabel(nullptr)
    , m_pDeadLine(nullptr)
    , m_pDeadLineLayout(nullptr)
    , m_pImprotLevelCom(nullptr)
    , m_pTaskValueEdit(nullptr)
    , m_pRepeatTypeCom(nullptr)
    , m_pBelongTypeCom(nullptr)
    , m_pDeductionCheck(nullptr)
    , m_pCountdownCheck(nullptr)
    , m_pCanPauseCheck(nullptr)
    , m_pStatusTypeCom(nullptr)
    , m_pRemarkEdit(nullptr)
    , m_pFailThresholdEdit(nullptr)
    , m_bEdit(bEdit)
    , m_qsActiveType(activeType)
    , m_bUnStartTask(bUnStrat)
{
    m_tasks = task;
    createUi(task);
}

CAddTaskDialog::~CAddTaskDialog()
{

}

void CAddTaskDialog::createUi(const STaskListPtr& task /* = nullptr */)
{
    QVBoxLayout* pMainLayout = new QVBoxLayout(this);
    //任务名称
    {
        QHBoxLayout* pNameLayout = new QHBoxLayout();
        QString qsTemp = getLanguegeShowText("task_attribute_name") + ": ";
        QLabel* pNameLabel = new QLabel(qsTemp);
        pNameLayout->addWidget(pNameLabel);

        m_pTaskNmaeEdit = new QLineEdit();
        m_pTaskNmaeEdit->setText((task)?(task->name):"");
        pNameLayout->addWidget(m_pTaskNmaeEdit);

        pMainLayout->addLayout(pNameLayout);
    }

    //重复类型
    {
        QHBoxLayout* pRepeatLayout = new QHBoxLayout();

        QString qsTemp = getLanguegeShowText("task_attribute_repeat_type") + ": ";
        QLabel* pRepeatLabel = new QLabel(qsTemp);
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

    //计划开始日期和计划截止日期，有些类型不需要开始和结束日期
    {
        QHBoxLayout* pTimeSettingLayout = new QHBoxLayout();

        QString qsTemp1 = getLanguegeShowText("task_attribute_start_date") + ": ";
        QString qsTemp2 = getLanguegeShowText("task_attribute_deadline") + ": ";

        m_pStartDateLabel = new QLabel(qsTemp1);
        m_pDeadLineLabel = new QLabel(qsTemp2);

        if (task)
        {
            m_pStartDate = new QDateEdit(textToDate(task->start_time));
            m_pDeadLine = new QDateEdit(textToDate(task->deadline));
        }
        else
        {
            m_pStartDate = new QDateEdit(QDate::currentDate());
            m_pDeadLine = new QDateEdit(QDate::currentDate().addMonths(1));
        }

        // 设置日历弹出功能
        m_pStartDate->setCalendarPopup(true);  // 关键：启用日历弹出
        m_pDeadLine->setCalendarPopup(true);  // 关键：启用日历弹出

        // 设置显示格式
        m_pStartDate->setDisplayFormat("yyyy-MM-dd"); 
        m_pDeadLine->setDisplayFormat("yyyy-MM-dd");

        m_pStartDateLayout = new QHBoxLayout();
        {
            m_pStartDateLayout->addWidget(m_pStartDateLabel);
            m_pStartDateLayout->addWidget(m_pStartDate);
        }

        m_pDeadLineLayout = new QHBoxLayout();
        {
            m_pDeadLineLayout->addWidget(m_pDeadLineLabel);
            m_pDeadLineLayout->addWidget(m_pDeadLine);
        }

        pTimeSettingLayout->addLayout(m_pStartDateLayout);
        pTimeSettingLayout->addLayout(m_pDeadLineLayout);
        updateDateUiShow();
     
        pMainLayout->addLayout(pTimeSettingLayout);
    }


    {
        QHBoxLayout* pImportLayout = new QHBoxLayout();

        QString qsTemp1 = getLanguegeShowText("task_attribute_import_level") + ": ";
        QString qsTemp2 = getLanguegeShowText("task_attribute_value") + ": ";
        QLabel* pTimeLabel = new QLabel(qsTemp1);
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

        //
        QLabel* pTaskValueLabel = new QLabel(qsTemp2);
        pImportLayout->addWidget(pTaskValueLabel);

        m_pTaskValueEdit = new QLineEdit();
        m_pTaskValueEdit->setEnabled(false);
        QString levelText = m_pImprotLevelCom->currentData().toString();
        m_pTaskValueEdit->setText(QString::number(importMap[levelText].default_value));
        pImportLayout->addWidget(m_pTaskValueEdit);

        m_mapImportType = importMap;
        pMainLayout->addLayout(pImportLayout);
    }

    {
        QHBoxLayout* pBelongLayout = new QHBoxLayout();

        QString qsTemp1 = getLanguegeShowText("task_attribute_belong_type") + ": ";
 
        QLabel* pBelongLabel = new QLabel(qsTemp1);
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
        QString qsTemp1 = getLanguegeShowText("task_attribute_is_deduction");
        QString qsTemp2 = getLanguegeShowText("task_attribute_is_countdown");
        QString qsTemp3 = getLanguegeShowText("task_attribute_is_can_pause");

        QHBoxLayout* pCheckLayout = new QHBoxLayout();
        //是否进行分值的减去，任务失败扣除累计分值
        m_pDeductionCheck = new QCheckBox(qsTemp1);
        m_pDeductionCheck->setChecked((task) ? (task->is_deduction) : false);
        pCheckLayout->addWidget(m_pDeductionCheck);
        //是否进行倒计时提醒，
        m_pCountdownCheck = new QCheckBox(qsTemp2);
        m_pCountdownCheck->setChecked((task) ? (task->is_countdown) : false);
        pCheckLayout->addWidget(m_pCountdownCheck);
        //是否能够进行暂停，如果false，则断掉后，直接判断为失败
        m_pCanPauseCheck = new QCheckBox(qsTemp3);
        m_pCanPauseCheck->setChecked((task) ? (task->is_can_pause) : true);
        pCheckLayout->addWidget(m_pCanPauseCheck);

        pMainLayout->addLayout(pCheckLayout);
    }

    //最大失败次数
    {
        QHBoxLayout* pFailThresholdLayout = new QHBoxLayout();
        QString qsTemp = getLanguegeShowText("task_attribute_fail_threshold") + ": ";
        QLabel* pFailThresholdLabel = new QLabel(qsTemp);
        pFailThresholdLayout->addWidget(pFailThresholdLabel);

        m_pFailThresholdEdit = new QLineEdit();
        m_pFailThresholdEdit->setText((task) ? (QString::number(task->fail_threshold)) : "3");
        pFailThresholdLayout->addWidget(m_pFailThresholdEdit);

        pMainLayout->addLayout(pFailThresholdLayout);
    }

    {
        QHBoxLayout* pStatusLayout = new QHBoxLayout();

        QString qsTemp = getLanguegeShowText("task_attribute_task_status") + ": ";
        QLabel* pStatusLabel = new QLabel(qsTemp);
        pStatusLayout->addWidget(pStatusLabel);

        QMap<QString, SStatusTypeDictionary> statusTypeMap = CDataCenter::GetInstance()->getStatusLevelDictionar();
        QMap<QString, SStatusTypeDictionary>::const_iterator iter = statusTypeMap.begin();
        m_pStatusTypeCom = new QComboBox();
        for (iter; iter != statusTypeMap.end(); ++iter)
        {
            m_pStatusTypeCom->addItem(iter->show_content, QVariant(iter->task_status));
        }
        QString taskStatus = (task) ? task->task_status : TASK_STATUS_UNSTART;
        m_pStatusTypeCom->setCurrentText(CDataCenter::GetInstance()->getStatusTypeShowContent(taskStatus));
        m_pStatusTypeCom->setEnabled(false);
        pStatusLayout->addWidget(m_pStatusTypeCom);

        pMainLayout->addLayout(pStatusLayout);
    }
    
    //提醒语句
    {
        QHBoxLayout* pStartTaskWarnLayout = new QHBoxLayout();
        QString qsTemp = getLanguegeShowText("task_start_warning") + ": ";
        QLabel* pStartTaskWarnLabel = new QLabel(qsTemp);
        pStartTaskWarnLabel->setStyleSheet("color: #FF0000;");
        pStartTaskWarnLayout->addWidget(pStartTaskWarnLabel);
        pMainLayout->addLayout(pStartTaskWarnLayout);
    }

    {
        QHBoxLayout* pRemarkLayout = new QHBoxLayout();
        QString qsTemp = getLanguegeShowText("task_attribute_remark") + ": ";
        QLabel* pRemarkLabel = new QLabel(qsTemp);
        pRemarkLayout->addWidget(pRemarkLabel);

        m_pRemarkEdit = new QLineEdit();
        m_pRemarkEdit->setFixedHeight(100);
        m_pRemarkEdit->setText((task) ? (task->remark) : "");
        pRemarkLayout->addWidget(m_pRemarkEdit);

        pMainLayout->addLayout(pRemarkLayout);
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

    updateUiEnableStatus();
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
    updateDateUiShow();
}

void CAddTaskDialog::makeTaskInformation(STaskListPtr& task,const QString& parent /* = "" */)
{
    if (m_pTaskNmaeEdit->text().isEmpty() || m_pTaskNmaeEdit->text() == QString(""))
    {
        return ;
    }

    if (nullptr == m_tasks)
    {
        m_tasks = QSharedPointer<STaskList>::create();
    }

    task = m_tasks;

    task->id = getNewTaskId(parent,0);
    task->name = m_pTaskNmaeEdit->text();
    task->start_date = dateToShowText(m_pStartDate->date());
    task->deadline = dateToShowText(m_pDeadLine->date());

    task->create_time = dateTimeToShowText(QDateTime::currentDateTime());

    task->import_level = m_pImprotLevelCom->currentData().toString();
    task->value = m_pTaskValueEdit->text().toInt();
    task->repeat_type = m_pRepeatTypeCom->currentData().toString();
    task->belong_type = m_pBelongTypeCom->currentData().toString();
    task->is_deduction = m_pDeductionCheck->isChecked();
    task->is_countdown = m_pCountdownCheck->isChecked();
    task->is_can_pause = m_pCanPauseCheck->isChecked();
    task->fail_threshold = m_pFailThresholdEdit->text().toInt();
    task->task_status = m_pStatusTypeCom->currentData().toString();
    task->remark = m_pRemarkEdit->text();
    task->parent_task = parent;
}

void CAddTaskDialog::getTaskInformation(STaskListPtr& task, const QString& parent /* = "" */)
{
    makeTaskInformation(task,"");
}

bool CAddTaskDialog::isContentAllRight()
{
    if (m_pTaskNmaeEdit->text().isEmpty())
    {
        QMessageBox::question(nullptr, "Title", "Name is can not be empty!");
        return false;
    }

    QDate begin = m_pStartDate->date();
    QDate end = m_pDeadLine->date();
    if (begin > end)
    {
        QMessageBox::question(nullptr, "Title", "End time should later then begin time!");
        return false;
    }
    return true;
}

void CAddTaskDialog::updateDateUiShow()
{
    if (nullptr == m_pRepeatTypeCom)
    {
        return;
    }

    QString repeatId = m_pRepeatTypeCom->currentData().toString();
    if (REPEAT_TYPE_FORGETTING_CURVE == repeatId)
    {
        m_pDeadLineLabel->hide();
        m_pDeadLine->hide();
        m_pDeadLine->setEnabled(true);
    }
    else if(REPEAT_TYPE_ALWAYS == repeatId)
    {
        m_pDeadLineLabel->show();
        m_pDeadLine->show();

        m_pDeadLine->setEnabled(false);
    }
    else
    {
        m_pDeadLine->setEnabled(true);
        m_pDeadLineLabel->show();
        m_pDeadLine->show();
    }
   
}

void CAddTaskDialog::updateUiEnableStatus()
{
    if (!m_bEdit || m_bUnStartTask)
        return;

    if (TASK_ACTIVETYPE_NO_ACTIVE == m_qsActiveType)
    {
        m_pRepeatTypeCom->setEnabled(false);
        m_pStartDate->setEnabled(false);
        m_pDeadLine->setEnabled(false);
        m_pImprotLevelCom->setEnabled(false);
        m_pBelongTypeCom->setEnabled(false);

        m_pDeductionCheck->setEnabled(false);
        m_pCountdownCheck->setEnabled(false);
        m_pCanPauseCheck->setEnabled(false);
    }
    else if (TASK_ACTIVETYPE_ACTIVE == m_qsActiveType)
    {
        m_pRepeatTypeCom->setEnabled(false);
        m_pStartDate->setEnabled(false);
        m_pDeadLine->setEnabled(false);
        m_pImprotLevelCom->setEnabled(false);
        m_pBelongTypeCom->setEnabled(false);

        m_pDeductionCheck->setEnabled(false);
        m_pCountdownCheck->setEnabled(false);
        m_pCanPauseCheck->setEnabled(false);
    }
    else if (TASK_ACTIVETYPE_FINISHED == m_qsActiveType)
    {
        m_pRepeatTypeCom->setEnabled(false);
        m_pStartDate->setEnabled(false);
        m_pDeadLine->setEnabled(false);
        m_pImprotLevelCom->setEnabled(false);
        m_pBelongTypeCom->setEnabled(false);

        m_pDeductionCheck->setEnabled(false);
        m_pCountdownCheck->setEnabled(false);
        m_pCanPauseCheck->setEnabled(false);
    }
}

