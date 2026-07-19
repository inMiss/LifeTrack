#include "MessageBox.h"
#include <QIcon>

CMessageBox::CMessageBox(const QString& text, Icon icon,
    const QString& title, QWidget* parent)
    : QDialog(parent)
{
    setWindowTitle(title.isEmpty() ? "提示" : title);
    setMinimumWidth(300);

    // 创建布局
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    QHBoxLayout* contentLayout = new QHBoxLayout();
    QHBoxLayout* buttonLayout = new QHBoxLayout();

    // 图标
    m_iconLabel = new QLabel();
    QString iconPath;
    switch (icon) {
    case Information:
        //iconPath = ":/icons/info.png";  // 使用你的图标路径
        m_iconLabel->setText("消息");
        break;
    case Warning:
       // iconPath = ":/icons/warning.png";
        m_iconLabel->setText("警告");
        break;
    case Critical:
       // iconPath = ":/icons/error.png";
        m_iconLabel->setText("错误");
        break;
    case Question:
        //iconPath = ":/icons/question.png";
        m_iconLabel->setText("问题");
        break;
    }
    m_iconLabel->setAlignment(Qt::AlignCenter);
    m_iconLabel->setFixedSize(32, 32);
    m_iconLabel->setStyleSheet("font-size: 24px;");

    // 文本
    m_textLabel = new QLabel(text);
    m_textLabel->setWordWrap(true);
    m_textLabel->setMinimumWidth(200);

    // 内容布局
    contentLayout->addWidget(m_iconLabel);
    contentLayout->addWidget(m_textLabel);
    contentLayout->setSpacing(15);

    // 按钮
    m_okButton = new QPushButton("确定");
    m_cancelButton = new QPushButton("取消");

    if (icon == Question || Warning == icon) {
        // 询问对话框显示确定和取消
        buttonLayout->addStretch();
        buttonLayout->addWidget(m_okButton);
        buttonLayout->addWidget(m_cancelButton);

        connect(m_okButton, &QPushButton::clicked, this, &QDialog::accept);
        connect(m_cancelButton, &QPushButton::clicked, this, &QDialog::reject);
    }
    else {
        // 提示对话框只显示确定
        buttonLayout->addStretch();
        buttonLayout->addWidget(m_okButton);
        m_cancelButton->hide();

        connect(m_okButton, &QPushButton::clicked, this, &QDialog::accept);
    }

    // 主布局
    mainLayout->addLayout(contentLayout);
    mainLayout->addSpacing(10);
    mainLayout->addLayout(buttonLayout);
    mainLayout->setSpacing(10);
    mainLayout->setContentsMargins(20, 20, 20, 20);

    // 设置默认按钮
    m_okButton->setDefault(true);
    m_okButton->setFocus();
}


