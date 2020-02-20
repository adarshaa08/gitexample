#include "entrydlg.h"
#include "FPB.h"

extern QString GLOBAL_AGENT_ACCNUM;
//extern paramdownload_structure paramdown_struct;

///////////////////////////////////////////////////////////////////
//
// This File is used to enter password and amount screens
//
//
//  Input :
//  Type  :
//  Output:
//
//  Date  :                           Author:
///////////////////////////////////////////////////////////////////


CEntryDlg::CEntryDlg(QWidget *parent, int flg) :
    QDialog(parent),
    ui(new Ui::CEntryDlg)
{

    char clbank_bin[10];
    QSqlQuery query;
     QFont lfont("Crimson Text", 14, QFont::Bold, true);



//    query.clear();
//    query.prepare("select flagvalue from paramflag where headername='BankBin'");

//    if(!query.exec())
//    {
//        qDebug() << query.lastError();
//        return;
//    }
//    else
//    {
//        while(query.next())
//        {

//            memset(clbank_bin,0,sizeof(clbank_bin));
//            strcpy(clbank_bin,query.value(0).toString().trimmed().toAscii().data());


//        }
//    }

    ui->setupUi(this);

    //RightFlag = 0;
    audioflag = 0;
    //this->setWindowFlags(Qt::FramelessWindowHint);
    this->setWindowFlags(Qt::CustomizeWindowHint);
    this->setGeometry(0,0,320,240);
    this->setStyleSheet("color: rgb(85, 0, 127);background-color: rgb(255, 255, 255)");
    QRegExp rx("\\d+");
    QRegExpValidator *validator1 = new QRegExpValidator(rx,this);
    ui->lineEdit->setValidator(validator1);

    ui->lineEdit->setFocus();
    switch(flg)
    {
    //Operator Login
    case 1:
    {
        audioflag = 1;
        ui->label->setFont(lfont);
        ui->lineEdit->clear();
        ui->label->setText("TASK - OPERATOR LOGIN");
        ui->lineEdit->setEchoMode(QLineEdit::Password);
        ui->lineEdit->setMaxLength(4);
        ui->lineEdit->setReadOnly(false);
        //ui->label_2->hide();
        ui->label_2->setText("Enter Password:");

        break;
    }
        //Visiontek Manager
//    case 2:
//    {
//        audioflag = 1;


//        ui->lineEdit->clear();
//        ui->label->setFont(lfont);
//        ui->label->setText("TASK - CONFIG MANAGER LOGIN");
//       // ui->label->setGeometry(110,10,141,20);
//        ui->lineEdit->setEchoMode(QLineEdit::Password);
//        ui->lineEdit->setMaxLength(6);
//        ui->lineEdit->setReadOnly(false);
//        //ui->label_2->hide();
//        ui->label_2->setText("Enter Password:");
//        break;
//    }
//        //Cash Withdrawal
    case 3:
    {
        audioflag = 0;
        ui->lineEdit->clear();
        ui->label->setFont(lfont);
        ui->label->setText("CASH WITHDRAWAL");
        ui->lineEdit->setEchoMode(QLineEdit::Normal);
        ui->lineEdit->setReadOnly(false);

        QRegExpValidator *validator1 = new QRegExpValidator(rx,this);
        ui->lineEdit->setMaxLength(5);
        ui->lineEdit->setValidator(validator1);
        ui->label_2->setText("Enter Amount:");
        break;

    }
//        //Cash Deposit
//    case 4:
//    {
//        audioflag = 0;
//        ui->lineEdit->clear();
//        ui->label->setFont(lfont);
//        ui->label->setText("TASK - CASH DEPOSIT");
//        ui->lineEdit->setEchoMode(QLineEdit::Normal);
//        ui->lineEdit->setReadOnly(false);

//        QRegExpValidator *validator = new QRegExpValidator(rx,this);
//       // ui->lineEdit->setMaxLength(7);
//        ui->lineEdit->setMaxLength(5); // akshay j added 23 DEC 2015
//        ui->lineEdit->setValidator(validator);
//        ui->label_2->setText("Enter Deposit Amount:");
//        break;
//    }
//        //Enter Account Number
//    case 5:
//    {
//        audioflag = 0;
//        ui->lineEdit->clear();
//        ui->label->setFont(lfont);
//        ui->label->setText("TASK - ENTER ACCOUNT NUMBER");
//        ui->lineEdit->setEchoMode(QLineEdit::Normal);
//        ui->lineEdit->setReadOnly(false);



//        QRegExp rx1("[0-9]\\d{0,20}");
//        QRegExpValidator *validator1 = new QRegExpValidator(rx1,this);
//        ui->lineEdit->setValidator(validator1);

//        if(memcmp(clbank_bin,"607396",6) == 0) // canara
//        {
//                ui->lineEdit->setMaxLength(13);

//        }
//       else if(memcmp(clbank_bin,"607580",6) == 0) // SYNDICATE
//        {
//               ui->lineEdit->setMaxLength(14);
//        }
//        else if((memcmp(clbank_bin,"607161",6) == 0) ||(memcmp(clbank_bin,"607746",6) == 0) ||(memcmp(clbank_bin,"508653",6) == 0) ||(memcmp(clbank_bin,"607890",6) == 0) ||(memcmp(clbank_bin,"607825",6) == 0) ||(memcmp(clbank_bin,"607735",6) == 0) ||(memcmp(clbank_bin,"607865",6) == 0) ||(memcmp(clbank_bin,"607788",6) == 0) ||(memcmp(clbank_bin,"607781",6) == 0) ||(memcmp(clbank_bin,"508651",6) == 0) ||(memcmp(clbank_bin,"607923",6) == 0) ||(memcmp(clbank_bin,"607806",6) == 0) ||(memcmp(clbank_bin,"508652",6) == 0) ||(memcmp(clbank_bin,"607841",6) == 0) ||(memcmp(clbank_bin,"607757",6) == 0) ||(memcmp(clbank_bin,"607843",6) == 0) ||(memcmp(clbank_bin,"607807",6) == 0) ||(memcmp(clbank_bin,"607826",6) == 0) ||(memcmp(clbank_bin,"607758",6) == 0) ||(memcmp(clbank_bin,"607862",6) == 0) ||(memcmp(clbank_bin,"607647",6) == 0)  ) // UBI and HARCO Aadarsha
//        {
//           ui->lineEdit->setMaxLength(19);
//        }
//        else
//        {
//            ui->lineEdit->setMaxLength(19);
//        }

//        //ui->lineEdit->setValidator(validator);
//        ui->label_2->setText("Account No:");
//        break;
//    }

//    case 6:
//    {
//        audioflag = 0;
//        ui->lineEdit->clear();
//        ui->label->setFont(lfont);
//        ui->label->setText("TASK - FUND TRANSFER");
//        ui->lineEdit->setEchoMode(QLineEdit::Normal);

//        QRegExpValidator *validator = new QRegExpValidator(rx,this);
//       // ui->lineEdit->setMaxLength(7);
//        ui->lineEdit->setMaxLength(5); // akshay j added 23 DEC 2015
//        ui->lineEdit->setValidator(validator);
//        ui->lineEdit->setReadOnly(false);
//        ui->label_2->setText("Enter Transfer Amount:");
//        break;
//    }

//        //Enter OPERATOR Account Number
//    case 7:
//    {
//        audioflag = 0;
//        ui->lineEdit->clear();
//        ui->label->setFont(lfont);
//        ui->label->setText("TASK - OPERATOR LOGIN ");
//        ui->lineEdit->setEchoMode(QLineEdit::Normal);


//        QRegExp rx1("[0-9]\\d{0,20}");
//        QRegExpValidator *validator1 = new QRegExpValidator(rx1,this);
//        ui->lineEdit->setValidator(validator1);
//        ui->lineEdit->setText(GLOBAL_AGENT_ACCNUM);
//        ui->lineEdit->setReadOnly(true);
//        ui->lineEdit->setMaxLength(14);
//        ui->label_2->setText("Account No:");
//        break;
//    }

//    case 8:
//    {
//        audioflag = 0;
//        ui->lineEdit->clear();
//        ui->label->setFont(lfont);
//        ui->label->setText("TASK - ENTER BENE-ACCOUNT NUMBER");
//        ui->lineEdit->setEchoMode(QLineEdit::Normal);
//        ui->lineEdit->setReadOnly(false);
//        QRegExp rx1("[0-9]\\d{0,20}");
//        QRegExpValidator *validator1 = new QRegExpValidator(rx1,this);
//        ui->lineEdit->setValidator(validator1);

//        if(memcmp(clbank_bin,"607396",6) == 0) // canara
//        {
//                ui->lineEdit->setMaxLength(13);
//        }
//       else if(memcmp(clbank_bin,"607580",6) == 0) // SYNDICATE
//        {
//               ui->lineEdit->setMaxLength(14);
//        }
//      else if((memcmp(clbank_bin,"607161",6) == 0) ||(memcmp(clbank_bin,"607746",6) == 0) ||(memcmp(clbank_bin,"508653",6) == 0) ||(memcmp(clbank_bin,"607890",6) == 0) ||(memcmp(clbank_bin,"607825",6) == 0) ||(memcmp(clbank_bin,"607735",6) == 0) ||(memcmp(clbank_bin,"607865",6) == 0) ||(memcmp(clbank_bin,"607788",6) == 0) ||(memcmp(clbank_bin,"607781",6) == 0) ||(memcmp(clbank_bin,"508651",6) == 0) ||(memcmp(clbank_bin,"607923",6) == 0) ||(memcmp(clbank_bin,"607806",6) == 0) ||(memcmp(clbank_bin,"508652",6) == 0) ||(memcmp(clbank_bin,"607841",6) == 0) ||(memcmp(clbank_bin,"607757",6) == 0) ||(memcmp(clbank_bin,"607843",6) == 0) ||(memcmp(clbank_bin,"607807",6) == 0) ||(memcmp(clbank_bin,"607826",6) == 0) ||(memcmp(clbank_bin,"607758",6) == 0) ||(memcmp(clbank_bin,"607862",6) == 0) ||(memcmp(clbank_bin,"607647",6) == 0)  ) // UBI
//        {
//           ui->lineEdit->setMaxLength(19);
//        }
//        else
//        {
//            ui->lineEdit->setMaxLength(19);
//        }
//        ui->label_2->setText("Account No:");
//        break;
//    }
//    case 10:
//    {
//        audioflag = 1;
//        ui->label->setFont(lfont);
//        ui->lineEdit->clear();
//        ui->label->setText("TASK - ZLOG LOGIN");
//        ui->lineEdit->setEchoMode(QLineEdit::Password);
//        ui->lineEdit->setMaxLength(4);
//        ui->lineEdit->setReadOnly(false);
//        //ui->label_2->hide();
//        ui->label_2->setText("Enter Password:");

//        break;
//    }

    default: break;
    }

}

CEntryDlg::~CEntryDlg()
{
    delete ui;
}

void CEntryDlg::on_pushButton_2_clicked()
{
    //RightFlag = 1;
    emit getText("-1");
    this->close();
}


void CEntryDlg::on_BtnOk_clicked()
{
//    if(audioflag == 0)
//    {
//        CMisc miscObj;
//        char amt[8];
//        memset(amt, 0, sizeof(amt));
//        strcpy(amt,ui->lineEdit->text().toAscii().data());
//        miscObj.play(amt);
//    }

//    if(RightFlag == 0)
        //QRightStr = ui->lineEdit->text();
//    else
//        QRightStr = "56565" ;

    emit getText(ui->lineEdit->text());
    //emit getText(QRightStr);
    this->close();
}
