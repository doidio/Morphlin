/********************************************************************************
** Form generated from reading UI file 'Morphlin.ui'
**
** Created by: Qt User Interface Compiler version 5.3.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MORPHLIN_H
#define UI_MORPHLIN_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MorphlinClass
{
public:
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QWidget *centralWidget;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MorphlinClass)
    {
        if (MorphlinClass->objectName().isEmpty())
            MorphlinClass->setObjectName(QStringLiteral("MorphlinClass"));
        MorphlinClass->resize(600, 400);
        menuBar = new QMenuBar(MorphlinClass);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        MorphlinClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(MorphlinClass);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        MorphlinClass->addToolBar(mainToolBar);
        centralWidget = new QWidget(MorphlinClass);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        MorphlinClass->setCentralWidget(centralWidget);
        statusBar = new QStatusBar(MorphlinClass);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        MorphlinClass->setStatusBar(statusBar);

        retranslateUi(MorphlinClass);

        QMetaObject::connectSlotsByName(MorphlinClass);
    } // setupUi

    void retranslateUi(QMainWindow *MorphlinClass)
    {
        MorphlinClass->setWindowTitle(QApplication::translate("MorphlinClass", "Morphlin", 0));
    } // retranslateUi

};

namespace Ui {
    class MorphlinClass: public Ui_MorphlinClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MORPHLIN_H
