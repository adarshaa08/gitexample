#ifndef POPUPMSGDLG_H
#define POPUPMSGDLG_H

#include "ui_popupmsgdlg.h"

#include <QDialog>
#include <QDebug>
#include <QMovie>
#include <QSize>
#include <QPixmap>
#include <QPalette>

namespace Ui {
class CPopupMsgDlg;
}

class CPopupMsgDlg : public QDialog
{
    Q_OBJECT
    
public:
    explicit CPopupMsgDlg(QWidget *parent = 0);
    ~CPopupMsgDlg();
    //void showPopup(char *);
    void showPopup(QString);
public:
    Ui::CPopupMsgDlg *ui;

    QMovie *movie;
};

#endif // POPUPMSGDLG_H
