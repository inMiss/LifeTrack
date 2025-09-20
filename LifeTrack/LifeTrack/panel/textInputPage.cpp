#include "textInputPage.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
CTextInputPage::CTextInputPage(QWidget *parent)
    : CCustomWidget(parent)
{
    createUi();
}

CTextInputPage::~CTextInputPage()
{}

void CTextInputPage::createUi()
{
    QVBoxLayout* pMainLayout = new QVBoxLayout(this);

    QHBoxLayout* pUpLayout = new QHBoxLayout();
    m_pShowTextLabel = new QLabel("Today Summary");
    pUpLayout->addWidget(m_pShowTextLabel);

    m_pSaveBtn = new QPushButton("Save");
    pUpLayout->addWidget(m_pSaveBtn);
    
    m_pTextEdit = new QLineEdit();
    m_pTextEdit->setText("Please write today summary...");
    m_pTextEdit->setFixedHeight(150);

    pMainLayout->addLayout(pUpLayout);
    pMainLayout->addWidget(m_pTextEdit);
    
}
