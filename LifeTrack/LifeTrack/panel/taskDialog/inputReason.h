#pragma once

#include <QWidget>
#include <QDialog>
#include <QLineEdit>
#include <QObject>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>

class CInputReason : public QDialog
{
    Q_OBJECT
public:
    CInputReason(QWidget* parent = nullptr);
    ~CInputReason();

    void createUi();
    QString getReason();
private:
    void on_confirmBen_onClick();
    void on_cancelBen_onClick();
private:
    QLabel* m_pReasonLabel;
    QLineEdit* m_pReasonEdit;
    QPushButton* m_okButton;
    QPushButton* m_cancelButton;
};
