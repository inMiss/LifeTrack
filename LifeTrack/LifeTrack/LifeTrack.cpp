#include "LifeTrack.h"

#include <QHBoxLayout>

#include "panel/mainPanel.h"
#include "panel/nearlyTaskPanel.h"
#include "panel/taskHistoryPanel.h"
#include "panel/analysisPanel.h"
#include "panel/systemPanel.h"

#include "../common/common.h"
#include "../language/language.h"

enum ETabID
{
    PTAB_MAINPAGE_ID = 0,
    PTAB_TSAKPAGE_ID,
    PTAB_GOALPAGE_ID,
    PTAB_ANALYSISPAGE_ID,
    PTAB_SYSTEMPAGE_ID
};

LifeTrack::LifeTrack(QWidget *parent)
    : QWidget(parent)
    , m_pPageManager(nullptr)
{
   // ui.setupUi(this);
    createUi();
    updateLanguageShowText();
}

LifeTrack::~LifeTrack()
{}

void LifeTrack::createUi()
{
    this->setFixedSize(1280, 800);

    QHBoxLayout* pMainLayout = new QHBoxLayout(this);

    m_pPageManager = new QTabWidget(this);
    m_pPageManager->setTabShape(QTabWidget::Triangular);

    //��ҳ
    CMainPanel* pMainPage = new CMainPanel(m_pPageManager);
    m_pPageManager->insertTab(PTAB_MAINPAGE_ID, pMainPage, "Main");

    //��������
    CNearlyTaskPanel* pTaskPage = new CNearlyTaskPanel(m_pPageManager);
    m_pPageManager->insertTab(PTAB_TSAKPAGE_ID, pTaskPage, "Nearly Tasks");

    //������ʷ����
    CTaskHistoryPanel* pGoalPage = new CTaskHistoryPanel(m_pPageManager);
    m_pPageManager->insertTab(PTAB_GOALPAGE_ID, pGoalPage, "History Tasks");

    //�ܽ����
    CAnalysisPanel* pAnalysisPage = new CAnalysisPanel(m_pPageManager);
    m_pPageManager->insertTab(PTAB_ANALYSISPAGE_ID, pAnalysisPage, "Analysis");

    //ϵͳ����
    CSystemPanel* pSystemPage = new CSystemPanel(m_pPageManager);
    m_pPageManager->insertTab(PTAB_SYSTEMPAGE_ID, pSystemPage, "System");

    pMainLayout->addWidget(m_pPageManager);
}

void LifeTrack::updateLanguageShowText()
{
    if (m_pPageManager)
    {
        m_pPageManager->setTabText(PTAB_MAINPAGE_ID, language::CLanguageLoad::GetInstance()->getShowText("mainPage"));
        m_pPageManager->setTabText(PTAB_TSAKPAGE_ID, language::CLanguageLoad::GetInstance()->getShowText("taskPage"));
        m_pPageManager->setTabText(PTAB_GOALPAGE_ID, language::CLanguageLoad::GetInstance()->getShowText("goalPage"));
        m_pPageManager->setTabText(PTAB_ANALYSISPAGE_ID, language::CLanguageLoad::GetInstance()->getShowText("analysisPage"));
        m_pPageManager->setTabText(PTAB_SYSTEMPAGE_ID, language::CLanguageLoad::GetInstance()->getShowText("systemPage"));
    }
}
