#include "impscommon.h"
#include "ui_FPB.h"
#include "json.h"
#include "Login.h"
#include "displaymessage.h"
#include "print.h"
#include "bankmaster.h"
#include <QSqlDatabase>
//#include "FPB.h"
searchWalkCust SearchWalkCust;
extern Login_leg2 configLoginLeg2;
extern AEPS_BANKMAST AEPSBM;
extern SEARCHBYIFSC byifsc;
extern POSTBENEVER_CHARGES_RESP beneverifcharges;
sCustDetails CustDet;
extern int isearchby;
extern int OTP_page;
int BankDetails_Search = 0;
extern int isearchby;
extern int count_bc_merchant_acc, flag_bc_merchant_account, Resend_flag, flag_bene_verfication_status;
extern int index_bc_merchant_acc, flag_Registered_bene_activated;
int AGE = 0, Flag_Again_IFSC_Search = 0;
unsigned char Total_BeneVerif_ChargesAmount[10];
extern unsigned char eBene_beneficiaryId[30][20], eBene_nickName[30][30], eBene_beneficiaryName[30][30], eBene_beneficiaryBank[30][35];
extern unsigned  eBene_beneficiaryBankCity[30][30], eBene_beneficiaryBranch[30][35], eBene_beneficiaryBankIfsc[30][15],eBene_status[30][3];
extern unsigned char eBene_beneficiaryEmailId[30][40], eBene_eBene_applicationId[30][15], eBene_beneficiaryType[30][5], eBene_verificationStatus[30][20],eBene_beneficiarysequence[30][5];



void FPB::on_dateEdit_3_dateChanged(const QDate &date1)
{

    char to_date[20] ,from_date[20];
    QString buff_tmp;
    QString dobDate;

    memset(to_date, 0, sizeof(to_date));
    memset(from_date, 0, sizeof(from_date));

    dobDate.clear();
    dobDate = ui->dateEdit_3->date().toString("yyyy-MM-dd") ;

    buff_tmp.clear();
#if DEBUG
    qDebug()<<"DOB ==== "<<dobDate;
#endif
    QDate date = QDate::currentDate();
    QString curDate = date.toString("yyyy-MM-dd");


    //    int today = ui->dateEdit_5->date().day();
    //    int tomonth = ui->dateEdit_5->date().month();
    //    int toyear = ui->dateEdit_5->date().year();

    //    int fromday = ui->dateEdit_6->date().day();
    //    int frommonth = ui->dateEdit_6->date().month();
    //    int fromyear = ui->dateEdit_6->date().year();

    QDate cal_dobDate = ui->dateEdit_3->date();
    QDate cal_curDate = QDate::currentDate();
#if DEBUG
    qDebug()<<"cal_dobDate = "<<cal_dobDate.toString("yyyy-MM-dd");
    qDebug()<<"cal_curDate = "<<cal_curDate.toString("yyyy-MM-dd");
#endif
    AGE = findAge(cal_dobDate, cal_curDate);


   // memclear((char *)CustDet.cDob);
    memset(CustDet.cDob, 0, sizeof((char *)CustDet.cDob));
    memcpy(CustDet.cDob, (char *)ui->dateEdit_3->date().toString("yyyy-MM-dd").trimmed().toAscii().data(), strlen((const char *)ui->dateEdit_3->date().toString("yyyy-MM-dd").trimmed().toAscii().data()));
    // memcpy(CustDet.cDob, (char *)dobDate.toAscii().trimmed().data(), strlen((const char*)dobDate.toAscii().trimmed().data()));
    strcpy(to_date,dobDate.toAscii().constData());
    strcpy(from_date,curDate.toAscii().constData());

    ui->Age_lineedit->clear();
    ui->Age_lineedit->setText(QString::number(AGE));
    ui->Age_lineedit->setDisabled(true);
    return;
}

void FPB::on_Next_back_button_clicked()
{
    CMisc miscObj;
    QSqlQuery query;
    query.clear();
    // int diff_days = getDifference(cal_dobDate, cal_curDate);
#if DEBUG
    qDebug()<<"AGE = "<<AGE;
    // qDebug()<<"dobDate "<<dobDate;
    //   qDebug()<<"curDate "<<curDate;
#endif

    if(AGE < 18)
    {
        uidmessage::instance()->hide_Message();
        miscObj.DisplayMsgBox((char *)"DOB should be greater than 18 years.");
        return;
    }
    if(strcmp(ui->Name_lineedit->text().trimmed().toAscii().data(), "")==0)
    {
        uidmessage::instance()->hide_Message();
        miscObj.DisplayMsgBox((char *)"Name is mandatory.");
        return;
    }
    if(ui->Name_lineedit->text().length() < 2)
    {
        uidmessage::instance()->hide_Message();
        miscObj.DisplayMsgBox((char *)"Name minimum length should be 2.");
        return;
    }

    //    QString format = "yyyy-MM-dd";
    //    memset(&CustDet,0,sizeof(CustDet));
    //    QString QCustName;
    //    QCustName.clear();
    //    QCustName = ui->cmbTitle->currentText().trimmed().toAscii().data();
    //    QString NameTitle;
    //    NameTitle.clear();
    //    NameTitle = ui->Name_lineedit->text().trimmed().toAscii().data();
    //    QString Fullname;
    //    Fullname.clear();
    //    Fullname = QCustName + " "+NameTitle;

    //  ui->Age_lineedit->setText(QString::number(AGE));
    //   ui->Age_lineedit->setDisabled(true);

    if(ui->Mobile_lineedit->text().mid(0,1)!= "9" && ui->Mobile_lineedit->text().mid(0,1) != "8" && ui->Mobile_lineedit->text().mid(0,1)!= "7" && ui->Mobile_lineedit->text().mid(0,1)!= "6")
    {
        miscObj.DisplayMsgBox((char *)"Mobile number should start from 9,8,7 or 6");
        return;
    }
    QString val1, val2;
    val1.clear();
    val2.clear();
    QStringList custFullname ;
    custFullname.clear();
    custFullname = ui->Name_lineedit->text().simplified().split(" ");

    int count_space = custFullname.size();
 #if DEBUG
    printf("count_space = %d\n",count_space);
#endif
    val1 = custFullname.at(0);
    if(count_space > 1)
        val2 = custFullname.at(1);
 #if DEBUG
    qDebug()<<"cust first name = "<<val1;
    qDebug()<<"cust last name = "<<val2;
 #endif
    //  strcpy(CustDet.cCustName,Fullname.toAscii().data());
    memset(CustDet.cCustName,     0, sizeof(CustDet.cCustName));
    memset(CustDet.cFirstName,    0, sizeof(CustDet.cFirstName));
    memset(CustDet.cLastName,     0, sizeof(CustDet.cLastName));
    //    memset(CustDet.cDob,          0, sizeof(CustDet.cDob));
    memset(CustDet.cAge,          0, sizeof(CustDet.cAge));
    memset(CustDet.cGender,       0, sizeof(CustDet.cGender));
    memset(CustDet.cMobileNumber, 0, sizeof(CustDet.cMobileNumber));

    memcpy(CustDet.cCustName, (char *)ui->Name_lineedit->text().trimmed().toAscii().data(), strlen((const char*) ui->Name_lineedit->text().trimmed().toAscii().data()));
    memcpy(CustDet.cFirstName, (char *)val1.toAscii().trimmed().data(), strlen((const char *)val1.toAscii().trimmed().data()));
    memcpy(CustDet.cLastName, (char *)val2.toAscii().trimmed().data(), strlen((const char *)val2.toAscii().trimmed().data()));
    //   strcpy(CustDet.cDob,ui->dateEdit_3->date().toString(format).trimmed().toAscii().data());
    strcpy(CustDet.cAge,ui->Age_lineedit->text().trimmed().toAscii().data());

    if(strcmp(ui->Gendertitle->currentText().trimmed().toAscii().data(), "MALE") == 0)
        memcpy(CustDet.cGender,"M",1);
    else if(strcmp(ui->Gendertitle->currentText().trimmed().toAscii().data(), "FEMALE") == 0)
        memcpy(CustDet.cGender,"F",1);
    else
        memcpy(CustDet.cGender,"T",1);


    strcpy(CustDet.cMobileNumber,ui->Mobile_lineedit->text().trimmed().toAscii().data());


    query.clear();
    query.prepare("select STATE__NAME from state");
    if(!query.exec())
    {
        qDebug() << query.lastError();
        return;
    }
    else
    {
        ui->Person_state_edit_comboBox->clear();
        while(query.next())
        {
            if(query.value(0).toString() != NULL)
            {
                ui->Person_state_edit_comboBox->addItem(query.value(0).toString());
            }
        }
    }

    ui->Person_state_edit_comboBox->setCurrentIndex(0);
    ui->Person_city_edit_2_comboBox->clear();
    ui->Person_GSTIN_edit->clear();
    ui->stackedWidget->setCurrentIndex(17);
    return;
}

void FPB::on_person_add_next_button_clicked()
{

//    QRegExp rx1("/^([0]{1}[1-9]{1}|[1-2]{1}[0-9]{1}|[3]{1}[0-7]{1})([a-zA-Z]{5}[0-9]{4}[a-zA-Z]{1}[1-9a-zA-Z]{1}[zZ]{1}[0-9a-zA-Z]{1})+$/g");
// //   QRegExp rx("[1-9]\\d{0,3}");
//    QRegExpValidator v(rx1, 0);
//    QString s;
//    int pos = 0;

//    s = "37adapm1724a2Z5";
//    v.validate(s, pos);    // returns Invalid

//    printf("pos = %d %d\n", pos,v);

//    s = "37adapm1724a2q5";
//    v.validate(s, pos);    // returns Invalid
//    printf("pos = %d %d\n", pos,v);



    int ret = 0;
    CMisc miscObj;
    if(strcmp(ui->Person_add_edit->text().trimmed().toAscii().data(), "")==0)
    {
        uidmessage::instance()->hide_Message();
        miscObj.DisplayMsgBox((char *)"Address cannot be blank.");
        return;
    }
    else if(ui->Person_add_edit->text().length() < 10)
    {
        uidmessage::instance()->hide_Message();
        miscObj.DisplayMsgBox((char *)"Address should be minimum 10 characters.");
        return;
    }

    if(strcmp(ui->Person_pincode_edit->text().trimmed().toAscii().data(), "")==0)
    {
        uidmessage::instance()->hide_Message();
        miscObj.DisplayMsgBox((char *)"Pincode cannot be blank.");
        return;
    }
    else if(ui->Person_pincode_edit->text().length() < 6)
    {
        uidmessage::instance()->hide_Message();
        miscObj.DisplayMsgBox((char *)"Pincode must be 6 digits.");
        return;
    }

    if((ui->Person_GSTIN_edit->text().length() > 0) && (ui->Person_GSTIN_edit->text().length() < 15))
    {
        uidmessage::instance()->hide_Message();
        miscObj.DisplayMsgBox((char *)"GSTIN Number should be min 15 digits.");
        return;
    }


    memset(&AEPSBM,0,sizeof(AEPSBM));
    memset(CustDet.cAddress, 0, sizeof(CustDet.cAddress));
    memset(CustDet.cstate, 0, sizeof(CustDet.cstate));
    memset(CustDet.cCity, 0, sizeof(CustDet.cCity));
    memset(CustDet.cPincode, 0, sizeof(CustDet.cPincode));
    memset(CustDet.cGSTINNumber, 0, sizeof(CustDet.cGSTINNumber));

    memcpy(CustDet.cAddress ,(char *)ui->Person_add_edit->text().trimmed().toAscii().data(), strlen((char *)ui->Person_add_edit->text().trimmed().toAscii().data()));
    memcpy(CustDet.cstate, (char *)ui->Person_state_edit_comboBox->currentText().trimmed().toAscii().data(), strlen((char *)ui->Person_state_edit_comboBox->currentText().trimmed().toAscii().data()));
    memcpy(CustDet.cCity, (char *)ui->Person_city_edit_2_comboBox->currentText().trimmed().toAscii().data(), strlen((char *)ui->Person_state_edit_comboBox->currentText().trimmed().toAscii().data()));
    memcpy(CustDet.cPincode,(char *) ui->Person_pincode_edit->text().trimmed().toAscii().data(), strlen((char *)ui->Person_add_edit->text().trimmed().toAscii().data()));
    memcpy(CustDet.cGSTINNumber,(char *) ui->Person_GSTIN_edit->text().trimmed().toAscii().data(), strlen((char *)ui->Person_GSTIN_edit->text().trimmed().toAscii().data()));

#if DEBUG
    printf("CustDet.cGSTINNumber = %s\n", CustDet.cGSTINNumber);
#endif

    flag_bitmap = 'W';
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
    if(ret = FUNC_OK)
    {
        ui->cmb_bankList->setCurrentIndex(0);
        //        ui->stackedWidget->setCurrentIndex(18);
    }
    //    else
    //        ui->stackedWidget->setCurrentIndex(18);

    ret = BeneVerificationCharges();

    if(ret != FUNCTION_SUCCESS && ret != FUNC_OK)
    {
        Flag_BeneVerif_Charges = 0;
        uidmessage::instance()->hide_Message();
        if(ret == SESSION_EXPIRED_FAILED)
            ui->stackedWidget->setCurrentIndex(0);
        else
            ui->stackedWidget->setCurrentIndex(17);
        return;
    }
    else if(ret == FUNCTION_SUCCESS || ret == FUNC_OK)
    {
        Flag_BeneVerif_Charges = 0;
        uidmessage::instance()->hide_Message();
        ui->OTP_Lineedit->clear();

        QString Charges;
        Charges.clear();
        bool decimal;
        int icharges = atoi((char *)beneverifcharges.ucRDPTCharges_RES);
        Charges = QString::number(icharges);

        decimal = Charges.contains('.');
        if(decimal == false)
        {
            Charges = Charges.append(".00");
        }

       // memclear((char *)Total_BeneVerif_ChargesAmount);
        memset(Total_BeneVerif_ChargesAmount, 0, sizeof((char *)Total_BeneVerif_ChargesAmount));

        memcpy(Total_BeneVerif_ChargesAmount, (char *)Charges.trimmed().toAscii().data(), strlen((const char*)Charges.trimmed().toAscii().data()));

        ui->Verify_button->setText("VERIFY @ "+Charges+" RS.");
        //        ui->Verify_button->setEnabled(true);
        //        ui->Verify_button->setStyleSheet("background-color: rgb(85, 85, 127); color: rgb(255, 255, 255)");

        ui->stackedWidget->setCurrentIndex(18);
        return;
    }

}


void FPB::on_Person_state_edit_comboBox_activated(const QString &arg1)
{
    QSqlQuery query;
    char state_code[5];
    query.clear();

    query.prepare("select STATE__CODE from state where STATE__NAME LIKE :state_name ");
    query.bindValue(":state_name", QString("%%1%").arg(ui->Person_state_edit_comboBox->currentText().trimmed().toAscii().data()));

    if(!query.exec())
    {
        qDebug() << query.lastError();
        return;
    }
    else
    {
        while(query.next())
        {
            if(query.value(0).toString() != NULL)
            {
                memset(state_code,0,sizeof(state_code));
                memcpy(state_code, (const char *)query.value(0).toString().toAscii().data(), strlen((const char *)query.value(0).toString().toAscii().data()));
       #if DEBUG
                printf("state_code = %s\n",state_code);
       #endif
            }
        }
    }
    memset(CustDet.cStatecode, 0, sizeof(CustDet.cStatecode));
    memcpy(CustDet.cStatecode, (char *)state_code, strlen(state_code));

    query.clear();
    query.prepare("select COUNTRY__CODE from state where STATE__CODE LIKE :state_code ");
    query.bindValue(":state_code", QString("%%1%").arg(state_code));
    if(!query.exec())
    {
        qDebug() << query.lastError();
        return;
    }
    else
    {
        while(query.next())
        {
            if(query.value(0).toString() != NULL)
            {
                memset(CustDet.cCountrycode, 0, sizeof(CustDet.cCountrycode));
                memcpy(CustDet.cCountrycode, (const char *)query.value(0).toString().toAscii().data(), strlen((const char *)query.value(0).toString().toAscii().data()));
         #if DEBUG
                printf("Country Code == %s\n",(const char *)query.value(0).toString().toAscii().data());
         #endif
            }
        }
    }


    query.clear();
    query.prepare("select CITY from branch where STATE_CODE LIKE :state_code ");
    query.bindValue(":state_code", QString("%%1%").arg(state_code));
    if(!query.exec())
    {
        qDebug() << query.lastError();
        return;
    }
    else
    {
        ui->Person_city_edit_2_comboBox->clear();

        while(query.next())
        {
            if(query.value(0).toString() != NULL)
            {
       #if DEBUG
                printf("CITY == %s\n",(const char *)query.value(0).toString().toAscii().data());
        #endif
                ui->Person_city_edit_2_comboBox->addItem(query.value(0).toString());
            }
        }
    }

    ui->Person_city_edit_2_comboBox->setCurrentIndex(0);
    return;
}

void FPB::on_cmb_state_2_activated(const QString &arg1)
{
    CMisc miscObj;
    QSqlQuery query;
    char state_code[5];
    query.clear();
    // query.prepare("select IFSC, DEFAULT_IIFSCCODE  from imps_mapping where BANK_NAME LIKE :bank ");
    // query.bindValue(":bank", QString("%%1%").arg(ui->cmb_bankList->currentText().trimmed().toAscii().data()));

    query.prepare("select STATE__CODE from state where STATE__NAME LIKE :state_name ");
    query.bindValue(":state_name", QString("%%1%").arg(ui->cmb_state_2->currentText().trimmed().toAscii().data()));
    //select STATE__CODE from state where STATE__NAME='Maharashtra';

    // query.prepare("select STATE__NAME from state");
    if(!query.exec())
    {
        qDebug() << query.lastError();
        return;
    }
    else
    {
        while(query.next())
        {
            if(query.value(0).toString() != NULL)
            {
                memset(state_code,0,sizeof(state_code));
                memcpy(state_code, (const char *)query.value(0).toString().toAscii().data(), strlen((const char *)query.value(0).toString().toAscii().data()));
          #if DEBUG
                printf("state_code = %s\n",state_code);
          #endif
            }
        }
    }

    query.clear();
    query.prepare("select DISTRICT__NAME from district where STATE__CODE LIKE :state_code ");
    query.bindValue(":state_code", QString("%%1%").arg(state_code));
    //     query.prepare("select DISTRICT__NAME from district where STATE__CODE='MH';");
    if(!query.exec())
    {
        qDebug() << query.lastError();
        return;
    }
    else
    {
        ui->cmb_District_2->clear();
        //memset(CustDet.cSearch_IFSC_Code, 0, sizeof(CustDet.cSearch_IFSC_Code));
        // memset(CustDet.cDefault_IFSC_Code, 0, sizeof(CustDet.cDefault_IFSC_Code));
        while(query.next())
        {
            if(query.value(0).toString() != NULL)
            {
                //memcpy(state_code, query.value(0).toString().toAscii().data(), strlen((const char *)query.value(0).toString().toAscii().data()));
                //memcpy(CustDet.cDefault_IFSC_Code, query.value(1).toString().toAscii().data(), strlen((const char *)query.value(1).toString().toAscii().data()));
                ui->cmb_District_2->addItem(query.value(0).toString());
            }
        }
    }
    ui->cmb_District_2->setCurrentIndex(0);
    // ui->cmb_bankList_4->setCurrentIndex(0);
    ui->stackedWidget->setCurrentIndex(20);
    return;

}

void FPB::on_Bene_next_button_clicked()
{
    CMisc miscObj;
    QSqlQuery query;
    char imps_status[5],Default_IFSC_Status[5];
    memset(imps_status, 0, sizeof(imps_status));
    memset(Default_IFSC_Status, 0, sizeof(Default_IFSC_Status));
    query.clear();

    ui->Remark_lineedit->clear();
   // memclear((char *)CustDet.cAddBeneWithouTxn);
    memset(CustDet.cAddBeneWithouTxn, 0, sizeof((char *)CustDet.cAddBeneWithouTxn));

    if(ui->checkBox_button->isChecked()) // check if add bene withut transaction
    {
        strcpy(CustDet.cAddBeneWithouTxn,"Y");
        ui->Next_Otp_Button_2->setVisible(true);
        ui->FINISH_PushButton_Pg34->setVisible(false);
    }
    else
    {
        strcpy(CustDet.cAddBeneWithouTxn,"N");
    }


    memset(CustDet.cAccountNumber, 0, sizeof(CustDet.cAccountNumber));
    memset(CustDet.cBankName, 0, sizeof(CustDet.cBankName));

    strcpy(CustDet.cAccountNumber,ui->Bene_acc_no_edit->text().trimmed().toAscii().data());
    strcpy(CustDet.cBankName,ui->cmb_bankList->currentText().trimmed().toAscii().data());

    if(ui->Bene_acc_no_edit->text().isEmpty() == true)
    {
        uidmessage::instance()->hide_Message();
        miscObj.DisplayMsgBox((char *)"Bene Account Number is compulsory");
        uidmessage::instance()->hide_Message();
        ui->Bene_acc_no_edit->setFocus();
        return;
    }
    if(ui->Bene_acc_no_edit->text().length() < 9)
    {
        uidmessage::instance()->hide_Message();
        miscObj.DisplayMsgBox((char *)"Enter minimum 9 digit account number.");
        uidmessage::instance()->hide_Message();
        ui->Bene_acc_no_edit->setFocus();
        return;
    }
    if(strcmp(CustDet.cBankName,"")==0)
    {
        ui->cmb_bankList->setFocus();
        uidmessage::instance()->hide_Message();
        //        miscObj.DisplayMsgBox("Bene Account Number is compulsory");
        //        uidmessage::instance()->hide_Message();
        return;
    }

    query.clear();
    query.prepare("select IMPS_STATUS, DEFAULT_IFSCSTATUS from imps_mapping where BANK_NAME LIKE :bankname ");
    query.bindValue(":bankname", QString("%%1%").arg(ui->cmb_bankList->currentText().trimmed().toAscii().data()));

    if(!query.exec())
    {
        qDebug() << query.lastError();
        return;
    }
    else
    {
        while(query.next())
        {

            memset(imps_status, 0, sizeof(imps_status));
            memset(Default_IFSC_Status, 0, sizeof(Default_IFSC_Status));
            memcpy(imps_status, query.value(0).toString().toAscii().data(), strlen((const char *)query.value(0).toString().toAscii().data()));
            memcpy(Default_IFSC_Status, query.value(1).toString().toAscii().data(), strlen((const char *)query.value(1).toString().toAscii().data()));

        }
    }
    if(strcmp(imps_status, "1") != 0)
    {
        uidmessage::instance()->hide_Message();
        miscObj.DisplayMsgBox((char *)"IMPS transaction not allowed for this beneficiary bank.");
        uidmessage::instance()->hide_Message();
        memset(imps_status, 0, sizeof(imps_status));
        ui->cmb_bankList->setFocus();
        return;
    }


  //  memclear((char *)Total_BeneVerif_ChargesAmount);
    memset(Total_BeneVerif_ChargesAmount, 0, sizeof((char *)Total_BeneVerif_ChargesAmount));

    if(flag_bitmap != 'R' && flag_bene_verfication_status != 1)
    {
        ui->Bene_Name_edit->clear();
        ui->IFSC_edit->clear();
        ui->Remark_lineedit->clear();
        char temp_arr[12];
        query.clear();
        query.prepare("select IFSC, DEFAULT_IIFSCCODE  from imps_mapping where BANK_NAME LIKE :bank ");
        query.bindValue(":bank", QString("%%1%").arg(ui->cmb_bankList->currentText().trimmed().toAscii().data()));

        if(!query.exec())
        {
            qDebug() << query.lastError();
            return;
        }
        else
        {
            memset(CustDet.cSearch_IFSC_Code, 0, sizeof(CustDet.cSearch_IFSC_Code));
            memset(CustDet.cDefault_IFSC_Code, 0, sizeof(CustDet.cDefault_IFSC_Code));
            memset(temp_arr, 0, sizeof(temp_arr));
            while(query.next())
            {
                if((query.value(0).toString() != NULL) && (query.value(1).toString() != NULL))
                {
                    memcpy(CustDet.cSearch_IFSC_Code, query.value(0).toString().toAscii().data(), strlen((const char *)query.value(0).toString().toAscii().data()));
                    memcpy(temp_arr, query.value(1).toString().toAscii().data(), strlen((const char *)query.value(1).toString().toAscii().data()));
                }
            }
        }

        memset(CustDet.cDefault_IFSC_Code, 0, sizeof(CustDet.cDefault_IFSC_Code));
        strcpy(CustDet.cDefault_IFSC_Code, (char *)IFSC_APPEND(temp_arr, CustDet.cAccountNumber));
 #if DEBUG
        printf("\n\nCustDet.cSearch_IFSC_Code = %s\n\n", CustDet.cSearch_IFSC_Code);
        printf("\n\nCustDet.cDefault_IFSC_Code = %s\n\n", CustDet.cDefault_IFSC_Code);
#endif
        int ret = BeneVerificationCharges();

        if(ret != FUNCTION_SUCCESS && ret != FUNC_OK)
        {
            Flag_BeneVerif_Charges = 0;
            uidmessage::instance()->hide_Message();
            if(ret == SESSION_EXPIRED_FAILED)
                ui->stackedWidget->setCurrentIndex(0);
            else
                ui->stackedWidget->setCurrentIndex(18);
            return;
        }
        else if(ret == FUNCTION_SUCCESS || ret == FUNC_OK)
        {
            Flag_BeneVerif_Charges = 0;
            uidmessage::instance()->hide_Message();
            ui->OTP_Lineedit->clear();

            QString Charges;
            Charges.clear();
            bool decimal;
            int icharges = atoi((char *)beneverifcharges.ucRDPTCharges_RES);
            Charges = QString::number(icharges);

            decimal = Charges.contains('.');
            if(decimal == false)
            {
                Charges = Charges.append(".00");
            }

           // memclear((char *)Total_BeneVerif_ChargesAmount);
            memset(Total_BeneVerif_ChargesAmount, 0, sizeof((char *)Total_BeneVerif_ChargesAmount));

            memcpy(Total_BeneVerif_ChargesAmount, (char *)Charges.trimmed().toAscii().data(), strlen((const char*)Charges.trimmed().toAscii().data()));

            ui->Verify_button->setFont(QFont("SansSerif",10));
            ui->Verify_button->setText("VERIFY @ "+Charges+" RS.");
            //        ui->Verify_button->setEnabled(true);
            //        ui->Verify_button->setStyleSheet("background-color: rgb(85, 85, 127); color: rgb(255, 255, 255)");

        }

        ui->IFSC_edit->setText(CustDet.cDefault_IFSC_Code);

    }
    else
    {
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
            memset(CustDet.cBenificiaryName , 0, sizeof(CustDet.cBenificiaryName ));
            memset(CustDet.cIFSC_Code , 0, sizeof(CustDet.cIFSC_Code ));
            memset(CustDet.cBranchName , 0, sizeof(CustDet.cBranchName ));
            memset(CustDet.cCity , 0, sizeof(CustDet.cCity ));

            memcpy(CustDet.cBenificiaryName, (char *)eBene_beneficiaryName[cur_index], strlen((char *)eBene_beneficiaryName[cur_index]));
            memcpy(CustDet.cIFSC_Code, (char *)eBene_beneficiaryBankIfsc[cur_index], strlen((char *)eBene_beneficiaryBankIfsc[cur_index]));
            memcpy(CustDet.cBranchName, (char *)eBene_beneficiaryBranch[cur_index], strlen((char *)eBene_beneficiaryBranch[cur_index]));
            memcpy(CustDet.cCity, (char *)eBene_beneficiaryBankCity[cur_index], strlen((char *)eBene_beneficiaryBankCity[cur_index]));
 #if DEBUG
            printf("CustDet.cBranchName = %s\n",CustDet.cBranchName);
            printf("CustDet.cCity = %s\n",CustDet.cCity);
#endif
        }
    }

    ui->stackedWidget->setCurrentIndex(19);
    return;

}

void FPB::on_Bene_IFSC_button_clicked()
{
    CMisc miscObj;
#ifdef DEBUG
    printf("CustDet.cBranchName222 = %s\n", CustDet.cBranchName);
#endif

    if(ui->Bene_Name_edit->text().isEmpty() == true)
    {
        uidmessage::instance()->hide_Message();
        miscObj.DisplayMsgBox((char *)"Bene name is compulsory");
        uidmessage::instance()->hide_Message();
        return;
    }


    memset(CustDet.cBenificiaryName , 0, sizeof(CustDet.cBenificiaryName ));
    memset(CustDet.cIFSC_Code , 0, sizeof(CustDet.cIFSC_Code ));
    memset(CustDet.cRemark , 0, sizeof(CustDet.cRemark ));
    memset(CustDet.cBranchName , 0, sizeof(CustDet.cBranchName ));


    strcpy(CustDet.cBenificiaryName ,ui->Bene_Name_edit->text().trimmed().toAscii().data());
    strcpy(CustDet.cIFSC_Code , ui->IFSC_edit->text().trimmed().toAscii().data());
    strcpy(CustDet.cRemark , ui->Remark_lineedit->text().trimmed().toAscii().data());
    strcpy(CustDet.cBranchName , ui->Branch_label->text().trimmed().toAscii().data());



    ui->OTP_Lineedit->setFocus();

    if(memcmp(CustDet.cAddBeneWithouTxn,"Y",1)==0)
    {
#if DEBUG
        qDebug()<<"CustDet.cAddBeneWithouTxn"<<CustDet.cAddBeneWithouTxn;
          printf("CustDet.cBranchName 111= %s\n", CustDet.cBranchName);
#endif
        //        char Message[1000];
        //        memset(Message,0,sizeof(Message));
        //        sprintf(Message,"\n Account Number:   %s\n IFSC:             %s\n Beneficiary Name: %s\n Bank Name:        %s\n Bank Branch:      %s\n Bank City:        %s\n Remark:           %s\n",CustDet.cAccountNumber,CustDet.cIFSC_Code,CustDet.cBenificiaryName,CustDet.cBankName,CustDet.cBranchName,CustDet.cCity,CustDet.cRemark);
        //        // ui->confimationScrollBar->setValue(ui->confimationScrollBar->maximum()); // 16052019
        //        //        ui->lblConfirmation->setText(QString::fromUtf8(Message));
        //        ui->listWidget_2->clear();
        //        ui->listWidget_2->addItem(Message);

        //-------------------------------------------------------------------------------

        char Message1[20][100];
        memset(Message1,0,sizeof(Message1));
        int l =0;
        sprintf(Message1[l],"          ");
        sprintf(Message1[l+1],"Account Number   : %s", (char *)CustDet.cAccountNumber);
        sprintf(Message1[l+2],"IFSC             : %s", (char *)CustDet.cIFSC_Code);
        sprintf(Message1[l+3],"Beneficiary Name : %s",  (char *)CustDet.cBenificiaryName);
        sprintf(Message1[l+4],"Bank Name        : %s",  (char *)CustDet.cBankName);
        sprintf(Message1[l+5],"Bank Branch      : %s",  (char *)CustDet.cBranchName);
        sprintf(Message1[l+6],"Bank City        : %s", (char *)CustDet.cCity);
        sprintf(Message1[l+7],"Remark           : %s", (char *)CustDet.cRemark);
        sprintf(Message1[l+8],"          ");

        ui->listWidget_2->clear();
        for(int k=0;k<9;k++)
            ui->listWidget_2->addItem((char *)Message1[k]);

        //-------------------------------------------------------------------------------



        ui->stackedWidget->setCurrentIndex(34);
    }
    else
    {
        ui->Fund_transfer_amount_edit->clear();
        ui->lbl_TotalAmount->clear();
        ui->lbl_CashContribution->clear();
        ui->Calculate_charge_button->setText("Calculate Charge");
        ui->Calculate_charge_button->setEnabled(true);
        ui->Calculate_charge_button->setStyleSheet("background-color: rgb(85, 0, 127); color: rgb(255, 255, 255)");
        ui->lbl_TotalAmount->setText("     0.00");
        Calculate_impscharges =0;
        ui->Cash_contri_label->setVisible(false);
        ui->lbl_CashContribution->setVisible(false);
        ui->stackedWidget->setCurrentIndex(21);
    }
    return;
}


void FPB::on_Ok_IMPS_Button_clicked()
{
    CMisc miscObj;
    uidmessage::instance()->hide_Message();
    ui->Next_Otp_Button_2->setVisible(true);
    ui->FINISH_PushButton_Pg34->setVisible(false);
    if(strcmp(ui->Fund_transfer_amount_edit->text().toAscii().trimmed().data(), "")==0)
    {
        miscObj.DisplayMsgBox((char *)"Amount is mandatory."); // 30042019 home Need Changes
        uidmessage::instance()->hide_Message();
        Calculate_impscharges = 0;
        ui->Fund_transfer_amount_edit->setFocus();
        return;
    }
    if(Calculate_impscharges != 1)
    {
        miscObj.DisplayMsgBox((char *)"Please calculate charge before proceeding.");
        uidmessage::instance()->hide_Message();
        Calculate_impscharges = 0;
        return;
    }

    //    if(Flag_Again_IFSC_Search != 1)
    //    {

    //    }


    //-------------------------------------------------------------------------------

    char Message1[20][100];
    memset(Message1,0,sizeof(Message1));
    int l =0;
    sprintf(Message1[l],"          ");
    sprintf(Message1[l+1],"Account Number   : %s", (char *)CustDet.cAccountNumber);
    sprintf(Message1[l+2],"IFSC             : %s", (char *)CustDet.cIFSC_Code);
    sprintf(Message1[l+3],"Beneficiary Name : %s",  (char *)CustDet.cBenificiaryName);
    sprintf(Message1[l+4],"Bank Name        : %s",  (char *)CustDet.cBankName);
    sprintf(Message1[l+5],"Bank Branch      : %s",  (char *)CustDet.cBranchName);
    sprintf(Message1[l+6],"Bank City        : %s", (char *)CustDet.cCity);
    sprintf(Message1[l+7],"Amount           : %s", (char *)CustDet.cTxnAmount);
    sprintf(Message1[l+8],"Charges          : %s", (char *)CustDet.cTxnCahrges);
    sprintf(Message1[l+9],"Transaction Type : %s", (char *)CustDet.cTxnType);
    sprintf(Message1[l+10],"Remark           : %s", (char *)CustDet.cRemark);
    sprintf(Message1[l+11],"          ");

    ui->listWidget_2->clear();
    for(int k=0;k<12;k++)
        ui->listWidget_2->addItem((char *)Message1[k]);

    //-------------------------------------------------------------------------------

    OTP_page =3;
    ui->stackedWidget->setCurrentIndex(34);
    return;
}



void FPB::on_BtnNew_Bene_clicked()
{
    if(count_bc_merchant_acc != 0 && flag_bc_merchant_account == 'B')
    {
        CMisc miscObj;
        uidmessage::instance()->hide_Message();
        miscObj.DisplayMsgBox((char *)"More than one bene addition not allowed for this account.");
        uidmessage::instance()->hide_Message();
        return;
    }

    int ret = 0;
    flag_bitmap = 'N';
    flag_bene_verfication_status = 0;
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
    }
    else
        ui->stackedWidget->setCurrentIndex(18);
    return;
}

void FPB::on_Next_Otp_Button_2_clicked()
{
    //before that send OTP request..
    int iRet = 0;

    ui->OTP_Lineedit->clear();
    uidmessage::instance()->hide_Message();
    uidmessage::instance()->Display_BusyMessage("Transaction Request in Progress");
    Resend_flag = 0;
    iRet = Otpimpsnft();
    uidmessage::instance()->hide_Message();
    if(iRet != FUNCTION_SUCCESS)
    {
        if(iRet == SESSION_EXPIRED_FAILED)
        {
            ui->OTP_Lineedit->clear();
            ui->OTP_Lineedit->setFocus();
            ui->stackedWidget->setCurrentIndex(0);
            return;
        }
        ui->stackedWidget->setCurrentIndex(34);
        return;
    }
    else if(iRet == FUNCTION_SUCCESS)
    {
        ui->OTP_Lineedit->clear();
        ui->OTP_Lineedit->setFocus();
        ui->stackedWidget->setCurrentIndex(22);
        return;
    }

}


void FPB::on_person_add_back_button_clicked()
{
    ui->stackedWidget->setCurrentIndex(16);
    return;
}

int FPB::WalkinCust()
{
#if DEBUG
    qDebug()<<"--------------------   WalkinCust   ------------------";
#endif
    HostComm hComm;
    CMisc miscObj;
    int ret = -3;
    char ErrorMsg[100];

    memset(ErrorMsg,0,sizeof(ErrorMsg));
    memset(SearchWalkCust.Commdata_SWK.ucMethodID_req,0, sizeof(SearchWalkCust.Commdata_SWK.ucMethodID_req));
    memset(SearchWalkCust.Commdata_SWK.ucTellerID_req,0, sizeof(SearchWalkCust.Commdata_SWK.ucTellerID_req));
    memset(SearchWalkCust.Commdata_SWK.ucSessionID_req,0, sizeof(SearchWalkCust.Commdata_SWK.ucSessionID_req));
    memset(SearchWalkCust.Commdata_SWK.ucIsEncrypt_req,0, sizeof(SearchWalkCust.Commdata_SWK.ucIsEncrypt_req));
    memset(SearchWalkCust.Commdata_SWK.ucChannelID_req,0, sizeof(SearchWalkCust.Commdata_SWK.ucChannelID_req));
    memset(SearchWalkCust.ucCustomerMob,0, sizeof(SearchWalkCust.ucCustomerMob));
    memset(SearchWalkCust.Commdata_SWK.ucSessionExpiryTime_req,0, sizeof(SearchWalkCust.Commdata_SWK.ucSessionExpiryTime_req));


    iGenerateRequestID((char *)username,(unsigned char *)&SearchWalkCust.Commdata_SWK.ucRequestID_req);
    memcpy(SearchWalkCust.Commdata_SWK.ucMethodID_req,"96",2);
    memcpy((char *)SearchWalkCust.Commdata_SWK.ucSessionID_req,configLoginLeg2.ucSessionId,strlen((char *)configLoginLeg2.ucSessionId));
    memcpy(SearchWalkCust.Commdata_SWK.ucTellerID_req,username,strlen((char *)username));
    memcpy(SearchWalkCust.ucCustomerMob, (char *) FC_MobileNumber, strlen((char *) FC_MobileNumber));
    memcpy(SearchWalkCust.Commdata_SWK.ucIsEncrypt_req,ISENCEYPT,5);
    memcpy((char *)SearchWalkCust.Commdata_SWK.ucSessionExpiryTime_req,(char *)configLoginLeg2.ucSessionExpiryTime,strlen((char *)configLoginLeg2.ucSessionExpiryTime));

    //    uidmessage::instance()->hide_Message();
    //    uidmessage::instance()->Display_BusyMessage("Transaction Request in Progress");

    ret	= makeJSON(15);
    if(ret!=FUNC_OK)
    {
        qDebug()<<"JSON Creation failed";
        return FUNC_ERROR;
    }
    /****************************Encryption function*********************************/
   if(!strcmp((char *)SearchWalkCust.Commdata_SWK.ucIsEncrypt_req,"true"))
    {
       int gSendData_len =strlen((char *)gSendData);
        RequestDetachEncryptAttach((char *)gSendData,1,gSendData_len);
        qDebug()<<"After encrypt function";
    }
    /***************************************************************/

    //      file_download( "http://103.1.112.205:4444//UIService.svc/ProcessRegistrationRequest", (char *)gSendData,"/usr/FINO/FPB/RESP/FINDCUST.TXT",120);



#ifdef UAT_HTTP     // File Download

    hComm.close_connection();
    res = hComm.open_connect(SERVERIP,SERVERPORT);
    if(res != 0)
    {
        qDebug() << "Failed to connect to the Server";
        uidmessage::instance()->hide_Message();
        uidmessage::instance()->DisplayMessage("Server Not Connected");
        qDebug()<<"FUNCTION_FAILED";
        ui->stackedWidget->setCurrentIndex(2);
        return FUNCTION_FAILED;
    }

    uidmessage::instance()->hide_Message();
    uidmessage::instance()->Display_BusyMessage("Transmitting .....");

    int len_gSendData = strlen((char *)gSendData);
    memset(BLEQ_Buff,0,sizeof(BLEQ_Buff));
    memset(oBuff,0,sizeof(oBuff));
    if((hComm.host_2Sendrecv((unsigned char *)gSendData,len_gSendData,( unsigned char*)oBuff,(int *)&BLEQ_len,(int)response_timeout1,0))!=SUCCESS)
    {
        printf("No Response From Server\n");
        hComm.close_connection();
        miscObj.DisplayMsgBox((char *)"Response Time-Out");
        uidmessage::instance()->hide_Message();
        qDebug()<<"***   Response Time-Out   ***";
        ui->stackedWidget->setCurrentIndex(1);
        return FUNCTION_FAILED;
    }
    hComm.close_connection();

#else     // File Download full condition
#if DEBUG
    printf("gSendData = %s\n\n",gSendData);
#endif
    memset(str_url,0,sizeof(str_url));
#ifdef PRODUCTION
    strcpy(str_url,URL_CREATION(IMPS_SEARCH_WALKING_CUST_URL_PROD));
#else
    strcpy(str_url,URL_CREATION((char *)IMPS_SEARCH_WALKING_CUST_URL));
#endif

    int retk = file_download(str_url, (char *)gSendData,"/usr/FINO/FPB/RESP/OUTPUT.TXT",120);
    if(retk!=0)
    {
        qDebug()<<"retk = "<<retk;
        uidmessage::instance()->hide_Message();
        return FUNCTION_FAILED;
    }

#endif
    /****************************Decryption function*********************************/
    if(!strcmp((char *)SearchWalkCust.Commdata_SWK.ucIsEncrypt_req,"true"))
    {
    int    oBuff_len = strlen(oBuff);
#if DEBUG
        qDebug()<<"Length of buffer"<<oBuff_len;
        printf("Buffer%s\n",oBuff);
 #endif
        ResponseDetachEncryptAttach(oBuff,2,oBuff_len);//TEMPORARY
    }
    /**************************************************************************/

    QString WALKBuff = QString::fromLocal8Bit(oBuff);
    bool check = WALKBuff.contains("ResponseCode");
    if(check == false)
    {
        miscObj.DisplayMsgBox((char *)"Invalid Data");
        uidmessage::instance()->hide_Message();
        return FUNCTION_FAILED;
    }

    WALKBuff.remove("\\");
    WALKBuff.remove("{");
    WALKBuff.remove("}");
    WALKBuff.remove('"');
    WALKBuff.remove("[");
    WALKBuff.remove("]");
#if DEBUG
    qDebug()<<"WALKBuff"<<WALKBuff;
#endif
    QStringList query = WALKBuff.split(",");
#if DEBUG
    for(int i=0 ; i < query.length() ; i++)
    {
        qDebug()<<"Query Response"<<query.at(i);
    }
#endif
    QStringList RequestId = query.filter("RequestId");
    QStringList DisplayMessage = query.filter("DisplayMessage");
    QStringList ResponseCode = query.filter("ResponseCode");
    QStringList ResponseMessage = query.filter("ResponseMessage");
    QStringList name = query.filter("name");
    //    uidmessage::instance()->hide_Message();

    QString str ,strToFind,value,value1,value2;
    memset(SearchWalkCust.ucResponseCode_resp, 0, sizeof(SearchWalkCust.ucResponseCode_resp));
    memset(SearchWalkCust.ucResponseMessage_resp, 0, sizeof(SearchWalkCust.ucResponseMessage_resp));
    str = RequestId.at(0);
    strToFind = ":";
    value1 = str.mid(str.indexOf(strToFind)+strToFind.length());
    //  memcpy(OTP.ucResponseCode,value1.trimmed().toAscii().data(), strlen(value1.trimmed().toAscii().data()));


    str.clear();
    str = DisplayMessage.at(0);
    value2 = str.mid(str.indexOf(strToFind)+strToFind.length());

    str.clear();
    str = ResponseCode.at(0);
    QString value3 = str.mid(str.indexOf(strToFind)+strToFind.length());
    memcpy(SearchWalkCust.ucResponseCode_resp, value3.trimmed().toAscii().data(), strlen(value3.trimmed().toAscii().data()));

    str.clear();
    str = ResponseMessage.at(0);
    QString value4 = str.mid(str.indexOf(strToFind)+strToFind.length());
    memcpy(SearchWalkCust.ucResponseMessage_resp, value4.trimmed().toAscii().data(), strlen(value4.trimmed().toAscii().data()));
#if DEBUG
    qDebug()<<"value1 = "<<value1;
    qDebug()<<"value2 = "<<value2;
    qDebug()<<"value3 = "<<value3;
    qDebug()<<"value4 = "<<value4;
#endif
    if(value3 =="0" && value4 == "Search Walkin Customer Success")
    {
//         CUST_TYPE = '\0';
         Flag_IMPS_Walkin = 'D';

        bool check = WALKBuff.contains("name");
        if(check == true)
        {
            str.clear();
            str = name.at(0);
            QString value5 = str.mid(str.indexOf(strToFind)+strToFind.length());
            memcpy(SearchWalkCust.SWCL_name_resp, value5.trimmed().toAscii().data(), strlen(value5.trimmed().toAscii().data()));
 #if DEBUG
            qDebug()<<"value5 = "<<value5;
            //        ui->stackedWidget->setCurrentIndex(22);
            qDebug()<<"*********      Search Walkin Customer Success      *********";
 #endif

        }
        WALKBuff.clear(); // 17072019
        return FUNC_NEXT;
    }
    else
    {
        if(value4 == "Record Not Found !")
        {
  #if DEBUG
            qDebug()<<"*********      In Record Not Found !      *********";
#endif
            Flag_IMPS_Walkin = 'S';
//            if(CUST_TYPE != 'W')
//                CUST_TYPE = 'S';
            WALKBuff.clear(); // 17072019
            return FUNC_NEW_CUST;
        }
        else if(strcmp((const char*)SearchWalkCust.ucResponseMessage_resp,"")!=0)
        {
            WALKBuff.clear(); // 17072019
            Flag_IMPS_Walkin = '\0';
            memset(ErrorMsg, 0, sizeof(ErrorMsg));
            sprintf(ErrorMsg,"%s",(char *)SearchWalkCust.ucResponseMessage_resp);
            miscObj.DisplayMsgBox(ErrorMsg);
            if(strcmp((char *)SearchWalkCust.ucResponseMessage_resp,"Session Expired.")==0)
            {
                uidmessage::instance()->hide_Message();
                return SESSION_EXPIRED_FAILED;
            }
        }
        WALKBuff.clear(); // 17072019
        return FUNCTION_FAILED;
    }

    return FUNCTION_SUCCESS;
}

int FPB::iSetSEARCHWALK(int tagID, unsigned char *value)
{
    switch(tagID)
    {
    case  1:
        strcpy((char *)value,SearchWalkCust.Commdata_SWK.ucRequestID_req);
        break;
    case  2:
        strcpy((char *)value,SearchWalkCust.Commdata_SWK.ucMethodID_req);
        break;
    case  3:
        strcpy((char *)value,SearchWalkCust.Commdata_SWK.ucSessionID_req);
        break;
    case  4:
        strcpy((char *)value,SearchWalkCust.Commdata_SWK.ucTellerID_req);
        break;
    case  5:
        strcpy((char *)value,SearchWalkCust.Commdata_SWK.ucIsEncrypt_req);
        break;
    case  8:
        strcpy((char *)value,(char *)SearchWalkCust.ucCustomerMob);
        break;
    case  6:
        strcpy((char *)value,SearchWalkCust.Commdata_SWK.ucSessionExpiryTime_req);
        break;
    default:
        return FUNC_ERROR;
    }
    return FUNC_OK;
}






void FPB::on_IFSC_ok_button_2_clicked()
{
    CMisc miscObj;
    if(isearchby == 0)
    {
        if(ui->txtIFSC_code_2->text().isEmpty()==true)
        {
            uidmessage::instance()->hide_Message();
            miscObj.DisplayMsgBox((char *)"Please enter IFSC code  ");
            uidmessage::instance()->hide_Message();
            return;
        }
        if(ui->txtIFSC_code_2->text().length() != 11)
        {
            uidmessage::instance()->hide_Message();
            miscObj.DisplayMsgBox((char *)"IFSC code should be 11 digit");
            uidmessage::instance()->hide_Message();
            return;
        }
    }
    else if(isearchby == 1)
    {
        if(strcmp(CustDet.cBranchstate,"")==0 && strcmp(CustDet.cBranchDistrict,"")==0 && strcmp(CustDet.cBankName,"")==0 && strcmp(CustDet.cCity,"")==0)
        {
            BankDetails_Search = 0;
            uidmessage::instance()->hide_Message();
            miscObj.DisplayMsgBox((char *)"Please click on search button before proceeding.");
            uidmessage::instance()->hide_Message();
            return;
        }
    }
    /*   strcpy(CustDet.cBranchstate,ui->cmb_state->currentText().toAscii().data());
    strcpy(CustDet.cBranchDistrict,ui->cmb_District->currentText().toAscii().data());
    strcpy(CustDet.cBBankName,ui->Bank_name_label->text().toAscii().data());
    strcpy(CustDet.cBranchName,ui->cmb_BranchName->currentText().toAscii().data());*/
#ifdef DEBUG
    printf("CustDet.cBranchName = %s\n", CustDet.cBranchName);
#endif

    ui->stackedWidget->setCurrentIndex(19);
}

void FPB::on_Search_IFSC_button_2_clicked()
{
    int iRet = 0;
    QString Bank_code;
    QSqlQuery query;
    CMisc miscObj;
    if(isearchby == 0) // search by IFSC
    {
        if(ui->txtIFSC_code_2->text().isEmpty()==true)
        {
            uidmessage::instance()->hide_Message();
            miscObj.DisplayMsgBox((char *)"Please enter IFSC code  ");
            uidmessage::instance()->hide_Message();
            return;
        }
        if(ui->txtIFSC_code_2->text().length() != 11)
        {
            uidmessage::instance()->hide_Message();
            miscObj.DisplayMsgBox((char *)"IFSC code should be 11 digit");
            uidmessage::instance()->hide_Message();
            return;
        }

        strcpy(CustDet.cIFSC_Code,ui->txtIFSC_code_2->text().trimmed().toAscii().data());
        uidmessage::instance()->hide_Message();
        uidmessage::instance()->Display_BusyMessage("Please wait...");

        iRet = Search_by_IFSC_Code();
        uidmessage::instance()->hide_Message();
        if(iRet != FUNC_OK && iRet != FUNCTION_SUCCESS)
        {
            Flag_Again_IFSC_Search = 0;

            if(iRet == SESSION_EXPIRED_FAILED)
            {
                ui->stackedWidget->setCurrentIndex(0);
                return;
            }
        }
        else
        {
            Flag_Again_IFSC_Search = 1;
#if DEBUG
            printf("byifsc.ucbranchDesc_Resp%s",byifsc.ucbranchDesc_Resp);
#endif
            ui->Bank_code_label->setText(QString::fromUtf8((const char*)byifsc.ucbankCode_Resp));
            ui->Bank_name_label->setText(QString::fromUtf8((const char*)byifsc.ucbankDesc_Resp));
            ui->Branch_label->setText(QString::fromUtf8((const char*)byifsc.ucbranchDesc_Resp));
            ui->City_label->setText(QString::fromUtf8((const char*)byifsc.uccity_Resp));
            ui->label_19->hide();
            ui->Ifsc_label->hide();
            ui->stackedWidget->setCurrentIndex(36);
            return;
        }
        return;
    }
    else if(isearchby == 1) // search by Branch
    {
        memset(CustDet.cBranchstate,0,sizeof(CustDet.cBranchstate));
        memset(CustDet.cBranchDistrict,0,sizeof(CustDet.cBranchDistrict));
        memset(CustDet.cBankName,0,sizeof(CustDet.cBankName));
        memset(CustDet.cCity,0,sizeof(CustDet.cCity));

        strcpy(CustDet.cBranchstate,ui->cmb_state_2->currentText().trimmed().toAscii().data());
        strcpy(CustDet.cBranchDistrict,ui->cmb_District_2->currentText().trimmed().toAscii().data());
        strcpy(CustDet.cBankName,ui->cmb_bankList_4->currentText().trimmed().toAscii().data());

        query.clear();
        query.prepare("SELECT  IFSC FROM imps_mapping WHERE BANK_NAME  = ? ");
        query.addBindValue(ui->cmb_bankList_4->currentText().trimmed());
        if(!query.exec())
            qDebug() << query.lastError();
        else
        {
            while(query.next())
            {
                Bank_code = query.value(0).toString();
#if DEBUG
                qDebug()<<"Bank code "<<Bank_code;
#endif
                strcpy((char *)CustDet.cBankcode,Bank_code.toLatin1().data());

            }
        }


        strcpy(CustDet.cCity,ui->cmb_BranchName_2->currentText().trimmed().toAscii().data());

        if(strcmp(CustDet.cBranchstate,"")==0 && strcmp(CustDet.cBranchDistrict,"")==0 && strcmp(CustDet.cBankName,"")==0 && strcmp(CustDet.cCity,"")==0)
        {
            BankDetails_Search = 0;
            //ui->cBranchstate->setFocus();
            uidmessage::instance()->hide_Message();
            return;
        }

        uidmessage::instance()->hide_Message();
        uidmessage::instance()->Display_BusyMessage("Please wait ...");
#if DEBUG
        printf("CustDet.cBankName = %s\n\n",CustDet.cBankName);
        printf("CustDet.cCity = %s\n\n",CustDet.cCity);
        printf("CustDet.cBranchstate = %s\n\n",CustDet.cBranchstate);
#endif
        iRet = Search_by_Bank_name();
        uidmessage::instance()->hide_Message();
        if(iRet != FUNC_OK && iRet != FUNCTION_SUCCESS)
        {
            Flag_Again_IFSC_Search = 0;

            if(iRet == SESSION_EXPIRED_FAILED)
            {
                ui->stackedWidget->setCurrentIndex(0);
                return;
            }
        }
        else
        {
            Flag_Again_IFSC_Search = 1;
        }
        return;
    }
    return;
}



void FPB::on_Previous_IMPS_Button_clicked()
{
    //    char Message[1000];
    //    memset(Message,0,sizeof(Message));
    //    sprintf(Message,"\n Account Number :   %s\n IFSC :             %s\n Beneficiary Name : %s\n Bank Name :        %s\n Amount :           %s\n Charges :          %s\n Transaction Type : %s\n Remark : %s\n",CustDet.cAccountNumber,CustDet.cIFSC_Code,CustDet.cBenificiaryName,CustDet.cBankName,CustDet.cTxnAmount,CustDet.cTxnCahrges,CustDet.cTxnType,CustDet.cRemark);
    //    ui->confimationScrollBar->setValue(ui->confimationScrollBar->maximum());
    //    ui->lblConfirmation->setText(QString::fromUtf8(Message));
    //    ui->stackedWidget->setCurrentIndex(23);
    //    return;

    ui->stackedWidget->setCurrentIndex(19);
    return;
}



void FPB::on_Next_back_button_36_clicked()
{
    QSqlQuery query;
    query.clear();
    isearchby = 1;
    ui->lblIFSC_Code_2->hide();
    ui->txtIFSC_code_2->hide();
    ui->cmb_state_2->setVisible(true);
    ui->cmb_District_2->setVisible(true);
    ui->Bank_name_label_2->hide();
    ui->cmb_BranchName_2->setVisible(true);
    ui->cmb_bankList_4->setVisible(true);

    ui->Next_back_button_36->setStyleSheet("color: rgb(255, 255, 255); background-color:  rgb(85, 0, 127)");
    ui->Personal_Back_button_26->setStyleSheet("color: rgb(85, 0, 127); background-color:  rgb(255, 255, 255)");

    //    if(!query.exec("select BankName from AEPSBankMaster"))
    //        qDebug() << query.lastError();
    //    else
    //    {
    //        qDebug()<<"In SuccessFull Query Execution :";
    //        //  miscObj.AudioTest((char *)"054");  //Please Select Bank
    //        ui->cmb_bankList_4->clear(); // clear list
    //        ui->cmb_bankList_4->setFont(QFont("Courier",18));
    //        while(query.next())
    //        {
    //            if((query.value(0).toString() != NULL))// && (query.value(1).toString() != NULL))
    //            {
    //                ui->cmb_bankList_4->addItem(query.value(0).toString());
    //            }
    //        }
    //    }

    query.clear();
    if(!query.exec("select BANK_NAME from imps_mapping where IMPS_STATUS = '1'"))
    {
        qDebug() << query.lastError();
        return;
    }
    else
    {

        ui->cmb_bankList_4->clear(); // clear list
        //ui->cmb_bankList_4->setFont(QFont("Courier",18));
        while(query.next())
        {
            if((query.value(0).toString() != NULL))// && (query.value(1).toString() != NULL))
            {
                ui->cmb_bankList_4->addItem(query.value(0).toString());
            }
        }
    }

    query.clear();
    query.prepare("select STATE__NAME from state");
    if(!query.exec())
    {
        qDebug() << query.lastError();
        return;
    }
    else
    {
        //memset(CustDet.cSearch_IFSC_Code, 0, sizeof(CustDet.cSearch_IFSC_Code));
        // memset(CustDet.cDefault_IFSC_Code, 0, sizeof(CustDet.cDefault_IFSC_Code));
        ui->cmb_state_2->clear();
        while(query.next())
        {
            if(query.value(0).toString() != NULL)
            {
                // memcpy(CustDet.cSearch_IFSC_Code, query.value(0).toString().toAscii().data(), strlen((const char *)query.value(0).toString().toAscii().data()));
                //memcpy(CustDet.cDefault_IFSC_Code, query.value(1).toString().toAscii().data(), strlen((const char *)query.value(1).toString().toAscii().data()));
                ui->cmb_state_2->addItem(query.value(0).toString());
            }
        }
    }

    //    query.prepare("select BANK_NAME from imps_mapping where IMPS_STATUS = '1'");
    //    if(!query.exec())
    //    {
    //        qDebug() << query.lastError();
    //        return;
    //    }
    //    else
    //    {
    //        ui->cmb_state_2->clear(); // clear list
    //        ui->cmb_state_2->setFont(QFont("Courier",18));
    //        while(query.next())
    //        {
    //            if((query.value(0).toString() != NULL))
    //            {
    //                ui->cmb_state_2->addItem(query.value(0).toString());
    //            }
    //        }
    //    }
    ui->cmb_state_2->setCurrentIndex(0);
    ui->cmb_bankList_4->setCurrentIndex(0);
    ui->cmb_District_2->clear(); // 13092019 C
    ui->cmb_BranchName_2->clear(); // 13092019 C
    // ui->cmb_bankList_4->setCurrentIndex(0);
    ui->stackedWidget->setCurrentIndex(20);
}

void FPB::on_Bene_Search_Button_clicked()
{
    ui->cmb_state_2->hide();
    ui->cmb_District_2->hide();
    ui->Bank_name_label_2->hide();
    ui->cmb_BranchName_2->hide();
    ui->cmb_bankList_4->hide();
    ui->lblIFSC_Code_2->setGeometry(20,90,281,25);
    ui->txtIFSC_code_2->setText((char*)ui->IFSC_edit->text().toAscii().trimmed().data());
    // ui->txtIFSC_code_2->setText(CustDet.cDefault_IFSC_Code);
    ui->Personal_Back_button_26->setStyleSheet("color: rgb(255, 255, 255); background-color:  rgb(85, 0, 127)"); // 13092019 C
    ui->Next_back_button_36->setStyleSheet("color: rgb(85, 0, 127); background-color:  rgb(255, 255, 255)"); // 13092019 C
    ui->stackedWidget->setCurrentIndex(20);
    return;
}
