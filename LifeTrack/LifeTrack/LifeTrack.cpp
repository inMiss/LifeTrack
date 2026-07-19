#include "LifeTrack.h"

#include <QHBoxLayout>

#include "common/common.h"
#include "language/language.h"

enum ETabID
{
    PTAB_MAINPAGE_ID = 0,
    PTAB_ANALYSISPAGE_ID,
    PTAB_SYSTEMPAGE_ID
};

LifeTrack::LifeTrack(QWidget *parent)
    : QWidget(parent)
    , m_pPageManager(nullptr)
    , m_pMainPage(nullptr)
    , m_pAnalysisPage(nullptr)
    , m_pSystemPage(nullptr)
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

    //主页
    m_pMainPage = new CMainPanel(m_pPageManager);
    m_pPageManager->insertTab(PTAB_MAINPAGE_ID, m_pMainPage, "Main");

    //总结分析--对上面的数据进行分析
    m_pAnalysisPage = new CAnalysisPanel(m_pPageManager);
    m_pPageManager->insertTab(PTAB_ANALYSISPAGE_ID, m_pAnalysisPage, "Analysis");
    connect(m_pAnalysisPage, &CAnalysisPanel::sigTaskOperation, this, &LifeTrack::slotTaskOperation);

    connect(m_pMainPage, &CMainPanel::sigUpdateCompleteTaskTable, m_pAnalysisPage, &CAnalysisPanel::slotUpdateTable);
    //系统设置--设置一些选择
    m_pSystemPage = new CSystemPanel(m_pPageManager);
    m_pPageManager->insertTab(PTAB_SYSTEMPAGE_ID, m_pSystemPage, "System");

    pMainLayout->addWidget(m_pPageManager);
}

void LifeTrack::updateLanguageShowText()
{
    if (m_pPageManager)
    {
        m_pPageManager->setTabText(PTAB_MAINPAGE_ID, language::CLanguageLoad::GetInstance()->getShowText("mainPage"));
        m_pPageManager->setTabText(PTAB_ANALYSISPAGE_ID, language::CLanguageLoad::GetInstance()->getShowText("analysisPage"));
        m_pPageManager->setTabText(PTAB_SYSTEMPAGE_ID, language::CLanguageLoad::GetInstance()->getShowText("systemPage"));
    }
}

void LifeTrack::slotTaskOperation(const QString& task_id, const int nRow, const QString& taskActiveType, const QString& taskOperType)
{
    if (m_pMainPage)
    {
        m_pMainPage->slotTaskOperation_outerUse(task_id, nRow, taskActiveType, taskOperType);
    }
}

