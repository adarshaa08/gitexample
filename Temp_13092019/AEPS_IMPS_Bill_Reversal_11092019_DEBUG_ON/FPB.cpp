#include "FPB.h"
#include "ui_FPB.h"
#include "database.h"
#include "displaymessage.h"
#include "getprofile.h"
#include "aadhaar.h"
#include "entrydlg.h"
#include "Login.h"
#include "impscommon.h"
//#include <QMovie>
//#include <QIODevice>
#include <fstream>
//this is really boring file
extern "C"
{
#include "gl11.h"
#include "http_client.h"
#include "uid_auth.h"
}

extern int ScannerType;
extern int show_bal;
int FP_Pos;
RD_SERVICE_STRUCTURE RD_Service_data;
Transaction Txn;
extern GProfile GETProfile;
extern Login_leg2 configLoginLeg2;
extern POSTBENEVER postbeneverif;
SUPPORTDATA_DEVICEINFO sd_deviceinfo;
int aadarsha;
char samaga[100];
/*****************Encrypted function variable and function called********/
extern "C" unsigned char * uid_random_bytes(unsigned char *rand, int len);
extern "C" int uid_aes_encrypt_256(unsigned char *in, int inlen, unsigned char *out,int *outlen, unsigned char *key, unsigned char *iv);
extern "C" unsigned char *MD5(const unsigned char *d, size_t n, unsigned char *md);
extern "C" void Base64Decode( char* b64message, unsigned char** buffer, size_t* length);
extern "C" int decrypt(unsigned char *ciphertext, int ciphertext_len, unsigned char *key,unsigned char *iv,unsigned char * plain_text);
extern "C" int fpb_rsa_encrypt_with_fino_pubkey(unsigned char *in, int inlen, unsigned char *outbuf, int *outlen);
extern "C" int hMacSha256(char *xml,int len, unsigned char *outbuff);
extern "C" void BinToHex(unsigned char*input,char*output,int len);
extern "C" char genRandom();
extern char * Convertstr(string str);
unsigned char cEncpPacketPartIn[1024*20];
unsigned char cEncpIntermitdatePacketPart[1024*20];
unsigned char cEncpPacketPartOut[1024*20];
unsigned char cFinalPacket[2048*10];
/************************************************************************/
unsigned char gTxnDateTime[20]; //for global date and time
extern Login_leg1 configLoginLeg1;
char gbankname[120];
char ibm[140];
char BankName_disp[100];
char AadhaarNo_disp[15];
char MobileNum_disp[12];
char Amount_disp[15];
int FLag_Confirmation_Disp = 0;
extern int Flag_TransactionStatus_WdRRN; // temp 20062019

FPB::FPB(QWidget *parent):QWidget(parent),ui(new Ui::FPB)
{
    ui->setupUi(this);
    QFile filename;

    this->setWindowFlags(Qt::FramelessWindowHint);
    this->setWindowModality(Qt::NonModal);
    filename.setFileName("/usr/FINO/FPB/fpbdatabase.db");
    if(filename.exists() == false)
    {
        if(dbaccess.openDB())
        {
            dbaccess.createTable();
        }
        else
        {
            //QMessageBox::critical(0,"Cannot Open Database","Unable to establish a database connection",QMessageBox::Cancel);
        }
    }
    else
    {
        dbaccess.openDB();
    }
    create();
    setWindowFlags(Qt::FramelessWindowHint);
    setWindowFlags(Qt::Window | Qt::CustomizeWindowHint);
    setWindowState(Qt::WindowFullScreen);
    show_bal=1;

    ui->lineEdit_userid->setFocus();

    QRegExp rx("\\d+");
    QRegExpValidator *validator1 = new QRegExpValidator(rx,this);
    ui->lineEdit_userid->setValidator(validator1);

    ui->Transaction_Summary_3->setHidden(false);
    //ui->Forget_button->setHidden(true);
    char ver[20];
    memset(ver, 0, sizeof(ver));
    sprintf(ver, "Ver : %s",APP_VERSION);
    ui->AppVersion_Label->setText(ver);

    FILE *fp2 = NULL;

    char buff_geo[100];
    memset(buff_geo, 0, sizeof(buff_geo));
    system("cd /var/");
    fp2 = fopen("/var/.geo_cell.txt", "rb"); // open
    if(fp2 == NULL)
    {
        memcpy(buff_geo,"Latitude:00.00,Longitude:00.00",22);
    }
    else
    {
        fseek(fp2, 0, SEEK_END);
        int len_geo = ftell(fp2);
        fseek(fp2, 0, SEEK_SET);

        fread(buff_geo,sizeof(unsigned char ),len_geo,fp2);
        fclose(fp2);
    }

    //--------------------------------------    TO DISABLE TRANSACTION STATUS WITH RRN   // temp 20062019  ---------------------

//    ui->label_TransactionID_pg41->setVisible(false);
//    ui->Transaction_Status_wdRRN_pushButton_PG12->setVisible(false);
//    ui->TransactionID_TransStWdRRN_lineEdit_pg41->setVisible(false);
//    ui->TransactionID_TransStWdRRN_OK_pushButton_pg41->setVisible(false);
//    ui->Transaction_Status_label_pg41->setVisible(false);
//    Flag_TransactionStatus_WdRRN = 0;

    //--------------------------------------------------------------------------------------------------------------------------
    input_validation();
    ui->stackedWidget->setCurrentIndex(0);

    return;
}

FPB::~FPB()
{
    delete ui;
}

void FPB::input_validation(void)
{
    ui->Forgot_Password_Button_User_pg1->hide();
    ui->Search_lineEdit->setValidator(new QRegExpValidator(QRegExp("[0-9]{0,18}"),this));
    ui->Bene_acc_no_edit->setValidator(new QRegExpValidator(QRegExp("[0-9]{0,18}"),this));
    ui->Bene_Name_edit->setValidator(new QRegExpValidator(QRegExp("[A-Za-z ]{0,148}"),this));
    ui->Fund_transfer_amount_edit->setValidator(new QRegExpValidator(QRegExp("[0-9]{0,20}"),this));
    ui->OTP_Lineedit->setValidator(new QRegExpValidator(QRegExp("[0-9]{0,4}"),this));
    ui->Name_lineedit->setValidator(new QRegExpValidator(QRegExp("[A-Za-z ]{0,148}"),this));
    ui->Age_lineedit->setValidator(new QRegExpValidator(QRegExp("[0-9]{0,3}"),this));
    ui->Mobile_lineedit->setValidator(new QRegExpValidator(QRegExp("[0-9]{0,10}"),this));
    //ui->Person_add_edit->setValidator(new QRegExpValidator(QRegExp("[A-Za-z0-9]{0,150}"),this));
    //ui->Person_state_edit->setValidator(new QRegExpValidator(QRegExp("[A-Za-z ]{0,30}"),this));
    //ui->Person_city_edit_2->setValidator(new QRegExpValidator(QRegExp("[A-Za-z ]{0,35}"),this));
    ui->Person_pincode_edit->setValidator(new QRegExpValidator(QRegExp("[0-9]{0,6}"),this));
    ui->TransactionID_TransStWdRRN_lineEdit_pg41->setValidator(new QRegExpValidator(QRegExp("[0-9]{0,50}"),this));
    ui->Person_GSTIN_edit->setValidator(new QRegExpValidator(QRegExp("[0-9]{2}([a-zA-Z]){5}([0-9]){4}([a-zA-Z]){1}([0-9]){1}([a-zA-Z]){1}([A-Za-z0-9]){1}"),this));

  //  [0-9]){2}([a-zA-Z]){5}([0-9]){4}([a-zA-Z]){1}([0-9]){1}([a-zA-Z]){1}([A-Za-z0-9]){1}
    return;
}

int FPB::iGenerateRequestID(char *UserName,unsigned char *ReqID)
{
    struct tm *tt;
    time_t  currentTime;

    //  memset(&tt,0,sizeof(struct tm));

    time(&currentTime);
    tt=localtime(&currentTime);

    memset(gTxnDateTime,0,sizeof(gTxnDateTime));
    sprintf((char*)gTxnDateTime,"%02d%02d%4d%02d%02d%02d", tt->tm_mday, tt->tm_mon+1, tt->tm_year+1900, tt->tm_hour, tt->tm_min, tt->tm_sec);

    memset(ReqID,0,sizeof(ReqID));
    sprintf((char *)ReqID,"%s_%s",UserName,(char*)gTxnDateTime);

    return true;
}

char * FPB::iTimestamp()
{
    char Timestamp[20];

    qDebug()<<"Date nd Time Menu";

    struct tm *tt;
    time_t  t;
    memset(&tt,0,sizeof(struct tm));
    time(&t);
    tt=localtime(&t);


    qDebug()<<"Localitiem out";


    memset(Timestamp,0,sizeof(Timestamp));
    sprintf((char*)Timestamp,"%02d%02d%4d%02d%02d%02d", tt->tm_mday, tt->tm_mon+1, tt->tm_year+1900, tt->tm_hour, tt->tm_min, tt->tm_sec);

    qDebug()<<"Date and Time = "<< Timestamp;



    return Timestamp;
}


int FPB::Check_Conditions()
{
    CMisc miscObj;
    /*************** Printer Check *********************/
    if(prn_open() < 0)
    {
        qDebug() << "PRINTER OPEN FAILED";
        miscObj.DisplayMsgBox((char *)"Printer Port Open Failed!... Please Contact Admin");
        return FUNCTION_FAILED;
    }

    if(prn_paperstatus() != 0)
    {
        miscObj.DisplayMsgBox((char *)"Please Put Paper!");
        prn_close();
        return FUNCTION_FAILED;
    }

    prn_close();
    /****************************************************/

    if(Check_Battery_Voltage() == FUNCTION_FAILED)
    {
        miscObj.DisplayMsgBox((char *)"Battery Low!! \n Please Plug-In Charger!");
        return FUNCTION_FAILED;
    }
}

int FPB::Check_Battery_Voltage()
{
    int AdapPresent = 0;
    int BattSt = 0;
    int BattVt = 0;
    int ret = 0;
    QString batValue;
    float F_batVal = 0.00;
    float F_AvgBatVol = 0.00;
    char buff[8];

    for(int i = 0; i < 3; i++)
    {
        ret = gl11_GetBatteryInfo(&AdapPresent, &BattSt, &BattVt);
        if(ret < 0)
            return -1;

        double stripBatVolt = ((8.4)*BattVt)/1024;

        batValue = QString::number(stripBatVolt, 'g', 3);
        F_batVal += batValue.toFloat(0);
    }

    F_AvgBatVol = F_batVal/3;
    //qDebug() << "Average Battery Voltage = " << F_AvgBatVol;

    memset(buff,0,sizeof(buff));
    sprintf(buff,"%0.2f",F_AvgBatVol);

    if((atoi(buff) < 7))  //6.8 or 7.00
        return -1;

    return 0;
}

int FPB::check_RD_connection()
{
    FILE *fp;
    char chBuffer[10];
    int status;

    memset(chBuffer, 0, sizeof(chBuffer));
    fp = popen("pidof rds  |wc -w ", "r");

    while (fgets(chBuffer, 10, fp) != NULL)
    {
        qDebug() << atoi(chBuffer);
    }

    status = pclose(fp);
    if (status == -1)
    {
        qDebug() << "Error closing pipe";
    }

    if(atoi(chBuffer)>= 1)
    {
        uidmessage::instance()->Display_BusyMessage("Connected to RD service");
        sleep(2);
        uidmessage::instance()->hide_Message();
        return FUNCTION_SUCCESS;
    }
    else
    {
        system("ln -s /etc/ppp/resolv.conf /etc/resolv.conf"); // Dhiral Softlink 5023
        //   qDebug()<<"Check RD Connection : ln -s /etc/ppp/resolv.conf /etc/resolv.conf"; // Dhiral Softlink 5023
        uidmessage::instance()->hide_Message();
        uidmessage::instance()->Display_BusyMessage("RD Service is not initialized, Please wait..."); // Dhiral Softlink 5023
        sleep(1);
        uidmessage::instance()->hide_Message();
        uidmessage::instance()->Display_BusyMessage("Please Reboot Device to connect RD Service"); // Dhiral Softlink 5023
        sleep(2);
        uidmessage::instance()->hide_Message();
        return FUNCTION_FAILED;
    }
}


void FPB::memclear(char * fun_name)
{
    memset(fun_name, 0, sizeof(fun_name));
    return;
}

int FPB::Stringtrim(QString str)
{
    int Stringlength=str.length();
    for(int i=0;i<=Stringlength;i++)
    {
        if(str[i]==' ' && str[i+1]==' ')
        {
            return -1;
        }
        else if(str[i]==' ')
        {
            return -2;
        }
        // !@#$%^&()'_+`~*?={}\/;:<>,.-|
        //        else if(str[i]=='[' || str[i]==']' || str[i]=='"' || str[i]=='\\' || str[i]=='!' || str[i]=='@' ||
        //                str[i]=='#' || str[i]=='$' || str[i]=='%' || str[i]=='^' || str[i]=='&' || str[i]=='(' ||
        //                str[i]==')' || str[i]=='\'' || str[i]=='_' || str[i]=='+' || str[i]=='`' || str[i]=='~' ||
        //                str[i]=='*' || str[i]=='?' || str[i]=='=' || str[i]=='/' || str[i]==';' || str[i]==':' ||
        //                str[i]=='{' || str[i]=='}' || str[i]==',' || str[i]=='<' || str[i]=='>' || str[i]=='.' ||
        //                str[i]=='-' || str[i]=='|')
        //        {
        //            return -3;
        //        }


    }
    return 0;
}

int FPB::iDisplayConsent(QString message)
{
    QDialog  *confirmation = new QDialog();
    confirmation->setWindowFlags(Qt::FramelessWindowHint);
    confirmation->setAutoFillBackground(true);
    confirmation->setStyleSheet("color: rgb(85, 0, 127);background-color: rgb(255, 255, 255)");

    confirmation->setGeometry(0, 0, 320, 240);

    QLabel *msg = new QLabel(confirmation);
    msg->setGeometry(30, 60, 280, 100);
    msg->setText(message);
    msg->setWordWrap(true);
    msg->setAlignment(Qt::AlignHCenter|Qt::AlignVCenter);


    QPushButton *yesButton = new QPushButton(confirmation);
    yesButton->setGeometry(60, 180, 75, 30);
    yesButton->setStyleSheet("color: rgb(255, 255, 255);background-color: rgb(85, 0, 127)");
    yesButton->setFont(QFont("Normal"));
    if(Flag_UpdateApp == 1 || Flag_UpdateApp == 2)
    {
        yesButton->setText("UPDATE");
    }
    else
    {
        yesButton->setText("YES");
    }
    QPushButton *noButton = new QPushButton(confirmation);
    noButton->setGeometry(200, 180, 75, 30);
    noButton->setFont(QFont("Normal"));
    noButton->setStyleSheet("color: rgb(255, 255, 255);background-color: rgb(85, 0, 127)");

    if(Flag_UpdateApp == 1)
    {
        noButton->setText("EXIT");
        Flag_UpdateApp =0;
    }
    else if(Flag_UpdateApp == 2)
    {
        noButton->setText("No THANKS");
        Flag_UpdateApp =0;
    }
    else
    {
        noButton->setText("NO");
    }

    connect(yesButton, SIGNAL(clicked()), confirmation, SLOT(accept()));
    connect(noButton, SIGNAL(clicked()), confirmation, SLOT(reject()));

    int ret = confirmation->exec();

    return ret;
}


int FPB::AEPSProcess()
{


    ui->lineEditUID1->setReadOnly(false);
    ui->lineEditUID1->setFocus();
    ui->lineEditUID2->setReadOnly(false);
    ui->lineEditUID3->setReadOnly(false);
    ui->PAGE4_OK_pushButton_8->setEnabled(false);
    ui->PAGE4_OK_pushButton_8->setStyleSheet("background-color: rgb(85, 85, 127); color: rgb(255, 255, 255)");

    ui->stackedWidget->setCurrentIndex(7);  // go to page ENTER Bank Search

    //        int ret =  BankSelection();
    //        if(ret == FUNCTION_FAILED)
    //        {
    //            qDebug()<<"Return Failed from Bank Selection";
    //            return FUNCTION_FAILED;
    //        }
    return FUNC_OK;
}

int FPB::BankSelection(char *Buff_BankName) //Dhiral
{
    CMisc miscObj;
    QSqlQuery query;
    query.clear();

    //    query.prepare("SELECT  BankName, NBIN from AEPSBankMaster where BankName LIKE '%'?'%'");
    //    query.addBindValue(Buff_BankName);

    //    query.prepare("SELECT BankName, NBIN FROM AEPSBankMaster WHERE BankName LIKE :bank");
    query.prepare("SELECT BANK_NAME, NBIN FROM imps_mapping WHERE BANK_NAME LIKE :bank");
    query.bindValue(":bank", QString("%%1%").arg(Buff_BankName));


    //query.prepare("SELECT * from AEPSBankMaster");
    if(!query.exec())
        qDebug() << query.lastError();
    else
    {
        //  miscObj.AudioTest((char *)"054");  //Please Select Bank
        ui->listWidget_Issuer->clear(); // clear list
        ui->listWidget_Issuer->setFont(QFont("Courier",18));
        while(query.next())
        {
            if((query.value(0).toString() != NULL) && (query.value(1).toString() != NULL))
            {
                ui->listWidget_Issuer->addItem(query.value(0).toString());
            }
        }
    }

    if(ui->listWidget_Issuer->count() == 0)
    {
        miscObj.DisplayMsgBox((char *)"Bank not found  ");
        return FUNCTION_FAILED;
    }
    return FUNC_OK;

}

void FPB::on_listWidget_Issuer_clicked(const QModelIndex &index)
{

    QString QBank,QBin, QAePSStatus;
    QSqlQuery query;
    query.clear();

    QBank.clear();
    QBin.clear();
    QAePSStatus.clear();
    //    query.prepare("SELECT BankName, NBIN  FROM AEPSBankMaster WHERE BankName = ?");

    query.prepare("SELECT BANK_NAME, NBIN, AEPSSTATUS  FROM imps_mapping WHERE BANK_NAME = ?");
    query.addBindValue(index.model()->data(index, Qt::DisplayRole).toString());
    if(!query.exec())
        qDebug() << "on_listWidget_Issuer_clicked" << ":" << query.lastError();
    else
    {
        while(query.next())
        {
            QBank = query.value(0).toString();
            QBin = query.value(1).toString();
            QAePSStatus = query.value(2).toString();
        }
    }


    if((strcmp(QAePSStatus.toAscii().trimmed().constData(), "false") == 0) || (strcmp(QAePSStatus.toAscii().trimmed().constData(), "0") == 0) || (strcmp(QBin.toAscii().trimmed().constData(), "608001") == 0))
    {
        CMisc miscObj;
        uidmessage::instance()->hide_Message();
        miscObj.DisplayMsgBox((char *)"AePS transaction not allowed for this bank.");
        uidmessage::instance()->hide_Message();
        return;
    }
    //IMPS transaction not allowed for this beneficiary bank.

    //    memset(&Txn,0,sizeof(struct Transaction)); // clearing Transaction Structure

    ui->lineEditUID1->clear();
    ui->lineEditUID2->clear();
    ui->lineEditUID3->clear();

    strcpy(Txn.bankname,QBank.toAscii().constData()); //Copying bank name in transaction structure.
    strcpy(Txn.bin,QBin.toAscii().constData()); //Copying bank bin in transaction structure.
#if DEBUG
    qDebug()<<"Txn.bankname = "<<Txn.bankname;
    qDebug()<<"Txn.bin = "<<Txn.bin;
#endif
    //    memset(gbankname,0,sizeof(gbankname));
    //    strncpy(gbankname, Txn.bankname, strlen(Txn.bankname));
    //    qDebug()<<"gbankname = "<<gbankname;



    //    ui->Bankname_Display_UIDPage_label->wordWrap();
    //    char ibm1[140];
    //    memset(ibm1,0,sizeof(ibm1));
    //    ibm1 = ibank();
    memset(gbankname,0, sizeof(gbankname));
    memset(BankName_disp,0, sizeof(BankName_disp));
    sprintf(gbankname, "Bank : %s", Txn.bankname);
    sprintf(BankName_disp, "%s", Txn.bankname);

    ui->Bankname_Display_UIDPage_label->clear();
    ui->Bankname_Display_MoNumPage_label->clear();
    ui->Bankname_Display_UIDPage_label->setStyleSheet("font-size: 11pt; font-weight: bold; color: rgb(85, 0, 127); background-color:  rgb(255, 255, 255)");
    ui->Bankname_Display_UIDPage_label->setText(gbankname);
    ui->lineEditUID1->setFocus();


    ui->UID_LabelName->setText((char *)configLoginLeg2.ucname);
    ui->UID_LabelName->setFont(QFont("SansSerif",10));
    ui->UID_LabelName->setAlignment(Qt::AlignRight | Qt::AlignBottom);
    QString bal1 = QString::fromUtf8((const char*)Balance_Display);
    ui->UID_LabelBal->setText(bal1);
    ui->UID_LabelBal->setFont(QFont("SansSerif",9));
    ui->UID_LabelBal->setAlignment(Qt::AlignRight | Qt::AlignBottom);
    ui->stackedWidget->setCurrentIndex(3);  // go to page ENTER UID
    return;
}

char* FPB::ibank()
{
    //    char ibm[140];
    memset(ibm,0, sizeof(ibm));
    sprintf(ibm, "Bank : %s", gbankname);
#if DEBUG
    printf("ibm = %s\n",ibm);
#endif
    return ibm;
}

void FPB::getInputText(QString argstr)
{
    entryDlgText = argstr;
}

void FPB::on_PAGE4_OK_pushButton_8_clicked()
{
    CMisc miscObj;
    int ret = 0;


    UnCheckFP();
    ret = Validate_UID(); // check if entered UID combination is correct or not
    if(ret == FUNCTION_FAILED)
    {
        miscObj.DisplayMsgBox((char *)"Invalid UID    ");

        ui->lineEditUID1->clear();
        ui->lineEditUID2->clear();
        ui->lineEditUID3->clear();

        ui->PAGE4_OK_pushButton_8->setEnabled(false);
        ui->PAGE4_OK_pushButton_8->setStyleSheet("background-color: rgb(85, 85, 127); color: rgb(255, 255, 255)");

        ui->lineEditUID1->setFocus();

        return;
    }

    //if(iDisplayConsent ("I give my consent for being authenticated for this transaction initiated by myself using my above mentioned Aadhaar no.") == false) // 27032019 As per Amit Sir
    if(iDisplayConsent ("I give my consent for being authenticated for this transaction using my Aadhaar no.") == false)
        return;

    if(Txn.Txntype == AEPS_BAL || Txn.Txntype == AEPS_MINI)
    {
        memset(AadhaarNo_disp,0,sizeof(AadhaarNo_disp));
        strcpy(Txn.uid,strUID.toAscii().constData());  // copy enter UID to UID struct
        strcpy(AadhaarNo_disp,strUID.toAscii().constData());  // copy enter UID to UID struct
        sprintf(Txn.Amount,"%012d",0);// Transaction amount.
        strcpy(Txn.uid,strUID.toAscii().constData());  // copy enter UID to UID struct
        ui->DispConfirmation_LabelName ->setText((char *)configLoginLeg2.ucname);
        ui->DispConfirmation_LabelName ->setFont(QFont("SansSerif",10));
        ui->DispConfirmation_LabelName ->setAlignment(Qt::AlignRight | Qt::AlignBottom);

        ui->DispConfirmation_LabelBal->setText(QString::fromUtf8((const char*)Balance_Display));
        ui->DispConfirmation_LabelBal->setFont(QFont("SansSerif",9));
        ui->DispConfirmation_LabelBal->setAlignment(Qt::AlignRight | Qt::AlignBottom);

        FLag_Confirmation_Disp = 1;
        //       printf("gbankname = %s, AadhaarNo_disp = %s, Amount_disp = %s\n",gbankname, AadhaarNo_disp, Amount_disp);
        char display_confirmpage1[10][120];
        memset(display_confirmpage1,0,sizeof(display_confirmpage1));
        sprintf(display_confirmpage1[0],"Bank Name      : %s",BankName_disp);
        sprintf(display_confirmpage1[1],"Aadhaar Number : %s",AadhaarNo_disp);

        int i=0;
        ui->listWidget_Issuer_10->clear(); // clear list
        ui->listWidget_Issuer_10->setFont(QFont("Courier",14));

        for(i=0;i<2;i++)
        {
#if DEBUG
            printf("display_confirmpage    =====    %s\n",display_confirmpage1[i]);
#endif
            ui->listWidget_Issuer_10->addItem((char *)display_confirmpage1[i]);
        }

        ui->stackedWidget->setCurrentIndex(29);
        return;
    }
    else if(Txn.Txntype == AEPS_WITH )
    {
        //  int iW_ret = iWithdrwlInput();
        //  memcpy(Txn.Mobileno,"9913384086",10); // Hard Coded
        ui->lineEdit_Mobile_Num_Page9->clear();
        ui->lineEdit_Mobile_Num_Page9->setCursorPosition(0);
        ui->OK_Mobile_Num_Page9->setEnabled(false);
        ui->OK_Mobile_Num_Page9->setStyleSheet("background-color: rgb(85, 85, 127); color: rgb(255, 255, 255)");

        ui->Bankname_Display_MoNumPage_label->setStyleSheet("font-size: 11pt; font-weight: bold; color: rgb(85, 0, 127); background-color:  rgb(255, 255, 255)");
        ui->Bankname_Display_MoNumPage_label->setText(gbankname);

        ui->CustMoNum_LabelName ->setText((char *)configLoginLeg2.ucname);
        ui->CustMoNum_LabelName ->setFont(QFont("SansSerif",10));
        ui->CustMoNum_LabelName ->setAlignment(Qt::AlignRight | Qt::AlignBottom);

        ui->CustMoNum_LabelBal->setText(QString::fromUtf8((const char*)Balance_Display));
        ui->CustMoNum_LabelBal->setFont(QFont("SansSerif",9));
        ui->CustMoNum_LabelBal->setAlignment(Qt::AlignRight | Qt::AlignBottom);

        QRegExp rx("\\d+");
        QRegExpValidator *validator1 = new QRegExpValidator(rx,this);
        ui->lineEdit_Mobile_Num_Page9->setValidator(validator1);
        ui->stackedWidget->setCurrentIndex(8);

        return;
    }

    strcpy(Txn.uid,strUID.toAscii().constData());  // copy enter UID to UID struct

    ui->stackedWidget->setCurrentIndex(4);  //  FINGER PRINT RADIO BUTTON PAGE
}

int FPB::Validate_UID()
{
    strUID="";

    strUID = ui->lineEditUID1->text() + ui->lineEditUID2->text()+ ui->lineEditUID3->text();

    if(strUID != NULL)
    {
        QByteArray arr = strUID.toLatin1();
        char *valUid = arr.data();

        if(validate_uid(valUid) == -1)
            return FUNCTION_FAILED;
    }
    else
        return FUNCTION_FAILED;

    return FUNCTION_SUCCESS;
}

void FPB::UnCheckFP()
{

    //    QRadioButton *rdbtn = (QRadioButton *)sender();

    if(ui->rdleft_3 != NULL)
    {
        ui->rdleft_3->setEnabled(true);
        ui->rdleft_3->setCheckable(false);
        ui->rdleft_3->setCheckable(true);
    }

    if(ui->rdLeftIndex_3 != NULL)
    {
        ui->rdLeftIndex_3->setEnabled(true);
        ui->rdLeftIndex_3->setCheckable(false);
        ui->rdLeftIndex_3->setCheckable(true);
    }

    if(ui->rdLeftMiddle_3 != NULL)
    {
        ui->rdLeftMiddle_3->setEnabled(true);
        ui->rdLeftMiddle_3->setCheckable(false);
        ui->rdLeftMiddle_3->setCheckable(true);
    }

    if(ui->rdLeftRing_3 != NULL)
    {
        ui->rdLeftRing_3->setEnabled(true);
        ui->rdLeftRing_3->setCheckable(false);
        ui->rdLeftRing_3->setCheckable(true);
    }

    if(ui->rdLeftLittle_3 != NULL)
    {
        ui->rdLeftLittle_3->setEnabled(true);
        ui->rdLeftLittle_3->setCheckable(false);
        ui->rdLeftLittle_3->setCheckable(true);
    }

    if(ui->rdRightThumb_3 != NULL)
    {
        ui->rdRightThumb_3->setEnabled(true);
        ui->rdRightThumb_3->setCheckable(false);
        ui->rdRightThumb_3->setCheckable(true);
    }

    if(ui->rdRightIndex_3 != NULL)
    {
        ui->rdRightIndex_3->setEnabled(true);
        ui->rdRightIndex_3->setCheckable(false);
        ui->rdRightIndex_3->setCheckable(true);
    }

    if(ui->rdRightMiddle_3 != NULL)
    {
        ui->rdRightMiddle_3->setEnabled(true);
        ui->rdRightMiddle_3->setCheckable(false);
        ui->rdRightMiddle_3->setCheckable(true);
    }

    if(ui->rdRightRing_3 != NULL)
    {
        ui->rdRightRing_3->setEnabled(true);
        ui->rdRightRing_3->setCheckable(false);
        ui->rdRightRing_3->setCheckable(true);
    }

    if(ui->rdRightLittle_3 != NULL)
    {
        ui->rdRightLittle_3->setEnabled(true);
        ui->rdRightLittle_3->setCheckable(false);
        ui->rdRightLittle_3->setCheckable(true);
    }
    return;
}

void FPB::on_pushButton_3_clicked()
{

    //        QMessageBox *popup = new QMessageBox(QMessageBox::Information, "Info", "Transaction Cancelled..", QMessageBox::Ok);
    //        popup->setWindowFlags(Qt::FramelessWindowHint);
    //        QTimer *msgBoxCloseTimer = new QTimer(this);
    //        msgBoxCloseTimer->setInterval(2000);
    //        msgBoxCloseTimer->setSingleShot(true);
    //        connect(msgBoxCloseTimer, SIGNAL(timeout()), popup, SLOT(reject()));
    //        msgBoxCloseTimer->start();
    //        popup->setStandardButtons(0);
    //        popup->exec();
    ui->stackedWidget->setCurrentIndex(29); // Confirmation Page
    return;

}

void FPB::on_rdRightThumb_3_clicked(bool checked)
{
    int i_ret = -12;
    if(checked == true)
    {
        FP_Pos = PBUF__POSITION__LEFT_THUMB;

        ui->rdleft_3->setCheckable(false);
        i_ret = rd_service_FP_capture();

        //Optical_FingerPrint(); // Dhiral EKYC 2.0

    }
    return;
}

void FPB::on_rdleft_3_clicked(bool checked)
{
    int i_ret = -12;
    if(checked == true)
    {
        FP_Pos = PBUF__POSITION__LEFT_INDEX;

        ui->rdLeftIndex_3->setCheckable(false);
        i_ret = rd_service_FP_capture();
        //Optical_FingerPrint(); // Dhiral EKYC 2.0
    }
    return;
}

void FPB::on_rdLeftMiddle_3_clicked(bool checked)
{
    int i_ret = -12;
    if(checked == true)
    {
        FP_Pos = PBUF__POSITION__LEFT_MIDDLE;

        ui->rdLeftMiddle_3->setCheckable(false);
        i_ret = rd_service_FP_capture();
        //Optical_FingerPrint(); // Dhiral EKYC 2.0

    }
    qDebug()<<"Left Middle iret"<<i_ret;
    return;
}

void FPB::on_rdLeftRing_3_clicked(bool checked)
{
    int i_ret = -12;
    if(checked == true)
    {
        FP_Pos = PBUF__POSITION__LEFT_RING;

        ui->rdLeftRing_3->setCheckable(false);
        i_ret = rd_service_FP_capture();
        //Optical_FingerPrint(); // Dhiral EKYC 2.0
    }
    return;
}


void FPB::on_rdLeftLittle_3_clicked(bool checked)
{
    int i_ret = -12;
    if(checked == true)
    {
        FP_Pos = PBUF__POSITION__LEFT_LITTLE;
        ui->rdLeftLittle_3->setCheckable(false);
        i_ret = rd_service_FP_capture();
        //Optical_FingerPrint(); // Dhiral EKYC 2.0
    }
    return;
}

void FPB::on_rdLeftIndex_3_clicked(bool checked)
{
    int i_ret = -12;
    if(checked == true)
    {
        FP_Pos = PBUF__POSITION__RIGHT_THUMB;

        ui->rdRightThumb_3->setCheckable(false);
        i_ret = rd_service_FP_capture();
        //Optical_FingerPrint(); // Dhiral EKYC 2.0
    }
    return;
}

void FPB::on_rdRightIndex_3_clicked(bool checked)
{
#if DEBUG
    qDebug()<<"Leg 3 RD click bin: "<<Txn.bin;
#endif
    int i_ret = -12;

    if(checked == true)
    {
        FP_Pos = PBUF__POSITION__RIGHT_INDEX;

        ui->rdRightIndex_3->setCheckable(false);
        i_ret = rd_service_FP_capture();
        //Optical_FingerPrint(); // Dhiral EKYC 2.0
    }
    return;
}

void FPB::on_rdRightMiddle_3_clicked(bool checked)
{
    int i_ret = -12;
    if(checked == true)
    {
        FP_Pos = PBUF__POSITION__RIGHT_MIDDLE;
        ui->rdRightMiddle_3->setCheckable(false);
        i_ret = rd_service_FP_capture();
        //Optical_FingerPrint(); // Dhiral EKYC 2.0

    }
    return;
}

void FPB::on_rdRightRing_3_clicked(bool checked)
{
    int i_ret = -12;
    if(checked == true)
    {
        FP_Pos = PBUF__POSITION__RIGHT_RING;

        ui->rdRightRing_3->setCheckable(false);
        i_ret = rd_service_FP_capture();
        //Optical_FingerPrint(); // Dhiral EKYC 2.0
    }
    return;
}

void FPB::on_rdRightLittle_3_clicked(bool checked)
{
    int i_ret = -12;
    if(checked == true)
    {
        FP_Pos = PBUF__POSITION__RIGHT_RING;

        ui->rdRightLittle_3->setCheckable(false);
        i_ret = rd_service_FP_capture();
    }
    return;
}

int FPB::rd_service_FP_capture()
{
    CMisc miscObj;
    int sockedfd;
    sockedfd =0;
    //   CPopMsgDlg *dlgPopup1 = new CPopMsgDlg(this);
    //***************************************************************************************************

    char *resp_buffer = NULL;

    if(ScannerType == OPTICAL)
        miscObj.DisplayMsgBox((char *)"Place finger on scanner after light glows");
    else if(ScannerType == CAPACITIVE)
        miscObj.DisplayMsgBox((char *)"Place finger on scanner after beep");

    //        dlgPopup1->ui->label->update();
    //        dlgPopup1->show();
    //        QCoreApplication::processEvents();
    //        QCoreApplication::processEvents();

    uidmessage::instance()->hide_Message();
    uidmessage::instance()->Display_BusyMessage("Please Wait...");


    //    unsigned char xmlData[500] = "", dataBuffer[500] = "";

    //    memset(xmlData,0,strlen((const char *)xmlData));
    //    memset(dataBuffer,0,strlen((const char *)dataBuffer));

    //    if(!strcmp(RD_DATA, "PP")) // "PP"
    //    {
    //        sprintf((char *)dataBuffer,"<PidOptions ver=\"1.0\"><Opts fCount=\"1\" fType=\"0\" env=\"PP\" format=\"1\" pidVer=\"2.0\" timeout=\"10000\" wadh=\"E0jzJ/P8UopUHAieZn8CKqS4WPMi5ZSYXgfnlfkWjrc=\"/><Demo></Demo><CustOpts></CustOpts><Bios></Bios></PidOptions>\n"); // format : For XML = "0", PROTOBUF = "1"   // EKYC 2.5
    //        qDebug()<<"Preproduction request";
    //    }
    //    else if(!strcmp(RD_DATA, "P")) // Dhiral Enter leg2 // "P"
    //    {
    //        sprintf((char *)dataBuffer,"<PidOptions ver=\"1.0\"><Opts fCount=\"1\" fType=\"0\" env=\"P\" format=\"1\" pidVer=\"2.0\" timeout=\"10000\" wadh=\"E0jzJ/P8UopUHAieZn8CKqS4WPMi5ZSYXgfnlfkWjrc=\"/><Demo></Demo><CustOpts></CustOpts><Bios></Bios></PidOptions>\n"); // format : For XML = "0", PROTOBUF = "1"   // EKYC 2.5
    //        qDebug()<<"Production request";
    //    }

    //    sprintf((char *)xmlData,"CAPTURE /rd/capture HTTP/1.1\nAccept: text/xml\nContent-Type: text/xml\nUser-Agent: axios/0.15.3\nHOST: 127.0.0.1:11100\nConnection: close\nContent-Length:%d\n%s",strlen((const char *)dataBuffer),dataBuffer);
    //    printf("Buffer : %s\n",xmlData);
    //    ret = rd_fp_capture_xml(sockfd,xmlData,&resp_buffer);
    //    // uidmessage::instance()->Display_BusyMessage("Please Wait....");
    //    //   cout<<"COut of return buffer"<<resp_buffer;
    //    if(ret == -1)
    //        printf("rd_fp_capture failed....\n");
    //    else if(ret == 0)
    //    {
    //        if(resp_buffer != NULL)
    //        {
    //            printf("********************** O/P **********************\n%s\n",resp_buffer);
    //            printf("*************************************************\n");
    //        }
    //    }
    /************************************************************************************/


    fp_capture struct_capture;
    sockedfd =0;


    struct_capture.Fp_Count = 1;
    sprintf(struct_capture.FP_Type,"%s","0");		//	0 - FMR
    sprintf(struct_capture.PID_Type,"%s","0");		//	0 - XML, 1 - ProtoBuf
    sprintf(struct_capture.pidVer,"%s","2.0");

    if(RD_DATA == "PP")
    {
        sprintf(struct_capture.env,"%s","PP"); // preproduction ENVT
        qDebug()<<"Preproduction request";
    }
    else if(RD_DATA == "P") // Dhiral Enter leg2
    {
        sprintf(struct_capture.env,"%s","P");
        qDebug()<<"production request";  // production ENVT
    }
    struct_capture.Timeout = 10000;

    //    if(ScannerType == OPTICAL)
    //        miscObj.DisplayMsgBox((char *)"Place finger on scanner after light glows");
    //    else if(ScannerType == CAPACITIVE)
    //        miscObj.DisplayMsgBox((char *)"Place finger on scanner after beep");

    rd_fp_capture_buffer(sockedfd,&struct_capture,&resp_buffer);

    //    if(ScannerType == OPTICAL)
    //        miscObj.DisplayMsgBox((char *)"Place finger on scanner after light glows");
    //    else if(ScannerType == CAPACITIVE)
    //        miscObj.DisplayMsgBox((char *)"Place finger on scanner after beep");

    //    //        dlgPopup1->ui->label->update();
    //    //        dlgPopup1->show();
    //    //        QCoreApplication::processEvents();
    //    //        QCoreApplication::processEvents();

    //    qDebug()<<"@@ ## $$ %% AEPS RD SERVICE capture_buffer %% $$ ## @@";
    //    rd_fp_capture_buffer(sockedfd,&struct_capture,&resp_buffer);

#if DEBUG
    qDebug()<<"Sizeof Resp Buff  :: "<<sizeof(resp_buffer);
 #endif
    if(resp_buffer == NULL)
    {
        miscObj.DisplayMsgBox((char *)"NULL Response from RD Server, Please Relaunch RD Service Again...");
        ui->stackedWidget->setCurrentIndex(1);
        //        dlgPopup1->close();
        //        dlgPopup1->deleteLater();
        //        qApp->processEvents();
        return FUNCTION_FAILED;

    }

    //    uidmessage::instance()->hide_Message();
    //   uidmessage::instance()->DisplayMessage("Please Wait....");


    process_rd_response(resp_buffer);

    //memset(EncPIDData,0,sizeof(EncPIDData));
    int encoutlen=0, in_buff_len=0;
    in_buff_len = strlen(resp_buffer);
    //uid_encode_b64((unsigned char *)resp_buffer, in_buff_len, EncPIDData, &encoutlen);
    memset(FINALENCDATA,0,sizeof(FINALENCDATA));
    uid_encode_b64((unsigned char *)resp_buffer, in_buff_len, FINALENCDATA, &encoutlen);
    QString ENC_Data ;
    ENC_Data.clear();
    ENC_Data = QString::fromUtf8((const char*)FINALENCDATA);
#if DEBUG
    qDebug()<<"ENC_Data in qstring == "<<ENC_Data;
#endif
    ENC_Data = ENC_Data.simplified().remove(' ');
#if DEBUG
    qDebug()<<"ENC_Data without space == "<<ENC_Data;
#endif
    //EncPIDData = (unsigned char*)ENC_Data.toLatin1().data();

    // EncPIDData = NULL;
    memset(EncPIDData,0,sizeof(EncPIDData));
    memcpy(EncPIDData,ENC_Data.toLatin1().data(),strlen(ENC_Data.toLatin1().data()));

    // qDebug()<<"uid_encode_b64 out ENC_Data" << EncPIDData;
#if DEBUG
    printf("EncPIDData == %s\n\n Len = %d",EncPIDData,encoutlen);
#endif
    if(RD_Service_data.error_info == "Capture timed out")
    {
        miscObj.DisplayMsgBox((char *)"Finger capture timed-Out, please Try Again...");

        UnCheckFP();  // unmark the FP check BOx
        uidmessage::instance()->hide_Message();
        ui->stackedWidget->setCurrentIndex(4); // Capture Finger Print Page
        return FUNCTION_FAILED;
    }
    else if(RD_Service_data.error_info == "Device needs to be re-initialized" )
    {
        miscObj.DisplayMsgBox((char *)"Re-initializing the device please wait");
        gl11_optcap_pwron();
        sleep(1);
        uidmessage::instance()->hide_Message();
        return FUNCTION_FAILED;
    }
    else
    {
        if(RD_Service_data.error_info.trimmed().length() == 0) // success conditionn
        {
            // NO action
        }
        else if(RD_Service_data.error_info.trimmed().length() > 1)
        {
            miscObj.DisplayMsgBox((char *)RD_Service_data.error_info.toAscii().data());
            uidmessage::instance()->hide_Message();
            return FUNCTION_FAILED;
        }

    }
    //    if(ScannerType == CAPACITIVE)
    //        miscObj.DisplayMsgBox((char *)"Capture success, press 'OK'");
    uidmessage::instance()->hide_Message();
    uidmessage::instance()->Display_BusyMessage("Finger Capture Success...");
    sleep(2);
    int iRet_txn = -3;
    if(Txn.Txntype == AEPS_BAL)
    {
        iRet_txn = Prepare_AEPS_BAL_Func();
        if(iRet_txn == FUNCTION_FAILED || iRet_txn == FUNC_ERROR || iRet_txn == SESSION_EXPIRED_FAILED)
        {
            qDebug()<<"Prepare_AEPS_BAL_Func Failed";
            uidmessage::instance()->hide_Message();
            if(iRet_txn == SESSION_EXPIRED_FAILED)
                ui->stackedWidget->setCurrentIndex(0);
            else
                ui->stackedWidget->setCurrentIndex(2);
            return FUNCTION_FAILED;
        }

    }
    else if(Txn.Txntype == AEPS_WITH)
    {
        iRet_txn = Prepare_AEPS_WITHDRWL_Func();
        if(iRet_txn == FUNCTION_FAILED || iRet_txn == FUNC_ERROR || iRet_txn == SESSION_EXPIRED_FAILED)
        {
            qDebug()<<"Prepare_AEPS_WITHDRWL_Func Failed";
            uidmessage::instance()->hide_Message();
            if(iRet_txn == SESSION_EXPIRED_FAILED)
                ui->stackedWidget->setCurrentIndex(0);
            else
                ui->stackedWidget->setCurrentIndex(2);
            return FUNCTION_FAILED;
        }
    }
    //    else if(Txn.Txntype == AEPS_MINI)
    //    {
    //        iRet_txn = Prepare_AEPS_MINISTAT_Func();
    //        if(iRet_txn == FUNCTION_FAILED || iRet_txn == FUNC_ERROR)
    //        {
    //            qDebug()<<"Prepare_AEPS_MiniStatement_Func Failed";
    //            uidmessage::instance()->hide_Message();
    //            ui->stackedWidget->setCurrentIndex(2);
    //            return FUNCTION_FAILED;
    //        }
    //    }

    return FUNCTION_SUCCESS;

}

void FPB::process_rd_response(char *buffer_data)
{
    QFile file_xmlleg1;
    FILE *fptr;
    char temp_length_buffer[6];
    //   qDebug()<<"Process RD Response 1";

    fptr = fopen("/usr/FINO/FPB/rd_response_packet.xml","wb");
    //   qDebug()<<"Process RD Response 2";

    memset(temp_length_buffer,0,sizeof(temp_length_buffer));
    //  qDebug()<<"Process RD Response 2";


    //  memcpy(temp_length_buffer,strstr(buffer_data,"<?xml version=\"1.0\"?>"),strlen(buffer_data));
    // qDebug()<<"Content length:"<<temp_length_buffer;

    if(fptr)
        fwrite(strstr(buffer_data,"<?xml version=\"1.0\"?>"),strlen(buffer_data), 1, fptr);
    qDebug()<<"Buffer copied";

    fclose(fptr);

    file_xmlleg1.setFileName("/usr/FINO/FPB/rd_response_packet.xml");

    if(!file_xmlleg1.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug() << "Error Reading Param File";
        return;
    }

    QXmlStreamReader xml(&file_xmlleg1);

    while(!xml.atEnd() && !xml.hasError())
    {

        QXmlStreamReader::TokenType token = xml.readNext();
        //      qDebug()<<"output: "<<xml.name().toString();


        if(xml.name().toString()=="Resp")
        {
            foreach(const QXmlStreamAttribute &attr, xml.attributes())
            {
                if (attr.name().toString() == QLatin1String("errCode"))
                {
                    //qDebug()<<"in errCode";

                    RD_Service_data.error_code.clear();
                    RD_Service_data.error_code = attr.value().toString();
#if DEBUG
                    qDebug()<<"errCode : "<< RD_Service_data.error_code;
#endif
                }
                else if(attr.name().toString() == QLatin1String("errInfo"))
                {
                    RD_Service_data.error_info.clear();
                    RD_Service_data.error_info = attr.value().toString();
#if DEBUG
                    qDebug()<<"errInfo : "<< RD_Service_data.error_info;
#endif
                }
                else if(attr.name().toString() == QLatin1String("fCount"))
                {
                    // qDebug()<<"in fCount";
                    qDebug()<<"fCount : "<<attr.value().toString();

                }
                else if(attr.name().toString() == QLatin1String("fType"))
                {
                    // qDebug()<<"in fType";
                    qDebug()<<"fType : "<<attr.value().toString();
                }
                else if(attr.name().toString() == QLatin1String("iCount"))
                {
                    // qDebug()<<"in iCount";
                    qDebug()<<"iCount : "<< attr.value().toString();
                }
            }
        }

        if(xml.name().toString()=="DeviceInfo")
        {
            foreach(const QXmlStreamAttribute &attr, xml.attributes())
            {
                if (attr.name().toString() == QLatin1String("dpId"))
                {

                    RD_Service_data.dpld.clear();
                    RD_Service_data.dpld = attr.value().toString();
#if DEBUG
                    qDebug()<<"dpId : "<< attr.value().toString();
#endif
                }
                else if(attr.name().toString() == QLatin1String("rdsId"))
                {
                    RD_Service_data.rdsld.clear();
                    RD_Service_data.rdsld = attr.value().toString();
#if DEBUG
                    qDebug()<<"rdsId : "<< attr.value().toString();
#endif
                }
                else if(attr.name().toString() == QLatin1String("rdsVer"))
                {
                    RD_Service_data.rdsver.clear();
                    RD_Service_data.rdsver = attr.value().toString();
#if DEBUG
                    qDebug()<<"rdsVer : "<<attr.value().toString();
#endif

                }
                else if(attr.name().toString() == QLatin1String("dc"))
                {
                    RD_Service_data.dc.clear();
                    RD_Service_data.dc = attr.value().toString();
#if DEBUG
                    qDebug()<<"dc : "<<attr.value().toString();
#endif
                }
                else if(attr.name().toString() == QLatin1String("mi"))
                {
                    RD_Service_data.mi.clear();
                    RD_Service_data.mi = attr.value().toString();
#if DEBUG
                    qDebug()<<"mi : "<< attr.value().toString();
#endif
                }

                else if(attr.name().toString() == QLatin1String("mc"))
                {
                    RD_Service_data.MC_data.clear();
                    RD_Service_data.MC_data = attr.value().toString();
#if DEBUG
                    qDebug()<<"mc : "<< attr.value().toString();
#endif
                }

            }
        }

        if(xml.name().toString()=="Skey")
        {
            qDebug()<<"In SKEY";

            foreach(const QXmlStreamAttribute &attr, xml.attributes())
            {
                if (attr.name().toString() == QLatin1String("ci"))
                {
                    RD_Service_data.ci.clear();
                    RD_Service_data.ci = attr.value().toString();
#if DEBUG
                    qDebug()<<"ci : "<< attr.value().toString();
#endif
                }
            }

            RD_Service_data.skey.clear();
            RD_Service_data.skey = xml.readElementText();
#if DEBUG
            qDebug()<<"SKEY Value"<<RD_Service_data.skey;
#endif
        }

        if(xml.name().toString()=="Hmac")
        {
            RD_Service_data.hmac.clear();
            RD_Service_data.hmac = xml.readElementText();
#if DEBUG
            qDebug()<<"Hmac Value"<<RD_Service_data.hmac;
#endif
        }

        if(xml.name().toString()=="Data")
        {
            RD_Service_data.PID_DATA.clear();
            RD_Service_data.PID_DATA = xml.readElementText();
#if DEBUG
            qDebug()<<"PIDDATA Value"<< RD_Service_data.PID_DATA;
#endif
        }


    }
    file_xmlleg1.close();


    return;
}


int FPB::on_OK_SBN_pushButton_10_clicked()
{
    char BSBuff[25];
    CMisc miscObj;
    memset(BSBuff,'\0',sizeof(BSBuff));
    QString BSCheck = ui->lineEdit_Bank_Search->text();
    if(BSCheck.isEmpty())
    {
        miscObj.DisplayMsgBox((char *)"Please Enter Bank Name to Search");
        return FUNCTION_FAILED;
    }

    memcpy(BSBuff,ui->lineEdit_Bank_Search->text().trimmed().toAscii().data(),strlen(ui->lineEdit_Bank_Search->text().trimmed().toAscii().data()));

    int ret =  BankSelection(BSBuff);
    if(ret == FUNCTION_FAILED)
    {
        qDebug()<<"Return Failed from Bank Selection";
        return FUNCTION_FAILED;
    }
    qDebug()<<"Going to Page 6";
    ui->BankSelection_LabelName->setText((char *)configLoginLeg2.ucname);
    ui->BankSelection_LabelName->setFont(QFont("SansSerif",10));
    ui->BankSelection_LabelName->setAlignment(Qt::AlignRight | Qt::AlignBottom);
    //    printf("Balance_Display mobile = %s\n",Balance_Display);
    QString bal1 = QString::fromUtf8((const char*)Balance_Display);
    ui->BankSelection_LabelBal->setText(bal1);
    ui->BankSelection_LabelBal->setFont(QFont("SansSerif",9));
    ui->BankSelection_LabelBal->setAlignment(Qt::AlignRight | Qt::AlignBottom);
    ui->stackedWidget->setCurrentIndex(6);  // go to page Bank Name Display
    return FUNC_OK;
}

void FPB::on_BACK_SBN_pushButton_9_clicked()
{
    ui->stackedWidget->setCurrentIndex(2);  // go to page Main Menu
    return;
}

void FPB::on_BtnBackIssuer_clicked()
{
    ui->stackedWidget->setCurrentIndex(7);  // go to page Search Bank Name
    return;
}

void FPB::on_lineEditUID1_textChanged(const QString &arg1)
{
    if(arg1.size() == 4)
        ui->lineEditUID2->setFocus();
}

void FPB::on_lineEditUID2_textChanged(const QString &arg1)
{
    if(arg1.size() == 4)
        ui->lineEditUID3->setFocus();
}

void FPB::on_lineEditUID3_textChanged(const QString &arg1)
{
    CMisc miscObj;
    if(arg1.size() == 4)
    {
        if(ui->lineEditUID1->text().size() != 4)
        {
            qDebug() << "Wrong Values UID1";
            miscObj.DisplayMsgBox((char *)"Enter Correct UID Number!");
            return;
        }

        if(ui->lineEditUID2->text().size() != 4)
        {
            qDebug() << "Wrong Values UID2";
            miscObj.DisplayMsgBox((char *)"Enter Correct UID Number!");
            return;
        }

        ui->PAGE4_OK_pushButton_8->setEnabled(true);
        ui->PAGE4_OK_pushButton_8->setStyleSheet("color: rgb(255, 255, 255);background-color: rgb(85, 0, 127)"); // pink and white
    }
}

void FPB::on_PAGE4_BACK_pushButton_7_clicked()
{
    ui->lineEditUID1->clear();
    ui->lineEditUID2->clear();
    ui->lineEditUID3->clear();
    ui->lineEditUID1->setFocus();
    ui->PAGE4_OK_pushButton_8->setEnabled(false);
    ui->PAGE4_OK_pushButton_8->setStyleSheet("background-color: rgb(85, 85, 127); color: rgb(255, 255, 255)");
    ui->BankName_LabelName->setText((char *)configLoginLeg2.ucname);
    ui->BankName_LabelName->setFont(QFont("SansSerif",10));
    ui->BankName_LabelName->setAlignment(Qt::AlignRight | Qt::AlignBottom);
    ui->BankName_LabelBal->setText(QString::fromUtf8((const char*)Balance_Display));
    ui->BankName_LabelBal->setFont(QFont("SansSerif",9));
    ui->BankName_LabelBal->setAlignment(Qt::AlignRight | Qt::AlignBottom);

    //ui->stackedWidget->setCurrentIndex(2);  // go to page Main Menu
    ui->stackedWidget->setCurrentIndex(7);  // go to Enter Bank Name page
    return;
}

void FPB::on_OK_Mobile_Num_Page9_clicked()
{
    CMisc miscObj;

    QRegExp regexp;
    regexp.setPattern("[A-Z a-z]");
#if DEBUG
    qDebug()<<"lineEdit_Mobile_Num_Page9 == "<<ui->lineEdit_Mobile_Num_Page9->text().isEmpty();
    qDebug()<<"ui->lineEdit_Mobile_Num_Page9->text().toInt() == "<<ui->lineEdit_Mobile_Num_Page9->text().toInt();
#endif
    if((ui->lineEdit_Mobile_Num_Page9->text()!=""))
    {
        //  qDebug()<<"ui->lineEdit_Mobile_Num_Page9->text().toAscii().data() == "<<ui->lineEdit_Mobile_Num_Page9->text().toAscii().data();
        if (memcmp(ui->lineEdit_Mobile_Num_Page9->text().toAscii().data(),"0000000000",10)==0)
        {
            miscObj.DisplayMsgBox((char *)"Please Enter Valid Mobile Number");
            return;
        }
        if (strlen((const char*)ui->lineEdit_Mobile_Num_Page9->text().toAscii().data())<10)
        {
            miscObj.DisplayMsgBox((char *)"Please Enter Valid Mobile Number");
            return;
        }

        if(ui->lineEdit_Mobile_Num_Page9->text().mid(0,1)!= "9" && ui->lineEdit_Mobile_Num_Page9->text().mid(0,1) != "8" && ui->lineEdit_Mobile_Num_Page9->text().mid(0,1)!= "7" && ui->lineEdit_Mobile_Num_Page9->text().mid(0,1)!= "6")
        {
            miscObj.DisplayMsgBox((char *)"Mobile number should start from 9,8,7 or 6");
            return;
        }
        memset(MobileNum_disp,0,sizeof(MobileNum_disp));
        memcpy(MobileNum_disp, ui->lineEdit_Mobile_Num_Page9->text().toAscii().data(), strlen(ui->lineEdit_Mobile_Num_Page9->text().toAscii().data()));
        memcpy(Txn.Mobileno,ui->lineEdit_Mobile_Num_Page9->text().toAscii().data(),strlen(ui->lineEdit_Mobile_Num_Page9->text().toAscii().data()));
        //   qDebug()<<"Entered Mobile Number == "<<QString::fromUtf8(Txn.Mobileno);
    }
    else
    {
        miscObj.DisplayMsgBox((char *)"Please Enter Mobile Number");
        return;
    }

    //    QSqlQuery query;
    //    char buff_MaxTransLimit[100], buff_MinTransLimit[100];
    //    memset(buff_MaxTransLimit, 0, sizeof(buff_MaxTransLimit));
    //    memset(buff_MinTransLimit, 0, sizeof(buff_MinTransLimit));

    //    query.clear();
    //    if(memcmp(Txn.bin,"608001",6)==0)
    //    {
    //        printf("ONUS BIN == %s\n",Txn.bin);
    //        query.prepare("SELECT MinTransLimit, MaxTransLimit from GetProfile where TransactionType = 'CASHWAEPSON'");  // CASH WITHDRAWAL AEPS ONUS
    //    }
    //    else
    //    {
    //        printf("OFFUS BIN == %s\n",Txn.bin);
    //        query.prepare("SELECT MinTransLimit, MaxTransLimit from GetProfile where TransactionType = 'CASHWAEPSACQ'"); // Cash withdrawl with AEPS acc

    //    }

    //    if(!query.exec())
    //    {
    //        qDebug() << query.lastError();
    //        qDebug() << "Get Profile database query selection failed";
    //        //        uidmessage::instance()->hide_Message();
    //        //        uidmessage::instance()->DisplayMessage("Record Not Found..");
    //        query.clear();
    //        query.finish();
    //        ui->stackedWidget->setCurrentIndex(2);
    //        return;
    //    }
    //    else
    //    {
    //        qDebug()<<"else while";
    //        while(query.next())
    //        {
    //            memset(buff_MaxTransLimit,0,sizeof(buff_MaxTransLimit));
    //            memset(buff_MinTransLimit, 0, sizeof(buff_MinTransLimit));
    //            strcpy((char*)buff_MinTransLimit, query.value(0).toString().trimmed().toAscii().data());
    //            strcpy((char*)buff_MaxTransLimit, query.value(1).toString().trimmed().toAscii().data());
    //            printf("Min Trans Limit = %s\n",buff_MinTransLimit);
    //            printf("Max Trans Limit = %s\n",buff_MaxTransLimit);
    //        }
    //    }

    //    int i_min = atoi((const char*)buff_MinTransLimit);
    //    int i_max = atoi((const char*)buff_MaxTransLimit);


    //    qDebug()<<"After Mobile no. enter";

    //    memset(disp_minmax, 0, sizeof(disp_minmax));
    //    while(1)
    //    {
    //        if(Txn.Txntype == AEPS_WITH )
    //            Ttype = 3;

    //        CEntryDlg dlg(this,Ttype);
    //        connect(&dlg,SIGNAL(getText(QString)),this,SLOT(getInputText(QString)));
    //        dlg.exec();
    //        EnteredAmt = entryDlgText;

    //        qDebug()<<"Entered Amount = "<<EnteredAmt;
    //        if(EnteredAmt == "-1")   //Cancel
    //        {
    //            EnteredAmt.clear();
    //            ui->stackedWidget->setCurrentIndex(8);
    //            return;
    //        }

    //        if(EnteredAmt.isEmpty())
    //        {
    //            miscObj.DisplayMsgBox((char *)"Please Enter Amount");
    //            continue;
    //        }
    //        if(EnteredAmt.toInt(0) == 0)
    //        {
    //            miscObj.DisplayMsgBox((char *)"Amount should be greater than 0");
    //            continue;
    //        }
    //        //if(memcmp((int*)EnteredAmt.toAscii().data(),(int*)buff_MinTransLimit,strlen(buff_MinTransLimit)) < 0)
    //        if(EnteredAmt.toInt(0) < i_min)
    //        {
    //            strcpy(disp_minmax,"Minimum amount should be ");
    //            strcat(disp_minmax,buff_MinTransLimit);
    //            miscObj.DisplayMsgBox((char *)disp_minmax);
    //            continue;
    //        }

    //        //if(memcmp((int*)EnteredAmt.toAscii().data(),(int*)buff_MaxTransLimit,strlen(buff_MaxTransLimit)) > 0)
    //        if(EnteredAmt.toInt(0) > i_max)
    //        {
    //            strcpy(disp_minmax,"Maximum amount should be ");
    //            strcat(disp_minmax,buff_MaxTransLimit);
    //            qDebug()<<"disp_min = ";
    //            miscObj.DisplayMsgBox((char *)disp_minmax);
    //            continue;
    //        }

    //        break;
    //    }

    //    sprintf(Txn.Amount,"%d",EnteredAmt.toInt(0));    //Converting To Paises
    //    qDebug() << "EnteredAmt = " << Txn.Amount;// Transaction amount.
    //    strcpy(Txn.uid,strUID.toAscii().constData());  // copy enter UID to UID struct
    //    qDebug()<<"Txn.bin == "<<Txn.bin;
    ui->Bankname_Display_EnterAmtPage_label->setStyleSheet("font-size: 11pt; font-weight: bold; color: rgb(85, 0, 127); background-color:  rgb(255, 255, 255)");
    ui->Bankname_Display_EnterAmtPage_label->setText(gbankname);

    ui->EnterAmount_LabelName ->setText((char *)configLoginLeg2.ucname);
    ui->EnterAmount_LabelName ->setFont(QFont("SansSerif",10));
    ui->EnterAmount_LabelName ->setAlignment(Qt::AlignRight | Qt::AlignBottom);

    ui->EnterAmountLabelBal->setText(QString::fromUtf8((const char*)Balance_Display));
    ui->EnterAmountLabelBal->setFont(QFont("SansSerif",9));
    ui->EnterAmountLabelBal->setAlignment(Qt::AlignRight | Qt::AlignBottom);
    ui->lineEdit_Enter_Amount_Page28->clear();
    QRegExp rx("\\d+");
    QRegExpValidator *validator1 = new QRegExpValidator(rx,this);
    ui->lineEdit_Enter_Amount_Page28->setValidator(validator1);
    ui->lineEdit_Enter_Amount_Page28->setFocus();
    ui->stackedWidget->setCurrentIndex(28);  //  Enter Amount Page
    return;
}

void FPB::on_lineEdit_Mobile_Num_Page9_textChanged(const QString &arg1)
{
    CMisc miscObj;
    if(arg1.size() == 10)
    {
        ui->OK_Mobile_Num_Page9->setEnabled(true);
        ui->OK_Mobile_Num_Page9->setStyleSheet("color: rgb(255, 255, 255); background-color: rgb(85, 0, 127)"); // pink and white
    }
}

void FPB::on_BACK_Mobile_Num_Page9_clicked()
{
    ui->lineEdit_Mobile_Num_Page9->clear();
    ui->OK_Mobile_Num_Page9->setEnabled(false);
    ui->OK_Mobile_Num_Page9->setStyleSheet("background-color: rgb(170, 23, 70); color: rgb(107, 107, 107)");

    ui->UID_LabelName->setText((char *)configLoginLeg2.ucname);
    ui->UID_LabelName->setFont(QFont("SansSerif",10));
    ui->UID_LabelName->setAlignment(Qt::AlignRight | Qt::AlignBottom);
    ui->UID_LabelBal->setText(Balance_Display);
    ui->UID_LabelBal->setFont(QFont("SansSerif",9));
    ui->UID_LabelBal->setAlignment(Qt::AlignRight | Qt::AlignBottom);
    //ui->stackedWidget->setCurrentIndex(7);  // go to page Main Menu
    ui->stackedWidget->setCurrentIndex(3);  // go to UID page
    return;
}



//void FPB::on_pushButton_AEPS_Txn_Menu_clicked()
//{


//    ui->stackedWidget->setCurrentIndex(2);
//    return;
//}

void FPB::on_PushButton_Back_SubTxn_Menu_clicked()
{
    ui->stackedWidget->setCurrentIndex(12);
    return;
}

void FPB::on_Button_display_clicked()
{
    ui->stackedWidget->setCurrentIndex(2);
    return;
}

//void FPB::on_AppUpdate_PushButton_clicked()
//{
//    int iret_AU = FTP_download_function("770071001");
//    if(iret_AU == FUNCTION_FAILED)
//    {
//        ui->stackedWidget->setCurrentIndex(9);
//    }
//    return;
//}

void FPB::on_MinimizePB_clicked()
{
    qDebug()<<"on_MinimizePB_clicked";
    system("matchbox-remote -desktop");
    return;
}





//void FPB::on_PAGE32_MainMenu_exit_button_clicked()
//{
//    if(iDisplayConsent ("Do you want to Exit ?") == false)
//        return;

//    uidmessage::instance()->DisplayMessage("Application Closing, Please Wait..");

//    //    ppp_close();
//    gl11_optcap_pwroff();
//    gl11_iodev_close();
//    exit(1);
//}

char * FPB::URL_CREATION(char *constring)
{
    qDebug()<<"IN";
    char str_url[500];
    memset(str_url,0,sizeof(str_url));
    strcpy(str_url,URL_SSL);
    strcat(str_url,constring);
#if DEBUG
    printf("COMPLETE_URL = %s\n",str_url);
#endif
    return str_url;
}

char * FPB::Convertstr1(string str)
{
    char* cstr;
    string fname = str + '\0';
    cstr = new char [fname.size()+1];
    strcpy( cstr, fname.c_str());

    return cstr;
}

QString FPB::serialize1(char *jsondata)
{
    //    QString BUFF;
    buff_tmp.clear();
    QString jsondata_conv;
    jsondata_conv = QString::fromUtf8((char *)(jsondata)).trimmed();

    jsondata_conv.replace("\n", "");
    jsondata_conv.replace("      ", "");
    jsondata_conv.replace('"',"\"");
    buff_tmp = jsondata_conv;
    return buff_tmp;

}

//QString FPB::deserialize1(char *jsondata)
//{
//    //    QString BUFF;
//    buff_tmp.clear();
//    QString jsondata_conv;
//    jsondata_conv = QString::fromUtf8((char *)(jsondata)).trimmed();

//    jsondata_conv.replace("","\n");
//  //  jsondata_conv.replace("      ", "");
//    jsondata_conv.replace("\"",'"');
//    buff_tmp = jsondata_conv;
//    return buff_tmp;

//}

void FPB::on_pushButton_Dash_AEPS_pg12_clicked()
{
    //    strcpy(configLoginLeg2.ucname, "Dhiral Rupani");
    //    strcpy(Balance_Display,"12394.25");
    ui->SubTransaction_Label1->setText((char *)configLoginLeg2.ucname);
    ui->SubTransaction_Label1->setFont(QFont("SansSerif",10));
    //    ui->SubTransaction_Label1->setFont(QFont("Bold"));
    ui->SubTransaction_Label1->setAlignment(Qt::AlignRight | Qt::AlignBottom);
    //    printf("Balance_Display = %s\n",Balance_Display);

    //    QString bal1 = QString::fromUtf8((const char*)Balance_Display);
    //    qDebug()<<"bal1 = "<<bal1;
    QString BLDISP = QString::fromUtf8((const char*)Balance_Display);
    // ui->SubTransaction_Label2->setText(QString::fromUtf8((const char*)Balance_Display));
    ui->SubTransaction_Label2->setText(BLDISP);
    ui->SubTransaction_Label2->setFont(QFont("SansSerif",9));
    ui->SubTransaction_Label2->setAlignment(Qt::AlignRight | Qt::AlignBottom);

    uidmessage::instance()->hide_Message();
    uidmessage::instance()->Display_BusyMessage(" Please wait...");

    int ibret = iBankMaster();
    uidmessage::instance()->hide_Message();
    if((ibret != FUNC_OK) && (ibret != FUNCTION_SUCCESS))
    {
        if(ibret == SESSION_EXPIRED_FAILED)
            ui->stackedWidget->setCurrentIndex(0);
        else
            ui->stackedWidget->setCurrentIndex(12);
        return;
    }

    ui->stackedWidget->setCurrentIndex(2);
    return;
}

int FPB::login_download_func(void)
{
    qDebug()<<"login_download_func";
    //    QString temp = QString :: fromUtf8((char *)configLoginLeg2.ucLastLogin);
    //    QString LLogin = "Last Login " + temp;
    //    qDebug()<<"LLogin == "<<LLogin;

    //    //QString temp1 = QString :: fromUtf8("********");//(char *)configLoginLeg2.ucavailableBalance);
    //    QString temp1 = "********";
    //    QString BL = "Balance : " + temp1;
    //    qDebug()<<"BL == "<<BL;
    //    ui->Dash_Latest_Balance_Label_pg12->setText(BL);

    //    ui->Last_login->setText(LLogin);
    //    ui->Main_Label_Merchant_Name->setText((char *)configLoginLeg2.ucname);
    //   printf("(char *)configLoginLeg2.ucavailableBalance == %s\n",(char *)configLoginLeg2.ucavailableBalance);
    //    ui->Search_lineEdit->setHidden(true);
    //    ui->Mobile_Button->setHidden(true);
    //    ui->Cust_Id_Button->setHidden(true);
    //    ui->Account_Button->setHidden(true);
    //    ui->Search_Button->setHidden(true);
    ui->MinimizePB->setHidden(true);

    int i_das = DownloadAppSettings(); // Need to change //TEMPORARY
    //  int i_das = FUNC_OK; //TEMPORARY
    uidmessage::instance()->hide_Message(); //TEMPORARY

    if(i_das == FUNC_OK)
    {
        //ui->Label_Branchno->setText((char *)configLoginLeg2.ucBAdescription);
        QString temp = QString :: fromUtf8((char *)configLoginLeg2.ucLastLogin);
        QString LLogin = "Last Login " + temp;
        //  qDebug()<<"LLogin == "<<LLogin;
        //QString temp1 = QString :: fromUtf8("********");//(char *)configLoginLeg2.ucavailableBalance);
        QString temp1;
        QString bal = QString::fromUtf8((const char*)configLoginLeg2.ucavailableBalance);
        bool decimal;
        decimal = bal.contains('.');
        if(decimal == false)
        {
            bal = bal.append(".00");
        }
        memset(configLoginLeg2.ucavailableBalance,0,strlen(configLoginLeg2.ucavailableBalance));
        memcpy(configLoginLeg2.ucavailableBalance,bal.toLatin1().data(),strlen(bal.toLatin1().data()));
        memset(Balance_Display,0,strlen(Balance_Display));
        sprintf(Balance_Display,"Balance : %s\n",bal.toLatin1().data(),strlen(bal.toLatin1().data()));
        strncpy(Mer_Latest_Balance, bal.toLatin1().data(), strlen(bal.toLatin1().data()));
        int i=0;
        int star_size = bal.length();
        qDebug()<<"Size of star"<<star_size;
        temp1.clear();
        for(i=0;i<star_size;i++)
        {
            temp1 = temp1.append('*');
        }
#if DEBUG
        qDebug()<<"Available Balance"<<bal;
#endif
        QString BL = "Balance : " + temp1;
#if DEBUG
        qDebug()<<"BL == "<<BL;
#endif
        // ui->Dash_Latest_Balance_Label_pg12->setText(BL);
        ui->Dash_Latest_Balance_Label_pg12->setText(BL);
        ui->Last_login->setText(LLogin);
        ui->Main_Label_Merchant_Name->setText((char *)configLoginLeg2.ucname);

        //       printf("Dash_Latest_Balance_Label_pg12 == %s\n",(char *)configLoginLeg2.ucavailableBalance);
        //        ui->Search_lineEdit->setHidden(true);
        //        ui->Mobile_Button->setHidden(true);
        //        ui->Cust_Id_Button->setHidden(true);
        //        ui->Account_Button->setHidden(true);
        //        ui->Search_Button->setHidden(true);
        ui->Transaction_Summary_3->setHidden(false);
        ui->MinimizePB->setHidden(true);
        //        ui->stackedWidget->setCurrentIndex(12);
        return FUNCTION_SUCCESS;
    }
    else
    {
        memset(&configLoginLeg1,0,sizeof(configLoginLeg1));
        memset(&configLoginLeg2,0,sizeof(configLoginLeg2));
        memset(gSendData,0,sizeof(gSendData));
        memset(Recvdata,0,sizeof(Recvdata));
        system("rm /usr/FINO/FPB/REQ/LEG2_REQ.TXT");
        system("rm /usr/FINO/FPB/REQ/LEG1_REQ.TXT");
        uidmessage::instance()->hide_Message();
        uidmessage::instance()->Display_BusyMessage("Downloading Application Setting Failed..");
        sleep(1);
        uidmessage::instance()->hide_Message();
        memset(username,0,sizeof(username));
        ui->lineEdit_userid->text().clear();
        ui->lineEdit_password->text().clear();
        //        ui->stackedWidget->setCurrentIndex(0);
        return FUNCTION_FAILED;
    }
}

int FPB::Enter_Amount_capture(void)
{
    char disp_minmax[120];
    QSqlQuery query;
    CMisc miscObj;
    QString EnteredAmt;
    char buff_MaxTransLimit[100], buff_MinTransLimit[100];
    memset(buff_MaxTransLimit, 0, sizeof(buff_MaxTransLimit));
    memset(buff_MinTransLimit, 0, sizeof(buff_MinTransLimit));

    query.clear();
    if(memcmp(Txn.bin,"608001",6)==0)
    {
#if DEBUG
        printf("ONUS BIN == %s\n",Txn.bin);
#endif
        query.prepare("SELECT MinTransLimit, MaxTransLimit from GetProfile where TransactionType = 'CASHWAEPSON'");  // CASH WITHDRAWAL AEPS ONUS
    }
    else
    {
        //     printf("OFFUS BIN == %s\n",Txn.bin);
        query.prepare("SELECT MinTransLimit, MaxTransLimit from GetProfile where TransactionType = 'CASHWAEPSACQ'"); // Cash withdrawl with AEPS acc

    }

    if(!query.exec())
    {
        qDebug() << query.lastError();
        qDebug() << "Get Profile database query selection failed";
        //        uidmessage::instance()->hide_Message();
        //        uidmessage::instance()->DisplayMessage("Record Not Found..");
        query.clear();
        query.finish();
        ui->stackedWidget->setCurrentIndex(2);
        return FUNCTION_FAILED;
    }
    else
    {
        qDebug()<<"else while";
        while(query.next())
        {
            memset(buff_MaxTransLimit,0,sizeof(buff_MaxTransLimit));
            memset(buff_MinTransLimit, 0, sizeof(buff_MinTransLimit));
            strcpy((char*)buff_MinTransLimit, query.value(0).toString().trimmed().toAscii().data());
            strcpy((char*)buff_MaxTransLimit, query.value(1).toString().trimmed().toAscii().data());
            //         printf("Min Trans Limit = %s\n",buff_MinTransLimit);
            //         printf("Max Trans Limit = %s\n",buff_MaxTransLimit);
        }
    }

    int i_min = atoi((const char*)buff_MinTransLimit);
    int i_max = atoi((const char*)buff_MaxTransLimit);



    memset(disp_minmax, 0, sizeof(disp_minmax));

    EnteredAmt =ui->lineEdit_Enter_Amount_Page28->text();

    //   qDebug()<<"Entered Amount = "<<EnteredAmt;
    if(EnteredAmt == "-1")   //Cancel
    {
        EnteredAmt.clear();
        return FUNCTION_FAILED;
    }

    if(EnteredAmt.isEmpty())
    {
        miscObj.DisplayMsgBox((char *)"Please Enter Amount");
        return FUNCTION_FAILED;
    }
    if(EnteredAmt.toInt(0) == 0)
    {
        miscObj.DisplayMsgBox((char *)"Amount should be greater than 0");
        return FUNCTION_FAILED;
    }
    //if(memcmp((int*)EnteredAmt.toAscii().data(),(int*)buff_MinTransLimit,strlen(buff_MinTransLimit)) < 0)
    if(EnteredAmt.toInt(0) < i_min)
    {
        strcpy(disp_minmax,"Minimum amount should be ");
        strcat(disp_minmax,buff_MinTransLimit);
        miscObj.DisplayMsgBox((char *)disp_minmax);
        return FUNCTION_FAILED;
    }

    if(EnteredAmt.toInt(0) > i_max)
    {
        strcpy(disp_minmax,"Maximum amount should be ");
        strcat(disp_minmax,buff_MaxTransLimit);
        qDebug()<<"disp_min = ";
        miscObj.DisplayMsgBox((char *)disp_minmax);
        return FUNCTION_FAILED;
    }


    sprintf(Txn.Amount,"%d",EnteredAmt.toInt(0));    //Converting To Paises
    //   qDebug() << "EnteredAmt = " << Txn.Amount;// Transaction amount.
    strcpy(Txn.uid,strUID.toAscii().constData());  // copy enter UID to UID struct
    //  qDebug()<<"Txn.bin == "<<Txn.bin;
    memset(AadhaarNo_disp, 0, sizeof(AadhaarNo_disp));
    memset(Amount_disp, 0, sizeof(Amount_disp));
    sprintf(Amount_disp,"%d",EnteredAmt.toInt(0));
    strcpy(AadhaarNo_disp,strUID.toAscii().constData());
    return FUNC_OK ;

}


int FPB::idisplayConfirmation(char display[][120])
{
    int i=0;
    qDebug()<<"Display";
    ui->listWidget_Issuer_10->clear(); // clear list
    ui->listWidget_Issuer_10->setFont(QFont("Courier",14));

    for(i=0;i<3;i++)
    {
#if DEBUG
        printf("display    =====    %s\n",display[i]);
#endif
        ui->listWidget_Issuer_10->addItem((char *)display[i]);
    }

    ui->stackedWidget->setCurrentIndex(29);
    return FUNCTION_SUCCESS;
}

void FPB::on_OK_Enter_Amout_Page28_clicked()
{
    int iRet = -2;
    iRet = Enter_Amount_capture();
    if(iRet != FUNC_OK)
    {
        ui->stackedWidget->setCurrentIndex(28); // Enter Amount Page
    }
    else if(iRet == FUNC_OK)
    {
        ui->DispConfirmation_LabelName ->setText((char *)configLoginLeg2.ucname);
        ui->DispConfirmation_LabelName ->setFont(QFont("SansSerif",10));
        ui->DispConfirmation_LabelName ->setAlignment(Qt::AlignRight | Qt::AlignBottom);

        ui->DispConfirmation_LabelBal->setText(QString::fromUtf8((const char*)Balance_Display));
        ui->DispConfirmation_LabelBal->setFont(QFont("SansSerif",9));
        ui->DispConfirmation_LabelBal->setAlignment(Qt::AlignRight | Qt::AlignBottom);

        FLag_Confirmation_Disp = 1;
        //       printf("gbankname = %s, AadhaarNo_disp = %s, Amount_disp = %s\n",gbankname, AadhaarNo_disp, Amount_disp);
        char display_confirmpage[10][120];
        sprintf(display_confirmpage[0],"Bank Name      : %s",BankName_disp);
        sprintf(display_confirmpage[1],"Aadhaar Number : %s",AadhaarNo_disp);
        sprintf(display_confirmpage[2],"Mobile Number  : %s",MobileNum_disp);
        sprintf(display_confirmpage[3],"Amount         : %s",Amount_disp);

        int i=0;
        qDebug()<<"Display";
        ui->listWidget_Issuer_10->clear(); // clear list
        ui->listWidget_Issuer_10->setFont(QFont("Courier",14));

        for(i=0;i<4;i++)
        {
#if DEBUG
            printf("display_confirmpage    =====    %s\n",display_confirmpage[i]);
#endif
            ui->listWidget_Issuer_10->addItem((char *)display_confirmpage[i]);
        }

        ui->stackedWidget->setCurrentIndex(29);
        //idisplayConfirmation(display_confirmpage);//icount);
        //idisplayTxnDetailes(display_confirmpage,1);//icount);
        return;
        //ui->stackedWidget->setCurrentIndex(4); // Finger Print Capture Page
    }
    return;
}

void FPB::on_BACK_Enter_Amout_Page28_clicked()
{
    ui->lineEdit_Mobile_Num_Page9->clear();
    ui->lineEdit_Enter_Amount_Page28->clear();
    ui->lineEdit_Mobile_Num_Page9->setFocus();
    ui->stackedWidget->setCurrentIndex(8);
    qDebug()<<"Inside the back button";
    return;
}


void FPB::on_Next_Button_Confirmation_clicked()
{
    ui->stackedWidget->setCurrentIndex(4); // Finger Print Capture Page
    return;
}

void FPB::on_Button_Confirmation_clicked()
{
    qDebug()<<"Txn.Txntype = "<<Txn.Txntype;
    if(Txn.Txntype == AEPS_BAL)
    {
        ui->Bankname_Display_UIDPage_label->setStyleSheet("font-size: 11pt; font-weight: bold; color: rgb(85, 0, 127); background-color:  rgb(255, 255, 255)");
        ui->Bankname_Display_UIDPage_label->setText(gbankname);

        ui->UID_LabelName->setText((char *)configLoginLeg2.ucname);
        ui->UID_LabelName->setFont(QFont("SansSerif",10));
        ui->UID_LabelName->setAlignment(Qt::AlignRight | Qt::AlignBottom);

        ui->UID_LabelBal->setText(QString::fromUtf8((const char*)Balance_Display));
        ui->UID_LabelBal->setFont(QFont("SansSerif",9));
        ui->UID_LabelBal->setAlignment(Qt::AlignRight | Qt::AlignBottom);
        ui->stackedWidget->setCurrentIndex(3); // Enter Amount Page
    }
    else
    {
        ui->Bankname_Display_EnterAmtPage_label->setStyleSheet("font-size: 11pt; font-weight: bold; color: rgb(85, 0, 127); background-color:  rgb(255, 255, 255)");
        ui->Bankname_Display_EnterAmtPage_label->setText(gbankname);

        ui->EnterAmount_LabelName ->setText((char *)configLoginLeg2.ucname);
        ui->EnterAmount_LabelName ->setFont(QFont("SansSerif",10));
        ui->EnterAmount_LabelName ->setAlignment(Qt::AlignRight | Qt::AlignBottom);

        ui->EnterAmountLabelBal->setText(QString::fromUtf8((const char*)Balance_Display));
        ui->EnterAmountLabelBal->setFont(QFont("SansSerif",9));
        ui->EnterAmountLabelBal->setAlignment(Qt::AlignRight | Qt::AlignBottom);
        ui->stackedWidget->setCurrentIndex(28); // Enter Amount Page

    }

    qDebug()<<"on_Button_Confirmation_clicked";
    //    ui->Bankname_Display_EnterAmtPage_label->setStyleSheet("font-size: 11pt; font-weight: bold; color: rgb(85, 0, 127); background-color:  rgb(255, 255, 255)");
    //    ui->Bankname_Display_EnterAmtPage_label->setText(gbankname);

    //    ui->EnterAmount_LabelName ->setText((char *)configLoginLeg2.ucname);
    //    ui->EnterAmount_LabelName ->setFont(QFont("SansSerif",10));
    //    ui->EnterAmount_LabelName ->setAlignment(Qt::AlignRight | Qt::AlignBottom);

    //    ui->EnterAmountLabelBal->setText(QString::fromUtf8((const char*)Balance_Display));
    //    ui->EnterAmountLabelBal->setFont(QFont("SansSerif",9));
    //    ui->EnterAmountLabelBal->setAlignment(Qt::AlignRight | Qt::AlignBottom);
    //    ui->stackedWidget->setCurrentIndex(28); // Enter Amount Page
    return;
}


void FPB::on_FirstTimeLogin_pushButton_pg30_clicked()
{
    Flag_FirstTime_Login = 0;
    ui->stackedWidget->setCurrentIndex(0);
    return;
}



/*******************************************************
      Detach only Requestdata fron request packet
*********************************************** *******/
int FPB:: RequestDetachEncryptAttach(char *cPacket, int iPacketType, int iPacketLen)
{
    char cTempBuff[100];
    char *WlkPtr = NULL;
    char *WlkPtr1 = NULL;
    int i=0,DestIndex=0, iEncpLenIn = 0,iEncpLenOut = 0;
    int iOpenBraceCnt = 0;
    int iCloseBraceCnt = 0;
    char *prtEncpStart = NULL;
    char *prtEncpEnd = NULL;
    unsigned char salt_value[20];

    memset(cTempBuff,0,sizeof(cTempBuff));
    memset(cEncpPacketPartIn,0,sizeof(cEncpPacketPartIn));

    strcpy(cTempBuff,"RequestData");
    WlkPtr = cPacket;
    while(i<iPacketLen)
    {
        cFinalPacket[DestIndex++] = *WlkPtr;
        if(memcmp(WlkPtr,cTempBuff,strlen(cTempBuff)) == 0)
        {
#if DEBUG
            qDebug()<<"@@@@@@@@@@@@@@@@@@@@@@@@@";
            printf("Pointer of WlkPtr%s\n",WlkPtr);
#endif
            WlkPtr++;
            while(*WlkPtr != '{' && i < iPacketLen)
            {
                cFinalPacket[DestIndex++] = *WlkPtr++;
                i++;
            }
            // cFinalPacket[DestIndex++] = '{'; //12th Feb
            break;
        }
        else
        {
            WlkPtr++;
            i++;
        }
    }
    prtEncpStart = WlkPtr;
    while(i<iPacketLen)
    {
        switch(*WlkPtr)
        {
        case '{':
            iOpenBraceCnt++;
            break;
        case '}':
            iCloseBraceCnt++;
            break;
        }
        if(iOpenBraceCnt == iCloseBraceCnt)
        {
            prtEncpEnd = WlkPtr;
            break;
        }
        WlkPtr++;
    }
    WlkPtr++;
    iEncpLenIn = (int) (WlkPtr-prtEncpStart);
    // prtEncpStart++;//12th Feb
    //iEncpLenIn--;//12th Feb
    memcpy(cEncpPacketPartIn,prtEncpStart,iEncpLenIn);
    memset((char *)salt_value,0,strlen((char *)salt_value));
    uid_random_bytes(salt_value,8);

    if(iPacketType == 1)
    {
        remove_slash(cEncpPacketPartIn);
    }
    else if(iPacketType == 2)
    {
  #if DEBUG
        printf("cEncpPacketPartIn %s\n\n",cEncpPacketPartIn);
  #endif
        deserialize1((char *)cEncpPacketPartIn);//to get data single form
    }
    encryption((unsigned char *)configLoginLeg2.ucSessionId,salt_value,(unsigned char *)cEncpPacketPartIn,(unsigned char *)cEncpIntermitdatePacketPart,&iEncpLenOut);
    sprintf((char *)cEncpPacketPartOut,"{\\\"RequestData\\\":\\\"%s\\\"}",cEncpIntermitdatePacketPart);
    iEncpLenOut = strlen((char *)cEncpPacketPartOut);


    WlkPtr1 = (char *)cEncpPacketPartOut;
    while(iEncpLenOut--)
        cFinalPacket[DestIndex++] = *WlkPtr1++;

    while(i<iPacketLen)
    {
        cFinalPacket[DestIndex++] = *WlkPtr++;
        i++;
    }
    memset(cPacket,0,strlen(cPacket));
    memcpy(cPacket,cFinalPacket,strlen((char *)cFinalPacket));
#if DEBUG
    qDebug()<<"7";
    system("cd /usr/FINO/FPB/REQ/");
    system("rm /usr/FINO/FPB/REQ/Encrypt.TXT");
    FILE *fp = NULL;
    fp =fopen("/usr/FINO/FPB/REQ/Encrypt.TXT","a");
    fwrite(cFinalPacket,strlen((char *)cFinalPacket),1,fp);
    //ttestall(0,100);
    sleep(1);
    fclose(fp);
#endif
    return 0;

}

/*******************************************************
     Remove '/' from Request and '/' from encrypted response
*********************************************** *******/

void FPB::remove_slash(unsigned char * Data)
{
    QString Slash_value ;
    Slash_value.clear();
    Slash_value = QString::fromUtf8((const char*)Data);
#if DEBUG
    qDebug()<<"ENC_Slash_valueData in qstring == "<<Slash_value;
#endif
    Slash_value = Slash_value.simplified().remove('\\');
#if DEBUG
    qDebug()<<"Slash_value without space == "<<Slash_value;
#endif
    memset((char *)Data,0,strlen((char *)Data));
    memcpy(Data,Slash_value.toLatin1().data(),strlen(Slash_value.toLatin1().data()));
#if DEBUG
    printf("FINALDATA == %s\n",Data);
#endif
    return;
}
/*******************************************************
     Remove Space from the request and Response
*********************************************** *******/
void FPB::remove_space(unsigned char * Data)
{
    QString Enc_value ;
    Enc_value.clear();
    Enc_value = QString::fromUtf8((const char*)Data);
#if DEBUG
    qDebug()<<"ENC_Data in qstring == "<<Enc_value;
#endif
    Enc_value = Enc_value.simplified().remove(' ');
#if DEBUG
    qDebug()<<"ENC_Data without space == "<<Enc_value;
#endif
    memset((char *)Data,0,strlen((char *)Data));
    memcpy(Data,Enc_value.toLatin1().data(),strlen(Enc_value.toLatin1().data()));
#if DEBUG
    printf("FINALDATA == %s\n",Data);
#endif
    return;
}

/*******************************************************
     Encrypt the request packet
*********************************************** *******/

int FPB::encryption(unsigned char * passpharse,unsigned char * salt,unsigned char * plaintext,unsigned char * DATA,int *outdata_len)
{

    unsigned char concat_hash[100];
    unsigned char key[100];
    unsigned char iv[50];
    int i;
    int out_len =0;
    unsigned char out_enc[1024*10];
    unsigned char plaintext_salt[1024*10];

    Derivedkey(passpharse,salt,concat_hash);
#if DEBUG
    printf("value-->");
    for(i=0;i<48;i++)
        printf("%02X",concat_hash[i]);
    printf("\n");
#endif
    memset((char *)key,0,strlen((char *)key));
    memset((char *)iv,0,strlen((char *)iv));
    memcpy(key,concat_hash,32);
    memcpy(iv,concat_hash+32,16);
#if DEBUG
    printf("Current Key-->");
    for(i=0;i<64;i++)
        printf("%02X",key[i]);
    printf("\n");

    printf("  iv-->");
    for(i=0;i<16;i++)
        printf("%02X",iv[i]);
    printf("\n");

    printf("Print the plaintext%s",(char*)plaintext);
#endif
    uid_aes_encrypt_256(plaintext,strlen((char*)plaintext), out_enc, &out_len,key,iv);
#if DEBUG
    printf("After AES Encryption:\n");
    for(i=0;i<out_len;i++)
        printf("%02X",out_enc[i]);
    printf("\n");
#endif
    memcpy(plaintext_salt,"Salted__",8);

    memcpy(plaintext_salt+8,salt,8);

    memcpy(plaintext_salt+16,out_enc,out_len);

    uid_encode_b64((unsigned char *)plaintext_salt, out_len+16, DATA, outdata_len);

    remove_space(DATA);

    return 1;
}


/*******************************************************
     Retrieve key and iv from sessiion Id and salt value
*********************************************** *******/

void FPB::Derivedkey(unsigned char* passpharse,unsigned char* salt,unsigned char* concat_hash)
{
    unsigned char prehash[100];
    unsigned char md5_firstvalue[50],md5_secondvalue[50],md5_thirdvalue[50];
    unsigned char ivalue[50];
    int i= 0;

    //First md5 value
    memset((char *)md5_firstvalue,0,strlen((char *)md5_firstvalue));
    memset((char *)prehash,0,strlen((char *)prehash));
    memcpy((char *)prehash,(char *)passpharse,36);
    memcpy((char *)prehash+36,(char *)salt,8);
#if DEBUG
    printf("prehash%s",prehash);
#endif
    MD5(prehash,44,md5_firstvalue);
#if DEBUG
    for(i=0;i<16;i++)
        printf("%02X",md5_firstvalue[i]);
    printf("\n");
#endif
    memcpy((char *)concat_hash,(char *)md5_firstvalue,16);//concat the string

    //Second md5 value

    memset((char *)md5_secondvalue,0,strlen((char *)md5_secondvalue));
    memset((char *)prehash,0,strlen((char *)prehash));
    memcpy((char *)prehash,(char *)md5_firstvalue,16);
#if DEBUG
    for(i=0;i<16;i++)
        printf("%02X",prehash[i]);
    printf("\n");
#endif
    memcpy((char *)prehash+16,(char *)passpharse,36);
    memcpy((char *)prehash+52,(char *)salt,8);
    MD5(prehash,60,md5_secondvalue);
#if DEBUG
    for(i=0;i<16;i++)
        printf("%02X",md5_secondvalue[i]);
    printf("\n");
#endif
    memcpy((char *)concat_hash+16,(char *)md5_secondvalue,16);//concat the string


    //Third md5 value

    memset((char *)md5_thirdvalue,0,strlen((char *)md5_thirdvalue));
    memset((char *)prehash,0,strlen((char *)prehash));
    memcpy((char *)prehash,(char *)md5_secondvalue,16);
    memcpy((char *)prehash+16,(char *)passpharse,36);
    memcpy((char *)prehash+52,(char *)salt,8);
    MD5(prehash,60,md5_thirdvalue);
#if DEBUG
    for(i=0;i<16;i++)
        printf("%02X",md5_thirdvalue[i]);
    printf("\n");
#endif
    memcpy((char *)concat_hash+32,(char *)md5_thirdvalue,16);
    return ;

}


/*******************************************************
     Detach only responsedata fron Response packet
*********************************************** *******/

int FPB:: ResponseDetachEncryptAttach(char *cPacket, int iPacketType, int iPacketLen)
{
    char cTempBuff[100];
    char *WlkPtr = NULL;
    char *WlkPtr1 = NULL;
    int i=0,DestIndex=0, iEncpLenIn = 0,iEncpLenOut = 0;
    char *prtEncpStart = NULL;
    memset(cTempBuff,0,sizeof(cTempBuff));
    memset(cEncpPacketPartIn,0,sizeof(cEncpPacketPartIn));
    memset(cEncpPacketPartOut,0,sizeof(cEncpPacketPartOut));
    strcpy(cTempBuff,"ResponseData");

    WlkPtr = cPacket;



    while(i<iPacketLen)
    {
        cFinalPacket[DestIndex++] = *WlkPtr;
        if(memcmp(WlkPtr,cTempBuff,strlen(cTempBuff)) == 0)
        {
            WlkPtr++;
            do
            {
                cFinalPacket[DestIndex++] = *WlkPtr;
                i++;
            }
            while(*WlkPtr++ != '"');
            do
            {
                cFinalPacket[DestIndex++] = *WlkPtr;
                i++;
            }
            while(*WlkPtr++ != '"');
#if DEBUG
            qDebug()<<"@@@@@@@@@@@@@@@@@@@@@@@@@";
            printf("Pointer of WlkPtr%s\n",WlkPtr);
#endif
            break;
        }
        else
        {
            WlkPtr++;
            i++;
        }
    }

    prtEncpStart = WlkPtr;

    while(*WlkPtr++ != '"');
    WlkPtr--;

    iEncpLenIn = (int) (WlkPtr-prtEncpStart);

    memcpy(cEncpPacketPartIn,prtEncpStart,iEncpLenIn);
#if DEBUG
    printf("cEncpPacketPartIn%s\n",cEncpPacketPartIn);
#endif
    Decryption((unsigned char *)cEncpPacketPartIn,(unsigned char *)configLoginLeg2.ucSessionId,cEncpPacketPartOut);
    iEncpLenOut = strlen((char *)cEncpPacketPartOut);

    WlkPtr1 = (char *)cEncpPacketPartOut;
    while(iEncpLenOut--)
        cFinalPacket[DestIndex++] = *WlkPtr1++;

    while(i<iPacketLen)
    {
        cFinalPacket[DestIndex++] = *WlkPtr++;
        i++;
    }
    memset(cPacket,0,strlen(cPacket));
    memcpy(cPacket,cFinalPacket,strlen((char *)cFinalPacket));
    WlkPtr = NULL;
#if DEBUG
    qDebug()<<"7";
    system("cd /usr/FINO/FPB/REQ/");
    system("rm /usr/FINO/FPB/REQ/Encrypt.TXT");
    FILE *fp = NULL;
    fp =fopen("/usr/FINO/FPB/REQ/Encrypt.TXT","a");
    fwrite(cFinalPacket,strlen((char *)cFinalPacket),1,fp);
    //ttestall(0,100);
    sleep(1);
    fclose(fp);
#endif
    return 0;

}


/*******************************************************
     Retrieve Encrypted Data from response
*********************************************** *******/

void FPB::Decryption(unsigned char* encypted_text, unsigned char* passpharse,unsigned char* plain_text)
{

    unsigned char salt_value[20];
    unsigned char concat_hash[100];
    unsigned char key[100];
    unsigned char iv[50];
    char* base64EncodeOutput = NULL;
    size_t test;
    int encypted_len=0;
    remove_slash(encypted_text);
#if DEBUG
    printf("Print %s\n ",encypted_text);
#endif
    encypted_len = strlen((char *)encypted_text);

    //Base64 decoding
    Base64Decode((char *)encypted_text, (unsigned char **)&base64EncodeOutput, &test);

#if DEBUG
    int i=0;
    printf("\nBase64------------->");
    for(i=0;i<test;i++)
        printf("%02X",base64EncodeOutput[i]);
    printf("\n");
#endif
    //Removing Salted__ and salt value from decoded data
    if (strncmp((const char*)base64EncodeOutput,"Salted__",8) == 0)
    {
#if DEBUG
        qDebug()<<"Inside the  comp loop";
        for(i=8;i<test;i++)
            printf("%02X",base64EncodeOutput[i]);
        printf("\n");
#endif
        memcpy((char *)salt_value,(char *)base64EncodeOutput+8,8);
        base64EncodeOutput += 16;
        test -= 16;
    }
#if DEBUG
    printf("\nsalt------------->");
    for(i=0;i<8;i++)
        printf("%02X",salt_value[i]);
    printf("\n");
#endif
    //Extracting key and iv
    Derivedkey((unsigned char *)passpharse,salt_value,concat_hash);

    memset((char *)key,0,strlen((char *)key));
    memset((char *)iv,0,strlen((char *)iv));
    memcpy(key,concat_hash,32);
    memcpy(iv,concat_hash+32,16);
#if DEBUG

    printf("\nbase64EncodeOutput------------->");
    for(i=0;i<test;i++)
        printf("%02X",base64EncodeOutput[i]);
    printf("\n");
#endif
    //Aes decryption of text
    decrypt((unsigned char *)base64EncodeOutput, test, key, iv,plain_text);
    base64EncodeOutput = NULL;
#if DEBUG
    printf("Plain text %s",plain_text);
    qDebug()<<"End of Encryption";
#endif
}


int FPB::validate_connection()
{

    char Gprsethernet[50];
    FILE *fp;
    const char *ptr;
    int fp_size=0;
    system((char *)"rm -f  /mnt/jffs2/gprs.txt");
    system("cat /var/gprs > /mnt/jffs2/gprs.txt");
    fp=fopen("/mnt/jffs2/gprs.txt","r");
    if(fp == NULL)
    {
        qDebug("Fail to open the file");
        return FUNCTION_FAILED ;
    }
    else
    {

        memset(Gprsethernet,0, sizeof(Gprsethernet));
        fp = fopen("/mnt/jffs2/gprs.txt","rb");
        fseek(fp, 0, SEEK_END);
        fp_size=ftell(fp);
        fprintf(stdout, "fp_size = %d\n", fp_size);
        fseek(fp, 0, SEEK_SET);
        int ret=fread(Gprsethernet,sizeof(unsigned char ),fp_size,fp);
        fread(Gprsethernet,fp_size,1,fp);

        if(strstr(Gprsethernet,"0,X") ||  strstr(Gprsethernet,"1,H")|| strstr(Gprsethernet,"2,H") || strstr(Gprsethernet,"2,E") || strstr(Gprsethernet,"1,E")|| strstr(Gprsethernet,"0,W"))
        {
            fclose(fp);
            return FUNCTION_SUCCESS;
        }
        else
        {
            fclose(fp);
            return FUNCTION_FAILED;
        }
    }
}


void FPB::on_Option_Button_clicked()
{

    ui->Bill_payment_Button->setVisible(true);
    ui->Reversal_Button->setVisible(true);
    ui->Remittance_button->setVisible(true);//added on 25/07/2019

    if(CUST_TYPE == 'W')
        ui->stackedWidget->setCurrentIndex(12);
    else
        ui->stackedWidget->setCurrentIndex(14);
    return;
}

void FPB::clear_IMPS(void)
{

    ui->Bene_acc_no_edit->clear();
    ui->cmb_bankList->clear();
    ui->cmb_Registered_Bene_List->clear();
    ui->Bene_Name_edit->clear();
    ui->IFSC_edit->clear();
    ui->Remark_lineedit->clear();
    ui->Fund_transfer_amount_edit->clear();
    ui->lbl_TotalAmount->clear();
    ui->lbl_CashContribution->clear();
    ui->OTP_Lineedit->clear();
    ui->Otp_display_label->clear();
    ui->Name_lineedit->clear();
    ui->Person_add_edit->clear();
    ui->Person_pincode_edit->clear();
    return;

}


void FPB::Clear_SupportData(void)
{
    memset(postbeneverif.sd_SPROD, 0, sizeof(postbeneverif.sd_SPROD));
    memset(postbeneverif.sd_PVTXNID, 0, sizeof(postbeneverif.sd_PVTXNID));
    memset(postbeneverif.sd_PVTXNDT, 0, sizeof(postbeneverif.sd_PVTXNDT));
    memset(postbeneverif.sd_IPADDR, 0, sizeof(postbeneverif.sd_IPADDR));
    memset(postbeneverif.sd_DEVICEID, 0, sizeof(postbeneverif.sd_DEVICEID));
    memset(postbeneverif.sd_APPID, 0, sizeof(postbeneverif.sd_APPID));
    memset(postbeneverif.sd_AUTHID, 0, sizeof(postbeneverif.sd_AUTHID));
    memset(postbeneverif.sd_LOCATION, 0, sizeof(postbeneverif.sd_LOCATION));
    memset(postbeneverif.sd_CELLID, 0, sizeof(postbeneverif.sd_CELLID));
    memset(postbeneverif.sd_MCC, 0, sizeof(postbeneverif.sd_MCC));
    memset(postbeneverif.sd_RPTFLG, 0, sizeof(postbeneverif.sd_RPTFLG));
    memset(postbeneverif.sd_PRTXNID, 0, sizeof(postbeneverif.sd_PRTXNID));
    memset(postbeneverif.sd_PRTXNAMT, 0, strlen((char *)postbeneverif.sd_PRTXNAMT));
    memset(postbeneverif.sd_SPLTSEQ, 0, sizeof(postbeneverif.sd_SPLTSEQ));
    memset(postbeneverif.sd_CHGAMT_CREDIT, 0, sizeof(postbeneverif.sd_CHGAMT_CREDIT));
    memset(postbeneverif.sd_CHGAMT, 0, sizeof(postbeneverif.sd_CHGAMT));
    memset(postbeneverif.sd_ZRFUT1, 0, sizeof(postbeneverif.sd_ZRFUT1));
    memset(postbeneverif.sd_ZRFUT2, 0, strlen((char *)postbeneverif.sd_ZRFUT2));
    memset(postbeneverif.sd_ZRFUT3, 0, sizeof(postbeneverif.sd_ZRFUT3));
    memset(postbeneverif.sd_ZRFUT4, 0, sizeof(postbeneverif.sd_ZRFUT4));
    memset(postbeneverif.sd_ZRFUT5, 0, sizeof(postbeneverif.sd_ZRFUT5));
    memset(postbeneverif.sd_ZRFUT6, 0, sizeof(postbeneverif.sd_ZRFUT6));
    memset(postbeneverif.sd_ZRFUT7, 0, sizeof(postbeneverif.sd_ZRFUT7));
    memset(postbeneverif.sd_ZRFUT8, 0, sizeof(postbeneverif.sd_ZRFUT8));
    memset(postbeneverif.sd_ZRFUT9, 0, sizeof(postbeneverif.sd_ZRFUT9));
    memset(postbeneverif.sd_ZRFUN2, 0, sizeof(postbeneverif.sd_ZRFUN2));
    memset(postbeneverif.sd_ZRFUN3, 0, sizeof(postbeneverif.sd_ZRFUN3));
    memset(postbeneverif.sd_ZRFUN4, 0, sizeof(postbeneverif.sd_ZRFUN4));
    memset(postbeneverif.sd_NETID, 0, sizeof(postbeneverif.sd_NETID));
    memset(postbeneverif.sd_MSGTYP, 0, sizeof(postbeneverif.sd_MSGTYP));
    memset(postbeneverif.sd_SPROD_Debit, 0, sizeof(postbeneverif.sd_SPROD_Debit));

    return;

}

void FPB::Clear_DeviceInfo(void)
{
    memset(sd_deviceinfo.ucSupportData_PVTXNID_req,         0, sizeof(sd_deviceinfo.ucSupportData_PVTXNID_req));
    memset(sd_deviceinfo.ucSupportData_PVTXNDT_req,         0, sizeof(sd_deviceinfo.ucSupportData_PVTXNDT_req));
    memset(sd_deviceinfo.ucSupportData_IPADDR_req,          0, sizeof(sd_deviceinfo.ucSupportData_IPADDR_req));
    memset(sd_deviceinfo.ucSupportData_DEVICEID_req,        0, sizeof(sd_deviceinfo.ucSupportData_DEVICEID_req));
    memset(sd_deviceinfo.ucSupportData_APPID_req,           0, sizeof(sd_deviceinfo.ucSupportData_APPID_req));
    memset(sd_deviceinfo.ucSupportData_MCCCellID_req,       0, sizeof(sd_deviceinfo.ucSupportData_MCCCellID_req));
    memset(sd_deviceinfo.ucSupportData_MCCLAC_req,          0, sizeof(sd_deviceinfo.ucSupportData_MCCLAC_req));
    memset(sd_deviceinfo.ucSupportData_MCCMCC_req,          0, sizeof(sd_deviceinfo.ucSupportData_MCCMCC_req));
    memset(sd_deviceinfo.ucSupportData_MCCMNC_req,          0, sizeof(sd_deviceinfo.ucSupportData_MCCMNC_req));
    memset(sd_deviceinfo.ucSupportData_MCC_req,             0, sizeof(sd_deviceinfo.ucSupportData_MCC_req));
    memset(sd_deviceinfo.ucSupportData_PRTXNID_req,         0, sizeof(sd_deviceinfo.ucSupportData_PRTXNID_req));
    memset(sd_deviceinfo.ucSupportData_SPLTSEQ_req,         0, sizeof(sd_deviceinfo.ucSupportData_SPLTSEQ_req));
    memset(sd_deviceinfo.ucSupportData_CHGAMT_req,          0, sizeof(sd_deviceinfo.ucSupportData_CHGAMT_req));
    memset(sd_deviceinfo.ucSupportData_CELLID_req,          0, sizeof(sd_deviceinfo.ucSupportData_CELLID_req));
    memset(sd_deviceinfo.ucSupportData_CELLIDLatitude_req,  0, sizeof(sd_deviceinfo.ucSupportData_CELLIDLatitude_req));
    memset(sd_deviceinfo.ucSupportData_CELLIDLongitude_req, 0, sizeof(sd_deviceinfo.ucSupportData_CELLIDLongitude_req));

    return;
}


int FPB::Login_encryption(char *cPacket, int iPacketLen,unsigned char *static_key)
{
#if DEBUG
    qDebug()<<"Inside Login encryption";
#endif
    // unsigned char salt_value[20];
    int iEncpLenOut =0;
    int ihashOutlen = 0;
    int rsab64outputlen =0;
    int rsalen =0;
    unsigned char hashvalue[400];
    unsigned char rsaoutput[400];
    unsigned char rsab64output[400];
    // unsigned char static_key[] = "3472e8b0-4660-4739-a6d4-10c656c63d8b";
    // unsigned char salt_value[] = "ABCDEFGH";

    unsigned char salt_value[10];
    char* jsondata;
    Json1::Value mainreq;
    memset((char*)salt_value,0,sizeof(salt_value));
    memset(cEncpPacketPartOut,0,sizeof(cEncpPacketPartOut));
    memset((char*)hashvalue,0,sizeof(hashvalue));
    memset((char*)rsaoutput,0,sizeof(rsaoutput));


#if DEBUG
    printf("static_key%s",static_key);
#endif

    srand(time(0));
    for(unsigned int i = 0; i < 8; ++i)
    {
        salt_value[i] += genRandom();
    }
 #if DEBUG
    printf("salt_value%s",salt_value);
#endif

    //  uid_random_bytes(salt_value,8);
    //  uid_random_bytes(static_key,36);//static_key must be 36 bit always

    encryption(static_key,salt_value,(unsigned char *)cPacket,(unsigned char *)cEncpPacketPartOut,&iEncpLenOut);

    hash_value(static_key,salt_value,hashvalue,ihashOutlen);

    fpb_rsa_encrypt_with_fino_pubkey(static_key,36,rsaoutput,&rsalen);
    uid_encode_b64(rsaoutput,rsalen, rsab64output, &rsab64outputlen);
    remove_space(rsab64output);
#if DEBUG
    printf("payloadData%s\n",cEncpPacketPartOut);
    printf("hashValue%s\n",hashvalue);
    printf("sessionId%s\n",rsab64output);
#endif

    mainreq["payloadData"]=(char *)cEncpPacketPartOut;
    mainreq["hashValue"]=(char *)hashvalue;
    mainreq["sessionId"]=(char *)rsab64output;
    jsondata = Convertstr(mainreq.toStyledString());
#if DEBUG
    qDebug()<<"------------->"<<jsondata;
#endif
    memset(cPacket,0,sizeof(cPacket));
    strncpy((char*)cPacket, jsondata, strlen(jsondata));
#if DEBUG
    printf("cPacket = %s\n",cPacket);
#endif
}

int FPB::LoginDetachEncryptAttach(char *cPacket, int iPacketLen,unsigned char * static_key,int iPacketType)
{
    system("cd /usr/FINO/FPB/RESP/");
    system("rm /usr/FINO/FPB/RESP/Encrypt.TXT");
    FILE *fp = NULL;
    fp =fopen("/usr/FINO/FPB/RESP/Encrypt.TXT","a");
    fwrite(cPacket,strlen((char *)cPacket),1,fp);
    //ttestall(0,100);
    sleep(1);
    fclose(fp);


    system(" sed 's/\\\\//g' /usr/FINO/FPB/RESP/Encrypt.TXT > /tmp/2.txt");
    system(" sed 's/\\\\//g' /tmp/2.txt > /usr/FINO/FPB/RESP/LEG2_RESP.TXT");

    Json1::Value root;
    root.clear();
    //QSqlQuery query;
    Json1::Reader reader;
    std::ifstream ReadData("/usr/FINO/FPB/RESP/LEG2_RESP.TXT");
    bool parsedSuccess = reader.parse(ReadData,root,false);

    ReadData.close();

    if(not parsedSuccess)
    {

        qDebug()<<"FpsMonthlyPdsData File not Found";
        return FUNCAL_FAILED;
    }
    else
    {
#if DEBUG
        qDebug()<<"payloadResponse   : "<<Convertstr(root["payloadResponse"].asString());
        qDebug()<<"dk : "<<Convertstr(root["dk"].asString());
        qDebug()<<"seq    : "<<Convertstr(root["seq"].asString());
#endif
        memset(cEncpPacketPartIn,0,sizeof(cEncpPacketPartIn));
        strcpy((char *)cEncpPacketPartIn,Convertstr(root["dk"].asString()));
        if(strlen((char *)cEncpPacketPartIn)>0)
        {
            iPacketType = 2;
        }
        else
        {
            iPacketType = 1;

        }

        if(iPacketType==2)
        {
            Decryption((unsigned char *)cEncpPacketPartIn, static_key,cEncpPacketPartOut);
            memset((char *)cEncpPacketPartIn,0,sizeof(cEncpPacketPartIn));
            memset(cFinalPacket,0,sizeof(cFinalPacket));

            strcpy((char *)cEncpPacketPartIn,Convertstr(root["payloadResponse"].asString()));
            Decryption((unsigned char *)cEncpPacketPartIn, cEncpPacketPartOut,cFinalPacket);

        }
        if(iPacketType==1)
        {
            memset(cFinalPacket,0,sizeof(cFinalPacket));
            strcpy((char *)cEncpPacketPartIn,Convertstr(root["payloadResponse"].asString()));
            Decryption((unsigned char *)cEncpPacketPartIn, static_key,cFinalPacket);

        }

        // strcpy(cEncpPacketPartIn,Convertstr(root["seq"].asString()));


        memset(cPacket,0,strlen(cPacket));
        memcpy(cPacket,cFinalPacket,strlen((char *)cFinalPacket));
        return FUNC_OK;
    }

}



int FPB::hash_value(unsigned char * passpharse,unsigned char * hash_salt,unsigned char * Hashpac,int Hashoutlen)
{
    unsigned char hashplaintext[100];
    unsigned char sha256_value[100];
    unsigned char concat_hash[100];
    unsigned char hashplain_salt[100];
    int i;
    int out_len =0;
    unsigned char out_enc[100];
    unsigned char key[100];
    unsigned char iv[50];
    unsigned char sha256_hex[100];

    memset((char *)hashplaintext,0,strlen((char *)hashplaintext));
    memset((char *)sha256_value,0,strlen((char *)sha256_value));
    memset((char *)hashplain_salt,0,strlen((char *)hashplain_salt));
    memset((char *)sha256_value,0,strlen((char *)sha256_value));

    //  convert statickey + LOGIN in sha256 value
    memcpy(hashplaintext,passpharse,36);
    memcpy(hashplaintext+36,"LOGIN",5);
    hMacSha256((char *)hashplaintext,41,sha256_value);

#if DEBUG
    printf("sha256 value-->");
    for(i=0;i<32;i++)
    {
        printf("%02X",sha256_value[i]);
    }
    printf("\n");

#endif

    //  convert sha256 value fron binary to Hex string
    BinToHex(sha256_value,(char *)sha256_hex,31);//<= inside the function
#if DEBUG
    printf("Hex string Value\n");
    printf("sha256_hex%s\n",sha256_hex);
#endif

    //Derive key and iv from
    Derivedkey(passpharse,hash_salt,concat_hash);
#if DEBUG
    printf("value-->");
    for(i=0;i<48;i++)
        printf("%02X",concat_hash[i]);
    printf("\n");
#endif

    memset((char *)key,0,strlen((char *)key));
    memset((char *)iv,0,strlen((char *)iv));
    memcpy(key,concat_hash,32);
    memcpy(iv,concat_hash+32,16);
#if DEBUG
    printf("The value of sha :##########  %d\n",strlen((char *)sha256_hex));
#endif

    uid_aes_encrypt_256(sha256_hex,64, out_enc, &out_len,key,iv);
#if DEBUG
    printf("After AES Encryption:\n");
    for(i=0;i<out_len;i++)
        printf("%02X",out_enc[i]);
    printf("\n");
#endif

    //concat Salted__ key word and plain salt with aes encryption value
    memcpy(hashplain_salt,"Salted__",8);
    memcpy(hashplain_salt+8,hash_salt,8);
    memcpy(hashplain_salt+16,out_enc,out_len);

    //Base64 convertion
    uid_encode_b64((unsigned char *)hashplain_salt, out_len+16, Hashpac, &Hashoutlen);
    remove_space(Hashpac);
#if DEBUG
    printf("Hashpac  :  %s",Hashpac);
#endif

}


int FPB::secretanswer_encryption(char * key,char *aes_input , char *data1)
{

    size_t test;
    char* base64EncodeOutput = NULL;

    int outdata_len =0,outlen = 0;
    unsigned char output[40];
    Base64Decode((char *)key, (unsigned char **)&base64EncodeOutput, &test);
    int size = test;

 #if DEBUG
    printf("test%d",test);
    qDebug()<<"size"<<size;
    for(int i=0;i<test;i++)
        printf("%02X",base64EncodeOutput[i]);
    printf("\n");
#endif

    // memcpy(aes_key,base64EncodeOutput,16);
    memset(output,0,sizeof(output));
    uid_aes_encryption_128((unsigned char *)aes_input, 6, output,&outdata_len, (unsigned char *)base64EncodeOutput, NULL);
 #if DEBUG
    qDebug()<<outdata_len;
#endif
    memset(data1,0,sizeof(data1));
    uid_encode_b64(output,outdata_len , (unsigned char *)data1, &outlen);
    remove_space((unsigned char *)data1);
    QString Enc_value = QString::fromUtf8((const char*)data1);
#if DEBUG
    qDebug()<<"Inside encrypt"<<Enc_value;
#endif

}

int FPB::deserialize1(char * jsondata)
{
    system("cd /usr/FINO/FPB/REQ/");
    system("rm /usr/FINO/FPB/REQ/Deserialize.TXT");
    system("rm /usr/FINO/FPB/REQ/serialize.txt");
    FILE *fp2 = NULL;
    FILE *fp1 =NULL;
    fp2 =fopen("/usr/FINO/FPB/REQ/Deserialize.TXT","a");
    fwrite(jsondata,strlen((char *)jsondata),1,fp2);
    sleep(1);
    fclose(fp2);

    system("sed 's/\\\\\"/\\\"/g' /usr/FINO/FPB/REQ/Deserialize.TXT > /tmp/2.txt");
    system("sed 's/\\\\\"/\\\"/g' /tmp/2.txt > /usr/FINO/FPB/REQ/serialize.txt");

    system("cd /usr/FINO/FPB/REQ/");
    fp1 = fopen("/usr/FINO/FPB/REQ/serialize.txt", "rb"); // open
    if (fp1 == NULL)
    {
        qDebug()<<"Can't Open serialize.txt File";
        uidmessage::instance()->hide_Message();
        //return FUNCTION_FAILED;
        return -1;
    }
    fseek(fp1, 0, SEEK_END);
    int template_len = ftell(fp1);
#if DEBUG
    qDebug()<<"3333333333"<<template_len;
#endif
    fseek(fp1, 0, SEEK_SET);
    memset(jsondata,0,strlen(jsondata));
    int ret=fread(jsondata,sizeof(char ),template_len,fp1);


    if(ret = 0)
        fclose(fp1);

    return 1;

}

char *FPB::IFSC_APPEND(char *myifsc, char *arr)          // To replace 'X' with account number digit in Default IFSC from Database
{
 #if DEBUG
    printf("IFSC = %s and Account number = %s\n",myifsc, arr);
  #endif
    char out[20];
    memset(out, 0, sizeof(out));
    memcpy(out, myifsc, strlen(myifsc));
    unsigned int hCount(0),p(0),q(0);
    QString myString = QString::fromUtf8(myifsc);
    for(QString::const_iterator itr(myString.begin()); itr != myString.end(); itr++)
    {
        p++;
        if(*itr == 'X')
        {
            if(p>5)
            {
                ++hCount;
                out[p-1] = arr[q++];

                if(*++itr == 'X')
                {
                    p++;
                    ++hCount;
                    out[p-1] = arr[q++];
                }
                else
                {
                    if(itr != myString.end())
                        --hCount;
                }
            }
        }
        if(p == 11)
            break;
    }
    out[p] = '\0';
    printf("hCount == %d and output = %s\n",hCount, out);
    return out;

}

