#pragma once

#include <QWidget>
#include <QStandardItemModel>
#include <QTableView>

#include <QLabel>
#include <QPushButton>
#include <QDateEdit>
#include <QEvent>
#include <QMap>
#include <QObject>
#include <QComboBox>

#include "../table/baseTable.h"
#include "../customWidget.h"
#include "../../management/dataCenter.h"
#include "../management/structCommon.h"
class CAnalysisPanel : public QWidget
{
    Q_OBJECT

signals:
    void sigTaskOperation(const QString& task_id, const int nRow, const QString& taskActiveType, const QString& taskOperType);
public slots :
    void slotsButtonClicked(int row, QString operatorId);
    void slotUpdateTable();
public:
    CAnalysisPanel(QWidget* parent = nullptr);
    ~CAnalysisPanel();

    void createUi();
    void updateTableData(const QMap<QString, STaskListPtr>& data);
    void updateTableData(const STaskListPtr& data);
    void deleteTableDataByRow(const int nRow);
    void initTableData();

private:
    void resetTableViewColumnSize();

private:
    CBaseTableModel* m_pTableModel;
    CBaseTableView* m_pTableView;

    QLabel* m_pShowLabel;
    STaskListPtr m_tempAddDiaTaskPtr;
};
