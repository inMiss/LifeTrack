#include "mainPanel.h"

#include <QHBoxLayout>
#include <QVBoxLayout>
#include "../taskDialog/addTaskDialog.h"
#include "../panel/MessageBox.h"
#include "../language/language.h"
#include "../common/commonFunc.h"
#include "../taskDialog/inputReason.h"
CMainPanel::CMainPanel(QWidget *parent)
    : QWidget(parent)
    , m_pUnActivePage(nullptr)
    , m_pActivePage(nullptr)
    , m_pAddTaskBtn(nullptr)
    , m_tempAddDiaTaskPtr(QSharedPointer<STaskList>::create())
{
    createUi();
}

void CMainPanel::createUi()
{
    QVBoxLayout* pMainLayout = new QVBoxLayout(this);

    m_pAddTaskBtn = new QPushButton("Add Task");
    m_pAddTaskBtn->setObjectName("m_pAddTaskBtn");
    connect(m_pAddTaskBtn, &QPushButton::clicked, this, &CMainPanel::addTaskBtn_click);
    pMainLayout->addWidget(m_pAddTaskBtn);
    //活跃中的任务
    m_pActivePage =  new CActiveTaskPage(this);
    connect(m_pActivePage, &CActiveTaskPage::sigTaskOperation, this, &CMainPanel::slotTaskOperation);
    //未活跃的任务
    m_pUnActivePage = new CUnActiveTaskPage(this);
    connect(m_pUnActivePage, &CUnActiveTaskPage::sigTaskOperation, this, &CMainPanel::slotTaskOperation);

    pMainLayout->addWidget(m_pActivePage);
    pMainLayout->addWidget(m_pUnActivePage);
}

CMainPanel::~CMainPanel()
{}
void CMainPanel::slotTaskOperation_outerUse(const QString& task_id, const int nRow, const QString& taskActiveType, const QString& taskOperType)
{
    slotTaskOperation(task_id, nRow, taskActiveType, taskOperType);
}

void CMainPanel::slotTaskOperation(const QString& task_id, const int nRow, const QString& taskActiveType, const QString& taskOperType)
{
    if (TASK_ACTIVETYPE_NO_ACTIVE == taskActiveType)
    {
        if(TASK_OPERATE_TYPE_START == taskOperType)
        {
            // 警告提示
            if (CMessageBox::warning(getLanguegeShowText("task_start_warning")))
            {
                CDataCenter::GetInstance()->setTaskStatus(task_id, TASK_STATUS_INPROGRESS);
                CDataCenter::GetInstance()->setTaskStartTime(task_id);

                CDataCenter::GetInstance()->updateActiveTableTask(task_id);

                CDataCenter::GetInstance()->addOrUpdateStatusChangeBytaskId(task_id, TASK_ACTIVETYPE_ACTIVE, getLanguegeShowText("reason_task_start"));
                CDataCenter::GetInstance()->addOrUpdateDailyHistoryBytaskId(task_id, TASK_ACTIVETYPE_ACTIVE, getLanguegeShowText("remark_task_start"));
                m_pActivePage->initTableData();
                m_pUnActivePage->initTableData();
            }
        }
        else if (TASK_OPERATE_TYPE_RESTART == taskOperType)
        {
            //如果这个任务不是记忆曲线
            if(REPEAT_TYPE_FORGETTING_CURVE != CDataCenter::GetInstance()->getTaskListRepeatTypeById(task_id))
            {
                CDataCenter::GetInstance()->setTaskStatus(task_id, TASK_STATUS_RESTART);
                //更新修改数据
                CDataCenter::GetInstance()->updateActiveTableTask(task_id);
                //由于是重新开始，应该需要添加日常任务
                CDataCenter::GetInstance()->addOrUpdateStatusChangeBytaskId(task_id, TASK_ACTIVETYPE_ACTIVE, getLanguegeShowText("reason_task_restart"));
                CDataCenter::GetInstance()->addOrUpdateDailyHistoryBytaskId(task_id, TASK_ACTIVETYPE_ACTIVE, getLanguegeShowText("remark_task_restart"));
                //
                m_pActivePage->initTableData();
                m_pUnActivePage->initTableData();
            }
            else//如果是记忆曲线，需要弹窗提示重新开始，然后修改相关属性
            {
                // 警告提示
                if (CMessageBox::question(getLanguegeShowText("forgetTask_restart_question")))
                {
                    CDataCenter::GetInstance()->setTaskFinishAndFailTimes(task_id, 0, 0);
                    CDataCenter::GetInstance()->setTaskStatus(task_id, TASK_STATUS_RESTART);

                    //更新修改数据
                    CDataCenter::GetInstance()->updateActiveTableTask(task_id);
                    //由于是重新开始，应该需要添加日常任务
                    CDataCenter::GetInstance()->addOrUpdateStatusChangeBytaskId(task_id, TASK_ACTIVETYPE_ACTIVE, getLanguegeShowText("reason_task_restart"));
                    CDataCenter::GetInstance()->addOrUpdateDailyHistoryBytaskId(task_id, TASK_ACTIVETYPE_ACTIVE, getLanguegeShowText("remark_task_restart"));
                    //
                    m_pActivePage->initTableData();
                    m_pUnActivePage->initTableData();
                }
            }
        }
        //非活跃池中的失败和完成时将这个任务移到完成列表中，因此需要弹窗提示和让用户输入相关原因
        //目前设定为状态会在今天改变，但是第二天会进入到完成列表中====================================
        else if (TASK_OPERATE_TYPE_FAILE == taskOperType || TASK_OPERATE_TYPE_FILISH == taskOperType)
        {
            CInputReason dialog;
            if (dialog.exec() == QDialog::Accepted)
            {
                QString reason = dialog.getReason();
                QString qsStatus = (TASK_OPERATE_TYPE_FAILE == taskOperType) ? (TASK_STATUS_FILED) : (TASK_STATUS_FINISH);
                CDataCenter::GetInstance()->setTaskStatus(task_id, qsStatus);
                CDataCenter::GetInstance()->setTaskStatusChangeReason(task_id, reason);

                CDataCenter::GetInstance()->updateActiveTableTask(task_id);
                CDataCenter::GetInstance()->addOrUpdateStatusChangeBytaskId(task_id, TASK_ACTIVETYPE_ACTIVE, reason);
                CDataCenter::GetInstance()->addOrUpdateDailyHistoryBytaskId(task_id, TASK_ACTIVETYPE_ACTIVE, reason);

                //
                m_pActivePage->initTableData();
                m_pUnActivePage->initTableData();
            }
        }
        //弹出修改窗，然后根据相关操作类型推断当前任务状态，不同的状态科可修改属性不同
        else if(TASK_OPERATE_TYPE_EDIT == taskOperType)
        {
            STaskListPtr result;
            if (!CDataCenter::GetInstance()->getTaskListById(task_id, result))
            {
                return;
            }

            CAddTaskDialog dialog(result, TASK_ACTIVETYPE_NO_ACTIVE, true);
            if (dialog.exec() == QDialog::Accepted)
            {
                dialog.getTaskInformation(result);
                CDataCenter::GetInstance()->updateActiveTableTask(result);
                //
                m_pActivePage->initTableData();
                m_pUnActivePage->initTableData();
            }
        }
        //删除当前任务，需要弹窗提示和列表数据
        else if (TASK_OPERATE_TYPE_DELETE == taskOperType)
        {
            // 警告提示
            if (CMessageBox::warning(getLanguegeShowText("task_delete_question")))
            {
                CDataCenter::GetInstance()->deleteUnStartTask(task_id);
                m_pUnActivePage->initTableData();
            }
        }
    }
    else if (TASK_ACTIVETYPE_ACTIVE == taskActiveType)
    {
        //弹窗提示，并且输入暂停原因，确认后才能暂停，然后进入不开启的池子
        if (TASK_OPERATE_TYPE_PAUSE == taskOperType)
        {
            CInputReason dialog;
            if (dialog.exec() == QDialog::Accepted)
            {
                QString reason = dialog.getReason();
                CDataCenter::GetInstance()->setTaskStatus(task_id, TASK_OPERATE_TYPE_PAUSE);
                CDataCenter::GetInstance()->setTaskStatusChangeReason(task_id, reason);

                CDataCenter::GetInstance()->updateActiveTableTask(task_id);
                CDataCenter::GetInstance()->addOrUpdateStatusChangeBytaskId(task_id, TASK_ACTIVETYPE_ACTIVE, reason);
                //
                m_pActivePage->initTableData();
                m_pUnActivePage->initTableData();
            }
        }
        //活跃池中的失败和完成时将只需要改变相关状态和finishtimes就行，不需要移动数据
        else if (TASK_OPERATE_TYPE_FAILE == taskOperType || TASK_OPERATE_TYPE_FILISH == taskOperType)
        {
            QString qsStatus = (TASK_OPERATE_TYPE_FAILE == taskOperType) ? (TASK_STATUS_FILED) : (TASK_STATUS_FINISH);
            CDataCenter::GetInstance()->setTaskStatus(task_id, qsStatus);

            CDataCenter::GetInstance()->updateActiveTableTask(task_id);
            //CDataCenter::GetInstance()->addOrUpdateStatusChangeBytaskId(task_id, TASK_ACTIVETYPE_ACTIVE, getLanguegeShowText("reason_task_restart");
            CDataCenter::GetInstance()->addOrUpdateDailyHistoryBytaskId(task_id, TASK_ACTIVETYPE_ACTIVE, getLanguegeShowText("reason_task_finish"));

            //
            m_pActivePage->initTableData();
            m_pUnActivePage->initTableData();
        }//弹出修改窗，然后根据相关操作类型推断当前任务状态，不同的状态科可修改属性不同
        else if (TASK_OPERATE_TYPE_EDIT == taskOperType)
        {
            STaskListPtr result;
            if (!CDataCenter::GetInstance()->getTaskListById(task_id, result))
            {
                return;
            }

            CAddTaskDialog dialog(result, TASK_ACTIVETYPE_ACTIVE, true);
            if (dialog.exec() == QDialog::Accepted)
            {
                dialog.getTaskInformation(result);
                CDataCenter::GetInstance()->updateActiveTableTask(result);
                //
                m_pActivePage->initTableData();
                m_pUnActivePage->initTableData();
            }
        }
    }
    else if (TASK_ACTIVETYPE_FINISHED == taskActiveType)
    {
        //这个地方重新开始的时候需要向上一层发送，甚至不会进入到这个分支，因为这个情况是在另外一个界面，而这里是给定一
        //但是通过设定就在这里处理，避免其他过得的代码添加=========
        if (TASK_OPERATE_TYPE_RESTART == taskOperType)
        {
            STaskListPtr result;
            if (!CDataCenter::GetInstance()->getTaskListById(task_id, result))
            {
                return;
            }

            CAddTaskDialog dialog(result);
            if (dialog.exec() == QDialog::Accepted)
            {
                dialog.getTaskInformation(result);

                result->finish_times = 0;
                result->fail_times = 0;
                result->task_status = TASK_STATUS_INPROGRESS;
                result->statue_resaon = "";

                CDataCenter::GetInstance()->addActiveTableTask(result);
                CDataCenter::GetInstance()->deleteCompleteTask(result->id);

                CDataCenter::GetInstance()->addOrUpdateStatusChangeBytaskId(result->id, TASK_ACTIVETYPE_ACTIVE, getLanguegeShowText("reason_task_restart"));
                CDataCenter::GetInstance()->addOrUpdateDailyHistoryBytaskId(result->id, TASK_ACTIVETYPE_ACTIVE, getLanguegeShowText("remark_task_restart"));
                //
                m_pActivePage->initTableData();
                emit sigUpdateCompleteTaskTable();
            }      
        } 
    }
}

void CMainPanel::addTaskBtn_click()
{
    //这个地方后续需要修改，因为所选择的不一定是第一个，应需要修改 
    // ----但是在实现的时候，目前的设定是不能修改已经开始了的任务，只能进行删除，或者只能修改特定无太大影响的属性
    //这个是留存上次设定的一些参数
    CAddTaskDialog dialog(m_tempAddDiaTaskPtr);
    if (dialog.exec() == QDialog::Accepted)
    {
         dialog.getTaskInformation(m_tempAddDiaTaskPtr);
        //m_tempAddDiaTaskPtr->id = QString::number(QDateTime::currentDateTime().toSecsSinceEpoch());
        m_pUnActivePage->updateTableData(m_tempAddDiaTaskPtr);
        CDataCenter::GetInstance()->addActiveTableTask(m_tempAddDiaTaskPtr);
        //CDataCenter::GetInstance()->addOrUpdateDailyHistoryBytaskId(m_tempAddDiaTaskPtr->id, TASK_ACTIVETYPE_NO_ACTIVE, getLanguegeShowText("remark_task_create"));
        CDataCenter::GetInstance()->addOrUpdateStatusChangeBytaskId(m_tempAddDiaTaskPtr->id, TASK_ACTIVETYPE_NO_ACTIVE, getLanguegeShowText("reason_task_create"));
        m_tempAddDiaTaskPtr.clear();
    }
    else
    {
       dialog.getTaskInformation(m_tempAddDiaTaskPtr);
    }
}