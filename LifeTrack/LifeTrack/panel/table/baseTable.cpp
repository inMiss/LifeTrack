#include "baseTable.h"
#include <QStandardItem>
#include <QDateTime>
#include <QDateTimeEdit>
#include <QHBoxLayout>
#include <QPushButton>
#include <QEvent>
#include <QApplication>
#include <QMouseEvent>
#include <QPainter>

#include "../../language/language.h"
#include "../../common/commonFunc.h"
#include "../../management/dataCenter.h"


//
// 该种列表涉及到的公共特殊公共函数
//
QList<ButtonData> getTaskOperatorButton(const QString& qsTaskStatus, const QString& qsActiveType/* = QString(TASK_ACTIVETYPE_NO_ACTIVE)*/)
{
    //
    QList<ButtonData> data;
    //这边需要添加状态输入，如果是完成列表，那就只有重新开始操作
    if (TASK_ACTIVETYPE_FINISHED == qsActiveType)
    {
        data.push_back(ButtonData(getLanguegeShowText("task_operator_type_reStart"), TASK_OPERATE_TYPE_RESTART));
        return data;
    }

    if (TASK_STATUS_UNSTART == qsTaskStatus)
    {
        data.push_back(ButtonData(getLanguegeShowText("task_operator_type_start"), TASK_OPERATE_TYPE_START));
        data.push_back(ButtonData(getLanguegeShowText("task_operator_type_edit"), TASK_OPERATE_TYPE_EDIT));
        data.push_back(ButtonData(getLanguegeShowText("task_operator_type_delete"), TASK_OPERATE_TYPE_DELETE));
    }
    else if(TASK_STATUS_INPROGRESS == qsTaskStatus || TASK_STATUS_RESTART == qsTaskStatus)
    {
        data.push_back(ButtonData(getLanguegeShowText("task_operator_type_pause"), TASK_OPERATE_TYPE_PAUSE));
        data.push_back(ButtonData(getLanguegeShowText("task_operator_type_edit"), TASK_OPERATE_TYPE_EDIT));
        data.push_back(ButtonData(getLanguegeShowText("task_operator_type_faile"), TASK_OPERATE_TYPE_FAILE));
        data.push_back(ButtonData(getLanguegeShowText("task_operator_type_finish"), TASK_OPERATE_TYPE_FILISH));
    }
    else if(TASK_STATUS_PAUSE == qsTaskStatus || TASK_STATUS_AUTOPAUSE == qsTaskStatus)
    {
        data.push_back(ButtonData(getLanguegeShowText("task_operator_type_reStart"), TASK_OPERATE_TYPE_RESTART));
        data.push_back(ButtonData(getLanguegeShowText("task_operator_type_edit"), TASK_OPERATE_TYPE_EDIT));
        data.push_back(ButtonData(getLanguegeShowText("task_operator_type_faile"), TASK_OPERATE_TYPE_FAILE));
        data.push_back(ButtonData(getLanguegeShowText("task_operator_type_finish"), TASK_OPERATE_TYPE_FILISH));
    }
    else//TASK_STATUS_FILED + TASK_STATUS_FINISH
    {
        
    }
    return data;
}

//////////////////////////////////
/////////代理
/////////////////////////////////
MultipleButtonDelegate::MultipleButtonDelegate(QObject* parent)
    : QStyledItemDelegate(parent)
    , m_hoverButtonIndex(-1)
    , m_pressedButtonIndex(-1)
{
    // 注册元类型
    qRegisterMetaType<ButtonData>();
    qRegisterMetaType<QList<ButtonData>>();
}

// QWidget* MultipleButtonDelegate::createEditor(QWidget* parent,
//     const QStyleOptionViewItem& option,
//     const QModelIndex& index) const
// {
//     // 创建容器
//     QWidget* widget = new QWidget(parent);
//     QHBoxLayout* layout = new QHBoxLayout(widget);
//     layout->setContentsMargins(5, 2, 5, 2);
//     layout->setSpacing(5);
// 
//     // 从模型中获取按钮数据列表
//     QList<ButtonData> buttons = index.data(Qt::UserRole + 1).value<QList<ButtonData>>();
// 
//     // 创建按钮
//     for (int i = 0; i < buttons.size(); ++i) {
//         QPushButton* btn = new QPushButton(buttons[i].text, widget);
//         btn->setProperty("row", index.row());
//         btn->setProperty("id", buttons[i].id);
//         connect(btn, &QPushButton::clicked, this, &MultipleButtonDelegate::onButtonClicked);
//         layout->addWidget(btn);
//     }
// 
//     layout->addStretch(); // 让按钮靠左对齐
//     return widget;
// }

void MultipleButtonDelegate::updateEditorGeometry(QWidget* editor,
    const QStyleOptionViewItem& option,
    const QModelIndex& index) const
{
    editor->setGeometry(option.rect);
}

QList<QRect> MultipleButtonDelegate::getButtonRects(const QStyleOptionViewItem& option,
    const QList<ButtonData>& buttons) const
{
    QList<QRect> rects;
    if (buttons.isEmpty()) return rects;

    // 获取格子可用区域
    QRect cellRect = option.rect;
    int cellWidth = cellRect.width();
    int cellHeight = cellRect.height();

    // 动态计算按钮大小（根据格子宽度和按钮数量）
    int spacing = 3;  // 按钮间距
    int btnWidth = (cellWidth - (buttons.size() - 1) * spacing - 10) / buttons.size();
    int btnHeight = cellHeight - 8;  // 上下留4像素边距

    // 限制最小和最大尺寸
    btnWidth = qBound(40, btnWidth, 80);
    btnHeight = qBound(22, btnHeight, 32);

    // 重新计算总宽度
    int totalWidth = buttons.size() * btnWidth + (buttons.size() - 1) * spacing;
    int startX = cellRect.x() + (cellWidth - totalWidth) / 2;
    int y = cellRect.y() + (cellHeight - btnHeight) / 2;

    for (int i = 0; i < buttons.size(); ++i) {
        QRect rect(startX + i * (btnWidth + spacing), y, btnWidth, btnHeight);
        rects.append(rect);
    }

    return rects;
}

void MultipleButtonDelegate::drawButton(QPainter* painter, const QRect& rect,
    const ButtonData& button, bool isPressed, bool isHovered) const
{
    QStyleOptionButton btnOption;
    btnOption.rect = rect;
    btnOption.text = button.text;
    btnOption.state = QStyle::State_Enabled;

    if (isPressed) {
        btnOption.state |= QStyle::State_Sunken;
    }
    if (isHovered) {
        btnOption.state |= QStyle::State_MouseOver;
    }

    QApplication::style()->drawControl(QStyle::CE_PushButton, &btnOption, painter);
}

void MultipleButtonDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option,
    const QModelIndex& index) const
{
    // 先绘制原有内容（如果有）
    //QStyledItemDelegate::paint(painter, option, index);

    // 获取按钮数据
    QList<ButtonData> buttons = index.data(Qt::UserRole + 1).value<QList<ButtonData>>();
    if (buttons.isEmpty()) return;

    // 获取按钮区域
    QList<QRect> rects = getButtonRects(option, buttons);

    // 获取鼠标位置（用于悬停效果）
    QPoint mousePos = QCursor::pos();
    QPoint widgetPos = option.widget->mapFromGlobal(mousePos);

    // 更新悬停状态
    m_hoverButtonIndex = -1;
    for (int i = 0; i < rects.size(); ++i) {
        if (rects[i].contains(widgetPos)) {
            m_hoverButtonIndex = i;
            break;
        }
    }

    // 绘制所有按钮
    for (int i = 0; i < buttons.size(); ++i) {
        bool isPressed = (m_pressedButtonIndex == i);
        bool isHovered = (m_hoverButtonIndex == i);
        drawButton(painter, rects[i], buttons[i], isPressed, isHovered);
    }
}

bool MultipleButtonDelegate::editorEvent(QEvent* event, QAbstractItemModel* model,
    const QStyleOptionViewItem& option,
    const QModelIndex& index)
{
    // 获取按钮数据
    QList<ButtonData> buttons = index.data(Qt::UserRole + 1).value<QList<ButtonData>>();
    if (buttons.isEmpty()) return false;

    // 获取按钮区域
    QList<QRect> rects = getButtonRects(option, buttons);

    // 处理鼠标事件
    if (event->type() == QEvent::MouseButtonPress) {
        QMouseEvent* mouseEvent = static_cast<QMouseEvent*>(event);
        for (int i = 0; i < rects.size(); ++i) {
            if (rects[i].contains(mouseEvent->pos())) {
                m_pressedButtonIndex = i;
                // 触发重绘
                const_cast<QWidget*>(option.widget)->update(option.rect);
                return true;
            }
        }
    }
    else if (event->type() == QEvent::MouseButtonRelease) {
        QMouseEvent* mouseEvent = static_cast<QMouseEvent*>(event);
        for (int i = 0; i < rects.size(); ++i) {
            if (rects[i].contains(mouseEvent->pos()) && m_pressedButtonIndex == i) {
                // 发射信号
                emit buttonClicked(index.row(), buttons[i].id);
                m_pressedButtonIndex = -1;
                const_cast<QWidget*>(option.widget)->update(option.rect);
                return true;
            }
        }
        m_pressedButtonIndex = -1;
        const_cast<QWidget*>(option.widget)->update(option.rect);
    }
    else if (event->type() == QEvent::MouseMove) {
        // 鼠标移动时更新悬停状态
        const_cast<QWidget*>(option.widget)->update(option.rect);
    }

    return false;
}

// QSize MultipleButtonDelegate::sizeHint(const QStyleOptionViewItem& option,
//     const QModelIndex& index) const
// {
//     QSize size = sizeHint(option, index);
// 
//     QList<ButtonData> buttons = index.data(Qt::UserRole + 1).value<QList<ButtonData>>();
//     if (!buttons.isEmpty()) {
//         // 增加行高，确保按钮有足够空间
//         size.setHeight(qMax(size.height(), 35));
//     }
// 
//     return size;
// }

void MultipleButtonDelegate::onButtonClicked() const
{
    QPushButton* btn = qobject_cast<QPushButton*>(sender());
    if (btn) {
        int row = btn->property("row").toInt();
        QString buttonId = btn->property("buttonId").toString();

        emit buttonClicked(row, buttonId);
    }
}

//////////////////////////////////
/////////代理
/////////////////////////////////

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
    head << getLanguegeShowText("table_taskList_start_date");
    head << getLanguegeShowText("table_taskList_deadline");
    head << getLanguegeShowText("table_taskList_create_time");
    head << getLanguegeShowText("table_taskList_start_time");
    head << getLanguegeShowText("table_taskList_finish_time");
    head << getLanguegeShowText("table_taskList_during_time");   
    head << getLanguegeShowText("table_taskList_import_level");
    head << getLanguegeShowText("table_taskList_value");
    head << getLanguegeShowText("table_taskList_sum_ponits");
    head << getLanguegeShowText("table_taskList_repeat_type");
    head << getLanguegeShowText("table_taskList_finish_times");
    head << getLanguegeShowText("table_taskList_fail_times");  
    head << getLanguegeShowText("table_taskList_belong_type");
    head << getLanguegeShowText("table_taskList_is_deduction");
    head << getLanguegeShowText("table_taskList_is_countdown");
    head << getLanguegeShowText("table_taskList_is_can_pause");
    head << getLanguegeShowText("operate");
    head << getLanguegeShowText("table_taskList_fail_threshold");
    head << getLanguegeShowText("table_taskList_task_status");   
    head << getLanguegeShowText("table_taskList_statue_resaon");
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
        this->addData(iter.value(), iter.value()->task_status);
    }
}

void CBaseTableModel::addData(const STaskListPtr& data, const QString& taskStatue /* = TASK_STATUS_UNSTART */)
{
    if (nullptr == data)
    {
        return;
    }

    if (m_bFilter && m_filterDate < textToDate(data->create_time))
    {
        return;
    }

    int nRowIndex = rowCount();

    bool bFinish = data->task_status == TASK_STATUS_FINISH;
    QStandardItem* itemCheck = new QStandardItem("");
    itemCheck->setEditable(false);
    itemCheck->setCheckable(true);
    itemCheck->setCheckState((bFinish) ? Qt::Checked : Qt::Unchecked);
    if (bFinish || TASK_ACTIVETYPE_FINISHED == m_qsTaskActiveType)
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

    QStandardItem* itemStarDate = new QStandardItem((data->start_date)/*data->start_time.toString("yyyy-MM-dd hh:mm:ss")*/);
    itemStarDate->setEditable(false);
    //QDateTimeEdit* startTime = new QDateTimeEdit(data->start_time);
    //itemStartTime->setData(startTime, Qt::UserRole + 1);
    setItem(nRowIndex, E_TASKETTABLE_STARTDATE, itemStarDate);

    QStandardItem* itemEndTime = new QStandardItem((data->deadline)/*data->end_time.toString("yyyy-MM-dd hh:mm:ss")*/);
    itemEndTime->setEditable(false);
    setItem(nRowIndex, E_TASKETTABLE_DEADLINE, itemEndTime);

    QStandardItem* itemCreateTime = new QStandardItem((data->create_time)/*data->create_time.toString("yyyy-MM-dd hh:mm:ss")*/);
    itemCreateTime->setEditable(false);
    setItem(nRowIndex, E_TASKETTABLE_CREATETIME, itemCreateTime);

    QStandardItem* itemStartTime = new QStandardItem((data->start_time)/*data->create_time.toString("yyyy-MM-dd hh:mm:ss")*/);
    itemStartTime->setEditable(false);
    setItem(nRowIndex, E_TASKETTABLE_STARTTIME, itemStartTime);

    QStandardItem* itemFinishTime = new QStandardItem((data->finish_time)/*data->finish_time.toString("yyyy-MM-dd hh:mm:ss")*/);
    itemFinishTime->setEditable(false);
    setItem(nRowIndex, E_TASKETTABLE_FINISHTIME, itemFinishTime);

    QStandardItem* itemDuringTime = new QStandardItem(QString::number(data->during_time));
    itemDuringTime->setEditable(false);
    setItem(nRowIndex, E_TASKETTABLE_DURINGTIME, itemDuringTime);

    QStandardItem* itemImportLevel = new QStandardItem(CDataCenter::GetInstance()->getImportLevelShowContent(data->import_level));
    itemImportLevel->setEditable(false);
    setItem(nRowIndex, E_TASKETTABLE_IMPORTLEVEL, itemImportLevel);

    QStandardItem* itemValue = new QStandardItem(QString::number(data->value));
    itemValue->setEditable(false);
    setItem(nRowIndex, E_TASKETTABLE_VALUE, itemValue);

    QStandardItem* itemSumPoints = new QStandardItem(QString::number(data->sum_points));
    itemSumPoints->setEditable(false);
    setItem(nRowIndex, E_TASKETTABLE_SUMPOINTS, itemSumPoints);

    QStandardItem* itemRepeatType = new QStandardItem(CDataCenter::GetInstance()->getRepeatTypeShowContent(data->repeat_type));
    itemRepeatType->setEditable(false);
    setItem(nRowIndex, E_TASKETTABLE_REPEATTYPE, itemRepeatType);

    QStandardItem* itemFinishTimes = new QStandardItem(QString::number(data->finish_times));
    itemFinishTimes->setEditable(false);
    setItem(nRowIndex, E_TASKETTABLE_FINISHTIMES, itemFinishTimes);

    QStandardItem* itemFailTimes = new QStandardItem(QString::number(data->fail_times));
    itemFailTimes->setEditable(false);
    setItem(nRowIndex, E_TASKETTABLE_FAILTIMES, itemFailTimes);

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
    QStandardItem* itemOperation = new QStandardItem();
    itemOperation->setData(QVariant::fromValue(getTaskOperatorButton(taskStatue, m_qsTaskActiveType)), Qt::UserRole + 1);
    itemOperation->setEditable(false);
    setItem(nRowIndex, E_TASKETTABLE_TASKOPERATOR, itemOperation);

    QStandardItem* itemFileThreshold = new QStandardItem(QString::number(data->fail_threshold));
    itemFileThreshold->setEditable(false);
    setItem(nRowIndex, E_TASKETTABLE_FAILTHRESHOLD, itemFileThreshold);

    QStandardItem* itemTaskStatus = new QStandardItem(CDataCenter::GetInstance()->getStatusTypeShowContent(data->task_status));
    itemTaskStatus->setEditable(false);
    setItem(nRowIndex, E_TASKETTABLE_TASKSTATUS, itemTaskStatus);

    QStandardItem* itemStatueReason = new QStandardItem((data->statue_resaon)/*data->finish_time.toString("yyyy-MM-dd hh:mm:ss")*/);
    itemStatueReason->setEditable(false);
    setItem(nRowIndex, E_TASKETTABLE_STATUERESAON, itemStatueReason);

    QStandardItem* itemParentId = new QStandardItem(data->parent_task);
    itemParentId->setEditable(false);
    setItem(nRowIndex, E_TASKETTABLE_PARENTTASK, itemParentId);

    //这个地方是认为输入的备注，最后是提供鼠标一上去显示和双击弹框显示
    QStandardItem* itemRemark = new QStandardItem(data->remark);
    itemRemark->setEditable(false);
    setItem(nRowIndex, E_TASKETTABLE_REMARK, itemRemark);

}

void CBaseTableModel::setFilterDate(const QDate& date, const bool bFilter /* = true */)
{
    m_filterDate = date;
    m_bFilter = bFilter;
}

void CBaseTableModel::clearAllData()
{
    const int nRow = rowCount();
    this->removeRows(0, nRow);
}

void CBaseTableModel::setTableActiveType(const QString& activeType)
{
    m_qsTaskActiveType = activeType;
}
/// /////////////////////////
/// CBaseTableView
/// ///////////////////////
CBaseTableView::CBaseTableView(QWidget* parent /* = nullptr */)
    : QTableView(parent)
{
    initColumShow();
}

CBaseTableView::~CBaseTableView()
{

}

void CBaseTableView::initColumShow()
{

}

