#include "findcust.h"
#include "ui_FPB.h"
#include "json.h"
#include "FPB.h"
#include "Login.h"
#include "displaymessage.h"
#include "print.h"
#include "impscommon.h"
#include <QSqlDatabase>
extern Login_leg2 configLoginLeg2;
extern Login_leg1 configLoginLeg1;
extern Transaction Txn;
extern Printfpb ConfigPrint;
extern unsigned char JSONTagBuff[5000];
extern FINDCUSTOMER FINDCUST;
extern sCustDetails CustDet;
extern int disp_acc_count, count_bc_merchant_acc;
int flag_findcust_focus = 0;
char flag_bc_merchant_account = '\0';

void FPB::on_Search_lineEdit_textChanged(const QString &arg1)
{
    if(arg1.length()==9) // 02052019
    {
        flag_findcust_focus = 1;
        ui->Cust_Id_Button->setStyleSheet("background-color: rgb(85, 0, 127); color: rgb(255, 255, 255)");
        ui->Mobile_Button->setStyleSheet("background-color: rgb(255, 255, 255); color: rgb(0, 0, 0)");
        ui->Account_Button->setStyleSheet("background-color: rgb(255, 255, 255); color: rgb(0, 0, 0)");
    }

    if(arg1.length()==0 || arg1.length()==10) // 02052019
    {
        flag_findcust_focus = 0;
        ui->Mobile_Button->setStyleSheet("background-color: rgb(85, 0, 127); color: rgb(255, 255, 255)");
        ui->Cust_Id_Button->setStyleSheet("background-color:rgb(255, 255, 255); color: rgb(0, 0, 0)");
        ui->Account_Button->setStyleSheet("background-color:rgb(255, 255, 255); color: rgb(0, 0, 0)");
        ui->Search_lineEdit->setPlaceholderText("MOBILE NUMBER");

    }

    if(arg1.length()==11) // 02052019
    {
        flag_findcust_focus = 2;
        ui->Account_Button->setStyleSheet("background-color: rgb(85, 0, 127); color: rgb(255, 255, 255)");
        ui->Cust_Id_Button->setStyleSheet("background-color: rgb(255, 255, 255); color: rgb(0, 0, 0)");
        ui->Mobile_Button->setStyleSheet("background-color: rgb(255, 255, 255); color: rgb(0, 0, 0)");
        ui->Search_lineEdit->setPlaceholderText("ACCOUNT NUMBER");
    }
    return;
}

void FPB::on_Search_Button_clicked()
{
    CMisc miscObj;
    int  ret = -3;
    QString Primary_no, Mobile_no, Start_date, Unclear_bal, Balance, FT_limit, Name, pan_no, Acc_type_disp, aadhar_no; // 02052019 aadhar_no
    memset(&FINDCUST,0,sizeof(FINDCUST));
    Check_Conditions();
    CUST_TYPE = '\0';
    count_bc_merchant_acc = 0;

    ui->Minstatement->setVisible(true);//20062019
    ui->Bill_payment_Button->setVisible(true);
     ui->Remittance_button->setVisible(true);//added on 25/07/2019

    if(ui->Search_lineEdit->text().isEmpty())
    {
        uidmessage::instance()->hide_Message();
        miscObj.DisplayMsgBox((char *)"Mobile number is mandatory.");
        uidmessage::instance()->hide_Message();
        ui->Search_lineEdit->setFocus();
        return;
    }

    if(flag_findcust_focus == 0)
    {
        if(ui->Search_lineEdit->text().mid(0,1)!= "9" && ui->Search_lineEdit->text().mid(0,1) != "8" && ui->Search_lineEdit->text().mid(0,1)!= "7" && ui->Search_lineEdit->text().mid(0,1)!= "6")
        {
            uidmessage::instance()->hide_Message();
            miscObj.DisplayMsgBox((char *)"Mobile number should begin with 6, 7, 8 or 9.");
            return;
        }
    }
#if DEBUG
    qDebug()<<"LENGTH = "<<ui->Search_lineEdit->text().length();
#endif
    if(ui->Search_lineEdit->text().length() > 0 && ui->Search_lineEdit->text().length() < 9)
    {
#if DEBUG
        qDebug()<<"LENGTH = "<<ui->Search_lineEdit->text().length();
#endif
        uidmessage::instance()->hide_Message();
        if(flag_findcust_focus == 0)
            miscObj.DisplayMsgBox((char *)"Mobile number must be 10 digits.");
        else
            miscObj.DisplayMsgBox((char *)"Customer ID should be Minimum 9 digits.");
        uidmessage::instance()->hide_Message();
        ui->Search_lineEdit->setFocus();
        return;
    }

    uidmessage::instance()->hide_Message();
    uidmessage::instance()->Display_BusyMessage("Please Wait...");

    memset(FINDCUST.ucSessionId_FCreq,            0, sizeof(FINDCUST.ucSessionId_FCreq));
    memset(FINDCUST.ucRequestID_FCreq,            0, sizeof(FINDCUST.ucRequestID_FCreq));
    memset(FINDCUST.ucSessionExpiryTime_FCreq,    0, sizeof(FINDCUST.ucSessionExpiryTime_FCreq));
    memset(FINDCUST.ucTellerID_FCreq,             0, sizeof(FINDCUST.ucTellerID_FCreq));
    memset(FINDCUST.ucIsEncrypt_FCreq,            0, sizeof(FINDCUST.ucIsEncrypt_FCreq));
    memset(FINDCUST.ucRDmobileNumber_FCreq,       0, sizeof(FINDCUST.ucRDmobileNumber_FCreq));
    memset(FINDCUST.ucRDchannelId_FCreq,          0, sizeof(FINDCUST.ucRDchannelId_FCreq));
    memset(FINDCUST.ucRDmobileNumber_FCreq,       0, sizeof(FINDCUST.ucRDmobileNumber_FCreq));
    memset(FINDCUST.ucMethodID_FCreq,             0, sizeof(FINDCUST.ucMethodID_FCreq));
    memset(FINDCUST.ucRDcustomerNo_FCreq,         0, sizeof(FINDCUST.ucRDcustomerNo_FCreq));
    memset(FINDCUST.ucRDaccountNo_FCreq,          0, sizeof(FINDCUST.ucRDaccountNo_FCreq));

    memcpy((char *)FINDCUST.ucSessionId_FCreq,configLoginLeg2.ucSessionId,strlen((char *)configLoginLeg2.ucSessionId));
    //memcpy((char *)FINDCUST.ucSessionExpiryTime_ABMreq,configLoginLeg2.ucSessionExpiryTime,strlen((char *)configLoginLeg2.ucSessionExpiryTime));
    memcpy(FINDCUST.ucSessionExpiryTime_FCreq,"1",1);//1 for unzipped response 111 for zipped response
    memcpy((char *)FINDCUST.ucTellerID_FCreq,         (char *)configLoginLeg2.ucUsername_req,strlen((char *)configLoginLeg2.ucUsername_req));
    memcpy(FINDCUST.ucMethodID_FCreq,"8",1);
    memcpy(FINDCUST.ucIsEncrypt_FCreq,ISENCEYPT,5);
    // memcpy(FINDCUST.ucRDmobileNumber_FCreq,ui->Search_lineEdit->text().trimmed().toAscii().data(), strlen((char *)ui->Search_lineEdit->text().trimmed().toAscii().data()));
    memcpy(FINDCUST.ucRDchannelId_FCreq,CHANNEL_ID,strlen(CHANNEL_ID));
    memcpy((char *)FINDCUST.ucSessionExpiryTime_FCreq,(char *)configLoginLeg2.ucSessionExpiryTime,strlen((char *)configLoginLeg2.ucSessionExpiryTime));
#if DEBUG
    printf("find customer input = %s\n",(char *)ui->Search_lineEdit->text().trimmed().toAscii().data());
#endif
    if(ui->Search_lineEdit->text().length() == 9)
    {
        memcpy(FINDCUST.ucRDmobileNumber_FCreq, "", 1);
        memcpy(FINDCUST.ucRDcustomerNo_FCreq, (char *)ui->Search_lineEdit->text().trimmed().toAscii().data(), strlen((char *)ui->Search_lineEdit->text().trimmed().toAscii().data()));
        memcpy(FINDCUST.ucRDaccountNo_FCreq, "", 1);
#if DEBUG
        printf("FC 9 = %s\n",FINDCUST.ucRDcustomerNo_FCreq);
#endif
    }
    else if(ui->Search_lineEdit->text().length() == 10)
    {
        memcpy(FINDCUST.ucRDmobileNumber_FCreq, (char *)ui->Search_lineEdit->text().trimmed().toAscii().data(), strlen((char *)ui->Search_lineEdit->text().trimmed().toAscii().data()));
        memcpy(FINDCUST.ucRDcustomerNo_FCreq, "", 1);
        memcpy(FINDCUST.ucRDaccountNo_FCreq,  "", 1);
 #if DEBUG
        printf("FC 10 = %s\n",FINDCUST.ucRDmobileNumber_FCreq);
  #endif
    }
    else if(ui->Search_lineEdit->text().length() == 11)
    {
        memcpy(FINDCUST.ucRDmobileNumber_FCreq, "", 1);
        memcpy(FINDCUST.ucRDcustomerNo_FCreq,   "", 1);
        memcpy(FINDCUST.ucRDaccountNo_FCreq, (char *)ui->Search_lineEdit->text().trimmed().toAscii().data(), strlen((char *)ui->Search_lineEdit->text().trimmed().toAscii().data()));
 #if DEBUG
        printf("FC 11 = %s\n",FINDCUST.ucRDaccountNo_FCreq);
   #endif
    }

    iGenerateRequestID((char*)configLoginLeg2.ucUsername_req,(unsigned char *)&FINDCUST.ucRequestID_FCreq);
    ret	= makeJSON(22);
    if(ret!=FUNC_OK)
    {
        qDebug()<<"JSON Creation failed";
        ui->Search_lineEdit->setFocus();
        return;// FUNC_ERROR;
    }
    /****************************Encryption function*********************************/
   if(!strcmp((char *)FINDCUST.ucIsEncrypt_FCreq,"true"))
    {
       int gSendData_len =strlen((char *)gSendData);
        RequestDetachEncryptAttach((char *)gSendData,1,gSendData_len);
        qDebug()<<"After encrypt function";
    }
    /***************************************************************/


    system("rm /usr/FINO/FPB/RESP/FINDCUST.TXT");   
    memset(str_url,0,sizeof(str_url));
#ifdef PRODUCTION
    strcpy(str_url,URL_CREATION(LOGIN_URL_PROD));
#else
    strcpy(str_url,URL_CREATION((char *)FIND_CUST));
#endif

    system("rm /usr/FINO/FPB/RESP/FINDCUST.TXT");
    int retk = file_download(str_url, (char *)gSendData,(char *)"/usr/FINO/FPB/RESP/FINDCUST.TXT",120);
    if(retk!=0)
    {
        sleep(1);
        uidmessage::instance()->hide_Message();
        return;
    }


    /****************************Decryption function*********************************/
    if(!strcmp((char *)FINDCUST.ucIsEncrypt_FCreq,"true"))

    {
        int oBuff_len = strlen(oBuff);
#if DEBUG
        qDebug()<<"Length of buffer"<<oBuff_len;
        printf("Buffer%s\n",oBuff);
#endif
        ResponseDetachEncryptAttach(oBuff,2,oBuff_len);//TEMPORARY

        system("rm /usr/FINO/FPB/RESP/FINDCUST.TXT");
        FILE *fp2 = NULL;
        fp2 =fopen("/usr/FINO/FPB/RESP/FINDCUST.TXT","a");
        fwrite(oBuff,strlen((char *)oBuff),1,fp2);
        sleep(1);
        fclose(fp2);
    }
    /*******************************************************************************/


    int ret_fc = findcust(2);

    memset(oBuff, 0, sizeof(oBuff)); // 17072019

    if(ret_fc != FUNCTION_SUCCESS && ret_fc != FUNC_NEW_CUST)
    {
        uidmessage::instance()->hide_Message();
        if(ret_fc == SESSION_EXPIRED_FAILED)
        {
            ui->stackedWidget->setCurrentIndex(0);
            return;
        }
        // ui->stackedWidget->setCurrentIndex(0);
        return;
    }

    uidmessage::instance()->hide_Message();
    uidmessage::instance()->Display_BusyMessage("Please Wait...");
    int ret_wc = WalkinCust();

    memset(oBuff, 0, sizeof(oBuff)); // 17072019

    if((ret_wc != FUNCTION_SUCCESS) && (ret_wc != FUNC_NEW_CUST))
    {
        uidmessage::instance()->hide_Message();
        if(ret_wc == SESSION_EXPIRED_FAILED)
        {
            uidmessage::instance()->hide_Message();
            ui->stackedWidget->setCurrentIndex(0);
            return;
        }
        else if(ret_wc == FUNC_NEXT)
        {
            uidmessage::instance()->hide_Message();
        }
        else
            return;
    }

    if(ret_fc != FUNC_NEW_CUST)
    {
        int ret_wcl = Walkin_Cust_Limit();

        memset(oBuff, 0, sizeof(oBuff)); // 17072019

        if(ret_wcl != FUNCTION_SUCCESS)
        {
            uidmessage::instance()->hide_Message();
            if(ret_wcl == SESSION_EXPIRED_FAILED)
            {
                uidmessage::instance()->hide_Message();
                ui->stackedWidget->setCurrentIndex(0);
                return;
            }
            else if(ret_wcl == FUNC_NEXT)
            {
                uidmessage::instance()->hide_Message();
            }
            else
                return;
        }
    }

    uidmessage::instance()->hide_Message();
    //    ui->Person_city_edit_2_comboBox->setText((char *)configLoginLeg2.uccity);
    ui->Person_pincode_edit->setText((char *)configLoginLeg2.ucpinCode);
    //    ui->Person_state_edit_comboBox->setText((char *)configLoginLeg2.ucstateDescription);
    ui->Age_lineedit->setDisabled(true);
    if(ui->Search_lineEdit->text().length() == 10)
    {
        ui->Mobile_lineedit->setText(ui->Search_lineEdit->text().trimmed());
        ui->Mobile_lineedit->setDisabled(true);
    }
    //    ui->pushButton_IMPS_Txn_Menu_2->setEnabled(true);
    //    ui->pushButton_IMPS_Txn_Menu_2->setStyleSheet("color: rgb(255, 255, 255);background-color: rgb(85, 0, 127)");
    //ui->stackedWidget->setCurrentIndex(10);
    if (CUST_TYPE == 'W' && Flag_IMPS_Walkin == 'S') // 25062019
    {
        disp_acc_count = 0;
        ui->Minstatement->setVisible(false);//20062019
        ui->stackedWidget->setCurrentIndex(15);
    }
    else
    {
        QString str = FC_MobileNumber;
#if DEBUG
        qDebug()<<"str = "<<str;
#endif
        Primary_no.clear();
        Mobile_no.clear();
        Start_date.clear();
        Unclear_bal.clear();
        Balance.clear();
        Name.clear();
        Acc_type_disp.clear();
        pan_no.clear();
        aadhar_no.clear(); // 02052019
        char temp[20];
        Name = QString::fromUtf8((const char*)FINDCUST.ucRDCLfullName_resp);
        Acc_type_disp = QString::fromUtf8((const char*)"Walkin"); //FINDCUST.ucRDCLdescription);

        if(strcmp((char *)FINDCUST.ucRDCLaadharNumber, "") == 0) // 02052019
        {
            aadhar_no = "NA";
        }
        else // 02052019
        {
            memset(temp,0,strlen(temp));
            sprintf(temp,"********%s", &FINDCUST.ucRDCLaadharNumber[strlen((const char*)FINDCUST.ucRDCLaadharNumber) - 4]);
            aadhar_no = QString::fromUtf8((const char*)temp);
        }

        Unclear_bal = "0.00";
        if(strcmp((char*)FINDCUST.ucRDCLtaxIdNumber, "") == 0)
        {
            pan_no = "NA";
        }
        else
        {

            pan_no.clear();
            memset(temp,0,strlen(temp));
            sprintf(temp,"******%s", &FINDCUST.ucRDCLtaxIdNumber[strlen ((char *)FINDCUST.ucRDCLtaxIdNumber) - 4]);
            pan_no = QString::fromUtf8((const char*)temp);
        }

        memset(temp,0,strlen(temp));
 #if DEBUG
        printf("** FINDCUST.ucRDCLnumber = %s **\n", &FINDCUST.ucRDCLnumber[1]);
#endif
        if(strlen((char *)FINDCUST.ucRDCLnumber) != 0)
            sprintf(temp,"*****%s", &FINDCUST.ucRDCLnumber[strlen ((char *)FINDCUST.ucRDCLnumber) - 4]);
        Primary_no = QString::fromUtf8((const char*)temp);

        memset(temp,0,strlen(temp));
        sprintf(temp,"******%s", &FC_MobileNumber[strlen (FC_MobileNumber) - 4]);
        Mobile_no =QString::fromUtf8((const char*)temp);
        //   Start_date = QString::fromUtf8((const char*)Balance_Display);
        Balance = "0.00";

        //  Unclear_bal = QString::fromUtf8((const char*)Balance_Display);
        //  Balance = QString::fromUtf8((const char*)Balance_Display);
        QSqlQuery query;
        QString temp_amt;
        unsigned char query_amount[15];
        memset(query_amount, 0, sizeof(query_amount));
        temp_amt.clear(); // 02052019
        query.clear();

        query.prepare("SELECT RFU from GetProfile where TransactionType = 'DMTIMPSP2A' AND ProductTypeID = '9999'");

        //query.prepare("SELECT RFU2 from PROFILE_TYPE_TRANSACTION where TRANSACTION_TYPE = 'DMTIMPSP2A' AND PRODUCT_TYPE_ID = '9999'");

        if(!query.exec())
        {
            qDebug() << query.lastError()<<"data base entry error";
            return ;
        }
        else
        {
            while(query.next())
            {
                temp_amt = query.value(0).toString().trimmed();
#if DEBUG
                qDebug()<<"temp_amt = "<<temp_amt;
#endif
            }
        }
        query.finish();
        temp_amt = temp_amt.mid(0, temp_amt.indexOf("|"));
        memset(query_amount, 0, sizeof(query_amount));
        memcpy(query_amount, temp_amt.toAscii().trimmed().data(), strlen(temp_amt.toAscii().trimmed().data()));

        float CustTotalAmount = atof((const char*)query_amount) - atof((const char*)Cust_CurrMonthAmount);
#if DEBUG
        qDebug()<<"temp_amt sd = "<<temp_amt;
        printf("\nQuery Cust Amount = %s\n", query_amount);
        printf("\nCustTotalAmount = %f\n", CustTotalAmount);
#endif
        ui->comboBox_3->clear();
        ui->comboBox_3->addItem((char *)"Walkin");
        for(int k=1; k<=disp_acc_count; k++)
        {
            memset(temp,0,strlen(temp));
            //   if(strcmp((char*)FINDCUST.ucRDCLdescription[k], "BC Merchant Account")==0)
            {
                sprintf(temp,"*******%s", &FINDCUST.ucRDCLACCnumber[k][strlen ((char *)FINDCUST.ucRDCLACCnumber[k]) - 4]);
                ui->comboBox_3->addItem((char *)temp);
            }
        }

        FT_limit = QString::number(CustTotalAmount);
        bool decimal;
        decimal = FT_limit.contains('.');
        if(decimal == false)
        {
            FT_limit = FT_limit.append(".00");
        }

        ui->comboBox_3->setCurrentIndex(0);
        ui->Customer_Name_label->setText(Name);
        ui->Primary_Number_disp->setText(Primary_no);
        ui->Mob_no_disp->setText(Mobile_no);
        ui->Start_date_disp->setText(Start_date);
        ui->Unclear_Bal_disp->setText(Unclear_bal);
        ui->Bal_disp->setText(Balance);
        ui->Ft_limit_disp->setText(FT_limit);
        ui->Pan_no_disp->setText(pan_no);
        ui->Acc_type_disp->setText(Acc_type_disp);
        ui->Aadhar_no_disp->setText(aadhar_no); // 02052019

        ui->Mobile_lineedit->setText(str);

        //        ui->Primary_Number->settext()
        ui->stackedWidget->setCurrentIndex(14); // 35
    }

    return;


}



void FPB::on_comboBox_3_currentIndexChanged(int index)
{

    char temp[20];

    QString Acc_type_disp;
    QString Unclear_bal;
    QString Balance;
    QString pan_no;
    //    pan_no.clear();
    Balance.clear();
    memset(temp, 0, sizeof(temp));
    if(index != 0)
    {
        //        if(strcmp((char*)FINDCUST.ucRDCLtaxIdNumber, "") == 0)
        //        {
        //            ui->Pan_no_disp->setText("NA");
        //        }
        //        else
        //        {

        //            pan_no.clear();
        //            memset(temp,0,strlen(temp));
        //            sprintf(temp,"******%s", &FINDCUST.ucRDCLtaxIdNumber[strlen ((char *)FINDCUST.ucRDCLtaxIdNumber) - 4]);
        //            pan_no = QString::fromUtf8((const char*)temp);
        //            ui->Pan_no_disp->setText(pan_no);
        //        }

        Acc_type_disp.clear();
        Unclear_bal.clear();

        Acc_type_disp = QString::fromUtf8((const char*)FINDCUST.ucRDCLdescription[index]);
        if(Acc_type_disp == "MSRTC Pocket - Low KYC")
        {
            ui->Ft_limit->setVisible(false);
            ui->Ft_limit_disp->setVisible(false);
        }
        else
        {
            ui->Ft_limit->setVisible(true);
            ui->Ft_limit_disp->setVisible(true);
            if(Acc_type_disp == "Wallet - Limited KYC")
                ui->Ft_limit_disp->setText("0.00");
        }
        ui->Acc_type_disp->setText(Acc_type_disp);
#if DEBUG
        printf("index = %d , unclear balanace ===== %s\n", index, (char *)FINDCUST.ucRDCLunclearedBalance[index]);
#endif
        if(strcmp((char *)FINDCUST.ucRDCLunclearedBalance[index], "") == 0)
        {
            Unclear_bal = "0.00";
        }
        else
        {
            Unclear_bal = QString::fromUtf8((const char*)FINDCUST.ucRDCLunclearedBalance[index]);
#if DEBUG
            qDebug()<<"Unclear_bal :::: "<<Unclear_bal;
#endif
            bool decimal;
            decimal = Unclear_bal.contains('.');
            if(decimal == false)
            {
                Unclear_bal = Unclear_bal.append(".00");
            }
        }
        ui->Unclear_Bal_disp->setText(Unclear_bal);


        if(strcmp((char *)FINDCUST.ucRDCLACavailableBalance[index], "") == 0)
        {
            Balance = "0.00";
        }
        else
        {
            Balance = QString::number(atof((const char*)FINDCUST.ucRDCLACavailableBalance[index]), 'f', 2);
            //Balance = QString::fromUtf8((const char*)FINDCUST.ucRDCLACavailableBalance[index]);
            bool decimal;
            decimal = Balance.contains('.');
            if(decimal == false)
            {
                Balance = Balance.append(".00");
            }
        }
        ui->Bal_no->setVisible(true);
        ui->Bal_disp->setVisible(true);
        ui->Bal_disp->setText(Balance);
    }
    else
    {
        Unclear_bal.clear();
        Acc_type_disp.clear();
        Balance.clear();

        Acc_type_disp = "Walkin";
        Unclear_bal = "0.00";
        Balance = "0.00";

        ui->Unclear_Bal_disp->setText(Unclear_bal);
        ui->Acc_type_disp->setText(Acc_type_disp);
        ui->Bal_disp->setText(Balance);
    }

    memset((char *)CustDet.cFindCustAccountNo, 0, sizeof((char *)CustDet.cFindCustAccountNo));
    memset((char *)CustDet.cFindCustAccountType, 0, sizeof((char *)CustDet.cFindCustAccountType));

    memcpy(CustDet.cFindCustAccountNo, (char *)FINDCUST.ucRDCLACCnumber[index], strlen((const char *)FINDCUST.ucRDCLACCnumber[index]));
    memcpy(CustDet.cFindCustAccountType, (char *)FINDCUST.ucRDCLdescription[index], strlen((const char *)FINDCUST.ucRDCLdescription[index]));
#if DEBUG
    printf("SELECTED ACCOUNT TYPE %s and ACCOUNT NO. %s\n", (char *)CustDet.cFindCustAccountType, (char *)CustDet.cFindCustAccountNo);
#endif
    if(memcmp((char *)CustDet.cFindCustAccountType, "BC Merchant Account", 19) == 0)
    {
  #if DEBUG
        qDebug()<<"flag_bc_merchant_account = 'B'";
  #endif
        flag_bc_merchant_account = 'B';
    }
    else
    {
        flag_bc_merchant_account = '\0';
#if DEBUG

        qDebug()<<"flag_bc_merchant_account = '\0'";
        qDebug()<<"flag_bc_merchant_account = "<<flag_bc_merchant_account;
#endif
    }


    if(flag_bc_merchant_account == 'B')
    {
        if(index != 0)
        {
            ui->Unclear_Bal->setVisible(false);
            ui->Unclear_Bal_disp->setVisible(false);
        }
        else
        {
            ui->Unclear_Bal->setVisible(true);
            ui->Unclear_Bal_disp->setVisible(true);
        }

        ui->Ft_limit->setVisible(false);
        ui->Ft_limit_disp->setVisible(false);
        ui->Bal_no->setVisible(false);
        ui->Bal_disp->setVisible(false);
    }
    else
    {
        ui->Ft_limit->setVisible(true);
        ui->Ft_limit_disp->setVisible(true);
        ui->Bal_no->setVisible(true);
        ui->Bal_disp->setVisible(true);
        ui->Unclear_Bal_disp->setVisible(true);
        ui->Unclear_Bal->setVisible(true);
    }

    return;

}

int FPB::iSetFINDCUSTOMER(int tagID, unsigned char *value)
{
    switch(tagID)
    {
    case  1:
        strcpy((char *)value,(char *)FINDCUST.ucRequestID_FCreq);
        break;
    case  2:
        strcpy((char *)value,(char *)FINDCUST.ucMethodID_FCreq);//Method id
        break;
    case  3:
        strcpy((char *)value,(char *)FINDCUST.ucSessionId_FCreq);//teller id
        break;
    case  4:
        strcpy((char *)value,(char *)FINDCUST.ucTellerID_FCreq);//teller id
        //strcpy((char *)value,(char *)FINDCUST.ucIsEncrypt_req);//is encrypt
        break;
    case  5:
        strcpy((char *)value,(char *)FINDCUST.ucRDmobileNumber_FCreq);//is encrypt
        break;
    case  6:
        strcpy((char *)value,(char *)FINDCUST.ucRDchannelId_FCreq);//is encrypt
        break;
    case  7:
        strcpy((char *)value,(char *)FINDCUST.ucSessionExpiryTime_FCreq);//is encrypt
        break;
    case  8:
        strcpy((char *)value,(char *)FINDCUST.ucRDcustomerNo_FCreq);//is encrypt
        break;
    case  9:
        strcpy((char *)value,(char *)FINDCUST.ucRDaccountNo_FCreq);//is encrypt
        break;

    case  10:
        strcpy((char *)value,(char *)FINDCUST.ucIsEncrypt_FCreq);//is encrypt
        break;

    default:
        return FUNC_ERROR;
    }
    return FUNC_OK;
}


int FPB::FetchExtraBene()
{
    int iRet = 0;
    Json1::Value RequestedData,SubData2;
    RequestedData.clear();
    SubData2.clear();
    char nullbuff[5];
    memset(nullbuff, 0, sizeof(nullbuff));
    strcpy(nullbuff,"");
    char buff1[100];
    memset(buff1,0,sizeof(buff1));

    //    uidmessage::instance()->hide_Message();
    //    uidmessage::instance()->Display_BusyMessage("Transaction Request in Progress");

    iGenerateRequestID((char *)username,(unsigned char *)buff1);

    RequestedData["RequestId"] = buff1;//"TLR1032_2620191138138";
    RequestedData["MethodId"] = 20;
    RequestedData["ChannelID"]= CHANNEL_ID;
    RequestedData["SessionId"] = (char *)configLoginLeg2.ucSessionId; //buff_tmp.toAscii().data();//"9d044213-09d9-400d-aa96-5e9a473ba494";
    RequestedData["TellerID"]= (char *)username; // Dhiral
    RequestedData["TokenId"]= "";
    RequestedData["X_Auth_Token"] ="";
    RequestedData["IsEncrypt"]= ISENCEYPT;
    RequestedData["SessionExpiryTime"]= (char *)configLoginLeg2.ucSessionExpiryTime;//"10/11/2018 11:58:44 PM";
    SubData2["customerId"] = FC_CustomerNumber;
    serialize1(Convertstr1(SubData2.toStyledString()));
    RequestedData["RequestData"] = buff_tmp.toAscii().data();


    char *Ret;
    Ret=Convertstr1(RequestedData.toStyledString());

#if DEBUG
    qDebug()<<"\nRequest Data :"<<Ret;
#endif
    memset(gSendData,0,sizeof(gSendData));
    strncpy((char*)gSendData, Ret, strlen(Ret));
 #if DEBUG
    printf("gSendData = %s\n",gSendData);
 #endif
    /****************************Encryption function*********************************/
   if(!strcmp((char *)ISENCEYPT,"true"))
    {
       int gSendData_len =strlen((char *)gSendData);
        RequestDetachEncryptAttach((char *)gSendData,1,gSendData_len);
        qDebug()<<"After encrypt function";
    }
    /***************************************************************/

    memset(str_url,0,sizeof(str_url));
#ifdef PRODUCTION
    strcpy(str_url,URL_CREATION(LOGIN_URL_PROD));
#else
    strcpy(str_url,URL_CREATION((char *)COMMON_URL));
#endif

    system("rm /usr/FINO/FPB/RESP/FetchExtraBene.TXT");
    int retk = file_download(str_url, (char *)gSendData,(char *)"/usr/FINO/FPB/RESP/FetchExtraBene.TXT",120);
    if(retk!=0)
    {
        sleep(1);
        uidmessage::instance()->hide_Message();
        return FUNCTION_FAILED;
    }




    /****************************Decryption function*********************************/
    if(!strcmp((char *)ISENCEYPT,"true"))

    {
        int oBuff_len = strlen(oBuff);
#if DEBUG
        qDebug()<<"Length of buffer"<<oBuff_len;
        printf("Buffer%s\n",oBuff);
#endif
        ResponseDetachEncryptAttach(oBuff,2,oBuff_len);//TEMPORARY

        system("rm /usr/FINO/FPB/RESP/FetchExtraBene.TXT");
        FILE *fp2 = NULL;
        fp2 =fopen("/usr/FINO/FPB/RESP/FetchExtraBene.TXT","a");
        fwrite(oBuff,strlen((char *)oBuff),1,fp2);
        sleep(1);
        fclose(fp2);
    }
    /*******************************************************************************/




   // file_download( "http://103.1.112.205:4444//UIService.svc/NonFinancialProcessRequest", Ret,"/usr/FINO/FPB/RESP/FetchExtraBene.TXT",120);
    iRet = FetchExtraBene_Parsing();
    return iRet;

}





