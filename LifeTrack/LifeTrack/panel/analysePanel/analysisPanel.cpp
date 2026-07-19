#include "analysisPanel.h"

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QDate>
#include <QObject>
#include <QMouseEvent>
#include <QHeaderView>

CAnalysisPanel::CAnalysisPanel(QWidget *parent)
    : QWidget(parent)
{
    
    createUi();
}

CAnalysisPanel::~CAnalysisPanel()
{}

void CAnalysisPanel::createUi()
{
    QVBoxLayout* pMainLayout = new QVBoxLayout(this);
    //
    QHBoxLayout* pUpLayout = new QHBoxLayout();
    m_pShowLabel = new QLabel("Complete Task:");
    pUpLayout->addWidget(m_pShowLabel);

    //
    m_pTableModel = new CBaseTableModel(true, this);
    m_pTableModel->setTableActiveType(TASK_ACTIVETYPE_FINISHED);
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
        this, &CAnalysisPanel::slotsButtonClicked);
    m_pTableView->horizontalHeader()->setSectionResizeMode(E_TASKETTABLE_TASKOPERATOR, QHeaderView::Fixed);
    pMainLayout->addLayout(pUpLayout);
    pMainLayout->addWidget(m_pTableView);

    initTableData();
    m_pTableView->show();
}

void CAnalysisPanel::initTableData()
{
    m_pTableModel->clearAllData();
    QMap<QString, STaskListPtr> data = CDataCenter::GetInstance()->getAllCompleteTasks();
    updateTableData(data);
}

void CAnalysisPanel::resetTableViewColumnSize()
{
    if (m_pTableView)
    {
        m_pTableView->resizeColumnsToContents();
        m_pTableView->setColumnWidth(E_TASKETTABLE_TASKOPERATOR, 200);
    }
}

void CAnalysisPanel::updateTableData(const QMap<QString, STaskListPtr>& data)
{
    m_pTableModel->addDatas(data);

    resetTableViewColumnSize();
}

void CAnalysisPanel::updateTableData(const STaskListPtr& data)
{
    m_pTableModel->addData(data);

    resetTableViewColumnSize();
}

void CAnalysisPanel::slotsButtonClicked(int row, QString operatorId)
{
    QString taskID = m_pTableModel->data(m_pTableModel->index(row, E_TASKETTABLE_ID)).toString();
    emit sigTaskOperation(taskID, row, QString(TASK_ACTIVETYPE_FINISHED), operatorId);
}

void CAnalysisPanel::deleteTableDataByRow(const int nRow)
{
    m_pTableModel->removeRow(nRow);
}

void CAnalysisPanel::slotUpdateTable()
{
    initTableData();
}
