#ifndef ENTRYDLG_H
#define ENTRYDLG_H

#include <QDialog>
#include <QDebug>
#include <QRegExp>

#include "ui_entrydlg.h"
#include "finomisc.h"

namespace Ui {
class CEntryDlg;
}

class CEntryDlg : public QDialog
{
    Q_OBJECT
    
public:
    explicit CEntryDlg(QWidget *parent = 0, int val=0);
    ~CEntryDlg();

    int audioflag;
    QString QRightStr;
    //int RightFlag;
    //QString getText();
private slots:
    void on_pushButton_2_clicked();

    void on_BtnOk_clicked();
signals:
    void getText(QString);
public:
    Ui::CEntryDlg *ui;

};

#endif // ENTRYDLG_H
