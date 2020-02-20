#include "impscommon.h"
#include "ui_FPB.h"
#include "json.h"
#include "FPB.h"
#include "Login.h"
#include "displaymessage.h"
#include "print.h"
#include <QSqlDatabase>

extern Login_leg2 configLoginLeg2;
FetchExtraBeneList FetchBeneList;
extern char flag_bc_merchant_account;


void FPB::on_Remittance_button_clicked()
{
    ui->checkBox_button->setChecked(false);
    CMisc miscObj;
    QString Trans_type;
    Trans_type.clear();
    if(flag_bc_merchant_account == 'B')
        Trans_type = "IMPSFTP2A";
    else
        Trans_type = "DMTIMPSP2A";

    int flag = 0;
    QSqlQuery query;
    char msgdisplay[45];
    memset(msgdisplay, 0, sizeof(msgdisplay));
    query.clear();
    if(flag_bc_merchant_account == 'B')
        query.prepare("Select  TransactionType from GetProfile where ProductTypeID = '1003'");
    else
        query.prepare("Select  TransactionType from GetProfile where ProductTypeID = '9999'");

    if(!query.exec())
    {
        qDebug() << query.lastError()<<"data base entry error";
        return ;
    }
    else
    {
        while(query.next())
        {
 #if DEBUG
            qDebug()<<"Query == "<<query.value(0).toString().trimmed();
 #endif
            if(Trans_type == query.value(0).toString().trimmed())
            {
                flag = 1;
            }
        }
    }

    query.finish();
    if(flag == 0)
    {
        sprintf(msgdisplay,"Feature currently not available");
        miscObj.DisplayMsgBox(msgdisplay);
        uidmessage::instance()->hide_Message();
        return ;
    }

    flag_bitmap = '\0';
    clear_IMPS();
    Flag_BeneVerif_Charges = 0;
    ui->Print_Receipt_Button_Pg34->hide();
    ui->Discard_otp_Button_2->setVisible(true);
    ui->cmb_Registered_Bene_List->clear();

    ui->Verify_button->setText("VERIFY");
    ui->Verify_button->setEnabled(true);
    ui->Verify_button->setStyleSheet("background-color: rgb(85, 0, 127); color: rgb(255, 255, 255)");

    uidmessage::instance()->hide_Message();
    uidmessage::instance()->Display_BusyMessage(" Please wait...");

    int ibret = iBankMaster();
    if((ibret != FUNC_OK) && (ibret != FUNCTION_SUCCESS))
    {
        uidmessage::instance()->hide_Message();
        if(ibret == SESSION_EXPIRED_FAILED)
        {
            ui->stackedWidget->setCurrentIndex(0);
            return;
        }
        ui->stackedWidget->setCurrentIndex(15);
        return;
    }


    Calculate_impscharges =0;


    if (CUST_TYPE != 'W')
    {
        uidmessage::instance()->hide_Message();
        uidmessage::instance()->Display_BusyMessage(" Please wait...");
        int iRet_fexb = FetchExtraBene();
        if(iRet_fexb != FUNCTION_SUCCESS && iRet_fexb != FUNC_OK)
        {
            uidmessage::instance()->hide_Message();
            if(iRet_fexb == SESSION_EXPIRED_FAILED)
            {
                clear_IMPS();
                ui->stackedWidget->setCurrentIndex(0);
                return;
            }
            else
            {
                if(iRet_fexb == FUNC_NEXT)
                {
                    int ret = 0;
                    flag_bitmap = 'N';
                    ui->Add_bene_label->setVisible(true);
                    //ui->Check_button->setVisible(true);
                    ui->checkBox_button->setVisible(true);
                    ui->BENE_List_label->hide();
                    ui->cmb_Registered_Bene_List->hide();

                    ui->cmb_bankList->setDisabled(false);
                    ui->Bene_acc_no_edit->setDisabled(false);
                    ui->Verify_button->setDisabled(false);
                    ui->Verify_button->setVisible(true);
                    ui->Bene_Search_Button->setDisabled(false);
                    ui->Bene_Search_Button->setVisible(true);
                    ui->Bene_Name_edit->setDisabled(false);
                    ui->IFSC_edit->setDisabled(false);
                    ui->Bene_acc_no_edit->clear();
                    ui->cmb_bankList->clear();
                    ui->Bene_Name_edit->clear();
                    ui->IFSC_edit->clear();
                    ui->pushButton_5->hide();
                    ret = BankList();
                    if(ret == FUNC_OK)
                    {
                        ui->cmb_bankList->setCurrentIndex(0);
                        ui->stackedWidget->setCurrentIndex(18);
                        return;
                    }
                }
                ui->stackedWidget->setCurrentIndex(15);
                qDebug()<<"Fetch Extra Bene Failed";
                return;
            }

        }

        uidmessage::instance()->hide_Message();
        ui->stackedWidget->setCurrentIndex(35);
    }
    else
    {
        QDate date = QDate::currentDate();
        QTime time = QTime::currentTime();
        QDateTime startDate(date, time);
        uidmessage::instance()->hide_Message();
        ui->dateEdit_3->setDateTime(startDate);
        ui->stackedWidget->setCurrentIndex(16);
    }

    return;
}

int FPB::fetchbene()
{
    HostComm hComm;
    CMisc miscObj;
    int FetchBeneList_len = 0, response_timeout1 = 120, ret = -3, res = -3, iRet = -3;
    char FetchBeneList_Buff[10000];
    char ErrorMsg[100];

    memset(ErrorMsg,0,sizeof(ErrorMsg));
    memset(FetchBeneList.Commdata_EBL.ucMethodID_req,0, sizeof(FetchBeneList.Commdata_EBL.ucMethodID_req));
    memset(FetchBeneList.Commdata_EBL.ucTellerID_req,0, sizeof(FetchBeneList.Commdata_EBL.ucTellerID_req));
    memset(FetchBeneList.Commdata_EBL.ucSessionID_req,0, sizeof(FetchBeneList.Commdata_EBL.ucSessionID_req));
    memset(FetchBeneList.Commdata_EBL.ucIsEncrypt_req,0, sizeof(FetchBeneList.Commdata_EBL.ucIsEncrypt_req));
    memset(FetchBeneList.Commdata_EBL.ucChannelID_req,0, sizeof(FetchBeneList.Commdata_EBL.ucChannelID_req));
    memset(FetchBeneList.uccustid,0, sizeof(FetchBeneList.uccustid));
    memset(FetchBeneList.Commdata_EBL.ucSessionExpiryTime_req,0, sizeof(FetchBeneList.Commdata_EBL.ucSessionExpiryTime_req));


    iGenerateRequestID((char *)username,(unsigned char *)&FetchBeneList.Commdata_EBL.ucRequestID_req);
    memcpy(FetchBeneList.Commdata_EBL.ucMethodID_req,"20",2);
    memcpy((char *)FetchBeneList.Commdata_EBL.ucSessionID_req,configLoginLeg2.ucSessionId,strlen((char *)configLoginLeg2.ucSessionId));
    memcpy(FetchBeneList.Commdata_EBL.ucTellerID_req,username,strlen((char *)username));
    memcpy(FetchBeneList.uccustid,"100020442",9);//hardcoded
    memcpy(FetchBeneList.Commdata_EBL.ucIsEncrypt_req,"false",5);
    memcpy((char *)FetchBeneList.Commdata_EBL.ucSessionExpiryTime_req,(char *)configLoginLeg2.ucSessionExpiryTime,strlen((char *)configLoginLeg2.ucSessionExpiryTime));

    ret	= makeJSON(14);
    if(ret!=FUNC_OK)
    {
        qDebug()<<"JSON Creation failed";
        return FUNC_ERROR;
    }

    uidmessage::instance()->hide_Message();
    hComm.close_connection();
    res = hComm.open_connect((char *)SERVERIP,(char *)SERVERPORT);
    if(res != 0)
    {
        qDebug() << "Failed to connect to the Server";
        uidmessage::instance()->hide_Message();
        uidmessage::instance()->DisplayMessage("Server Not Connected");
        qDebug()<<"FUNCTION_FAILED";
        ui->stackedWidget->setCurrentIndex(0);
        return FUNCTION_FAILED;
    }


    int len_gSendData = strlen((char *)gSendData);
    memset(FetchBeneList_Buff,0,sizeof(FetchBeneList_Buff));
    if((hComm.host_2Sendrecv((unsigned char *)gSendData,len_gSendData,( unsigned char*)FetchBeneList_Buff,(int *)&FetchBeneList_len,(int)response_timeout1,0))!=SUCCESS)
    {
        printf("No Response From Server\n");
        hComm.close_connection();
        miscObj.DisplayMsgBox((char *)"Response Time-Out");
        uidmessage::instance()->hide_Message();
        qDebug()<<"***   Response Time-Out   ***";
        ui->stackedWidget->setCurrentIndex(1);
    }
    hComm.close_connection();


    QString FetchBeneListBuff = QString::fromLocal8Bit(FetchBeneList_Buff);
    bool check = FetchBeneListBuff.contains("ResponseCode");
    if(check == false)
    {
        miscObj.DisplayMsgBox((char *)"Invalid Data");
        uidmessage::instance()->hide_Message();
        return FUNCTION_FAILED;
    }


    FetchBeneListBuff.remove("\\");
    FetchBeneListBuff.remove("{");
    FetchBeneListBuff.remove("}");
    FetchBeneListBuff.remove('"');
    FetchBeneListBuff.remove("[");
    FetchBeneListBuff.remove("]");
    qDebug()<<"FetchBeneListBuff"<<FetchBeneListBuff;
    QStringList query = FetchBeneListBuff.split(",");
    for(int i=0 ; i < query.length() ; i++)
        qDebug()<<"Query Response"<<query.at(i);
    //      QStringList RequestId = query.filter("RequestId");
    //      QStringList DisplayMessage = query.filter("DisplayMessage");
    //      QStringList ResponseCode = query.filter("ResponseCode");
    //      uidmessage::instance()->hide_Message();

    //      QString str = RequestId.at(2);

    //      QString strToFind = "RequestId:";
    //      QString value = str.mid(str.indexOf(strToFind)+strToFind.length());

    //      str = ResponseCode.at(0);
    //      strToFind = ":";
    //      QString value1 = str.mid(str.indexOf(strToFind)+strToFind.length());

    //      str = DisplayMessage.at(2);
    //      QString value2 = str.mid(str.indexOf(strToFind)+strToFind.length());

    //      qDebug()<<"value"<<value;
    //      qDebug()<<"value1"<<value1;
    //      qDebug()<<"value2"<<value2;
    //       memcpy(FetchBeneList.ucRDRequestId,value.trimmed().toAscii().data(), strlen(value.trimmed().toAscii().data()));
    //       qDebug()<<"FetchBeneList.ucRDRequestId"<<FetchBeneList.ucRDRequestId;
    //           if(value1 =="0")
    //           {

    //                ui->stackedWidget->setCurrentIndex(22);
    //                return FUNCTION_SUCCESS;
    //           }
    //           else
    //           {

    //               sprintf(ErrorMsg,"Response Code:%s \n Message:%s",(char *)FORPASS_State.ucResponseCode,(char *)FORPASS_State.ucResponseMessage);
    //               miscObj.DisplayMsgBox(ErrorMsg);
    //               uidmessage::instance()->hide_Message();
    //               return FUNCTION_FAILED;
    //           }


    //            return FUNCTION_SUCCESS;



}



int FPB::iSetFETCH(int tagID, unsigned char *value)
{
    switch(tagID)
    {
    case  1:
        strcpy((char *)value,FetchBeneList.Commdata_EBL.ucRequestID_req);
        break;
    case  2:
        strcpy((char *)value,FetchBeneList.Commdata_EBL.ucMethodID_req);
        break;
    case  3:
        strcpy((char *)value,FetchBeneList.Commdata_EBL.ucSessionID_req);
        break;
    case  4:
        strcpy((char *)value,FetchBeneList.Commdata_EBL.ucTellerID_req);
        break;
    case  5:
        strcpy((char *)value,FetchBeneList.Commdata_EBL.ucIsEncrypt_req);
        break;
    case  7:
        strcpy((char *)value,(char *)FetchBeneList.uccustid);
        break;
    case  6:
        strcpy((char *)value,FetchBeneList.Commdata_EBL.ucSessionExpiryTime_req);
        break;
    default:
        return FUNC_ERROR;
    }
    return FUNC_OK;
}


int FPB::makeJSON(int ivalue)//----------------------
{
    char tempbuff[30];
    int iRet=-2,slen;
    memset(gSendData,0,sizeof(gSendData));


    iRet = iMakeRequest(ivalue);//changed by vaishnavi
    if (iRet!=FUNC_OK)
    {
        uidmessage::instance()->hide_Message();
        return FUNC_ERROR;
    }

    slen=strlen((char *)Recvdata);
    slen-=2;
    memset(gSendData,0,sizeof(gSendData));//AEPSProcessRequest
    if(ivalue == 14/* || ivalue == 15*/ || ivalue == 16 || ivalue == 18)
    {
        qDebug()<<"14";
        strncpy((char *)gSendData,"POST /UIService.svc/NonFinancialProcessRequest HTTP/1.1\r\n",59);
    }
    else if(ivalue == 13)
    {
        qDebug()<<"13";
        strncpy((char *)gSendData,"POST /UIService.svc/ProcessLoginRequest HTTP/1.1\r\n",52);
    }
    else if(ivalue == 17 || ivalue == 20 )
    {
        qDebug()<<"17";
        strncpy((char *)gSendData,"POST /UIService.svc/UtilityOtp HTTP/1.1\r\n",43);
    }
    else if(ivalue == 21)
    {
        qDebug()<<"21..";
        //       strncpy((char *)gSendData,"POST /UIService.svc/ProcessRegistrationRequest HTTP/1.1\r\n",59);
        //strncpy((char *)gSendData,"POST UIService.svc/NonFinancialProcessRequest HTTP/1.1\r\n",58);

    }

    if(ivalue != 21 && ivalue != 15 && ivalue != 22 && ivalue != 19)
    {
        strncat((char *)gSendData,"Host: 202.58.125.20\r\n",27);//9013
        strncat((char *)gSendData,"Content-Type: application/json; charset=utf-8\r\n",49);
        sprintf(tempbuff,"Content-Length: %d\r\n",slen);
        strncat((char *)gSendData,tempbuff,strlen(tempbuff));
    }

    strcat((char *)gSendData,(char *)Recvdata);
    //#if DEBUG
    FILE *fp2 = NULL;
    if(ivalue == 14)
    {
        qDebug()<<"14";
        system("cd /usr/FINO/FPB/REQ/");
        system("rm /usr/FINO/FPB/REQ/FetchBeneList_REQ.TXT");
        fp2 =fopen("/usr/FINO/FPB/REQ/FetchBeneList_REQ.TXT","a");
    }
    else if(ivalue == 15)
    {
        qDebug()<<"15";
        system("cd /usr/FINO/FPB/REQ/");
        system("rm /usr/FINO/FPB/REQ/SearchWalk_REQ.TXT");
        fp2 =fopen("/usr/FINO/FPB/REQ/SearchWalk_REQ.TXT","a");
    }
    else if(ivalue == 16)
    {
        qDebug()<<"16";
        system("cd /usr/FINO/FPB/REQ/");
        system("rm /usr/FINO/FPB/REQ/SearchWalk_REQ.TXT");
        fp2 =fopen("/usr/FINO/FPB/REQ/SearchWalk_REQ.TXT","a");
    }
    else if(ivalue == 17)
    {
        qDebug()<<"17";
        system("cd /usr/FINO/FPB/REQ/");
        system("rm /usr/FINO/FPB/REQ/Cashoutotp_REQ.TXT");
        fp2 =fopen("/usr/FINO/FPB/REQ/Cashoutotp_REQ.TXT","a");
    }
    else if(ivalue == 18)
    {
        qDebug()<<"18";
        system("cd /usr/FINO/FPB/REQ/");
        system("rm /usr/FINO/FPB/REQ/CUSTLMT_REQ.TXT");
        fp2 =fopen("/usr/FINO/FPB/REQ/CUSTLMT_REQ.TXT","a");
    }
    else if(ivalue == 19)
    {
        qDebug()<<"19";
        system("cd /usr/FINO/FPB/REQ/");
        system("rm /usr/FINO/FPB/REQ/OTPIMPS_REQ.TXT");
        fp2 =fopen("/usr/FINO/FPB/REQ/OTPIMPS_REQ.TXT","a");
    }
    else if(ivalue == 20)
    {
        qDebug()<<"20";
        system("cd /usr/FINO/FPB/REQ/");
        system("rm /usr/FINO/FPB/REQ/OTPBENE_REQ.TXT");
        fp2 =fopen("/usr/FINO/FPB/REQ/OTPBENE_REQ.TXT","a");
    }
    else if(ivalue == 22)
    {
        qDebug()<<"21";
        system("cd /usr/FINO/FPB/REQ/");
        system("rm /usr/FINO/FPB/REQ/FINDCUST_REQ.TXT");
        fp2 =fopen("/usr/FINO/FPB/REQ/FINDCUST_REQ.TXT","a");
    }
    fwrite(gSendData,strlen((char *)gSendData),1,fp2);
    //ttestall(0,100);
    sleep(1);
    fclose(fp2);

    //#endif

    //    system("cd /usr/FINO/FPB/REQ/");
    //    fp2 = fopen("/usr/FINO/FPB/REQ/FINDCUST_REQ.TXT", "rb"); // open
    //    fseek(fp2, 0, SEEK_SET);
    //    fread(gSendData,sizeof(unsigned char ),strlen((char *)gSendData),fp2);
    //    fclose(fp2);

    return FUNC_OK;

}
