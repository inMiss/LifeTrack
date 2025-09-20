#include "todayTaskPage.h"
#include "addTaskDialog.h"

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QDate>
#include <QObject>
#include <QMouseEvent>

CTodayTaskPage::CTodayTaskPage(QWidget *parent)
    : CCustomWidget(parent)
{
    createUi();
}

CTodayTaskPage::~CTodayTaskPage()
{

}

void CTodayTaskPage::createUi()
{
    QVBoxLayout* pMainLayout = new QVBoxLayout(this);

    //
    QHBoxLayout* pUpLayout = new QHBoxLayout();
    m_pShowLabel = new QLabel("Today Task:");
    pUpLayout->addWidget(m_pShowLabel);

    m_pAddTaskBtn = new QPushButton("Add Task");
    m_pAddTaskBtn->setObjectName("m_pAddTaskBtn");
    connect(m_pAddTaskBtn, &QPushButton::clicked, this, &CTodayTaskPage::addTaskBtn_click);
    pUpLayout->addWidget(m_pAddTaskBtn);

    m_pDateEdit = new QDateEdit(QDate::currentDate());
    connect(m_pDateEdit, &QDateEdit::dateChanged, this, &CTodayTaskPage::onDateEditChange);
    pUpLayout->addWidget(m_pDateEdit);
    //
    m_pTableModel = new CBaseTableModel(true, this);
    m_pTableView = new QTableView();
    m_pTableView->setModel(m_pTableModel);
    m_pTableView->resizeColumnsToContents();
    m_pTableView->hideColumn(E_TASKETTABLE_ID);
    m_pTableView->hideColumn(E_TASKETTABLE_DURINGTIME);
    m_pTableView->hideColumn(E_TASKETTABLE_VALUE);
    m_pTableView->hideColumn(E_TASKETTABLE_FINISHTIMES);
    m_pTableView->hideColumn(E_TASKETTABLE_SUMPOINTS);
    m_pTableView->hideColumn(E_TASKETTABLE_FINISHTIME);
    m_pTableView->hideColumn(E_TASKETTABLE_PAUSETIME);
    m_pTableView->hideColumn(E_TASKETTABLE_PARENTTASK);
    
    pMainLayout->addLayout(pUpLayout);
    pMainLayout->addWidget(m_pTableView);
    initTableData();
    m_pTableView->show();
}

void CTodayTaskPage::initTableData()
{
    QMap<QString, STaskListPtr> data = CDataCenter::GetInstance()->getTasksByDate(m_pDateEdit->date());
    updateTableData(data);
}

void CTodayTaskPage::updateTableData(const QMap<QString, STaskListPtr>& data)
{
    QDate currentDay = m_pDateEdit->date();
    m_pTableModel->setCurrentDate(currentDay);
    m_pTableModel->addDatas(data);
//     QMap<QString, STaskListPtr>::const_iterator iter = data.begin();
//     for (iter; iter != data.end(); ++iter)
//     {
//         if (currentDay == textToDate(iter.value()->start_time)/*iter.value()->start_time.date()*/)
//         {
//             m_pTableModel->addData(iter.value());
//         }
//     }
    m_pTableView->resizeColumnsToContents();
}

//
void CTodayTaskPage::addTaskBtn_click()
{
    STaskListPtr tempTask = nullptr;
    if (m_tempAddDiaTaskPtr.size() != 0)
    {
        tempTask = m_tempAddDiaTaskPtr[0];
    }
    CAddTaskDialog dialog(tempTask);
    if (dialog.exec() == QDialog::Accepted)
    {
        m_tempAddDiaTaskPtr = dialog.getTaskInformation();
        //m_tempAddDiaTaskPtr->id = QString::number(QDateTime::currentDateTime().toSecsSinceEpoch());
        m_pTableModel->addDatas(m_tempAddDiaTaskPtr);
        CDataCenter::GetInstance()->addNewTasks(m_tempAddDiaTaskPtr);
        m_tempAddDiaTaskPtr.clear();
    }
    else
    {
        m_tempAddDiaTaskPtr = dialog.getTaskInformation();
    }
}

void CTodayTaskPage::onDateEditChange()
{
    m_pTableModel->clearAllData();
    //m_pTableModel->setCurrentDate(m_pDateEdit->date());
    initTableData();
}

// bool CTodayTaskPage::event(QEvent* event)
// {
//     if (event->type() == QEvent::MouseButtonPress)
//     {
//         QObject* objectTemp = sender();
//         QString temp =  objectTemp->objectName();
//     }
// 
//     return QWidget::event(event);
// }

// void CTodayTaskPage::mousePressEvent(QMouseEvent* event)
// {
//     //QObject* objectTemp = sender();
//     QPushButton* button = qobject_cast<QPushButton*>(sender());
//     if (button) {
//         QString temp = button->objectName();
//     }
//     
// 
// 
//     QWidget::mousePressEvent(event);
// }
