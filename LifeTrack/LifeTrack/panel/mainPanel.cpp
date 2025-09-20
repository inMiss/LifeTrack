#include "mainPanel.h"

#include <QHBoxLayout>
#include <QVBoxLayout>
CMainPanel::CMainPanel(QWidget *parent)
    : QWidget(parent)
{
    createUi();
}

void CMainPanel::createUi()
{
    QVBoxLayout* pMainLayout = new QVBoxLayout(this);

    QHBoxLayout* pUpLayout = new QHBoxLayout();

    m_pTodayTaskPage = new CTodayTaskPage(this);
    pUpLayout->addWidget(m_pTodayTaskPage);

    QHBoxLayout* pDownLayout = new QHBoxLayout();
    m_pTextInputPage = new CTextInputPage(this);
    pDownLayout->addWidget(m_pTextInputPage);

    m_pTextShowPage = new CTextShowPage(this);

    pMainLayout->addWidget(m_pTextShowPage);
    pMainLayout->addLayout(pUpLayout);
    pMainLayout->addLayout(pDownLayout);
}

CMainPanel::~CMainPanel()
{}
