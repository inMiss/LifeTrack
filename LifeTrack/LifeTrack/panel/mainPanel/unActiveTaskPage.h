#pragma once

#include <QWidget>
#include <QStandardItemModel>
#include <QTableView>
#include <QObject>

#include <QLabel>
#include <QPushButton>
#include <QDateEdit>
#include <QEvent>
#include <QMap>
#include <QComboBox>

#include "../table/baseTable.h"
#include "../customWidget.h"
#include "../../management/dataCenter.h"
#include "../management/structCommon.h"
class CUnActiveTaskPage : public CCustomWidget
{
    Q_OBJECT

signals:
    void sigTaskOperation(const QString& task_id, const int nRow, const QString& taskActiveType, const QString& taskOperType);
private slots:
    void slotsButtonClicked(int row, QString operatorId) ;
    void slotsFilterComBoxChange(int index);
public:
    CUnActiveTaskPage(QWidget *parent = nullptr);
    ~CUnActiveTaskPage();

    void createUi();
    void updateTableData(const QMap<QString, STaskListPtr>& data);
    void updateTableData(const STaskListPtr& data);
    void deleteTableDataByRow(const int nRow);
    void initTableData();
protected:
    //bool event(QEvent* even) override;
    //void mousePressEvent(QMouseEvent* event) override;
private:
    
    
    void resetTableViewColumnSize();

private:
    void onDateEditChange();
private:
    CBaseTableModel* m_pTableModel;
    CBaseTableView* m_pTableView;

    QLabel* m_pShowLabel;
    QComboBox* m_pFilterTypeCombox;
    STaskListPtr m_tempAddDiaTaskPtr;
};
