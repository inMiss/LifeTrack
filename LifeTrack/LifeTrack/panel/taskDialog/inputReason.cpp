#include "inputReason.h"
#include "../language/language.h"
#include "../common/commonFunc.h"

#include <QString>
#include <QMap>
#include <QVariant>

#include <QVBoxLayout>

CInputReason::CInputReason(QWidget* parent /* = nullptr */)
    : QDialog(parent)
    , m_pReasonEdit(nullptr)
    , m_pReasonLabel(nullptr)

{
    createUi();
}

CInputReason::~CInputReason()
{

}

void CInputReason::createUi()
{
    QVBoxLayout* pMainLayout = new QVBoxLayout(this);
    //任务名称
    {
        QString qsTemp = getLanguegeShowText("inputReason_default_text") + ": ";
        m_pReasonLabel = new QLabel(qsTemp);
        pMainLayout->addWidget(m_pReasonLabel);

        qsTemp = getLanguegeShowText("inputReason_default_editShow") + ": ";
        m_pReasonEdit = new QLineEdit();
        m_pReasonEdit->setText(qsTemp);
        pMainLayout->addWidget(m_pReasonEdit);
    }

    {
        // 按钮
        m_okButton = new QPushButton(getLanguegeShowText("messageBox_btn_confirm"));
        m_cancelButton = new QPushButton(getLanguegeShowText("messageBox_btn_cancel"));
        connect(m_okButton, &QPushButton::clicked, this, &CInputReason::on_confirmBen_onClick);
        connect(m_cancelButton, &QPushButton::clicked, this, &CInputReason::on_cancelBen_onClick);
        QHBoxLayout* pBtnLayout = new QHBoxLayout();
        pBtnLayout->addWidget(m_okButton);
        pBtnLayout->addWidget(m_cancelButton);
        pMainLayout->addLayout(pBtnLayout);
    }

}


QString CInputReason::getReason()
{
    return m_pReasonEdit->text();
}

void CInputReason::on_confirmBen_onClick()
{
    accept();
}

void CInputReason::on_cancelBen_onClick()
{
    reject();
}