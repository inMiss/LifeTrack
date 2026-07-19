#include "activeTaskPage.h"
#include "../taskDialog/addTaskDialog.h"
#include "../common/common.h"
#include "../common/commonFunc.h"

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QDate>
#include <QObject>
#include <QMouseEvent>
#include <QHeaderView>

CActiveTaskPage::CActiveTaskPage(QWidget *parent)
    : CCustomWidget(parent)
    , m_tempAddDiaTaskPtr(nullptr)
    , m_pFilterTypeCombox(nullptr)
{
    createUi();
}

CActiveTaskPage::~CActiveTaskPage()
{

}

void CActiveTaskPage::createUi()
{
    QVBoxLayout* pMainLayout = new QVBoxLayout(this);

    //
    QHBoxLayout* pUpLayout = new QHBoxLayout();
    m_pShowLabel = new QLabel("Today Task:");
    pUpLayout->addWidget(m_pShowLabel);

    {
        m_pFilterTypeCombox = new QComboBox();
        m_pFilterTypeCombox->addItem(getLanguegeShowText("taskShowTable_filter_today"), QVariant(e_taskFilter_type_today));
        m_pFilterTypeCombox->addItem(getLanguegeShowText("taskShowTable_filter_nearWeek"), QVariant(e_taskFilter_type_nearWeek));
        m_pFilterTypeCombox->addItem(getLanguegeShowText("taskShowTable_filter_nearMonth"), QVariant(e_taskFilter_type_nearMonth));
        m_pFilterTypeCombox->addItem(getLanguegeShowText("taskShowTable_filter_All"), QVariant(e_taskFilter_type_all));
        m_pFilterTypeCombox->setCurrentText(getLanguegeShowText("taskShowTable_filter_All"));
        connect(m_pFilterTypeCombox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &CActiveTaskPage::slotsFilterComBoxChange);

        pUpLayout->addWidget(m_pFilterTypeCombox);    
    }

    //
    m_pTableModel = new CBaseTableModel(true, this);
    m_pTableModel->setTableActiveType(TASK_ACTIVETYPE_NO_ACTIVE);
    m_pTableView = new CBaseTableView();
    m_pTableView->setModel(m_pTableModel);
    m_pTableView->resizeColumnsToContents();
    m_pTableView->hideColumn(E_TASKETTABLE_ID);
    m_pTableView->hideColumn(E_TASKETTABLE_DURINGTIME);
    m_pTableView->hideColumn(E_TASKETTABLE_VALUE);
    m_pTableView->hideColumn(E_TASKETTABLE_FINISHTIMES);
    m_pTableView->hideColumn(E_TASKETTABLE_SUMPOINTS);
    m_pTableView->hideColumn(E_TASKETTABLE_FINISHTIME);
    m_pTableView->hideColumn(E_TASKETTABLE_PARENTTASK);
    
    // 在MainWindow中设置表格的编辑触发方式
    m_pTableView->setEditTriggers(QAbstractItemView::CurrentChanged |
        QAbstractItemView::DoubleClicked);

    // 创建新的代理
    MultipleButtonDelegate* pMulButDele = new MultipleButtonDelegate(m_pTableView);
    // 为指定列设置代理
    m_pTableView->setItemDelegateForColumn(E_TASKETTABLE_TASKOPERATOR, pMulButDele);
    // 连接代理的按钮点击信号到自己的信号
    connect(pMulButDele, &MultipleButtonDelegate::buttonClicked,
        this, &CActiveTaskPage::slotsButtonClicked);
    m_pTableView->horizontalHeader()->setSectionResizeMode(E_TASKETTABLE_TASKOPERATOR, QHeaderView::Fixed);
    resetTableViewColumnSize();
    
    pMainLayout->addLayout(pUpLayout);
    pMainLayout->addWidget(m_pTableView);
    initTableData();
    m_pTableView->show();
}

void CActiveTaskPage::initTableData()
{
    m_pTableModel->clearAllData();
    QMap<QString, STaskListPtr> data = CDataCenter::GetInstance()->getActiveTableViewTasks();
    updateTableData(data);
}

void CActiveTaskPage::resetTableViewColumnSize()
{
    if (m_pTableView)
    {
        m_pTableView->resizeColumnsToContents();
        m_pTableView->setColumnWidth(E_TASKETTABLE_TASKOPERATOR, 200);
    }
}

void CActiveTaskPage::updateTableData(const QMap<QString, STaskListPtr>& data)
{
    m_pTableModel->addDatas(data);

    resetTableViewColumnSize();
}

void CActiveTaskPage::updateTableData(const STaskListPtr& data)
{
    m_pTableModel->addData(data);

    resetTableViewColumnSize();
}

void CActiveTaskPage::onDateEditChange()
{
    m_pTableModel->clearAllData();
    initTableData();
}

void CActiveTaskPage::slotsButtonClicked(int row, QString operatorId)
{
    QString taskID = m_pTableModel->data(m_pTableModel->index(row, E_TASKETTABLE_ID)).toString();
    emit sigTaskOperation(taskID, row, QString(TASK_ACTIVETYPE_ACTIVE), operatorId);
}

void CActiveTaskPage::deleteTableDataByRow(const int nRow)
{
    m_pTableModel->removeRow(nRow);
}

void CActiveTaskPage::slotsFilterComBoxChange(int index)
{
    int nType = m_pFilterTypeCombox->currentData().toInt();
    QDate tempDate = QDate::currentDate();
    if (e_taskFilter_type_today == nType)
    {
        m_pTableModel->setFilterDate(tempDate, true);
    }
    else if (e_taskFilter_type_nearWeek)
    {
        tempDate.addDays(7);
        m_pTableModel->setFilterDate(tempDate, true);
    }
    else if (e_taskFilter_type_nearMonth)
    {
        tempDate.addMonths(1);
        m_pTableModel->setFilterDate(tempDate, true);
    }
    else if (e_taskFilter_type_all)
    {
        m_pTableModel->setFilterDate(tempDate, false);
    }
    initTableData();
}
// bool CActiveTaskPage::event(QEvent* event)
// {
//     if (event->type() == QEvent::MouseButtonPress)
//     {
//         QObject* objectTemp = sender();
//         QString temp =  objectTemp->objectName();
//     }
// 
//     return QWidget::event(event);
// }

// void CActiveTaskPage::mousePressEvent(QMouseEvent* event)
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
