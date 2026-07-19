#pragma once

#include <QWidget>
#include <QComboBox>
#include <QLineEdit>
#include <QPushButton>
#include <QObject>
#include <QLabel>

#include "../management/dataCenter.h"

class CSystemPanel : public QWidget
{
    Q_OBJECT

protected Q_SLOTS:
    

public:
    CSystemPanel(QWidget *parent = nullptr);
    ~CSystemPanel();

    void createUi();
private:

private:
    QLabel* m_pLanguageLable;
    QComboBox* m_pLanguageCom;

    QLabel* m_pRewardLabel;
    QComboBox* m_pRewardTypeCom;
    QLineEdit* m_pRewardEdit;
    QLabel* m_pRewardValueLabel;
    QLineEdit* m_pRewardValueEdit;   
    QLabel* m_pRewardHadLabel;
    QComboBox* m_pRewardHadCom;
    QPushButton* m_pRewardAddBtn;

    QLabel* m_pFirstBelongTypeLabel;
    QLineEdit* m_pFirstBelongTypeEdit;
    QPushButton* m_pFirstBelongTypeBtn;
    QLabel* m_pFirstBelongTypeHadLabel;//实时展示所有的一级类型
    QComboBox* m_pFirstBelongTypeHadCom;

    QLabel* m_pSecondBelongTypeLabel;
    QLineEdit* m_pSecondBelongTypeEdit;
    QPushButton* m_pSecondBelongTypeBtn;
    QLabel* m_pSecondBelongTypeHadLabel;//实时展示所有的二级类型，但是每次一级类型变化时，需要进行选择
    QComboBox* m_pSecondBelongTypeHadCom;

    QPushButton* m_pAboutBtn;
    QPushButton* m_pUpdateRecordBtn;
    QPushButton* m_pLocalNetDataChangeBtn;
};
