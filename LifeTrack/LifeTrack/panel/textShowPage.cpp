#include "textShowPage.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
CTextShowPage::CTextShowPage(QWidget *parent)
    : CCustomWidget(parent)
{
    createUi();
}

CTextShowPage::~CTextShowPage()
{}

void CTextShowPage::createUi()
{
    QHBoxLayout* pMainLayout = new QHBoxLayout(this);

    m_pTextLabel = new QLabel("Again And Again,one day ,you will be success!!");
    pMainLayout->addWidget(m_pTextLabel);
    m_pAuthorLabel = new QLabel(" ----- Author");
    pMainLayout->addWidget(m_pAuthorLabel);
}
