#include "popmsgdlg.h"
#include "ui_popmsgdlg.h"

CPopMsgDlg::CPopMsgDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CPopMsgDlg)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::FramelessWindowHint);
    QPalette Palet = this->palette();
    QPixmap pix(":/images/Please-wait.png");
    QPixmap pix_scaled(pix.scaled(ui->label->width(),ui->label->height(),  Qt::IgnoreAspectRatio, Qt::SmoothTransformation ));
    Palet.setBrush(QPalette::Window, QBrush(pix_scaled));
    ui->label->setPalette(Palet);
   // ui->label->setGeometry();
    ui->label->setText("PLEASE WAIT....");
    //ui->label->setGeometry();
    QFont font;
    font.bold();
    ui->label->setFont(font);
    ui->label->setAutoFillBackground(true);
}

CPopMsgDlg::~CPopMsgDlg()
{
    delete ui;
}


void CPopMsgDlg::showPopup(QString str)
{
    ui->label->setText(str);
    update();

    return;
}
