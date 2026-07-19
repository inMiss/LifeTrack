#pragma once

#include <QWidget>
#include <QObject>
#include <QTableView>
#include <qdate>
#include <QAbstractTableModel>
#include <QStandardItemModel>
#include <QStyledItemDelegate>
#include <QStyleOptionButton>

#include <QItemDelegate>

#include "../../management/structCommon.h"

//
// 代理所涉及到的一些预定义
//

//表格可能使用到的代理
// 按钮数据结构
struct ButtonData {
    QString text;   // 显示的文字
    QString id;         // 按钮ID，用于区分不同含义-具体的操作

    ButtonData(const QString& t = "", const QString& id = "") : text(t), id(id) {}
    // 为了让QMetaType能够正确工作，需要提供默认构造函数和拷贝构造函数
    ButtonData(const ButtonData& other) = default;
    ButtonData& operator=(const ButtonData& other) = default;
};

// 注册元类型，以便在QVariant中使用
Q_DECLARE_METATYPE(ButtonData)
Q_DECLARE_METATYPE(QList<ButtonData>)

//
// 该种列表涉及到的公共特殊公共函数
//
QList<ButtonData> getTaskOperatorButton(const QString& qsTaskStatus, const QString& qsActiveType = QString(TASK_ACTIVETYPE_NO_ACTIVE));
//

class MultipleButtonDelegate : public QStyledItemDelegate
{
    Q_OBJECT
signals:
    void buttonClicked(int row, QString operatorId) const;
private slots:
    void onButtonClicked() const;
public:
    //explicit 修饰构造函数，让其构造只能显示调用，不可隐士
    explicit MultipleButtonDelegate(QObject* parent = nullptr);

//     // 创建按钮
//     QWidget* createEditor(QWidget* parent, const QStyleOptionViewItem& option,
//         const QModelIndex& index) const override;

    // 更新位置（必须实现）
    void updateEditorGeometry(QWidget* editor, const QStyleOptionViewItem& option,
        const QModelIndex& index) const override;

    // 绘制按钮（一直显示）
    void paint(QPainter* painter, const QStyleOptionViewItem& option,
        const QModelIndex& index) const override;

    // 处理鼠标点击
    bool editorEvent(QEvent* event, QAbstractItemModel* model,
        const QStyleOptionViewItem& option,
        const QModelIndex& index) override;

    // 返回大小提示
//     QSize sizeHint(const QStyleOptionViewItem& option,
//         const QModelIndex& index) const override;

private:
    // 获取按钮区域
    QList<QRect> getButtonRects(const QStyleOptionViewItem& option,
        const QList<ButtonData>& buttons) const;

    // 绘制单个按钮
    void drawButton(QPainter* painter, const QRect& rect,
        const ButtonData& button, bool isPressed, bool isHovered) const;

    mutable int m_hoverButtonIndex;  // 当前悬停的按钮
    mutable int m_pressedButtonIndex; // 当前按下的按钮
};

//task list
enum ETaskTableColumn
{
    E_TASKETTABLE_CHECK = 0,
    E_TASKETTABLE_ID,
    E_TASKETTABLE_NAME,
    E_TASKETTABLE_STARTDATE,
    E_TASKETTABLE_DEADLINE,
    E_TASKETTABLE_CREATETIME,
    E_TASKETTABLE_STARTTIME,
    E_TASKETTABLE_FINISHTIME,
    E_TASKETTABLE_DURINGTIME,
    E_TASKETTABLE_IMPORTLEVEL,
    E_TASKETTABLE_VALUE,
    E_TASKETTABLE_SUMPOINTS,
    E_TASKETTABLE_REPEATTYPE,
    E_TASKETTABLE_FINISHTIMES,
    E_TASKETTABLE_FAILTIMES,
    E_TASKETTABLE_BELONGTYPE,
    E_TASKETTABLE_DEDUCTION,
    E_TASKETTABLE_COUNTDOWN,
    E_TASKETTABLE_CANPAUSE,
    E_TASKETTABLE_TASKOPERATOR,
    E_TASKETTABLE_FAILTHRESHOLD,
    E_TASKETTABLE_TASKSTATUS,
    E_TASKETTABLE_STATUERESAON,
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
    void addData(const STaskListPtr& data, const QString& taskStatue = TASK_STATUS_UNSTART);
    void setFilterDate(const QDate& date, const bool bFilter = true);
    void clearAllData();
    void setTableActiveType(const QString& activeType);
private:
    void initHorizontalHeaderLabels();   
private:
    QDate m_filterDate;//设定最大的创建日期
    bool  m_bFilter;
    QString m_qsTaskActiveType;//当前表格是哪个表格
};

//所有任务
class CBaseTableView : public QTableView
{
    Q_OBJECT

signals:
    void buttonClicked(int row, QString operatorId) const;

public:
    CBaseTableView(QWidget* parent = nullptr);
    ~CBaseTableView();
private:
    void initColumShow();
};