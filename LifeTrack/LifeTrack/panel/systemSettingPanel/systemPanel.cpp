#include "systemPanel.h"

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QVariant>
#include <QMap>
CSystemPanel::CSystemPanel(QWidget *parent)
    : QWidget(parent)
{
    createUi();
}

CSystemPanel::~CSystemPanel()
{}

void CSystemPanel::createUi()
{
    QVBoxLayout* pMainLayout = new QVBoxLayout(this);

    {
        QHBoxLayout* pLanguageLayout = new QHBoxLayout();
        m_pLanguageLable = new QLabel("Language:");
        m_pLanguageCom = new QComboBox();
        m_pLanguageCom->addItem(QString("chinese"), QVariant(e_language_chinese));
        m_pLanguageCom->addItem(QString("english"), QVariant(e_language_english));

        pLanguageLayout->addWidget(m_pLanguageLable);
        pLanguageLayout->addWidget(m_pLanguageCom);

        pMainLayout->addLayout(pLanguageLayout);
    }

    {
        QHBoxLayout* pRewardLayout = new QHBoxLayout();
        m_pRewardLabel = new QLabel("Reward:");
        pRewardLayout->addWidget(m_pRewardLabel);

        {
            QVBoxLayout* pRewardSetLayout = new QVBoxLayout();
            {
                QHBoxLayout* pValueLayout = new QHBoxLayout();

                m_pRewardTypeCom = new QComboBox();
                QMap<QString, SRewardsTypeDictionary> rewardType = CDataCenter::GetInstance()->getRewardsTypeDictionary();
                QMap<QString, SRewardsTypeDictionary>::const_iterator iter = rewardType.begin();
                for (iter; iter != rewardType.end(); ++iter)
                {
                    m_pRewardTypeCom->addItem(iter->show_content, iter->type);
                }
                pValueLayout->addWidget(m_pRewardTypeCom);

                m_pRewardEdit = new QLineEdit();
                pValueLayout->addWidget(m_pRewardEdit);

                m_pRewardValueLabel = new QLabel(QString("达标分值:"));
                pValueLayout->addWidget(m_pRewardValueLabel);

                m_pRewardValueEdit = new QLineEdit();
                pValueLayout->addWidget(m_pRewardValueEdit);

                pRewardSetLayout->addLayout(pValueLayout);
            }

            {
                QHBoxLayout* pShowLayout = new QHBoxLayout();

                m_pRewardHadLabel = new QLabel(QString("奖励展示:"));
                pShowLayout->addWidget(m_pRewardHadLabel);

                m_pRewardHadCom = new QComboBox();
                QMap<QString, SRewards> mapReward = CDataCenter::GetInstance()->getAllRewards();
                QMap<QString, SRewards>::const_iterator iter = mapReward.begin();
                for (iter; iter != mapReward.end(); ++iter)
                {
                    m_pRewardTypeCom->addItem(iter->content, iter->id);
                }
                pShowLayout->addWidget(m_pRewardHadCom);

                m_pRewardAddBtn = new QPushButton("Add");
                pShowLayout->addWidget(m_pRewardAddBtn);

                pRewardSetLayout->addLayout(pShowLayout);
            }

            pRewardLayout->addLayout(pRewardSetLayout);
        }
        pMainLayout->addLayout(pRewardLayout);
    }

    QString currentFirstType = "";
    {
        QHBoxLayout* pFirstLayout = new QHBoxLayout();
        m_pFirstBelongTypeLabel = new QLabel("First Belong:");
        pFirstLayout->addWidget(m_pFirstBelongTypeLabel);

        {
            QHBoxLayout* pAddLayout = new QHBoxLayout();

            m_pFirstBelongTypeEdit = new QLineEdit();
            pAddLayout->addWidget(m_pFirstBelongTypeEdit);

            m_pFirstBelongTypeBtn = new QPushButton("Add");
            pAddLayout->addWidget(m_pFirstBelongTypeBtn);

            m_pFirstBelongTypeHadLabel = new QLabel("All Type:");
            pAddLayout->addWidget(m_pFirstBelongTypeHadLabel);

            m_pFirstBelongTypeHadCom = new QComboBox();
            QMap<QString, SBelongTypeDictionary> mapRewardDic = CDataCenter::GetInstance()->getBelongTypeDictionary();
            QMap<QString, SBelongTypeDictionary>::const_iterator iter = mapRewardDic.begin();
            for (iter; iter != mapRewardDic.end(); ++iter)
            {
                if(iter->parent_belong_id.isEmpty())
                {
                    m_pFirstBelongTypeHadCom->addItem(iter->show_content, iter->belong_id);
                }
            }
            pAddLayout->addWidget(m_pFirstBelongTypeHadCom);
            currentFirstType = m_pFirstBelongTypeHadCom->currentData().toString();
            pFirstLayout->addLayout(pAddLayout);
        }
        pMainLayout->addLayout(pFirstLayout);
    }

    {
        QHBoxLayout* pSecondLayout = new QHBoxLayout();
        m_pSecondBelongTypeLabel = new QLabel("Second Belong:");
        pSecondLayout->addWidget(m_pSecondBelongTypeLabel);

        {
            QHBoxLayout* pAddLayout = new QHBoxLayout();

            m_pSecondBelongTypeEdit = new QLineEdit();
            pAddLayout->addWidget(m_pSecondBelongTypeEdit);

            m_pSecondBelongTypeBtn = new QPushButton("Add");
            pAddLayout->addWidget(m_pSecondBelongTypeBtn);

            m_pSecondBelongTypeHadLabel = new QLabel("All Type:");
            pAddLayout->addWidget(m_pSecondBelongTypeHadLabel);

            m_pSecondBelongTypeHadCom = new QComboBox();
            QMap<QString, SBelongTypeDictionary> mapRewardDic = CDataCenter::GetInstance()->getBelongTypeDictionary();
            QMap<QString, SBelongTypeDictionary>::const_iterator iter = mapRewardDic.begin();
            for (iter; iter != mapRewardDic.end(); ++iter)
            {
                if (iter->parent_belong_id == currentFirstType)
                {
                    m_pSecondBelongTypeHadCom->addItem(iter->show_content, iter->belong_id);
                }
            }
            pAddLayout->addWidget(m_pSecondBelongTypeHadCom);
            pSecondLayout->addLayout(pAddLayout);
        }
        pMainLayout->addLayout(pSecondLayout);
    }

    m_pAboutBtn = new QPushButton("About");
    pMainLayout->addWidget(m_pAboutBtn);

    m_pUpdateRecordBtn = new QPushButton("Update Record");
    pMainLayout->addWidget(m_pUpdateRecordBtn);

    m_pLocalNetDataChangeBtn = new QPushButton("Syn Net");
    m_pLocalNetDataChangeBtn->hide();
    pMainLayout->addWidget(m_pLocalNetDataChangeBtn);
}
