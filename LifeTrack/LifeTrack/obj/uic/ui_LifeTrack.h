/********************************************************************************
** Form generated from reading UI file 'LifeTrack.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_LIFETRACK_H
#define UI_LIFETRACK_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_LifeTrackClass
{
public:
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QWidget *centralWidget;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *LifeTrackClass)
    {
        if (LifeTrackClass->objectName().isEmpty())
            LifeTrackClass->setObjectName(QString::fromUtf8("LifeTrackClass"));
        LifeTrackClass->resize(600, 400);
        menuBar = new QMenuBar(LifeTrackClass);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        LifeTrackClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(LifeTrackClass);
        mainToolBar->setObjectName(QString::fromUtf8("mainToolBar"));
        LifeTrackClass->addToolBar(mainToolBar);
        centralWidget = new QWidget(LifeTrackClass);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        LifeTrackClass->setCentralWidget(centralWidget);
        statusBar = new QStatusBar(LifeTrackClass);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        LifeTrackClass->setStatusBar(statusBar);

        retranslateUi(LifeTrackClass);

        QMetaObject::connectSlotsByName(LifeTrackClass);
    } // setupUi

    void retranslateUi(QMainWindow *LifeTrackClass)
    {
        LifeTrackClass->setWindowTitle(QCoreApplication::translate("LifeTrackClass", "LifeTrack", nullptr));
    } // retranslateUi

};

namespace Ui {
    class LifeTrackClass: public Ui_LifeTrackClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_LIFETRACK_H
