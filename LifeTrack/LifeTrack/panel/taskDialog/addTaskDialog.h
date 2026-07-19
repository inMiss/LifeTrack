#pragma once

#include <QWidget>
#include <QDialog>
#include <QLineEdit>
#include <QDateTimeEdit>
#include <QComboBox>
#include <QCheckBox>
#include <QObject>
#include <QHBoxLayout>
#include <QLabel>

#include "../management/dataCenter.h"
#include "../table/baseTable.h"
class CAddTaskDialog : public QDialog
{
    Q_OBJECT

protected Q_SLOTS:
    void slot_importTypeChange(int index);
    void slot_repeatTypeChange(int index);
public:
    CAddTaskDialog(const STaskListPtr& task = nullptr, const QString& activeType = QString(TASK_ACTIVETYPE_NO_ACTIVE), const bool bEdit = false, const bool bUnStrat = false, QWidget* parent = nullptr);
    ~CAddTaskDialog();

    void createUi(const STaskListPtr& task = nullptr);
    void getTaskInformation(STaskListPtr& task, const QString& parent = "");
private:
    void on_confirmBen_onClick();
    void on_cancelBen_onClick();

    bool isContentAllRight();
    void makeTaskInformation(STaskListPtr& task,const QString& parent = "");
    void updateDateUiShow();

    void updateUiEnableStatus();
private:
    QLineEdit* m_pTaskNmaeEdit;

    QLabel* m_pStartDateLabel;
    QDateEdit* m_pStartDate;
    QHBoxLayout* m_pStartDateLayout;

    QLabel* m_pDeadLineLabel;
    QDateEdit* m_pDeadLine;
    QHBoxLayout* m_pDeadLineLayout;

    QComboBox* m_pImprotLevelCom;
    QLineEdit* m_pTaskValueEdit;
    QComboBox* m_pRepeatTypeCom;
    QComboBox* m_pBelongTypeCom;

    QCheckBox* m_pDeductionCheck;
    QCheckBox* m_pCountdownCheck;
    QCheckBox* m_pCanPauseCheck;

    QLineEdit* m_pFailThresholdEdit;

    QComboBox* m_pStatusTypeCom;
    QLineEdit* m_pRemarkEdit;

    QMap<QString, SImportLevelDictionary> m_mapImportType;
    STaskListPtr m_tasks;
    bool m_bEdit;
    QString m_qsActiveType;
    bool m_bUnStartTask;
};
