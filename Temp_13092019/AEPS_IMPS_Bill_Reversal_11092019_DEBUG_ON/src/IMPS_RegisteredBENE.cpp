#include "ui_FPB.h"
#include "displaymessage.h"
#include "entrydlg.h"
#include "Login.h"
#include "impscommon.h"
#include <QSqlDatabase>

char iNickName[30];
extern char * Convertstr(string str);
extern int BankDetails_Search;
extern POSTBENEVER postbeneverif;
extern searchWalkCust SearchWalkCust;
extern OTPBENEVERIF OtpBeneVer;
extern sCustDetails CustDet;

extern unsigned char eBene_beneficiaryId[30][20], eBene_nickName[30][30], eBene_beneficiaryName[30][30], eBene_beneficiaryBank[30][35];
extern unsigned  eBene_beneficiaryBankCity[30][30], eBene_beneficiaryBranch[30][35], eBene_beneficiaryBankIfsc[30][15],eBene_status[30][3];
extern unsigned char eBene_beneficiaryEmailId[30][40], eBene_eBene_applicationId[30][15], eBene_beneficiaryType[30][5], eBene_verificationStatus[30][20],eBene_beneficiarysequence[30][5];
extern int count_bene, count_bc_merchant_acc, index_bc_merchant_acc;
extern int flag_bc_merchant_account, flag_bene_verfication_status;
int flag_Registered_bene_activated = 0;
QString Del_seq,Del_type,Del_app,Del_name,Del_account;
int Del_index;
char flag_bitmap = '\0';
DELETEBENE deletebene;
extern Login_leg2 configLoginLeg2;

void FPB::on_BtnRegistered_Bene_clicked()
{
    flag_bitmap = 'R';
    flag_bene_verfication_status = 0;
    flag_Registered_bene_activated = 0;
    ui->checkBox_button->setChecked(false);
    ui->Add_bene_label->hide();
    //ui->Check_button->hide();
    ui->checkBox_button->hide();
    ui->BENE_List_label->setVisible(true);
    ui->cmb_Registered_Bene_List->setVisible(true);
    ui->cmb_bankList->clear();

    ui->Bene_acc_no_edit->clear();
    ui->cmb_bankList->clear();
    ui->Bene_Name_edit->clear();
    ui->IFSC_edit->clear();

    ui->cmb_bankList->setDisabled(true);
    ui->Bene_acc_no_edit->setDisabled(true);
    ui->Bene_Name_edit->setDisabled(true);
    ui->IFSC_edit->setDisabled(true);

    ui->pushButton_5->setVisible(true);

    ui->cmb_Registered_Bene_List->clear(); // clear list
    ui->cmb_Registered_Bene_List->setFont(QFont("Courier",18));

    memset(CustDet.cAddBeneWithouTxn, 0, sizeof(CustDet.cAddBeneWithouTxn));

    if(flag_bc_merchant_account == 'B')
    {
 #if DEBUG
        qDebug()<<"index_bc_merchant_acc = "<<index_bc_merchant_acc;
 #endif
        ui->Cash_contri_label->setVisible(false);
        ui->lbl_CashContribution->setVisible(false);

        ui->cmb_Registered_Bene_List->addItem((char *)eBene_beneficiaryName[index_bc_merchant_acc]);
#if DEBUG
        printf("current bene index1 = %d\n", index_bc_merchant_acc);
        printf("Account number1 = %s\n",eBene_beneficiaryId[index_bc_merchant_acc]);
#endif
        ui->Bene_acc_no_edit->setText((char *)eBene_beneficiaryId[index_bc_merchant_acc]);
        ui->cmb_bankList->addItem((char *)eBene_beneficiaryBank[index_bc_merchant_acc]);
        ui->cmb_bankList->setCurrentIndex(0);
        ui->Bene_Name_edit->setText((char *)eBene_beneficiaryName[index_bc_merchant_acc]);
        ui->IFSC_edit->setText((char *)eBene_beneficiaryBankIfsc[index_bc_merchant_acc]);
        memset(iNickName, 0, sizeof(iNickName));
        memcpy(iNickName, eBene_nickName[index_bc_merchant_acc], strlen((char *)eBene_nickName[index_bc_merchant_acc]));
    }
    else
    {
        for(int i=0; i<count_bene; i++)
        {
            ui->cmb_Registered_Bene_List->addItem((char *)eBene_beneficiaryName[i]);

        }

        int cur_index = ui->cmb_Registered_Bene_List->currentIndex();
#if DEBUG
        printf("current bene index2 = %d\n", cur_index);
        printf("Account number2 = %s\n",eBene_beneficiaryId[cur_index]);
#endif
        ui->Bene_acc_no_edit->setText((char *)eBene_beneficiaryId[cur_index]);
        ui->cmb_bankList->addItem((char *)eBene_beneficiaryBank[cur_index]);
        ui->cmb_bankList->setCurrentIndex(0);
        ui->Bene_Name_edit->setText((char *)eBene_beneficiaryName[cur_index]);
        ui->IFSC_edit->setText((char *)eBene_beneficiaryBankIfsc[cur_index]);
        memset(iNickName, 0, sizeof(iNickName));
        memcpy(iNickName, eBene_nickName[cur_index], strlen((char *)eBene_nickName[cur_index]));

    }
    ui->cmb_bankList->setDisabled(true);
    ui->Bene_acc_no_edit->setDisabled(true);
    ui->Verify_button->hide();
    ui->Bene_Search_Button->hide();


    ui->Bene_acc_no_edit->setValidator(new QRegExpValidator(QRegExp("[0-9]{0,18}"),this));
    ui->cmb_Registered_Bene_List->setCurrentIndex(0);
    ui->stackedWidget->setCurrentIndex(18);
    return;
}

void FPB::on_cmb_Registered_Bene_List_activated(const QString &arg1)
{
    flag_Registered_bene_activated = 1;
    int cur_index = ui->cmb_Registered_Bene_List->currentIndex();
    Del_index = cur_index;
 #if DEBUG
    printf("current bene index = %d\n", cur_index);
    printf("Account number = %s\n",eBene_beneficiaryId[cur_index]);
    printf("Application ID = %s\n",eBene_eBene_applicationId[cur_index]);
    printf("beneficiaryType = %s\n",eBene_beneficiaryType[cur_index]);
    printf("beneficiarysequence = %s\n",eBene_beneficiarysequence[cur_index]);
#endif
    ui->Bene_acc_no_edit->clear();
    ui->cmb_bankList->clear();
    ui->Bene_Name_edit->clear();
    ui->IFSC_edit->clear();



    memset(CustDet.cBenificiaryName , 0, sizeof(CustDet.cBenificiaryName ));
    memset(CustDet.cIFSC_Code , 0, sizeof(CustDet.cIFSC_Code ));
    memset(CustDet.cBranchName , 0, sizeof(CustDet.cBranchName ));
    memset(CustDet.cCity , 0, sizeof(CustDet.cCity ));


    if(flag_bc_merchant_account == 'B')
    {
        memcpy(CustDet.cBenificiaryName, (char *)eBene_beneficiaryName[index_bc_merchant_acc], strlen((char *)eBene_beneficiaryName[index_bc_merchant_acc]));
        memcpy(CustDet.cIFSC_Code, (char *)eBene_beneficiaryBankIfsc[index_bc_merchant_acc], strlen((char *)eBene_beneficiaryBankIfsc[index_bc_merchant_acc]));
        memcpy(CustDet.cBranchName, (char *)eBene_beneficiaryBranch[index_bc_merchant_acc], strlen((char *)eBene_beneficiaryBranch[index_bc_merchant_acc]));
        memcpy(CustDet.cCity, (char *)eBene_beneficiaryBankCity[index_bc_merchant_acc], strlen((char *)eBene_beneficiaryBankCity[index_bc_merchant_acc]));

        ui->Bene_acc_no_edit->setText((char *)eBene_beneficiaryId[index_bc_merchant_acc]);
        ui->cmb_bankList->addItem((char *)eBene_beneficiaryBank[index_bc_merchant_acc]);
        ui->cmb_bankList->setCurrentIndex(0);
        ui->Bene_Name_edit->setText((char *)eBene_beneficiaryName[index_bc_merchant_acc]);
        ui->IFSC_edit->setText((char *)eBene_beneficiaryBankIfsc[index_bc_merchant_acc]);
        memset(iNickName, 0, sizeof(iNickName));
        memcpy(iNickName, eBene_nickName[index_bc_merchant_acc], strlen((char *)eBene_nickName[index_bc_merchant_acc]));
    }
    else
    {
        memcpy(CustDet.cBenificiaryName, (char *)eBene_beneficiaryName[cur_index], strlen((char *)eBene_beneficiaryName[cur_index]));
        memcpy(CustDet.cIFSC_Code, (char *)eBene_beneficiaryBankIfsc[cur_index], strlen((char *)eBene_beneficiaryBankIfsc[cur_index]));
        memcpy(CustDet.cBranchName, (char *)eBene_beneficiaryBranch[cur_index], strlen((char *)eBene_beneficiaryBranch[cur_index]));
        memcpy(CustDet.cCity, (char *)eBene_beneficiaryBankCity[cur_index], strlen((char *)eBene_beneficiaryBankCity[cur_index]));

        ui->Bene_acc_no_edit->setText((char *)eBene_beneficiaryId[cur_index]);
        ui->cmb_bankList->addItem((char *)eBene_beneficiaryBank[cur_index]);
        ui->cmb_bankList->setCurrentIndex(0);
        ui->Bene_Name_edit->setText((char *)eBene_beneficiaryName[cur_index]);
        ui->IFSC_edit->setText((char *)eBene_beneficiaryBankIfsc[cur_index]);
        memset(iNickName, 0, sizeof(iNickName));
        memcpy(iNickName, eBene_nickName[cur_index], strlen((char *)eBene_nickName[cur_index]));
    }
#if DEBUG
    printf("CustDet.cBranchName = %s\n",CustDet.cBranchName);
    printf("CustDet.cCity = %s\n",CustDet.cCity);
#endif

    Del_seq.clear();
    Del_type.clear();
    Del_app.clear();
    Del_account.clear();
    Del_name.clear();

    if(flag_bc_merchant_account == 'B')
    {
        Del_account = QString::fromUtf8((const char*)eBene_beneficiaryId[index_bc_merchant_acc]);
        Del_name = QString::fromUtf8((const char*)eBene_beneficiaryName[index_bc_merchant_acc]);
        Del_app = QString::fromUtf8((const char*)eBene_eBene_applicationId[index_bc_merchant_acc]);
        Del_type = QString::fromUtf8((const char*)eBene_beneficiaryType[index_bc_merchant_acc]);
        Del_seq = QString::fromUtf8((const char*)eBene_beneficiarysequence[index_bc_merchant_acc]);
    }
    else
    {
        Del_account = QString::fromUtf8((const char*)eBene_beneficiaryId[cur_index]);
        Del_name = QString::fromUtf8((const char*)eBene_beneficiaryName[cur_index]);
        Del_app = QString::fromUtf8((const char*)eBene_eBene_applicationId[cur_index]);
        Del_type = QString::fromUtf8((const char*)eBene_beneficiaryType[cur_index]);
        Del_seq = QString::fromUtf8((const char*)eBene_beneficiarysequence[cur_index]);
    }


    ui->stackedWidget->setCurrentIndex(18);
    return;

}

void FPB::on_Back_PushButton_Pg35_clicked()
{
    //    CMisc miscObj; // 02052019
    //    uidmessage::instance()->hide_Message(); // 02052019
    //    int ret = miscObj.DisplayMsgBox_SHG("Are you sure, you want to discard the process?"); // 02052019
    //    uidmessage::instance()->hide_Message(); // 02052019
    //    if(ret == true) // 02052019
    //    { // 02052019
    //        ui->stackedWidget->setCurrentIndex(12); // 02052019
    //    } // 02052019
    ui->stackedWidget->setCurrentIndex(15); // 02052019
    //    else
    //        ui->stackedWidget->setCurrentIndex(35);
    return;
}

void FPB::on_pushButton_5_clicked()
{
    CMisc miscObj;

    if((flag_bc_merchant_account == 'B') || (memcmp(iNickName, "EMD", 3)==0))
    {
        uidmessage::instance()->hide_Message();
        miscObj.DisplayMsgBox((char *)"Bene cannot be deleted for this account.");
        uidmessage::instance()->hide_Message();
        return;
    }

    if(flag_Registered_bene_activated == 0)
    {
        int cur_index = -1;
        if(flag_bc_merchant_account == 'B')
            cur_index = index_bc_merchant_acc;
        else
            cur_index = ui->cmb_Registered_Bene_List->currentIndex();
#if DEBUG
        printf("current bene index = %d\n", cur_index);
        printf("Account number = %s\n",eBene_beneficiaryId[cur_index]);
        printf("Application ID = %s\n",eBene_eBene_applicationId[cur_index]);
        printf("beneficiaryType = %s\n",eBene_beneficiaryType[cur_index]);
        printf("beneficiarysequence = %s\n",eBene_beneficiarysequence[cur_index]);
#endif


        Del_seq.clear();
        Del_type.clear();
        Del_app.clear();
        Del_account.clear();
        Del_name.clear();

        if(flag_bc_merchant_account == 'B')
        {
            Del_account = QString::fromUtf8((const char*)eBene_beneficiaryId[index_bc_merchant_acc]);
            Del_name = QString::fromUtf8((const char*)eBene_beneficiaryName[index_bc_merchant_acc]);
            Del_app = QString::fromUtf8((const char*)eBene_eBene_applicationId[index_bc_merchant_acc]);
            Del_type = QString::fromUtf8((const char*)eBene_beneficiaryType[index_bc_merchant_acc]);
            Del_seq = QString::fromUtf8((const char*)eBene_beneficiarysequence[index_bc_merchant_acc]);
        }
        else
        {
            Del_account = QString::fromUtf8((const char*)eBene_beneficiaryId[cur_index]);
            Del_name = QString::fromUtf8((const char*)eBene_beneficiaryName[cur_index]);
            Del_app = QString::fromUtf8((const char*)eBene_eBene_applicationId[cur_index]);
            Del_type = QString::fromUtf8((const char*)eBene_beneficiaryType[cur_index]);
            Del_seq = QString::fromUtf8((const char*)eBene_beneficiarysequence[cur_index]);
        }

    }

    uidmessage::instance()->hide_Message();
    char temp[100];
    memset(temp, 0, sizeof(temp));
    sprintf(temp,"Are you sure, you want to delete Beneficiary \"%s(A/C-%s)\"?",Del_name.trimmed().toAscii().data(),Del_account.trimmed().toAscii().data());
    int ret = miscObj.DisplayMsgBox_SHG(temp);
    uidmessage::instance()->hide_Message();
    if(ret == true)
    {
        int iRet_del = DeleteBene();
        if(iRet_del!=FUNC_OK && iRet_del!=FUNCTION_SUCCESS)
        {
            uidmessage::instance()->hide_Message();
            if(iRet_del == SESSION_EXPIRED_FAILED)
                ui->stackedWidget->setCurrentIndex(0);
            else
                ui->stackedWidget->setCurrentIndex(18);
            return;
        }
        else
        {
            Deletedata();
            uidmessage::instance()->hide_Message();
            if(count_bene == 0)
                ui->stackedWidget->setCurrentIndex(15);
            else
                ui->stackedWidget->setCurrentIndex(18);
            return;
        }
    }


    ui->stackedWidget->setCurrentIndex(18);
    return;
}


int FPB::DeleteBene()
{
    uidmessage::instance()->hide_Message();
    uidmessage::instance()->Display_BusyMessage("Progressing Please wait...");
    char ErrorMsg[100];
    HostComm hComm;
    CMisc miscObj;
    Check_Conditions();
    int ret;
    Json1::Value RequestedData,mainreq;
    char *jsondata = NULL;;
    memset(ErrorMsg,0,sizeof(ErrorMsg));
    memset(deletebene.ucRequestID_req,0, sizeof(deletebene.ucRequestID_req));
    memset(deletebene.ucMethodID_req,0, sizeof(deletebene.ucMethodID_req));
    memset(deletebene.ucTellerID_req,0, sizeof(deletebene.ucTellerID_req));
    memset(deletebene.ucappId_req,0, sizeof(deletebene.ucappId_req));
    memset(deletebene.ucSessionID_req,0, sizeof(deletebene.ucSessionID_req));
    memset(deletebene.ucIsEncrypt_req,0, sizeof(deletebene.ucIsEncrypt_req));
    memset(deletebene.ucchannelId_req,0, sizeof(deletebene.ucchannelId_req));
    memset(deletebene.ucappId_req,0, sizeof(deletebene.ucappId_req));
    memset(deletebene.ucbeneficiaryType_req,0, sizeof(deletebene.ucbeneficiaryType_req));
    memset(deletebene.ucbeneficiarySequence_req,0, sizeof(deletebene.ucbeneficiarySequence_req));
    memset(deletebene.ucchannelId_req,0, sizeof(deletebene.ucchannelId_req));


    iGenerateRequestID((char *)username,(unsigned char *)deletebene.ucRequestID_req);
    memcpy((char *)deletebene.ucMethodID_req,"23",2);
    memcpy((char *)deletebene.ucTellerID_req,username,strlen((char *)username));

    memcpy((char *)deletebene.ucappId_req,Del_app.trimmed().toAscii().data(),strlen(Del_app.trimmed().toAscii().data()));
    memcpy((char *)deletebene.ucbeneficiaryType_req,Del_type.trimmed().toAscii().data(),strlen(Del_type.trimmed().toAscii().data()));
    memcpy((char *)deletebene.ucbeneficiarySequence_req,Del_seq.trimmed().toAscii().data(),strlen(Del_seq.trimmed().toAscii().data()));
    memcpy((char *)deletebene.ucchannelId_req,CHANNEL_ID,strlen(CHANNEL_ID));
    memcpy((char *)deletebene.ucSessionID_req,configLoginLeg2.ucSessionId,strlen((char *)configLoginLeg2.ucSessionId));
    memcpy((char *)deletebene.ucIsEncrypt_req,ISENCEYPT,strlen(ISENCEYPT));

    mainreq["RequestId"] =(char *)deletebene.ucRequestID_req;
    mainreq["MethodId"] = (char *)deletebene.ucMethodID_req;
    mainreq["ChannelID"] = (char *)deletebene.ucTellerID_req;
    mainreq["SessionId"] = (char *)deletebene.ucSessionID_req;
    mainreq["TellerID"]=(char *)deletebene.ucTellerID_req;
    mainreq["TokenId"]= "";
    mainreq["X_Auth_Token"] ="";
    mainreq["IsEncrypt"]= (char *)deletebene.ucIsEncrypt_req;
    mainreq["SessionExpiryTime"]= "";
    RequestedData["customerNumber"] =(char *)FC_CustomerNumber;
    RequestedData["appId"] =(char *)deletebene.ucappId_req;
    RequestedData["beneficiaryType"] = (char *)deletebene.ucbeneficiaryType_req;
    RequestedData["beneficiarySequence"] = (char *)deletebene.ucbeneficiarySequence_req;


    QString buff_tmp = serialize1(Convertstr(RequestedData.toStyledString()));

    mainreq["RequestData"] = buff_tmp.toAscii().data();

    jsondata = Convertstr(mainreq.toStyledString());
#if DEBUG
    qDebug()<<"------------->"<<jsondata;
#endif
    //-------------------------------------------------------------
    memset(gSendData,0,sizeof(gSendData));
    strncpy((char*)gSendData, jsondata, strlen(jsondata));
    /****************************Encryption function*********************************/
    if(!strcmp((char *)deletebene.ucIsEncrypt_req,"true"))
    {
        int gSendData_len =strlen((char *)gSendData);
        RequestDetachEncryptAttach((char *)gSendData,1,gSendData_len);
    }
    /***************************************************************/


#if DEBUG
    printf("gSendData = %s\n\n",gSendData);
#endif
    //-------------------------------------------------------------

#ifdef PRODUCTION
    strcpy(str_url,URL_CREATION(COMMON_URL_PROD));
#else
    strcpy(str_url,URL_CREATION((char *)COMMON_URL));
#endif

    ret = NetworkConnect(0,str_url, 2);
    if(ret != FUNCTION_SUCCESS)
    {
        uidmessage::instance()->hide_Message();
        if(ret == RESPONSE_TIMEOUT)
        {
            qDebug()<<"Response Timeout";
        }
        return FUNCTION_FAILED;
    }
    /****************************Decryption function*********************************/
    if(!strcmp((char *)deletebene.ucIsEncrypt_req,"true"))
    {
        int oBuff_len = strlen(oBuff);
 #if DEBUG
        qDebug()<<"Length of buffer"<<oBuff_len;
        printf("Buffer%s\n",oBuff);
#endif
        ResponseDetachEncryptAttach(oBuff,2,oBuff_len);//TEMPORARY

    }
    /*******************************************************************************/

    QString DELBUFF = QString::fromLocal8Bit(oBuff);
    bool check = DELBUFF.contains("ResponseCode");
    if(check == false)
    {
        miscObj.DisplayMsgBox((char *)"Invalid Data");
        uidmessage::instance()->hide_Message();
        return FUNCTION_FAILED;
    }
    DELBUFF.remove("\\");
    DELBUFF.remove("{");
    DELBUFF.remove("}");
    DELBUFF.remove('"');
    DELBUFF.remove("[");
    DELBUFF.remove("]");
    QStringList query = DELBUFF.split(",");
 #if DEBUG
    qDebug()<<"WALKBuff"<<DELBUFF;
    for(int i=0 ; i < query.length() ; i++)
    {
        qDebug()<<"Query Response"<<query.at(i);
    }
#endif
    QStringList ResponseCode = query.filter("ResponseCode");
    QStringList ResponseMessage = query.filter("ResponseMessage");
    QString str ,strToFind,value,value1,value2;
    memset(deletebene.ucResponseCode, 0, sizeof(deletebene.ucResponseCode));
    memset(deletebene.ucResponseMessage, 0, sizeof(deletebene.ucResponseMessage));
    str = ResponseCode.at(0);
    strToFind = ":";

    value1 = str.mid(str.indexOf(strToFind)+strToFind.length());


    if(value1 =="0")
    {
        //        uidmessage::instance()->hide_Message();
        return FUNC_OK;
    }
    else
    {
        str = ResponseMessage.at(0);
        strToFind = ":";
        value2 = str.mid(str.indexOf(strToFind)+strToFind.length());
        memset(deletebene.ucResponseMessage, 0, sizeof(deletebene.ucResponseMessage));
        memcpy(deletebene.ucResponseMessage,value2.trimmed().toAscii().data(), strlen(value2.trimmed().toAscii().data()));
        if(strcmp((char *)deletebene.ucResponseMessage,"Session Expired.")==0)
        {
            uidmessage::instance()->hide_Message();
            sprintf(ErrorMsg,"%s",(char *)deletebene.ucResponseMessage);
            miscObj.DisplayMsgBox(ErrorMsg);
            uidmessage::instance()->hide_Message();
            return SESSION_EXPIRED_FAILED;
        }
        uidmessage::instance()->hide_Message();
        sprintf(ErrorMsg,"%s",(char *)deletebene.ucResponseMessage);
        miscObj.DisplayMsgBox(ErrorMsg);
        uidmessage::instance()->hide_Message();
        return FUNCTION_FAILED;
    }

    return FUNCTION_SUCCESS;

}



int FPB::Deletedata()
{

    int i;
    for(i=0; i<count_bene; i++)
    {
        if(i==Del_index)
        {
            for(int j=i; j<(count_bene-1); j++)
            {
                memset((char *)eBene_beneficiaryId[j],0,strlen((char *)eBene_beneficiaryId[j]));
                memset((char *)eBene_nickName[j],0,strlen((char *)eBene_nickName[j]));
                memset((char *)eBene_beneficiaryName[j],0,strlen((char *)eBene_beneficiaryName[j]));
                memset((char *)eBene_beneficiaryBank[j],0,strlen((char *)eBene_beneficiaryBank[j]));
                memset((char *)eBene_beneficiaryBankCity[j],0,strlen((char *)eBene_beneficiaryBankCity[j]));
                memset((char *)eBene_beneficiaryBranch[j],0,strlen((char *)eBene_beneficiaryBranch[j]));
                memset((char *)eBene_beneficiaryBankIfsc[j],0,strlen((char *)eBene_beneficiaryBankIfsc[j]));
                memset((char *)eBene_status[j],0,strlen((char *)eBene_status[j]));
                memset((char *)eBene_beneficiaryEmailId[j],0,strlen((char *)eBene_beneficiaryEmailId[j]));
                memset(eBene_eBene_applicationId[j],0,strlen((char *)eBene_eBene_applicationId[j]));
                memset((char *)eBene_verificationStatus[j],0,strlen((char *)eBene_verificationStatus[j]));
                memset((char *)eBene_beneficiaryType[j],0,strlen((char *)eBene_beneficiaryType[j]));
                memset((char *)eBene_beneficiarysequence[j],0,strlen((char *)eBene_beneficiarysequence[j]));



                memcpy((char *)eBene_beneficiaryId[j],(char *)eBene_beneficiaryId[j+1],strlen((char *)eBene_beneficiaryId[j+1]));
                memcpy((char *)eBene_nickName[j],(char *)eBene_nickName[j+1],strlen((char *)eBene_nickName[j+1]));
                memcpy((char *)eBene_beneficiaryName[j],(char *)eBene_beneficiaryName[j+1],strlen((char *)eBene_beneficiaryName[j+1]));
                memcpy((char *)eBene_beneficiaryBank[j],(char *)eBene_beneficiaryBank[j+1],strlen((char *)eBene_beneficiaryBank[j+1]));
                memcpy((char *)eBene_beneficiaryBankCity[j],(char *)eBene_beneficiaryBankCity[j+1],strlen((char *)eBene_beneficiaryBankCity[j+1]));
                memcpy((char *)eBene_beneficiaryBranch[j],(char *)eBene_beneficiaryBranch[j+1],strlen((char *)eBene_beneficiaryBranch[j+1]));
                memcpy((char *)eBene_beneficiaryBankIfsc[j],(char *)eBene_beneficiaryBankIfsc[j+1],strlen((char *)eBene_beneficiaryBankIfsc[j+1]));
                memcpy((char *)eBene_status[j],(char *)eBene_status[j+1],strlen((char *)eBene_status[j+1]));
                memcpy((char *)eBene_beneficiaryEmailId[j],(char *)eBene_beneficiaryEmailId[j+1],strlen((char *)eBene_beneficiaryEmailId[j+1]));
                memcpy((char *)eBene_eBene_applicationId[j],(char *)eBene_eBene_applicationId[j+1],strlen((char *)eBene_eBene_applicationId[j+1]));
                memcpy((char *)eBene_verificationStatus[j],(char *)eBene_verificationStatus[j+1],strlen((char *)eBene_verificationStatus[j+1]));
                memcpy((char *)eBene_beneficiaryType[j],(char *)eBene_beneficiaryType[j+1],strlen((char *)eBene_beneficiaryType[j+1]));
                memcpy((char *)eBene_beneficiarysequence[j],(char *)eBene_beneficiarysequence[j+1],strlen((char *)eBene_beneficiarysequence[j+1]));
            }
            break;
        }
    }
    count_bene = (count_bene-1);

    ui->cmb_Registered_Bene_List->clear(); // clear list
    ui->cmb_Registered_Bene_List->setFont(QFont("Courier",18));
    for(int i=0; i<count_bene; i++)
    {
        ui->cmb_Registered_Bene_List->addItem((char *)eBene_beneficiaryName[i]);
    }
    ui->cmb_bankList->setDisabled(true);
    ui->Bene_acc_no_edit->setDisabled(true);
    ui->Verify_button->hide();
    ui->Bene_Search_Button->hide();

    int cur_index = ui->cmb_Registered_Bene_List->currentIndex();
#if DEBUG
    printf("\ncount_bene = %d\ncurrent bene index = %d\n",count_bene, cur_index);
    printf("Account number = %s\n",eBene_beneficiaryId[cur_index]);
#endif
    ui->Bene_acc_no_edit->setText((char *)eBene_beneficiaryId[cur_index]);
    ui->cmb_bankList->addItem((char *)eBene_beneficiaryBank[cur_index]);
    ui->cmb_bankList->setCurrentIndex(0);
    ui->Bene_Name_edit->setText((char *)eBene_beneficiaryName[cur_index]);
    ui->IFSC_edit->setText((char *)eBene_beneficiaryBankIfsc[cur_index]);

    ui->Bene_acc_no_edit->setValidator(new QRegExpValidator(QRegExp("[0-9]{0,18}"),this));
    ui->cmb_Registered_Bene_List->setCurrentIndex(0);
    return 1;
}
