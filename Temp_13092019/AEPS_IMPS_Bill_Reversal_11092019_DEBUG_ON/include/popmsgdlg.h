#ifndef POPMSGDLG_H
#define POPMSGDLG_H

#include "ui_popmsgdlg.h"

#include <QDialog>
#include <QPalette>
#include <QPixmap>


namespace Ui {
class CPopMsgDlg;
}

class CPopMsgDlg : public QDialog
{
    Q_OBJECT
    
public:
    explicit CPopMsgDlg(QWidget *parent = 0);
    ~CPopMsgDlg();
    void showPopup(QString);
    
public:
    Ui::CPopMsgDlg *ui;
};

#endif // POPMSGDLG_H
