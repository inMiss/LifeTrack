#pragma once

#include <QWidget>
#include <QDialog>
#include <QLineEdit>
#include <QDateTimeEdit>
#include <QComboBox>
#include <QCheckBox>
#include <QObject>

#include "../management/dataCenter.h"
#include "baseTable.h"
class CAddTaskDialog : public QDialog
{
    Q_OBJECT

protected Q_SLOTS:
    void slot_importTypeChange(int index);
    void slot_repeatTypeChange(int index);
public:
    CAddTaskDialog(const STaskListPtr& task = nullptr,QWidget *parent = nullptr);
    ~CAddTaskDialog();

    void createUi(const STaskListPtr& task = nullptr);
    QMap<QString, STaskListPtr> getTaskInformation();
private:
    void on_confirmBen_onClick();
    void on_cancelBen_onClick();

    bool isContentAllRight();
    void makeTaskInformation(STaskListPtr& task,const QString& parent = "");
    void updateTableData(const QString& repeatType);
private:
    QLineEdit* m_pTaskNmaeEdit;
    QDateTimeEdit* m_pBeignDate;
    QDateTimeEdit* m_pEndData;
    QComboBox* m_pImprotLevelCom;
    QLineEdit* m_pTaskValueEdit;
    QComboBox* m_pRepeatTypeCom;
    QComboBox* m_pBelongTypeCom;

    QCheckBox* m_pDeductionCheck;
    QCheckBox* m_pCountdownCheck;
    QCheckBox* m_pCanPauseCheck;

    QComboBox* m_pStatusTypeCom;
    QLineEdit* m_pRemarkEdit;

    QMap<QString, SImportLevelDictionary> m_mapImportType;

    CBaseTableModel* m_pTableModel;
    QTableView* m_pTableView;
    QMap<QString, STaskListPtr> m_mapTasks;
};
