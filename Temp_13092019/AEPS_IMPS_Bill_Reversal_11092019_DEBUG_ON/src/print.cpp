#include "print.h"
#include "ui_FPB.h"
#include "json.h"
#include "FPB.h"
#include "Login.h"
#include "displaymessage.h"
#include "fino_print.h"
#include "impscommon.h" // 02052019

Printfpb ConfigPrint;
extern Transaction Txn;
extern sCustDetails CustDet; // 02052019
extern int Reversal_type;

int FPB::print_aes_transaction()
{
    uidmessage::instance()->hide_Message();
    uidmessage::instance()->Display_BusyMessage("Printing....");
    char szDate[16], szTime[16];
    char prn_buf[256];
    char Space[] = "                             ";
    char Line[]  ="----------------------------------------";
    CMisc miscObj;
    FinoPrint objPrn;

    memset(szDate, 0, sizeof(szDate));
    memset(szTime, 0, sizeof(szTime));
    GetLocalTime(szDate, szTime);
    if(prn_open() < 0)   // check for printer platon
    {
        miscObj.DisplayMsgBox((char *)"Printer Open Failed...");
        return -1;
    }

    if(prn_paperstatus() != 0)  // check for paper error
    {
        gl11_lcdbklt_on();
        miscObj.DisplayMsgBox((char *)"Please Put Paper!");
        prn_close();
        return -1;
    }

    if(objPrn.PrintLogo(1) < 0)
    {
        qDebug() << "LOGO Not Printed";
        prn_write_text((unsigned char *)"     No Logo Available", strlen("     No Logo Available"), 2);
    }

    //--------------------------------header start---------------------------------------------//
    if(prn_txt(Line, 0) < 0)   // print header start line
        return -1;
    if(Txn.Txntype == AEPS_BAL)
    {
        if(prn_txt("   BALANCE ENQUIRY  ", 2) < 0)   // print header start line
            return -1;
    }
    if(Txn.Txntype == AEPS_WITH)
    {
        if(prn_txt("    CASH WITHDRAWAL  ", 2) < 0)   // print header start line
            return -1;
    }
    if(prn_txt(Line, 0) < 0)   // print header start line
        return -1;
    memset(prn_buf, 0, sizeof(prn_buf));
    sprintf(prn_buf, "Date: %s        Time: %s", szDate, szTime);  // print date & time
    if(prn_txt(prn_buf, 0) < 0)
        return -1;
    memset(prn_buf, 0, sizeof(prn_buf));
    sprintf(prn_buf, "BC NAME : %s",(char *)ConfigPrint.ucBCName);  // B C name print
    if(prn_txt(prn_buf, 0) < 0)
        return FUNCTION_FAILED;
    memset(prn_buf, 0, sizeof(prn_buf));
    sprintf(prn_buf, "AGENT ID : %s",(char *)ConfigPrint.ucAgentID);  // B C name print
    if(prn_txt(prn_buf, 0) < 0)
        return FUNCTION_FAILED;
    memset(prn_buf, 0, sizeof(prn_buf));
    sprintf(prn_buf, "BC Location : %s",ConfigPrint.ucBCLocation);  // B C name print
    if(prn_txt(prn_buf, 0) < 0)
        return FUNCTION_FAILED;
    memset(prn_buf, 0, sizeof(prn_buf));
    sprintf(prn_buf, "TERMINAL ID : %s",(char *)ConfigPrint.ucTerminalID);  // B C name print
    if(prn_txt(prn_buf, 0) < 0)
        return FUNCTION_FAILED;

    memset(prn_buf, 0, sizeof(prn_buf));
    sprintf(prn_buf, "CUSTOMER AADHAAR NO : %s",(char *)ConfigPrint.ucCustAadharNo);  // B C name print
    if(prn_txt(prn_buf, 0) < 0)
        return FUNCTION_FAILED;

    memset(prn_buf, 0, sizeof(prn_buf));
    sprintf(prn_buf, "CUSTOMER NAME : %s",(char *)ConfigPrint.ucCustName);  // B C name print
    if(prn_txt(prn_buf, 0) < 0)
        return FUNCTION_FAILED;

    memset(prn_buf, 0, sizeof(prn_buf));
    sprintf(prn_buf, "STAN : %s",(char *)ConfigPrint.ucSTAN);  // B C name print
    if(prn_txt(prn_buf, 0) < 0)
        return FUNCTION_FAILED;

    memset(prn_buf, 0, sizeof(prn_buf));
    sprintf(prn_buf, "RRN : %s",(char *)ConfigPrint.ucRRN);  // B C name print
    if(prn_txt(prn_buf, 0) < 0)
        return FUNCTION_FAILED;

    memset(prn_buf, 0, sizeof(prn_buf));
    sprintf(prn_buf, "UIDAI AUTH CODE : %s",(char *)ConfigPrint.ucAUTHCode);  // B C name print
    if(prn_txt(prn_buf, 0) < 0)
        return FUNCTION_FAILED;


    memset(prn_buf, 0, sizeof(prn_buf));
    sprintf(prn_buf, "TRANSACTION STATUS : %s",(char *)ConfigPrint.ucTranStatus);  // B C name print
    if(prn_txt(prn_buf, 0) < 0)
        return FUNCTION_FAILED;



    //    printf("ConfigPrint.ucACBalance p = %s\n",ConfigPrint.ucACBalance);
    //    printf("ConfigPrint.ucBCLocationp p = %s\n",ConfigPrint.ucBCLocation);
    memset(prn_buf, 0, sizeof(prn_buf));
    sprintf(prn_buf, "A/C Balance : %s",(char *)ConfigPrint.ucACBalance);  // B C name print
    if(prn_txt(prn_buf, 0) < 0)
        return FUNCTION_FAILED;
    if(Txn.Txntype == AEPS_WITH)
    {
        memset(prn_buf, 0, sizeof(prn_buf));
        //        printf("Txn.Amount %s\n",Txn.Amount);
        //        printf("ConfigPrint.ucAmount == %s\n",ConfigPrint.ucAmount);
        sprintf(prn_buf, "TRANSACTION AMOUNT : %s",(char *)ConfigPrint.ucAmount);  // B C name print
        if(prn_txt(prn_buf, 0) < 0)
            return FUNCTION_FAILED;
    }

    if(prn_txt(Space, 0) < 0)   // print header end line
        return -1;
    if(prn_txt("Thank You. Visit soon !  ", 0) < 0)   // print header start line
        return -1;
    if(prn_txt(Space, 0) < 0)   // print header end line
        return -1;
    memset(prn_buf, 0, sizeof(prn_buf));
    sprintf(prn_buf, "            FPB VT Ver %s\n",APP_VERSION);  // B C name print
    if(prn_txt(prn_buf, 0) < 0)
        return FUNCTION_FAILED;
    if(prn_txt("-------------End Of Reciept-------------", 0) < 0)   // print header start line
        return -1;
    prn_write_text((unsigned char*)"\n\n\n\n\n\n",6,1);
    prn_close();

    memset(&ConfigPrint,0,sizeof(ConfigPrint));
    uidmessage::instance()->hide_Message();
    return FUNC_OK;

}

//int FPB::print_aes_mini_stat(Transaction *Txn,int iCount)
//{
//    char szDate[16], szTime[16];
//    char prn_buf[256];
//    char Space[] = "                             ";
//    char Line[]  ="------------------------------";
//    CMisc miscObj;
//    FinoPrint objPrn;
//    char cTemp[20];

//    memset(szDate, 0, sizeof(szDate));
//    memset(szTime, 0, sizeof(szTime));
//    GetLocalTime(szDate, szTime);

//    if(prn_open() < 0)   // check for printer platon
//    {
//        miscObj.DisplayMsgBox((char *)"Printer Open Failed...");
//        return -1;
//    }

//    if(prn_paperstatus() != 0)  // check for paper error
//    {
//        gl11_lcdbklt_on();
//        miscObj.DisplayMsgBox((char *)"Please Put Paper!");
//        prn_close();
//        return -1;
//    }


//    if(objPrn.PrintLogo(50) < 0)
//    {
//        qDebug() << "LOGO Not Printed";
//        prn_write_text((unsigned char *)"     No Logo Available", strlen("     No Logo Available"), 2);
//    }

//    //--------------------------------header start---------------------------------------------//
//    if(prn_txt(Line, 0) < 0)   // print header start line
//        return -1;

//    if(Txn->Txntype == AEPS_BAL)
//    {
//        if(prn_txt("   MINI STATEMENT   ", 2) < 0)   // print header start line
//            return -1;
//    }


//    if(prn_txt(Line, 0) < 0)   // print header start line
//        return -1;

//    memset(prn_buf, 0, sizeof(prn_buf));
//    sprintf(prn_buf, "Date: %s        Time: %s", szDate, szTime);  // print date & time
//    if(prn_txt(prn_buf, 0) < 0)
//        return -1;

//    memset(prn_buf, 0, sizeof(prn_buf));
//    sprintf(prn_buf, "BC NAME : %s",(char *)ConfigPrint.ucBCName);  // B C name print
//    if(prn_txt(prn_buf, 0) < 0)
//        return FUNCTION_FAILED;

//    memset(prn_buf, 0, sizeof(prn_buf));
//    sprintf(prn_buf, "AGENT ID : %s",(char *)ConfigPrint.ucAgentID);  // B C name print
//    if(prn_txt(prn_buf, 0) < 0)
//        return FUNCTION_FAILED;

//    memset(prn_buf, 0, sizeof(prn_buf));
//    sprintf(prn_buf, "BC Location : %s",ConfigPrint.ucBCLocation);  // B C name print
//    if(prn_txt(prn_buf, 0) < 0)
//        return FUNCTION_FAILED;

//    memset(prn_buf, 0, sizeof(prn_buf));
//    sprintf(prn_buf, "TERMINAL ID : %s",(char *)ConfigPrint.ucTerminalID);  // B C name print
//    if(prn_txt(prn_buf, 0) < 0)
//        return FUNCTION_FAILED;

//    memset(prn_buf, 0, sizeof(prn_buf));
//    sprintf(prn_buf, "CUSTOMER AADHAAR NO : %s",(char *)ConfigPrint.ucCustAadharNo);  // B C name print
//    if(prn_txt(prn_buf, 0) < 0)
//        return FUNCTION_FAILED;

//    memset(prn_buf, 0, sizeof(prn_buf));
//    sprintf(prn_buf, "CUSTOMER NAME : %s",(char *)ConfigPrint.ucCustName);  // B C name print
//    if(prn_txt(prn_buf, 0) < 0)
//        return FUNCTION_FAILED;

//    memset(prn_buf, 0, sizeof(prn_buf));
//    sprintf(prn_buf, "STAN : %s",(char *)ConfigPrint.ucSTAN);  // B C name print
//    if(prn_txt(prn_buf, 0) < 0)
//        return FUNCTION_FAILED;

//    memset(prn_buf, 0, sizeof(prn_buf));
//    sprintf(prn_buf, "RRN : %s",(char *)ConfigPrint.ucRRN);  // B C name print
//    if(prn_txt(prn_buf, 0) < 0)
//        return FUNCTION_FAILED;

//    memset(prn_buf, 0, sizeof(prn_buf));
//    sprintf(prn_buf, "UIDAI AUTH CODE : %s",(char *)ConfigPrint.ucAUTHCode);  // B C name print
//    if(prn_txt(prn_buf, 0) < 0)
//        return FUNCTION_FAILED;


//    memset(prn_buf, 0, sizeof(prn_buf));
//    sprintf(prn_buf, "TRANSACTION STATUS : %s",(char *)ConfigPrint.ucTranStatus);  // B C name print
//    if(prn_txt(prn_buf, 0) < 0)
//        return FUNCTION_FAILED;

//    memset(prn_buf, 0, sizeof(prn_buf));
//    sprintf(prn_buf, "A/C Balance : %s",(char *)ConfigPrint.ucACBalance);  // B C name print
//    if(prn_txt(prn_buf, 0) < 0)
//        return FUNCTION_FAILED;
//    if(Txn->Txntype == AEPS_BAL)
//    {
//        memset(prn_buf, 0, sizeof(prn_buf));
//        sprintf(prn_buf, "TRANSACTION AMOUNT : ",(char *)ConfigPrint.ucAmount);  // B C name print
//        if(prn_txt(prn_buf, 0) < 0)
//            return FUNCTION_FAILED;
//    }

//    if(Txn->Txntype == AEPS_MINI)
//    {

//        memset(prn_buf, 0, sizeof(prn_buf));
//        strcpy(prn_buf, "\n REF  D/C   AMT    DATE   MOT    TYPE");
//        prn_txt(prn_buf, 1);
//        int i =0;
//        while ( i < iCount) // original code
//        {
//            qDebug()<<"MINI STATEMENT LOOP";

//            memset(prn_buf, 0, sizeof(prn_buf));


//          //  if ( i == 0 )
//              //  sprintf(prn_buf, "\n %s   %s   %s      %s     %s     %s",(char*)dsply_data[i].AEPS_MINI.RefNo,(char *)dsply_data[i].AEPS_MINI.DebitCredit,(char *)dsply_data[i].AEPS_MINI.Amount,(char *)dsply_data[i].AEPS_MINI.Date,(char *)AEPS_MINI.ModeOfTxn,(char *)dsply_data[i].AEPS_MINI.Type);
//         //   else
//               // sprintf(prn_buf, "\n %s   %s   %s      %s     %s     %s",(char*)dsply_data[i].AEPS_MINI.RefNo,(char *)dsply_data[i].AEPS_MINI.DebitCredit,(char *)dsply_data[i].AEPS_MINI.Amount,(char *)dsply_data[i].AEPS_MINI.Date,(char *)AEPS_MINI.ModeOfTxn,(char *)dsply_data[i].AEPS_MINI.Type);

//            if(prn_txt(prn_buf, 0) < 0)
//                return FUNCTION_FAILED;

//            i++;icount

//        }
//    }


//    if(prn_txt(Space, 0) < 0)   // print header end line
//        return -1;
//    if(prn_txt("     Thank You. Visit soon !  ", 2) < 0)   // print header start line
//        return -1;
//    if(prn_txt("-------------End Of Reciept-------------", 2) < 0)   // print header start line
//        return -1;

//    prn_write_text((unsigned char*)"\n\n\n\n\n\n",6,1);
//    prn_close();

//    memset(&ConfigPrint,0,sizeof(ConfigPrint));

//}

int FPB::GetLocalTime(char *szDate, char *szTime)
{
    struct tm *tt;
    time_t currentTime;

    time(&currentTime);
    tt=localtime(&currentTime);


    sprintf(szTime,"%02d:%02d:%02d", (tt)->tm_hour, (tt)->tm_min, (tt)->tm_sec);
    sprintf(szDate,"%02d/%02d/%04d", (tt)->tm_mday, (tt)->tm_mon+1, (tt)->tm_year+1900);

    return 0;
}

int FPB::idisplayTxnDetailes(char display[][50],int icount)
{
    int i=0;
    ui->listWidget_Issuer_2->clear(); // clear list
    ui->listWidget_Issuer_2->setFont(QFont("Courier",14));

    ui->label_Txn_Menu_2->setFont(QFont("SansSerif",20));
    ui->label_Txn_Menu_2->setFont(QFont("Bold"));


    if(Txn.Txntype == AEPS_LTTS)
    {
        //ui->label_Txn_Menu_2->setText("             LAST TEN TRANSACTIONS      ");
        ui->Button_display->setText(" OK ");
    }
    if(Txn.Txntype == AEPS_LTS)
    {
        //ui->label_Txn_Menu_2->setText("              LAST TRANSACTION      ");
        ui->Button_display->setText(" OK ");
    }


    for(i=0;i<(9*icount);i++)
    {
#if DEBUG
        printf("display    =====    %s\n",display[i]);
#endif
        ui->listWidget_Issuer_2->addItem((char *)display[i]);
    }

    //    uidmessage::instance()->hide_Message();
    ui->stackedWidget->setCurrentIndex(11);
}

int FPB::IMPS_AddBenePrint()
{
    uidmessage::instance()->hide_Message();
    uidmessage::instance()->Display_BusyMessage("Printing....");
    char szDate[16], szTime[16];
    char prn_buf[256];
    char Space[] = "                             ";
    char Line[]  ="----------------------------------------";
    CMisc miscObj;
    FinoPrint objPrn;

    memset(szDate, 0, sizeof(szDate));
    memset(szTime, 0, sizeof(szTime));
    GetLocalTime(szDate, szTime);
    if(prn_open() < 0)   // check for printer platon
    {
        miscObj.DisplayMsgBox((char *)"Printer Open Failed...");
        return -1;
    }
    if(prn_paperstatus() != 0)  // check for paper error
    {
        gl11_lcdbklt_on();
        miscObj.DisplayMsgBox((char *)"Please Put Paper!");
        prn_close();
        return -1;
    }


    if(objPrn.PrintLogo(1) < 0) // 02052019
    {
        qDebug() << "LOGO Not Printed";
        prn_write_text((unsigned char *)"     No Logo Available", strlen("     No Logo Available"), 2);
    }

    //--------------------------------header start---------------------------------------------//
    if(prn_txt(Line, 0) < 0)   // print header start line
        return -1;

    if(prn_txt("    Fund Transfer  ", 2) < 0)   // print header start line
        return -1;

    if(prn_txt(Line, 0) < 0)   // print header start line
        return -1;

    memset(prn_buf, 0, sizeof(prn_buf));
    sprintf(prn_buf, "Date: %s        Time: %s", szDate, szTime);  // print date & time
    if(prn_txt(prn_buf, 0) < 0)
        return -1;

    memset(prn_buf, 0, sizeof(prn_buf));
    sprintf(prn_buf, "Customer Name : %s",(char *)ConfigPrint.ucCustName);  // B C name print
    if(prn_txt(prn_buf, 0) < 0)
        return FUNCTION_FAILED;

    memset(prn_buf, 0, sizeof(prn_buf));
    sprintf(prn_buf, "Customer Mobile No. : %s",(char *)ConfigPrint.ucCustMobNo);  // B C name print
    if(prn_txt(prn_buf, 0) < 0)
        return FUNCTION_FAILED;

    memset(prn_buf, 0, sizeof(prn_buf));
    sprintf(prn_buf, "Beneficiary Account No. : %s",(char *)ConfigPrint.ucBeneAccNo);  // B C name print
    if(prn_txt(prn_buf, 0) < 0)
        return FUNCTION_FAILED;

    memset(prn_buf, 0, sizeof(prn_buf));
    sprintf(prn_buf, "Beneficiary Name : %s",(char *)ConfigPrint.ucBeneName);  // B C name print
    if(prn_txt(prn_buf, 0) < 0)
        return FUNCTION_FAILED;

    if((memcmp(CustDet.cAddBeneWithouTxn,"Y",1)!=0)) // 02052019
    {

        memset(prn_buf, 0, sizeof(prn_buf));
        sprintf(prn_buf, "Amount : Rs %s",(char *)ConfigPrint.ucAmount);
        if(prn_txt(prn_buf, 0) < 0)
            return FUNCTION_FAILED;

        memset(prn_buf, 0, sizeof(prn_buf));
        sprintf(prn_buf, "Charges : Rs %s",(char *)ConfigPrint.ucCharges);
        if(prn_txt(prn_buf, 0) < 0)
            return FUNCTION_FAILED;

        memset(prn_buf, 0, sizeof(prn_buf));
        sprintf(prn_buf, "Transaction Type : %s",(char *)ConfigPrint.uctxntype);
        if(prn_txt(prn_buf, 0) < 0)
            return FUNCTION_FAILED;

    }

    memset(prn_buf, 0, sizeof(prn_buf));
    sprintf(prn_buf, "Total Amount : Rs %s",(char *)ConfigPrint.ucTotalAmt);  // B C name print
    if(prn_txt(prn_buf, 0) < 0)
        return FUNCTION_FAILED;

    if((memcmp(CustDet.cAddBeneWithouTxn,"Y",1)!=0)) // 02052019
    {
        memset(prn_buf, 0, sizeof(prn_buf)); // 02052019
        sprintf(prn_buf, "Ref No. : %s",(char *)ConfigPrint.ucRefNo); // 02052019
        if(prn_txt(prn_buf, 0) < 0) // 02052019
            return FUNCTION_FAILED; // 02052019
    }

    if(prn_txt(Space, 0) < 0)   // print header end line
        return -1;
    if(prn_txt("Thank You Visit soon !  ", 0) < 0)   // print header start line
        return -1;
    if(prn_txt(Space, 0) < 0)   // print header end line
        return -1;
    memset(prn_buf, 0, sizeof(prn_buf));
    sprintf(prn_buf, "            FPB VT Ver %s\n",APP_VERSION);  // B C name print
    if(prn_txt(prn_buf, 0) < 0)
        return FUNCTION_FAILED;
    if(prn_txt("-------------End Of Reciept-------------", 0) < 0)   // print header start line
        return -1;
    prn_write_text((unsigned char*)"\n\n\n\n\n\n",6,1);
    prn_close();

    memset(&ConfigPrint,0,sizeof(ConfigPrint));
    uidmessage::instance()->hide_Message();
    return FUNC_OK;
}

void FPB::on_Print_Receipt_Button_Pg34_clicked()
{
    CMisc miscObj;
    if(IMPS_AddBenePrint() == FUNC_OK)
    {
        qDebug()<<" Print SUCCESS";
    }
    else
    {
        qDebug()<<" Print FAILED";
        miscObj.DisplayMsgBox((char *)"Print Slip Failed");
        uidmessage::instance()->hide_Message();
        //  return FUNCTION_FAILED;
    }

    if(Flag_IMPS_Walkin == 'S' || CUST_TYPE == 'W' )
    {
        CUST_TYPE = '\0';
        Flag_IMPS_Walkin = '\0';
        ui->stackedWidget->setCurrentIndex(12);
        return;
    }
    else
    {
        ui->stackedWidget->setCurrentIndex(15);
        return;
    }

  //  ui->stackedWidget->setCurrentIndex(15);
    return;

}


int FPB::prn_txt(char *prn_buf, int iFontSize)
{
    //qDebug()<<prn_buf;

    int ret = 0;

    ret = prn_write_text((unsigned char *)prn_buf, strlen(prn_buf), iFontSize);

    return ret;
}

char *ltrim(char *s)

{
    while(isspace(*s)) s++;

    return s;

}

char *rtrim(char *s)

{
    char* back = s + strlen(s);

    while(isspace(*--back));

    *(back+1) = '\0';

    return s;

}
char *trimStr(char *s)

{

    return rtrim(ltrim(s));
}

int FPB::Reversal_Print()
{
    uidmessage::instance()->hide_Message();
    uidmessage::instance()->Display_BusyMessage("Printing....");
    char szDate[16], szTime[16];
    char prn_buf[256];
    char Space[] = "                             ";
    char Line[]  ="----------------------------------------";
    CMisc miscObj;
    FinoPrint objPrn;

    memset(szDate, 0, sizeof(szDate));
    memset(szTime, 0, sizeof(szTime));
    GetLocalTime(szDate, szTime);
    if(prn_open() < 0)   // check for printer platon
    {
        miscObj.DisplayMsgBox((char *)"Printer Open Failed...");
        return -1;
    }
    if(prn_paperstatus() != 0)  // check for paper error
    {
        gl11_lcdbklt_on();
        miscObj.DisplayMsgBox((char *)"Please Put Paper!");
        prn_close();
        return -1;
    }


        if(objPrn.PrintLogo(1) < 0) // 02052019
        {
            qDebug() << "LOGO Not Printed";
            prn_write_text((unsigned char *)"     No Logo Available", strlen("     No Logo Available"), 2);
        }

    //--------------------------------header start---------------------------------------------//
    if(prn_txt(Line, 0) < 0)   // print header start line
        return -1;

        if(Reversal_type == 1)
        {
            if(prn_txt("    Fund Transfer  ", 2) < 0)   // print header start line
            return -1;
        }
        else if(Reversal_type == 0)
        {
            if(prn_txt("        REFUND  ", 2) < 0)   // print header start line
            return -1;
        }

    if(prn_txt(Line, 0) < 0)   // print header start line
        return -1;

    memset(prn_buf, 0, sizeof(prn_buf));
    sprintf(prn_buf, "Date: %s        Time: %s", szDate, szTime);  // print date & time
    if(prn_txt(prn_buf, 0) < 0)
        return -1;

    memset(prn_buf, 0, sizeof(prn_buf));
    sprintf(prn_buf, "Customer Name : %s",(char *)ConfigPrint.ucCustName);  // B C name print
    if(prn_txt(prn_buf, 0) < 0)
        return FUNCTION_FAILED;

    memset(prn_buf, 0, sizeof(prn_buf));
    sprintf(prn_buf, "Customer Mobile No. : %s",(char *)ConfigPrint.ucCustMobNo);  // B C name print
    if(prn_txt(prn_buf, 0) < 0)
        return FUNCTION_FAILED;

    if(Reversal_type == 1)
    {
        memset(prn_buf, 0, sizeof(prn_buf));
        sprintf(prn_buf, "Amount : Rs %s",(char *)ConfigPrint.ucAmount);
        if(prn_txt(prn_buf, 0) < 0)
            return FUNCTION_FAILED;

        memset(prn_buf, 0, sizeof(prn_buf));
        sprintf(prn_buf, "Charges : Rs %s",(char *)ConfigPrint.ucCharges);
        if(prn_txt(prn_buf, 0) < 0)
            return FUNCTION_FAILED;

        memset(prn_buf, 0, sizeof(prn_buf));
        sprintf(prn_buf, "Transaction Type : %s",(char *)ConfigPrint.uctxntype);
        if(prn_txt(prn_buf, 0) < 0)
            return FUNCTION_FAILED;

        memset(prn_buf, 0, sizeof(prn_buf));
        sprintf(prn_buf, "Total Amount : Rs %s",(char *)ConfigPrint.ucTotalAmt);
        if(prn_txt(prn_buf, 0) < 0)
            return FUNCTION_FAILED;
    }
    else if(Reversal_type == 0)
    {
        memset(prn_buf, 0, sizeof(prn_buf));
        sprintf(prn_buf, "Amount : Rs %s",(char *)ConfigPrint.ucTotalAmt);
        if(prn_txt(prn_buf, 0) < 0)
            return FUNCTION_FAILED;

    }

    memset(prn_buf, 0, sizeof(prn_buf)); // 02052019
    sprintf(prn_buf, "Transaction ID : %s",(char *)ConfigPrint.ucRefNo); // 02052019
    if(prn_txt(prn_buf, 0) < 0) // 02052019
        return FUNCTION_FAILED; // 02052019


    if(prn_txt(Space, 0) < 0)   // print header end line
        return -1;
    if(prn_txt("Thank You Visit soon !  ", 0) < 0)   // print header start line
        return -1;
    if(prn_txt(Space, 0) < 0)   // print header end line
        return -1;
    memset(prn_buf, 0, sizeof(prn_buf));
    sprintf(prn_buf, "            FPB VT Ver %s\n",APP_VERSION);  // B C name print
    if(prn_txt(prn_buf, 0) < 0)
        return FUNCTION_FAILED;
    if(prn_txt("-------------End Of Reciept-------------", 0) < 0)   // print header start line
        return -1;
    prn_write_text((unsigned char*)"\n\n\n\n\n\n",6,1);
    prn_close();

    memset(&ConfigPrint,0,sizeof(ConfigPrint));
    uidmessage::instance()->hide_Message();
    return FUNC_OK;
}


int FPB::Billpayment_Print()
{
    uidmessage::instance()->hide_Message();
    uidmessage::instance()->Display_BusyMessage("Printing....");
    char szDate[16], szTime[16];
    char prn_buf[256];
    char Space[] = "                             ";
    char Line[]  ="----------------------------------------";
    CMisc miscObj;
    FinoPrint objPrn;

    memset(szDate, 0, sizeof(szDate));
    memset(szTime, 0, sizeof(szTime));
    GetLocalTime(szDate, szTime);
    if(prn_open() < 0)   // check for printer platon
    {
        miscObj.DisplayMsgBox((char *)"Printer Open Failed...");
        return -1;
    }
    if(prn_paperstatus() != 0)  // check for paper error
    {
        gl11_lcdbklt_on();
        miscObj.DisplayMsgBox((char *)"Please Put Paper!");
        prn_close();
        return -1;
    }


        if(objPrn.PrintLogo(1) < 0) // 02052019
        {
            qDebug() << "LOGO Not Printed";
            prn_write_text((unsigned char *)"     No Logo Available", strlen("     No Logo Available"), 2);
        }

    //--------------------------------header start---------------------------------------------//
    if(prn_txt(Line, 0) < 0)   // print header start line
        return -1;

    if(prn_txt("    Bill Payment  ", 2) < 0)   // print header start line
        return -1;

    if(prn_txt(Line, 0) < 0)   // print header start line
        return -1;

    memset(prn_buf, 0, sizeof(prn_buf));
    sprintf(prn_buf, "Date: %s        Time: %s", szDate, szTime);
    if(prn_txt(prn_buf, 0) < 0)
        return -1;

    memset(prn_buf, 0, sizeof(prn_buf));
    sprintf(prn_buf, "Customer Name : %s",(char *)ConfigPrint.ucCustName);
    if(prn_txt(prn_buf, 0) < 0)
        return FUNCTION_FAILED;

    memset(prn_buf, 0, sizeof(prn_buf));
    sprintf(prn_buf, "Customer Mobile No. : %s",(char *)ConfigPrint.ucCustMobNo);
    if(prn_txt(prn_buf, 0) < 0)
        return FUNCTION_FAILED;

//    memset(prn_buf, 0, sizeof(prn_buf));
//    sprintf(prn_buf, "Customer Name : %s",(char *)ConfigPrint.ucCustName);
//    if(prn_txt(prn_buf, 0) < 0)
//        return FUNCTION_FAILED;






    memset(prn_buf, 0, sizeof(prn_buf));
    sprintf(prn_buf, "Service Provider : %s",(char *)ConfigPrint.ucServiceProvider);
    if(prn_txt(prn_buf, 0) < 0)
        return FUNCTION_FAILED;

    memset(prn_buf, 0, sizeof(prn_buf));
    sprintf(prn_buf, "Consumer number : %s",(char *)ConfigPrint.ucConsumerId);
    if(prn_txt(prn_buf, 0) < 0)
        return FUNCTION_FAILED;

    memset(prn_buf, 0, sizeof(prn_buf));
    sprintf(prn_buf, "Bill Amount :Rs %s",(char *)ConfigPrint.ucbillamt);
    if(prn_txt(prn_buf, 0) < 0)
        return FUNCTION_FAILED;

    memset(prn_buf, 0, sizeof(prn_buf));
    sprintf(prn_buf, "Customer Convenience Fee :Rs %s",(char *)ConfigPrint.ucCharges);
    if(prn_txt(prn_buf, 0) < 0)
        return FUNCTION_FAILED;

    memset(prn_buf, 0, sizeof(prn_buf));
    sprintf(prn_buf, "Total Amount :Rs %s",(char *)ConfigPrint.ucTotalAmt);
    if(prn_txt(prn_buf, 0) < 0)
        return FUNCTION_FAILED;


    memset(prn_buf, 0, sizeof(prn_buf)); // 02052019
    sprintf(prn_buf, "Transaction id: %s",(char *)ConfigPrint.ucRefNo); // 02052019
    if(prn_txt(prn_buf, 0) < 0) // 02052019
        return FUNCTION_FAILED; // 02052019

    printf("(char *)ConfigPrint.ucBBPS_TransID == %s\n", (char *)ConfigPrint.ucBBPS_TransID);    

    if((strcmp((char *)ConfigPrint.ucBBPS_TransID,"NA")!=0) && (strcmp((char *)ConfigPrint.ucBBPS_TransID,"")!=0))
    {
        memset(prn_buf, 0, sizeof(prn_buf));
        sprintf(prn_buf, "BBPS_TransID : %s",(char *)ConfigPrint.ucBBPS_TransID);
        if(prn_txt(prn_buf, 0) < 0)
            return FUNCTION_FAILED;
    }

    memset(prn_buf, 0, sizeof(prn_buf));
    sprintf(prn_buf, "Status : Success");
    if(prn_txt(prn_buf, 0) < 0)
        return FUNCTION_FAILED;


    if(prn_txt(Space, 0) < 0)   // print header end line
        return -1;
    if(prn_txt("Thank You Visit soon !  ", 0) < 0)   // print header start line
        return -1;
    if(prn_txt(Space, 0) < 0)   // print header end line
        return -1;
    memset(prn_buf, 0, sizeof(prn_buf));
    sprintf(prn_buf, "            FPB VT Ver %s\n",APP_VERSION);  // B C name print
    if(prn_txt(prn_buf, 0) < 0)
        return FUNCTION_FAILED;
    if(prn_txt("-------------End Of Reciept-------------", 0) < 0)   // print header start line
        return -1;
    prn_write_text((unsigned char*)"\n\n\n\n\n\n",6,1);
    prn_close();

    memset(&ConfigPrint,0,sizeof(ConfigPrint));
    uidmessage::instance()->hide_Message();
    return FUNC_OK;
}


int FPB::Mini_Print()
{
    uidmessage::instance()->hide_Message();
    uidmessage::instance()->Display_BusyMessage("Printing....");
    char szDate[16], szTime[16];
    char prn_buf[256];
    char Space[] = "                             ";
    char Line[]  ="----------------------------------------";
    CMisc miscObj;
    FinoPrint objPrn;

    memset(szDate, 0, sizeof(szDate));
    memset(szTime, 0, sizeof(szTime));
    GetLocalTime(szDate, szTime);
    if(prn_open() < 0)   // check for printer platon
    {
        miscObj.DisplayMsgBox((char *)"Printer Open Failed...");
        return -1;
    }
    if(prn_paperstatus() != 0)  // check for paper error
    {
        gl11_lcdbklt_on();
        miscObj.DisplayMsgBox((char *)"Please Put Paper!");
        prn_close();
        return -1;
    }


    if(objPrn.PrintLogo(1) < 0) // 02052019
    {
        qDebug() << "LOGO Not Printed";
        prn_write_text((unsigned char *)"     No Logo Available", strlen("     No Logo Available"), 2);
    }

    //--------------------------------header start---------------------------------------------//
    if(prn_txt(Line, 0) < 0)   // print header start line
        return -1;

    if(prn_txt("    Fund Transfer  ", 2) < 0)   // print header start line
        return -1;

    if(prn_txt(Line, 0) < 0)   // print header start line
        return -1;

    memset(prn_buf, 0, sizeof(prn_buf));
    sprintf(prn_buf, "Date: %s        Time: %s", szDate, szTime);  // print date & time
    if(prn_txt(prn_buf, 0) < 0)
        return -1;

    memset(prn_buf, 0, sizeof(prn_buf));
    sprintf(prn_buf, "Customer Name : %s",(char *)ConfigPrint.ucCustName);  // B C name print
    if(prn_txt(prn_buf, 0) < 0)
        return FUNCTION_FAILED;

    memset(prn_buf, 0, sizeof(prn_buf));
    sprintf(prn_buf, "Customer Mobile No. : %s",(char *)ConfigPrint.ucCustMobNo);  // B C name print
    if(prn_txt(prn_buf, 0) < 0)
        return FUNCTION_FAILED;


    memset(prn_buf, 0, sizeof(prn_buf));
    sprintf(prn_buf, "Beneficiary Name : %s",(char *)ConfigPrint.ucBeneName);  // B C name print
    if(prn_txt(prn_buf, 0) < 0)
        return FUNCTION_FAILED;

        memset(prn_buf, 0, sizeof(prn_buf));
        sprintf(prn_buf, "Txn Type : %s",(char *)ConfigPrint.uctxntype);
        if(prn_txt(prn_buf, 0) < 0)
            return FUNCTION_FAILED;

        memset(prn_buf, 0, sizeof(prn_buf));
        sprintf(prn_buf, "Amount : Rs %s",(char *)ConfigPrint.ucAmount);
        if(prn_txt(prn_buf, 0) < 0)
            return FUNCTION_FAILED;


        memset(prn_buf, 0, sizeof(prn_buf));
        sprintf(prn_buf, "Ref No. : %s",(char *)ConfigPrint.ucRefNo);
        if(prn_txt(prn_buf, 0) < 0)
            return FUNCTION_FAILED;

    if(prn_txt(Space, 0) < 0)   // print header end line
        return -1;
    if(prn_txt("Thank You Visit soon !  ", 0) < 0)   // print header start line
        return -1;
    if(prn_txt(Space, 0) < 0)   // print header end line
        return -1;
    memset(prn_buf, 0, sizeof(prn_buf));
    sprintf(prn_buf, "            FPB VT Ver %s\n",APP_VERSION);  // B C name print
    if(prn_txt(prn_buf, 0) < 0)
        return FUNCTION_FAILED;
    if(prn_txt("-------------End Of Reciept-------------", 0) < 0)   // print header start line
        return -1;
    prn_write_text((unsigned char*)"\n\n\n\n\n\n",6,1);
    prn_close();

    memset(&ConfigPrint,0,sizeof(ConfigPrint));
    uidmessage::instance()->hide_Message();
    return FUNC_OK;
}
