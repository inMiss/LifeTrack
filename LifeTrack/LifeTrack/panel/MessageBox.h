#pragma once

#include <QDialog>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>

// 十六进制颜色
//label->setStyleSheet("color: #FF0000;");   // 红色
//label->setStyleSheet("color: #00FF00;");   // 绿色
//label->setStyleSheet("color: #0000FF;");   // 蓝色


class CMessageBox : public QDialog
{
    Q_OBJECT
    enum Icon {
        Information,  // 信息
        Warning,      // 警告
        Critical,     // 错误
        Question      // 询问
    };
public:

    explicit CMessageBox(const QString& text, Icon icon = Information,
        const QString& title = "", QWidget* parent = nullptr);

    // 静态便捷方法
    static bool info(const QString& text, const QString& title = "提示", QWidget* parent = nullptr) {
        CMessageBox msg(text, Information, title, parent);
        return msg.exec() == QDialog::Accepted;
    }

    static bool warning(const QString& text, const QString& title = "警告", QWidget* parent = nullptr) {
        CMessageBox msg(text, Warning, title, parent);
        return msg.exec() == QDialog::Accepted;
    }

    static bool error(const QString& text, const QString& title = "错误", QWidget* parent = nullptr) {
        CMessageBox msg(text, Critical, title, parent);
        return msg.exec() == QDialog::Accepted;
    }

    static bool question(const QString& text, const QString& title = "询问", QWidget* parent = nullptr) {
        CMessageBox msg(text, Question, title, parent);
        return msg.exec() == QDialog::Accepted;
    }

private:
    QLabel* m_iconLabel;
    QLabel* m_textLabel;
    QPushButton* m_okButton;
    QPushButton* m_cancelButton;
};
