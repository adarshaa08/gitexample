/********************************************************************************
** Form generated from reading UI file 'popmsgdlg.ui'
**
** Created: Sun Nov 15 18:49:50 2015
**      by: Qt User Interface Compiler version 4.7.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_POPMSGDLG_H
#define UI_POPMSGDLG_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>

QT_BEGIN_NAMESPACE

class Ui_CPopMsgDlg
{
public:
    QLabel *label;

    void setupUi(QDialog *CPopMsgDlg)
    {
        if (CPopMsgDlg->objectName().isEmpty())
            CPopMsgDlg->setObjectName(QString::fromUtf8("CPopMsgDlg"));
        CPopMsgDlg->resize(320, 240);
        label = new QLabel(CPopMsgDlg);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(-2, -2, 331, 251));
        //label->setGeometry(QRect(-2, -2, 300, 30));
        label->setMinimumSize(QSize(320, 240));
        label->setAlignment(Qt::AlignCenter);

        retranslateUi(CPopMsgDlg);

        QMetaObject::connectSlotsByName(CPopMsgDlg);
    } // setupUi

    void retranslateUi(QDialog *CPopMsgDlg)
    {
        CPopMsgDlg->setWindowTitle(QApplication::translate("CPopMsgDlg", "Dialog", 0, QApplication::UnicodeUTF8));
        label->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class CPopMsgDlg: public Ui_CPopMsgDlg {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_POPMSGDLG_H
