#include "FPB.h"
#include "ui_FPB.h"
#include "userprofile.h"
#include "json.h"
#include "Login.h"
#include "displaymessage.h"
#include "print.h"
#include "impscommon.h"
#include <QSqlDatabase>

unsigned int Flag_TransactionStatus_WdRRN = 0;
unsigned char Trans_Status[50+1];
extern Login_leg2 configLoginLeg2;
extern char * Convertstr(string str);
extern TxnStatement *Statement, iZero, iTransStatus, ChildList[500];
extern Merchant_detail Merdetail;
QString DisplayTxnStatus;
QList<QString> TransStatusList, ChildTransStatusList;
extern QString labelDB(QString label);
int iCLcount = 0, ResetFlag_TransStatus = 0; // TS09072019
extern int Flag_TS_Page;


void FPB::on_Transaction_Status_wdRRN_pushButton_PG12_clicked()
{
    Flag_TransactionStatus_WdRRN = 1;
    ui->label_TransactionID_pg41->setVisible(true);
    ui->TransactionID_TransStWdRRN_lineEdit_pg41->setVisible(true);
    ui->TransactionID_TransStWdRRN_OK_pushButton_pg41->setVisible(true);
    ui->Transaction_Status_label_pg41->setVisible(true);
    ui->listWidget_Reversal_data->clear();
    ui->TransactionID_TransStWdRRN_lineEdit_pg41->clear();
    ui->TransactionID_TransStWdRRN_lineEdit_pg41->setFocus();
    ui->listWidget_Reversal_data->setGeometry(5, 69, 310, 141);
    ui->stackedWidget->setCurrentIndex(41);


    return;
}

void FPB::on_PushButton_Back_Txn_Menu_clicked()
{
    ui->stackedWidget->setCurrentIndex(41);
    return;
}

int dbChildList(char *iParentTxnID, int flag_Child)
{
#if DEBUG
    printf("\n iParentTxnID == %s \n",iParentTxnID);
#endif
    QSqlQuery query;
    query.clear();
    int count = 0;
    int i=0, j=0;
    if(flag_Child == 0)
    {
        query.prepare("SELECT count(*) from DETAILED_STATEMENT where PARENT_TXN_ID = :parentTxnId AND SPLIT_SEQUENCE != '0' AND SPLIT_SEQUENCE != 'null'");
        query.bindValue(":parentTxnId", (char *)iParentTxnID);
    }
    else
    {
        query.prepare("SELECT TRANTYPE,CHANNEL ,DATE ,CUSTOMER_NO ,ACCOUNT_NO ,AMOUNT ,BRANCH ,USER_ID ,SEQUENCE_NO ,REFERENCE_NO ,REVERSAL_FLAG ,NARRATIVE ,TIME ,CUSTOMER_NAME ,CUSTOMER_MOBILE ,SPLIT_SEQUENCE ,DEBIT_CREDIT ,CHARGE_AMOUNT ,PAYOUT_AMOUNT ,COMMISSION_AMOUNT ,TDS_AMOUNT ,ORIGINAL_SEQUENCE_NO ,PARENT_TXN_ID ,PARENT_TXN_AMOUNT ,PARENT_TXN_AMOUNT_CUR ,CHARGE_AMOUNT_CUR  ,TRANSACTION_TYPE_NAME ,TRANSACTION_TYPE_PRODUCT ,TRANSACTION_TYPE_DESCRIPTION ,STATUS ,STATUS_DESC ,RFU_DATE_DESC ,RFU_DATE_VAL ,RFU_NUMERIC1_DESC ,RFU_NUMERIC1_VAL ,RFU_NUMERIC2_DESC ,RFU_NUMERIC2_VAL ,RFU_NUMERIC3_DESC ,RFU_NUMERIC3_VAL ,RFU_NUMERIC4_DESC ,RFU_NUMERIC4_VAL  ,RFU_TEXT1_DESC ,RFU_TEXT1_VAL ,RFU_TEXT2_DESC ,RFU_TEXT2_VAL ,RFU_TEXT3_DESC ,RFU_TEXT3_VAL, RFU_TEXT4_DESC ,RFU_TEXT4_VAL, RFU_TEXT5_DESC ,RFU_TEXT5_VAL, RFU_TEXT6_DESC ,RFU_TEXT6_VAL, RFU_TEXT7_DESC ,RFU_TEXT7_VAL, RFU_TEXT8_DESC ,RFU_TEXT8_VAL, RFU_TEXT9_DESC ,RFU_TEXT9_VAL, RFU_TEXT10_DESC ,RFU_TEXT10_VAL ,DEBIT_AMOUNT ,CREDIT_AMOUNT, CLSTATUS from DETAILED_STATEMENT where PARENT_TXN_ID = :parentTxnId AND SPLIT_SEQUENCE != '0' AND SPLIT_SEQUENCE != 'null'"); // TS09072019
        query.bindValue(":parentTxnId", (char *)iParentTxnID);
    }

    if(!query.exec())
    {

#if DEBUG
        qDebug() << query.lastError();
#endif
        return FUNCTION_FAILED;
    }
    else
    {
        while(query.next())
        {
            if(flag_Child == 0)
            {
                count = query.value(0).toInt();
#if DEBUG
                qDebug()<<"Child count"<<count;
#endif
            }
            else
            {
                strcpy(ChildList[j].transactionType,           query.value(0).toString().trimmed().toAscii().data());
                strcpy(ChildList[j].channel,                   query.value(1).toString().trimmed().toAscii().data());
                strcpy(ChildList[j].transactionDate,           query.value(2).toString().trimmed().toAscii().data());
                strcpy(ChildList[j].customerNo,                query.value(3).toString().trimmed().toAscii().data());
                strcpy(ChildList[j].accountNo,                 query.value(4).toString().trimmed().toAscii().data());
                strcpy(ChildList[j].transactionAmount,         query.value(5).toString().trimmed().toAscii().data());
                strcpy(ChildList[j].branchCode,                query.value(6).toString().trimmed().toAscii().data());
                strcpy(ChildList[j].userId,                    query.value(7).toString().trimmed().toAscii().data());
                strcpy(ChildList[j].sequenceNumber,            query.value(8).toString().trimmed().toAscii().data());
                strcpy(ChildList[j].referenceId,               query.value(9).toString().trimmed().toAscii().data());
                strcpy(ChildList[j].reversalFlag,              query.value(10).toString().trimmed().toAscii().data());
                strcpy(ChildList[j].transactionDesc,           query.value(11).toString().trimmed().toAscii().data());
                strcpy(ChildList[j].time,                      query.value(12).toString().trimmed().toAscii().data());
                strcpy(ChildList[j].customerName,              query.value(13).toString().trimmed().toAscii().data());
                strcpy(ChildList[j].customerMobile,            query.value(14).toString().trimmed().toAscii().data());
                strcpy(ChildList[j].splitSequence,             query.value(15).toString().trimmed().toAscii().data());
                strcpy(ChildList[j].debitCredit,               query.value(16).toString().trimmed().toAscii().data());
                strcpy(ChildList[j].chargeAmount,              query.value(17).toString().trimmed().toAscii().data());
                strcpy(ChildList[j].payoutAmount,              query.value(18).toString().trimmed().toAscii().data());
                strcpy(ChildList[j].commissionAmount,          query.value(19).toString().trimmed().toAscii().data());
                strcpy(ChildList[j].tdsAmount,                 query.value(20).toString().trimmed().toAscii().data());
                strcpy(ChildList[j].originalTxnSequence,       query.value(21).toString().trimmed().toAscii().data());
                strcpy(ChildList[j].parentTxnId,               query.value(22).toString().trimmed().toAscii().data());
                strcpy(ChildList[j].parentTxnAmount,           query.value(23).toString().trimmed().toAscii().data());
                strcpy(ChildList[j].parentTxnAmountCurrent,    query.value(24).toString().trimmed().toAscii().data());
                strcpy(ChildList[j].chargeAmountCurrent,       query.value(25).toString().trimmed().toAscii().data());
                strcpy(ChildList[j].transactionTypeName,       query.value(26).toString().trimmed().toAscii().data());
                strcpy(ChildList[j].TransactionTypeProduct,    query.value(27).toString().trimmed().toAscii().data());
                strcpy(ChildList[j].TransactionTypeDescription,query.value(28).toString().trimmed().toAscii().data());
                strcpy(ChildList[j].Status,                    query.value(29).toString().trimmed().toAscii().data());
                strcpy(ChildList[j].StatusDesc,                query.value(30).toString().trimmed().toAscii().data());
                strcpy(ChildList[j].rfuDateDesc,               query.value(31).toString().trimmed().toAscii().data());
                strcpy(ChildList[j].rfuDateVal,                query.value(32).toString().trimmed().toAscii().data());
                strcpy(ChildList[j].rfuNumeric1Desc,           query.value(33).toString().trimmed().toAscii().data());
                strcpy(ChildList[j].rfuNumeric1Val,            query.value(34).toString().trimmed().toAscii().data());
                strcpy(ChildList[j].rfuNumeric2Desc,           query.value(35).toString().trimmed().toAscii().data());
                strcpy(ChildList[j].rfuNumeric2Val,            query.value(36).toString().trimmed().toAscii().data());
                strcpy(ChildList[j].rfuNumeric3Desc,           query.value(37).toString().trimmed().toAscii().data());
                strcpy(ChildList[j].rfuNumeric3Val,            query.value(38).toString().trimmed().toAscii().data());
                strcpy(ChildList[j].rfuNumeric4Desc,           query.value(39).toString().trimmed().toAscii().data());
                strcpy(ChildList[j].rfuNumeric4Val,            query.value(40).toString().trimmed().toAscii().data());
                strcpy(ChildList[j].rfuText1Desc,              query.value(41).toString().trimmed().toAscii().data());
                strcpy(ChildList[j].rfuText1Val,               query.value(42).toString().trimmed().toAscii().data());
                strcpy(ChildList[j].rfuText2Desc,              query.value(43).toString().trimmed().toAscii().data());
                strcpy(ChildList[j].rfuText2Val,               query.value(44).toString().trimmed().toAscii().data());
                strcpy(ChildList[j].rfuText3Desc,              query.value(45).toString().trimmed().toAscii().data());
                strcpy(ChildList[j].rfuText3Val,               query.value(46).toString().trimmed().toAscii().data());
                strcpy(ChildList[j].rfuText4Desc,              query.value(47).toString().trimmed().toAscii().data());
                strcpy(ChildList[j].rfuText4Val,               query.value(48).toString().trimmed().toAscii().data());
                strcpy(ChildList[j].rfuText5Desc,              query.value(49).toString().trimmed().toAscii().data());
                strcpy(ChildList[j].rfuText5Val,               query.value(50).toString().trimmed().toAscii().data());
                strcpy(ChildList[j].rfuText6Desc,              query.value(51).toString().trimmed().toAscii().data());
                strcpy(ChildList[j].rfuText6Val,               query.value(52).toString().trimmed().toAscii().data());
                strcpy(ChildList[j].rfuText7Desc,              query.value(53).toString().trimmed().toAscii().data());
                strcpy(ChildList[j].rfuText7Val,               query.value(54).toString().trimmed().toAscii().data());
                strcpy(ChildList[j].rfuText8Desc,              query.value(55).toString().trimmed().toAscii().data());
                strcpy(ChildList[j].rfuText8Val,               query.value(56).toString().trimmed().toAscii().data());
                strcpy(ChildList[j].rfuText9Desc,              query.value(57).toString().trimmed().toAscii().data());
                strcpy(ChildList[j].rfuText9Val,               query.value(58).toString().trimmed().toAscii().data());
                strcpy(ChildList[j].rfuText10Desc,             query.value(59).toString().trimmed().toAscii().data());
                strcpy(ChildList[j].rfuText10Val,              query.value(60).toString().trimmed().toAscii().data());
                strcpy(ChildList[j].debitAmount,               query.value(61).toString().trimmed().toAscii().data());
                strcpy(ChildList[j].creditAmount,              query.value(62).toString().trimmed().toAscii().data());
                strcpy(ChildList[j].status,                    query.value(63).toString().trimmed().toAscii().data()); // TS09072019
                j++;
            }
        }
    }

    if(flag_Child == 0)
        return count;
    else
        return FUNCTION_SUCCESS;
}

void FPB::on_TransactionID_TransStWdRRN_OK_pushButton_pg41_clicked()
{
    if(ResetFlag_TransStatus == 1)
    {
        *Statement = iZero;
        free(Statement);
    }
    ResetFlag_TransStatus = 0;

    CMisc miscObj;

    if(ui->TransactionID_TransStWdRRN_lineEdit_pg41->text().length() == 0)
    {
        uidmessage::instance()->hide_Message();
        miscObj.DisplayMsgBox((char *)"Please provide TransactionId.");
        uidmessage::instance()->hide_Message();
        return;
    }

    memset(Trans_Status, 0, sizeof(Trans_Status));
    memcpy(Trans_Status, (char *)ui->TransactionID_TransStWdRRN_lineEdit_pg41->text().toAscii().trimmed().data(), strlen((char *)ui->TransactionID_TransStWdRRN_lineEdit_pg41->text().toAscii().trimmed().data()));


    uidmessage::instance()->hide_Message();
    uidmessage::instance()->Display_BusyMessage("Loading data, please wait...");

    int i=0;

    int iTSret = TransactionStatus();
    if((iTSret != FUNC_OK) && (iTSret != FUNCTION_SUCCESS))
    {
        uidmessage::instance()->hide_Message();
        ui->TransactionID_TransStWdRRN_lineEdit_pg41->clear();
        ui->listWidget_Reversal_data->clear();
        ui->TransactionStatus_listWidget_PG10->clear();
        ui->TS_ParentChild_listWidget_PG11->clear();
        if(iTSret == SESSION_EXPIRED_FAILED)
        {
            ui->listWidget_Reversal_data->clear();
            ui->stackedWidget->setCurrentIndex(0);
            return;
        }

        ui->stackedWidget->setCurrentIndex(41);
        return;
    }
    else
    {
        QSqlQuery query;
        query.clear();
        //        query.prepare("SELECT TRANTYPE,CHANNEL ,DATE ,CUSTOMER_NO ,ACCOUNT_NO ,AMOUNT ,BRANCH ,USER_ID ,SEQUENCE_NO ,REFERENCE_NO ,REVERSAL_FLAG ,NARRATIVE ,TIME ,CUSTOMER_NAME ,CUSTOMER_MOBILE ,SPLIT_SEQUENCE ,DEBIT_CREDIT ,CHARGE_AMOUNT ,PAYOUT_AMOUNT ,COMMISSION_AMOUNT ,TDS_AMOUNT ,ORIGINAL_SEQUENCE_NO ,PARENT_TXN_ID ,PARENT_TXN_AMOUNT ,PARENT_TXN_AMOUNT_CUR ,CHARGE_AMOUNT_CUR  ,TRANSACTION_TYPE_NAME ,TRANSACTION_TYPE_PRODUCT ,TRANSACTION_TYPE_DESCRIPTION ,STATUS ,STATUS_DESC ,RFU_DATE_DESC ,RFU_DATE_VAL ,RFU_NUMERIC1_DESC ,RFU_NUMERIC1_VAL ,RFU_NUMERIC2_DESC ,RFU_NUMERIC2_VAL ,RFU_NUMERIC3_DESC ,RFU_NUMERIC3_VAL ,RFU_NUMERIC4_DESC ,RFU_NUMERIC4_VAL  ,RFU_TEXT1_DESC ,RFU_TEXT1_VAL ,RFU_TEXT2_DESC ,RFU_TEXT2_VAL ,RFU_TEXT3_DESC ,RFU_TEXT3_VAL, RFU_TEXT4_DESC ,RFU_TEXT4_VAL, RFU_TEXT5_DESC ,RFU_TEXT5_VAL, RFU_TEXT6_DESC ,RFU_TEXT6_VAL, RFU_TEXT7_DESC ,RFU_TEXT7_VAL, RFU_TEXT8_DESC ,RFU_TEXT8_VAL, RFU_TEXT9_DESC ,RFU_TEXT9_VAL, RFU_TEXT10_DESC ,RFU_TEXT10_VAL ,DEBIT_AMOUNT ,CREDIT_AMOUNT from DETAILED_STATEMENT where SPLIT_SEQUENCE = '0' OR (SPLIT_SEQUENCE = '1'  AND ORIGINAL_SEQUENCE_NO = '' )");
        query.prepare("SELECT TRANTYPE,CHANNEL ,DATE ,CUSTOMER_NO ,ACCOUNT_NO ,AMOUNT ,BRANCH ,USER_ID ,SEQUENCE_NO ,REFERENCE_NO ,REVERSAL_FLAG ,NARRATIVE ,TIME ,CUSTOMER_NAME ,CUSTOMER_MOBILE ,SPLIT_SEQUENCE ,DEBIT_CREDIT ,CHARGE_AMOUNT ,PAYOUT_AMOUNT ,COMMISSION_AMOUNT ,TDS_AMOUNT ,ORIGINAL_SEQUENCE_NO ,PARENT_TXN_ID ,PARENT_TXN_AMOUNT ,PARENT_TXN_AMOUNT_CUR ,CHARGE_AMOUNT_CUR  ,TRANSACTION_TYPE_NAME ,TRANSACTION_TYPE_PRODUCT ,TRANSACTION_TYPE_DESCRIPTION ,STATUS ,STATUS_DESC ,RFU_DATE_DESC ,RFU_DATE_VAL ,RFU_NUMERIC1_DESC ,RFU_NUMERIC1_VAL ,RFU_NUMERIC2_DESC ,RFU_NUMERIC2_VAL ,RFU_NUMERIC3_DESC ,RFU_NUMERIC3_VAL ,RFU_NUMERIC4_DESC ,RFU_NUMERIC4_VAL  ,RFU_TEXT1_DESC ,RFU_TEXT1_VAL ,RFU_TEXT2_DESC ,RFU_TEXT2_VAL ,RFU_TEXT3_DESC ,RFU_TEXT3_VAL, RFU_TEXT4_DESC ,RFU_TEXT4_VAL, RFU_TEXT5_DESC ,RFU_TEXT5_VAL, RFU_TEXT6_DESC ,RFU_TEXT6_VAL, RFU_TEXT7_DESC ,RFU_TEXT7_VAL, RFU_TEXT8_DESC ,RFU_TEXT8_VAL, RFU_TEXT9_DESC ,RFU_TEXT9_VAL, RFU_TEXT10_DESC ,RFU_TEXT10_VAL ,DEBIT_AMOUNT ,CREDIT_AMOUNT,CLSTATUS  from DETAILED_STATEMENT"); // TS09072019
        // select * from detailed_statement where split_sequence = '0' OR (split_sequence = '1'  AND original_sequence_no = '' );

        if(!query.exec())
        {
            uidmessage::instance()->hide_Message();
#if DEBUG
            qDebug()<<"Fetch query failed";
#endif
            return;
        }
        else
        {
            ResetFlag_TransStatus = 1;
            //            Statement = (TxnStatement*)malloc(sizeof(Statement)*iTransStatus.iTxnSCount);
            Statement = (TxnStatement*)malloc(sizeof *Statement);
            *Statement = iZero;

#if DEBUG
            printf("iTransStatus.iTxnSCount = %d\n", iTransStatus.iTxnSCount);
#endif
            Statement =(TxnStatement*) malloc(sizeof(TxnStatement)*iTransStatus.iTxnSCount);

            while(query.next()) //  (char *)query.value(0).toString().trimmed().toAscii().data(), strlen((char *)query.value(0).toString().trimmed().toAscii().data())
            {

                strcpy(Statement[i].transactionType, (char *)query.value(0).toString().toAscii().data()); //, strlen((char *)query.value(0).toString().trimmed().toAscii().data()));
#if DEBUG
                printf("transactionType = %s\n", Statement[i].transactionType);
#endif
                strcpy(Statement[i].channel, (char *)query.value(1).toString().toAscii().data()); //, strlen((char *)query.value(1).toString().trimmed().toAscii().data()));
#if DEBUG
                printf("channel = %s\n", Statement[i].channel);
#endif
                strcpy(Statement[i].transactionDate, (char *)query.value(2).toString().trimmed().toAscii().data()); //, strlen((char *)query.value(2).toString().trimmed().toAscii().data()));
#if DEBUG
                qDebug()<<"Statement[i].transactionDate"<<Statement[i].transactionDate;
#endif
                strcpy(Statement[i].customerNo, (char *)query.value(3).toString().trimmed().toAscii().data()); //, strlen((char *)query.value(3).toString().trimmed().toAscii().data()));
#if DEBUG
                qDebug()<<"Statement[i].customerNo"<<Statement[i].customerNo;
#endif
                strcpy(Statement[i].accountNo, (char *)query.value(4).toString().trimmed().toAscii().data()); //, strlen((char *)query.value(4).toString().trimmed().toAscii().data()));
#if DEBUG
                qDebug()<<"Statement[i].accountNo"<<Statement[i].accountNo;
#endif
                strcpy(Statement[i].transactionAmount, (char *)query.value(5).toString().trimmed().toAscii().data()); //, strlen((char *)query.value(5).toString().trimmed().toAscii().data()));
#if DEBUG
                qDebug()<<"Statement[i].transactionAmount"<<Statement[i].transactionAmount;
#endif
                strcpy(Statement[i].branchCode, (char *)query.value(6).toString().trimmed().toAscii().data()); //, strlen((char *)query.value(6).toString().trimmed().toAscii().data()));
#if DEBUG
                qDebug()<<"Statement[i].branchCode"<<Statement[i].branchCode;
#endif
                strcpy(Statement[i].userId, (char *)query.value(7).toString().trimmed().toAscii().data()); //, strlen((char *)query.value(7).toString().trimmed().toAscii().data()));
#if DEBUG
                qDebug()<<"Statement[i].userId"<<Statement[i].userId;
#endif
                strcpy(Statement[i].sequenceNumber, (char *)query.value(8).toString().trimmed().toAscii().data()); //, strlen((char *)query.value(8).toString().trimmed().toAscii().data()));
#if DEBUG
                qDebug()<<"Statement[i].sequenceNumber"<<Statement[i].sequenceNumber;
#endif
                strcpy(Statement[i].referenceId, (char *)query.value(9).toString().trimmed().toAscii().data()); //, strlen((char *)query.value(9).toString().trimmed().toAscii().data()));
#if DEBUG
                qDebug()<<"Statement[i].referenceId"<<Statement[i].referenceId;
#endif
                //                strcpy(Statement[i].status, (char *)query.value(11).toString().trimmed().toAscii().data()); //, strlen((char *)query.value(11).toString().trimmed().toAscii().data()));
                //                qDebug()<<"Statement[i].status"<<Statement[i].status;
                strcpy(Statement[i].reversalFlag, (char *)query.value(10).toString().trimmed().toAscii().data()); //, strlen((char *)query.value(10).toString().trimmed().toAscii().data()));
#if DEBUG
                qDebug()<<"Statement[i].reversalFlag"<<Statement[i].reversalFlag;
#endif
                strcpy(Statement[i].transactionDesc, (char *)query.value(11).toString().trimmed().toAscii().data()); //, strlen((char *)query.value(11).toString().trimmed().toAscii().data()));
#if DEBUG

                qDebug()<<"Statement[i].transactionDesc"<<Statement[i].transactionDesc;
#endif
                strcpy(Statement[i].time, (char *)query.value(12).toString().trimmed().toAscii().data()); //, strlen((char *)query.value(12).toString().trimmed().toAscii().data()));
#if DEBUG
                qDebug()<<"Statement[i].time"<<Statement[i].time;
#endif
                strcpy(Statement[i].customerName, (char *)query.value(13).toString().trimmed().toAscii().data()); //, strlen((char *)query.value(13).toString().trimmed().toAscii().data()));
#if DEBUG
                qDebug()<<"Statement[i].customerName"<<Statement[i].customerName;
#endif
                strcpy(Statement[i].customerMobile, (char *)query.value(14).toString().trimmed().toAscii().data()); //, strlen((char *)query.value(14).toString().trimmed().toAscii().data()));
#if DEBUG
                qDebug()<<"Statement[i].customerMobile"<<Statement[i].customerMobile;
#endif
                strcpy(Statement[i].splitSequence, (char *)query.value(15).toString().trimmed().toAscii().data()); //, strlen((char *)query.value(15).toString().trimmed().toAscii().data()));
#if DEBUG
                qDebug()<<"Statement[i].splitSequence"<<Statement[i].splitSequence;
#endif
                strcpy(Statement[i].debitCredit, (char *)query.value(16).toString().trimmed().toAscii().data()); //, strlen((char *)query.value(16).toString().trimmed().toAscii().data()));
#if DEBUG
                qDebug()<<"Statement[i].debitCredit"<<Statement[i].debitCredit;
#endif
                strcpy(Statement[i].chargeAmount, (char *)query.value(17).toString().trimmed().toAscii().data()); //, strlen((char *)query.value(17).toString().trimmed().toAscii().data()));
#if DEBUG

                qDebug()<<"Statement[i].chargeAmount"<<Statement[i].chargeAmount;
#endif
                strcpy(Statement[i].payoutAmount, (char *)query.value(18).toString().trimmed().toAscii().data()); //, strlen((char *)query.value(18).toString().trimmed().toAscii().data()));
#if DEBUG
                qDebug()<<"Statement[i].payoutAmount"<<Statement[i].payoutAmount;
#endif
                strcpy(Statement[i].commissionAmount, (char *)query.value(19).toString().trimmed().toAscii().data()); //, strlen((char *)query.value(19).toString().trimmed().toAscii().data()));
#if DEBUG
                qDebug()<<"Statement[i].commissionAmount"<<Statement[i].commissionAmount;
#endif
                strcpy(Statement[i].tdsAmount, (char *)query.value(20).toString().trimmed().toAscii().data()); //, strlen((char *)query.value(20).toString().trimmed().toAscii().data()));
#if DEBUG
                qDebug()<<"Statement[i].tdsAmount"<<Statement[i].tdsAmount;
#endif
                strcpy(Statement[i].originalTxnSequence, (char *)query.value(21).toString().trimmed().toAscii().data()); //, strlen((char *)query.value(21).toString().trimmed().toAscii().data()));
#if DEBUG
                qDebug()<<"Statement[i].originalTxnSequence"<<Statement[i].originalTxnSequence;
#endif
                strcpy(Statement[i].parentTxnId, (char *)query.value(22).toString().trimmed().toAscii().data()); //, strlen((char *)query.value(22).toString().trimmed().toAscii().data()));
#if DEBUG
                qDebug()<<"Statement[i].parentTxnId"<<Statement[i].parentTxnId;
#endif
                strcpy(Statement[i].parentTxnAmount, (char *)query.value(23).toString().trimmed().toAscii().data()); //, strlen((char *)query.value(23).toString().trimmed().toAscii().data()));
#if DEBUG
                qDebug()<<"Statement[i].parentTxnAmount"<<Statement[i].parentTxnAmount;
#endif
                strcpy(Statement[i].parentTxnAmountCurrent, (char *)query.value(24).toString().trimmed().toAscii().data()); //, strlen((char *)query.value(24).toString().trimmed().toAscii().data()));
#if DEBUG
                qDebug()<<"Statement[i].parentTxnAmountCurrent"<<Statement[i].parentTxnAmountCurrent;
#endif
                strcpy(Statement[i].chargeAmountCurrent, (char *)query.value(25).toString().trimmed().toAscii().data()); //, strlen((char *)query.value(25).toString().trimmed().toAscii().data()));
#if DEBUG
                qDebug()<<"Statement[i].chargeAmountCurrent"<<Statement[i].chargeAmountCurrent;
#endif
                strcpy(Statement[i].transactionTypeName, (char *)query.value(26).toString().trimmed().toAscii().data()); //, strlen((char *)query.value(26).toString().trimmed().toAscii().data()));
#if DEBUG
                qDebug()<<"Statement[i].transactionTypeName"<<Statement[i].transactionTypeName;
#endif
                strcpy(Statement[i].TransactionTypeProduct, (char *)query.value(27).toString().trimmed().toAscii().data()); //, strlen((char *)query.value(27).toString().trimmed().toAscii().data()));
#if DEBUG

                qDebug()<<"Statement[i].TransactionTypeProduct"<<Statement[i].TransactionTypeProduct;
#endif
                strcpy(Statement[i].TransactionTypeDescription, (char *)query.value(28).toString().trimmed().toAscii().data()); //, strlen((char *)query.value(28).toString().trimmed().toAscii().data()));
#if DEBUG
                qDebug()<<"Statement[i].TransactionTypeDescription"<<Statement[i].TransactionTypeDescription;
#endif
                strcpy(Statement[i].Status, (char *)query.value(29).toString().trimmed().toAscii().data()); //, strlen((char *)query.value(29).toString().trimmed().toAscii().data()));
#if DEBUG
                qDebug()<<"Statement[i].Status"<<Statement[i].Status;
#endif
                strcpy(Statement[i].StatusDesc, (char *)query.value(30).toString().trimmed().toAscii().data()); //, strlen((char *)query.value(30).toString().trimmed().toAscii().data()));
#if DEBUG
                qDebug()<<"Statement[i].StatusDesc"<<Statement[i].StatusDesc;
#endif
                strcpy(Statement[i].rfuDateDesc, (char *)query.value(31).toString().trimmed().toAscii().data()); //, strlen((char *)query.value(31).toString().trimmed().toAscii().data()));
#if DEBUG
                qDebug()<<"Statement[i].rfuDateDesc"<<Statement[i].rfuDateDesc;
#endif
                strcpy(Statement[i].rfuDateVal, (char *)query.value(32).toString().trimmed().toAscii().data()); //, strlen((char *)query.value(32).toString().trimmed().toAscii().data()));
#if DEBUG
                qDebug()<<"Statement[i].rfuDateVal"<<Statement[i].rfuDateVal;
#endif
                strcpy(Statement[i].rfuNumeric1Desc, (char *)query.value(33).toString().trimmed().toAscii().data()); //, strlen((char *)query.value(33).toString().trimmed().toAscii().data()));
#if DEBUG

                qDebug()<<"Statement[i].rfuNumeric1Desc"<<Statement[i].rfuNumeric1Desc;
#endif
                strcpy(Statement[i].rfuNumeric1Val, (char *)query.value(34).toString().trimmed().toAscii().data()); //, strlen((char *)query.value(34).toString().trimmed().toAscii().data()));
#if DEBUG
                qDebug()<<"Statement[i].rfuNumeric1Val"<<Statement[i].rfuNumeric1Val;
#endif
                strcpy(Statement[i].rfuNumeric2Desc, (char *)query.value(35).toString().trimmed().toAscii().data()); //, strlen((char *)query.value(35).toString().trimmed().toAscii().data()));
#if DEBUG
                qDebug()<<"Statement[i].rfuNumeric2Desc"<<Statement[i].rfuNumeric2Desc;
#endif
                strcpy(Statement[i].rfuNumeric2Val, (char *)query.value(36).toString().trimmed().toAscii().data()); //, strlen((char *)query.value(36).toString().trimmed().toAscii().data()));
#if DEBUG
                qDebug()<<"Statement[i].rfuNumeric2Val"<<Statement[i].rfuNumeric2Val;
#endif
                strcpy(Statement[i].rfuNumeric3Desc, (char *)query.value(37).toString().trimmed().toAscii().data()); //, strlen((char *)query.value(37).toString().trimmed().toAscii().data()));
#if DEBUG
                qDebug()<<"Statement[i].rfuNumeric3Desc"<<Statement[i].rfuNumeric3Desc;
#endif
                strcpy(Statement[i].rfuNumeric3Val, (char *)query.value(38).toString().trimmed().toAscii().data()); //, strlen((char *)query.value(38).toString().trimmed().toAscii().data()));
#if DEBUG
                qDebug()<<"Statement[i].rfuNumeric3Val"<<Statement[i].rfuNumeric3Val;
#endif
                strcpy(Statement[i].rfuNumeric4Desc, (char *)query.value(39).toString().trimmed().toAscii().data()); //, strlen((char *)query.value(39).toString().trimmed().toAscii().data()));
#if DEBUG

                qDebug()<<"Statement[i].rfuNumeric4Desc"<<Statement[i].rfuNumeric4Desc;
#endif
                strcpy(Statement[i].rfuNumeric4Val, (char *)query.value(40).toString().trimmed().toAscii().data()); //, strlen((char *)query.value(40).toString().trimmed().toAscii().data()));
#if DEBUG
                qDebug()<<"Statement[i].rfuNumeric4Val"<<Statement[i].rfuNumeric4Val;
#endif
                strcpy(Statement[i].rfuText1Desc, (char *)query.value(41).toString().trimmed().toAscii().data()); //, strlen((char *)query.value(41).toString().trimmed().toAscii().data()));
#if DEBUG
                qDebug()<<"Statement[i].rfuText1Desc"<<Statement[i].rfuText1Desc;
#endif
                strcpy(Statement[i].rfuText1Val, (char *)query.value(42).toString().trimmed().toAscii().data()); //, strlen((char *)query.value(42).toString().trimmed().toAscii().data()));
#if DEBUG
                qDebug()<<"Statement[i].rfuText1Val"<<Statement[i].rfuText1Val;
#endif
                strcpy(Statement[i].rfuText2Desc, (char *)query.value(43).toString().trimmed().toAscii().data()); //, strlen((char *)query.value(43).toString().trimmed().toAscii().data()));
#if DEBUG
                qDebug()<<"Statement[i].rfuText2Desc"<<Statement[i].rfuText2Desc;
#endif
                strcpy(Statement[i].rfuText2Val, (char *)query.value(44).toString().trimmed().toAscii().data()); //, strlen((char *)query.value(44).toString().trimmed().toAscii().data()));
#if DEBUG
                qDebug()<<"Statement[i].rfuText2Val"<<Statement[i].rfuText2Val;
#endif
                strcpy(Statement[i].rfuText3Desc, (char *)query.value(45).toString().trimmed().toAscii().data()); //, strlen((char *)query.value(45).toString().trimmed().toAscii().data()));
#if DEBUG
                qDebug()<<"Statement[i].rfuText3Desc"<<Statement[i].rfuText3Desc;
#endif
                strcpy(Statement[i].rfuText3Val, (char *)query.value(46).toString().trimmed().toAscii().data()); //, strlen((char *)query.value(46).toString().trimmed().toAscii().data()));
#if DEBUG
                qDebug()<<"Statement[i].rfuText3Val"<<Statement[i].rfuText3Val;
#endif
                strcpy(Statement[i].rfuText4Desc, (char *)query.value(47).toString().trimmed().toAscii().data()); //, strlen((char *)query.value(47).toString().trimmed().toAscii().data()));
#if DEBUG
                qDebug()<<"Statement[i].rfuText4Desc"<<Statement[i].rfuText4Desc;
#endif
                strcpy(Statement[i].rfuText4Val, (char *)query.value(48).toString().trimmed().toAscii().data()); //, strlen((char *)query.value(48).toString().trimmed().toAscii().data()));
#if DEBUG
                qDebug()<<"Statement[i].rfuText4Val"<<Statement[i].rfuText4Val;
#endif
                strcpy(Statement[i].rfuText5Desc, (char *)query.value(49).toString().trimmed().toAscii().data()); //, strlen((char *)query.value(49).toString().trimmed().toAscii().data()));
#if DEBUG
                qDebug()<<"Statement[i].rfuText5Desc"<<Statement[i].rfuText5Desc;
#endif
                strcpy(Statement[i].rfuText5Val, (char *)query.value(50).toString().trimmed().toAscii().data()); //, strlen((char *)query.value(50).toString().trimmed().toAscii().data()));
#if DEBUG
                qDebug()<<"Statement[i].rfuText5Val"<<Statement[i].rfuText5Val;
#endif
                strcpy(Statement[i].rfuText6Desc, (char *)query.value(51).toString().trimmed().toAscii().data()); //, strlen((char *)query.value(51).toString().trimmed().toAscii().data()));
#if DEBUG
                qDebug()<<"Statement[i].rfuText6Desc"<<Statement[i].rfuText6Desc;
#endif
                strcpy(Statement[i].rfuText6Val, (char *)query.value(52).toString().trimmed().toAscii().data()); //, strlen((char *)query.value(52).toString().trimmed().toAscii().data()));
#if DEBUG
                qDebug()<<"Statement[i].rfuText6Val"<<Statement[i].rfuText6Val;
#endif
                strcpy(Statement[i].rfuText7Desc, (char *)query.value(53).toString().trimmed().toAscii().data()); //, strlen((char *)query.value(53).toString().trimmed().toAscii().data()));
#if DEBUG
                qDebug()<<"Statement[i].rfuText7Desc"<<Statement[i].rfuText7Desc;
#endif
                strcpy(Statement[i].rfuText7Val, (char *)query.value(54).toString().trimmed().toAscii().data()); //, strlen((char *)query.value(54).toString().trimmed().toAscii().data()));
#if DEBUG
                qDebug()<<"Statement[i].rfuText7Val"<<Statement[i].rfuText7Val;
#endif
                strcpy(Statement[i].rfuText8Desc, (char *)query.value(55).toString().trimmed().toAscii().data()); //, strlen((char *)query.value(55).toString().trimmed().toAscii().data()));
#if DEBUG
                qDebug()<<"Statement[i].rfuText8Desc"<<Statement[i].rfuText8Desc;
#endif
                strcpy(Statement[i].rfuText8Val, (char *)query.value(56).toString().trimmed().toAscii().data()); //, strlen((char *)query.value(56).toString().trimmed().toAscii().data()));
#if DEBUG
                qDebug()<<"Statement[i].rfuText8Val"<<Statement[i].rfuText8Val;
#endif
                strcpy(Statement[i].rfuText9Desc, (char *)query.value(57).toString().trimmed().toAscii().data()); //, strlen((char *)query.value(57).toString().trimmed().toAscii().data()));
#if DEBUG
                qDebug()<<"Statement[i].rfuText9Desc"<<Statement[i].rfuText9Desc;
#endif
                strcpy(Statement[i].rfuText9Val, (char *)query.value(58).toString().trimmed().toAscii().data()); //, strlen((char *)query.value(58).toString().trimmed().toAscii().data()));
#if DEBUG
                qDebug()<<"Statement[i].rfuText9Val"<<Statement[i].rfuText9Val;
#endif
                strcpy(Statement[i].rfuText10Desc, (char *)query.value(59).toString().trimmed().toAscii().data()); //, strlen((char *)query.value(59).toString().trimmed().toAscii().data()));
#if DEBUG
                qDebug()<<"Statement[i].rfuText10Desc"<<Statement[i].rfuText10Desc;
#endif
                strcpy(Statement[i].rfuText10Val, (char *)query.value(60).toString().trimmed().toAscii().data()); //, strlen((char *)query.value(60).toString().trimmed().toAscii().data()));
#if DEBUG
                qDebug()<<"Statement[i].rfuText10Val"<<Statement[i].rfuText10Val;
#endif
                strcpy(Statement[i].debitAmount, (char *)query.value(61).toString().trimmed().toAscii().data()); //, strlen((char *)query.value(61).toString().trimmed().toAscii().data()));
#if DEBUG
                qDebug()<<"Statement[i].debitAmount"<<Statement[i].debitAmount;
#endif
                strcpy(Statement[i].creditAmount, (char *)query.value(62).toString().trimmed().toAscii().data()); //, strlen((char *)query.value(62).toString().trimmed().toAscii().data()));
#if DEBUG
                qDebug()<<"Statement[i].creditAmount"<<Statement[i].creditAmount;
#endif
                strcpy(Statement[i].status, (char *)query.value(63).toString().trimmed().toAscii().data()); //, strlen((char *)query.value(62).toString().trimmed().toAscii().data())); // TS09072019
#if DEBUG
                qDebug()<<"Statement[i].status"<<Statement[i].status;
#endif // TS09072019
                i++;

            }
        }

        memset(&ChildList, 0, sizeof(ChildList));
        TransStatusList.clear();
        DisplayTxnStatus.clear();
        ui->listWidget_Reversal_data->clear();

        char tempbuff[100];
        memset(tempbuff, 0, sizeof(tempbuff));

        bool decimal;
        QString temp_amount;
        int m=0;
        iTransStatus.iPratentCount = 0;

#if DEBUG
        qDebug()<<  "iTSCount == "<<iTransStatus.iTxnSCount;
#endif
        for(int j=0; j<i;j++,m++)
        {
            // select * from detailed_statement where split_sequence = '0' OR (split_sequence = '1'  AND original_sequence_no = '' );

            if((strcmp(Statement[j].splitSequence, "0")==0) || ((strcmp(Statement[j].splitSequence, "1")==0) && ((strcmp(Statement[j].originalTxnSequence, "null")==0) || (strcmp(Statement[j].originalTxnSequence, "")==0))))
            {
                iTransStatus.iPratentCount++;
                iCLcount = dbChildList(Statement[j].parentTxnId, 0);
                qDebug()<<"4";

                if(iCLcount > 0)
                {
#if DEBUG
                    qDebug()<<"Child List Available";
#endif
                    int ret = dbChildList(Statement[j].parentTxnId, 1);
                    if(ret != FUNCTION_SUCCESS)
                        return;
                }

#if DEBUG
                qDebug()<<"iCLcount = "<<iCLcount;
#endif


                DisplayTxnStatus.clear();

                DisplayTxnStatus += QString::fromUtf8(Statement[j].transactionDate) + " " + QString::fromUtf8(Statement[j].time);

                DisplayTxnStatus += "      " + (((strcmp(Statement[j].customerName, "null")==0) || (strcmp(Statement[j].customerName, "")==0)) ? "" : (QString::fromUtf8(Statement[j].customerName))) + "\n";


                QString temp_parenttxnAmount    = ((strcmp(Statement[j].parentTxnAmount, "null")==0) || (strcmp(Statement[j].parentTxnAmount, "")==0)  || (strcmp(Statement[j].parentTxnAmount, "0")==0)) ? "0" : (QString::fromUtf8(Statement[j].parentTxnAmount));
                QString temp_parenttxnAmountCur = ((strcmp(Statement[j].parentTxnAmountCurrent, "null")==0) || (strcmp(Statement[j].parentTxnAmountCurrent, "")==0) || (strcmp(Statement[j].parentTxnAmountCurrent, "0")==0)) ? "0" : (QString::fromUtf8(Statement[j].parentTxnAmountCurrent));
                QString temp_txnAmount = ((strcmp(Statement[j].transactionAmount, "null")==0) || (strcmp(Statement[j].parentTxnAmountCurrent, "")==0) || (strcmp(Statement[j].transactionAmount, "0")==0)) ? "0" : (QString::fromUtf8(Statement[j].transactionAmount));
                QString temp_ChargeAmount = ((strcmp(Statement[j].chargeAmount, "null")==0) || (strcmp(Statement[j].parentTxnAmountCurrent, "")==0) || (strcmp(Statement[j].chargeAmount, "0")==0)) ? "0" : (QString::fromUtf8(Statement[j].chargeAmount));
                QString temp_ChargeAmountCur = ((strcmp(Statement[j].chargeAmountCurrent, "null")==0) || (strcmp(Statement[j].parentTxnAmountCurrent, "")==0) || (strcmp(Statement[j].chargeAmountCurrent, "0")==0)) ? "0" : (QString::fromUtf8(Statement[j].chargeAmountCurrent));

                temp_amount.clear();
                temp_amount = temp_parenttxnAmount.trimmed();
                decimal = '\0';
                decimal = temp_amount.contains('.');
                if(decimal == false)
                {
                    temp_amount = temp_amount.append(".00");
                    memset(Statement[j].parentTxnAmount, 0, sizeof(Statement[j].parentTxnAmount));
                    strcpy(Statement[j].parentTxnAmount, temp_amount.toAscii().trimmed().data());
                    temp_amount.clear();
                    decimal = '\0';
                }

                temp_amount = temp_parenttxnAmountCur.trimmed();
                decimal = temp_amount.contains('.');
                if(decimal == false)
                {
                    temp_amount = temp_amount.append(".00");
                    memset(Statement[j].parentTxnAmountCurrent, 0, sizeof(Statement[j].parentTxnAmountCurrent));
                    strcpy(Statement[j].parentTxnAmountCurrent, temp_amount.toAscii().trimmed().data());
                    temp_amount.clear();
                    decimal = '\0';
                }

                temp_amount = temp_txnAmount.trimmed();
                decimal = temp_amount.contains('.');
                if(decimal == false)
                {
                    temp_amount = temp_amount.append(".00");
                    memset(Statement[j].transactionAmount, 0, sizeof(Statement[j].transactionAmount));
                    strcpy(Statement[j].transactionAmount, temp_amount.toAscii().trimmed().data());
                    temp_amount.clear();
                    decimal = '\0';
                }

                temp_amount = temp_ChargeAmount.trimmed();
                decimal = temp_amount.contains('.');
                if(decimal == false)
                {
                    temp_amount = temp_amount.append(".00");
                    memset(Statement[j].chargeAmount, 0, sizeof(Statement[j].chargeAmount));
                    strcpy(Statement[j].chargeAmount, temp_amount.toAscii().trimmed().data());
                    temp_amount.clear();
                    decimal = '\0';
                }

                temp_amount = temp_ChargeAmountCur.trimmed();
                decimal = temp_amount.contains('.');
                if(decimal == false)
                {
                    temp_amount = temp_amount.append(".00");
                    memset(Statement[j].chargeAmountCurrent, 0, sizeof(Statement[j].chargeAmountCurrent));
                    strcpy(Statement[j].chargeAmountCurrent, temp_amount.toAscii().trimmed().data());
                    temp_amount.clear();
                    decimal = '\0';
                }

#if DEBUG
                qDebug()<<"parentTxnAmount = "<<temp_parenttxnAmount;
                qDebug()<<"parentTxnAmountCurrent = "<<temp_parenttxnAmountCur;
                qDebug()<<"transactionAmount = "<<temp_txnAmount;
                qDebug()<<"chargeAmount = "<<temp_ChargeAmount;
                qDebug()<<"chargeAmountCurrent = "<<temp_ChargeAmountCur;
#endif

                double parentTxnAmount = temp_parenttxnAmount.toDouble();
                double parentTxnAmountCur = temp_parenttxnAmountCur.toDouble();
                double amount = temp_txnAmount.toDouble();
                double charges = temp_ChargeAmount.toDouble();
                double newCharges = temp_ChargeAmountCur.toDouble();

                QString CrDrSign;
                CrDrSign.clear();
                if(strcmp(Statement[j].debitCredit, "Cr") == 0)
                    CrDrSign = "+ ";
                else if(strcmp(Statement[j].debitCredit, "Dr") == 0)
                    CrDrSign = "- ";
                else
                    CrDrSign = "";
#if DEBUG
                qDebug()<<"CrDrSign = "<<CrDrSign;
#endif
                memset(tempbuff, 0, sizeof(tempbuff));
                sprintf(tempbuff, "%-28s", (Statement[j].TransactionTypeDescription));
                DisplayTxnStatus += (QString::fromUtf8(tempbuff)) + (((strcmp(Statement[j].customerMobile, "null")==0) || ((strcmp(Statement[j].customerMobile, "0")==0))) ? "" : QString::fromUtf8(Statement[j].customerMobile)) + "\n";



                if(strcmp(Statement[j].splitSequence, "1")==0)
                {
                    if(parentTxnAmount == parentTxnAmountCur)
                    {
#if DEBUG
                        qDebug()<<"parentTxnAmount == parentTxnAmountCur";
#endif
                        DisplayTxnStatus += "                            " + CrDrSign + QString::fromUtf8(Statement[j].parentTxnAmount) + "\n";
                        DisplayTxnStatus += QString::fromUtf8(Statement[j].parentTxnId);
                        DisplayTxnStatus += "                " + CrDrSign + QString::fromUtf8(Statement[j].chargeAmount) + "    Success" + "  \n";

                    }
                    else if(parentTxnAmountCur == 0)
                    {
#if DEBUG
                        qDebug()<<"parentTxnAmountCur == 0";
#endif
                        if((strcmp(Statement[j].originalTxnSequence, "null") == 0 || strcmp(Statement[j].originalTxnSequence, "") == 0) &&
                                (strcmp(Statement[j].reversalFlag, "false") == 0 || strcmp(Statement[j].reversalFlag, "null") == 0  || strcmp(Statement[j].reversalFlag, "") == 0))
                        {
#if DEBUG
                            qDebug()<<"Status = ''";
#endif
                            DisplayTxnStatus += "                            " + CrDrSign + QString::fromUtf8(Statement[j].parentTxnAmount) + "\n";
                            DisplayTxnStatus += QString::fromUtf8(Statement[j].parentTxnId);
                            DisplayTxnStatus += "                " + CrDrSign + QString::fromUtf8(Statement[j].chargeAmount) + "" + "  \n";

                        }
                        else if(!((strcmp(Statement[j].originalTxnSequence, "null") == 0) || (strcmp(Statement[j].originalTxnSequence, "") == 0)))
                        {
#if DEBUG
                            qDebug()<<"Status = 'Success'";
#endif
                            DisplayTxnStatus += "                            " + CrDrSign + QString::fromUtf8(Statement[j].parentTxnAmount) + "\n";
                            DisplayTxnStatus += QString::fromUtf8(Statement[j].parentTxnId);
                            DisplayTxnStatus += "                " + CrDrSign + QString::fromUtf8(Statement[j].chargeAmount) + "    Success" + "  \n";

                        }
                        else
                        {
#if DEBUG
                            qDebug()<<"Status = 'Fail'";
#endif
                            DisplayTxnStatus += "                            " + CrDrSign + QString::fromUtf8(Statement[j].parentTxnAmountCurrent) + " / " + QString::fromUtf8(Statement[j].parentTxnAmount) + "\n";
                            DisplayTxnStatus += QString::fromUtf8(Statement[j].parentTxnId);
                            DisplayTxnStatus += "                " + CrDrSign + QString::fromUtf8(Statement[j].chargeAmountCurrent) + " / " + QString::fromUtf8(Statement[j].chargeAmount) + "    Fail" + "  \n";
                        }
                    }
                    else if(parentTxnAmountCur < parentTxnAmount)
                    {
#if DEBUG
                        qDebug()<<"parentTxnAmountCur < parentTxnAmount";
#endif
                        DisplayTxnStatus += "                            " + CrDrSign + QString::fromUtf8(Statement[j].parentTxnAmountCurrent) + " / " + QString::fromUtf8(Statement[j].parentTxnAmount) + "\n";
                        DisplayTxnStatus += QString::fromUtf8(Statement[j].parentTxnId);
                        DisplayTxnStatus += "                " + CrDrSign + QString::fromUtf8(Statement[j].chargeAmountCurrent) + " / " + QString::fromUtf8(Statement[j].chargeAmount) + "    Partial" + "  \n";

                    }
                    else
                    {
                        DisplayTxnStatus += "                            " + CrDrSign + QString::fromUtf8(Statement[j].parentTxnAmount) + "\n";
                        DisplayTxnStatus += QString::fromUtf8(Statement[j].parentTxnId);
                        DisplayTxnStatus += "                " + CrDrSign + QString::fromUtf8(Statement[j].chargeAmount) + "" + "  \n";
                    }
                }
                else
                {
#if DEBUG
                    qDebug()<<"splitSequence isnot equal to 1";
#endif
                    DisplayTxnStatus += "                            " + CrDrSign + QString::fromUtf8(Statement[j].transactionAmount) + "\n";

                    if(strcmp(Statement[j].StatusDesc, "Success") == 0)
                    {
#if DEBUG
                        qDebug()<<"StatusDesc == Success";
#endif
                        DisplayTxnStatus += (QString::fromUtf8(Statement[j].parentTxnId)) + "                " + CrDrSign + QString::fromUtf8(Statement[j].chargeAmount) + "    Success" + "  \n";
                    }
                    else if(strcmp(Statement[j].StatusDesc, "Fail") == 0 || strcmp(Statement[j].StatusDesc, "Failed") == 0)
                    {
#if DEBUG
                        qDebug()<<"StatusDesc == Fail";
#endif
                        DisplayTxnStatus += (QString::fromUtf8(Statement[j].parentTxnId)) + "                " + CrDrSign + QString::fromUtf8(Statement[j].chargeAmount) + "    Failed" + "  \n";
                    }
                    else if(strcmp(Statement[j].StatusDesc, "Pending") == 0)
                    {
#if DEBUG
                        qDebug()<<"StatusDesc == Pending";
#endif
                        DisplayTxnStatus += (QString::fromUtf8(Statement[j].parentTxnId)) + "                " + CrDrSign + QString::fromUtf8(Statement[j].chargeAmount) + "    Pending" + "  \n";
                    }
                    else
                    {
                        DisplayTxnStatus += (QString::fromUtf8(Statement[j].parentTxnId)) + "                " + CrDrSign + QString::fromUtf8(Statement[j].chargeAmount) + (QString::fromUtf8(Statement[j].StatusDesc)) + "  \n";
                    }
                    //  ui->listWidget_Reversal_data->addItem(DisplayTxnStatus);

                }

                TransStatusList<<DisplayTxnStatus;
                DisplayTxnStatus += "---------------------------------------------";
                ui->listWidget_Reversal_data->addItem(DisplayTxnStatus);
                DisplayTxnStatus.clear();
#if DEBUG
                        qDebug()<<"check == "<<DisplayTxnStatus;
#endif
            }
        }

        uidmessage::instance()->hide_Message();
        ui->stackedWidget->setCurrentIndex(41);
        return;

    }
    uidmessage::instance()->hide_Message();
    return;

}



void FPB::on_TS_ParentChild_listWidget_PG11_clicked(const QModelIndex &index)
{
    QString Display;
    //    char tempbuff[500];
    //    memset(tempbuff, 0, sizeof(tempbuff));

    int currentIndex = ui->TS_ParentChild_listWidget_PG11->currentRow();

    ui->TransactionStatus_listWidget_PG10->clear();

    int i = 0;
    if(currentIndex != 0)
    {
        i = (currentIndex -1);
        char Message1[100][200];
        memset(Message1,0,sizeof(Message1));
        currentIndex = currentIndex - 1;

        Display.clear();
        Display = ChildTransStatusList[currentIndex];

        int k = 0;
        sprintf(Message1[k], "%s", Display.toAscii().trimmed().data());
#if DEBUG
        qDebug()<<"QLIST Display == "<<Display;
#endif
        k++;

        if(i == currentIndex)
        {
            Display = "                                  ";
            sprintf(Message1[k], "%s", Display.toAscii().trimmed().data());
            k++;

            QString TransType = QString::fromUtf8(Statement[i].transactionType);


            if(TransType.contains("BILLPAY") == true)
            {
                if( !(strcmp(Statement[i].accountNo,"")==0) && !(strcmp(Statement[i].accountNo,"null")==0))
                {
                    Display.clear();
                    Display = "\nFrom A/C";
                    sprintf(Message1[k], "%-25s : %s", Display.toAscii().trimmed().data(), Statement[i].accountNo);
                    k++;
                }
#if DEBUG
                printf("Statement[i].transactionType == %s\n", Statement[i].transactionType);
#endif
                if( !(strcmp(Statement[i].rfuText2Desc,"")==0) && !(strcmp(Statement[i].rfuText2Desc,"")==0) && !(strcmp(Statement[i].rfuText2Val,"")==0) && !(strcmp(Statement[i].rfuText2Val,"null")==0))
                {
                    Display.clear();
                    Display = QString::fromUtf8(Statement[i].rfuText2Desc); // + " : "  + QString::fromUtf8(Statement[i].rfuNumeric3Val)) + "  \n";
                    sprintf(Message1[k], "%-25s : %s", Display.toAscii().trimmed().data(), Statement[i].rfuText2Val);
                    k++;
                }

                if( !(strcmp(Statement[i].rfuText5Val,"")==0) && !(strcmp(Statement[i].rfuText5Val,"null")==0))
                {
                    QString q1 = QString::fromUtf8(Statement[i].rfuText5Val);

                    bool check = q1.contains(",");
                    if(check == true)
                    {

                        QStringList split = q1.split(",");
                        QString label = split.at(0);
                        qDebug()<< "label == "<<label;
                        Display.clear();

                        QString iLabel = labelDB(label);

                        Display = (((iLabel == "") || (iLabel == "null"))? "Authenticator 1" : iLabel);
#if DEBUG
                        qDebug()<<"iLabel ==  "<<iLabel;
                        qDebug()<<"Display ==  "<<Display;
#endif
                        sprintf(Message1[k], "%-25s : %s", Display.toAscii().trimmed().data(), split.at(1).toAscii().trimmed().data());
                        k++;
                    }
                    else
                    {
                        Display.clear();
                        Display = QString::fromUtf8(Statement[i].rfuText5Desc);
                        sprintf(Message1[k], "%-25s : %s", "Authenticator 1", Statement[i].rfuText5Val);
                        k++;
                    }
                }
                if( !(strcmp(Statement[i].rfuText6Val,"")==0) && !(strcmp(Statement[i].rfuText6Val,"null")==0))
                {
                    QString q1 = QString::fromUtf8(Statement[i].rfuText6Val);

                    bool check = q1.contains(",");
                    if(check == true)
                    {

                        QStringList split = q1.split(",");
                        QString label = split.at(0);
                        qDebug()<< "label6 == "<<label;

                        Display.clear();
                        QString iLabel = labelDB(label);
                        Display = (((iLabel == "") || (iLabel == "null"))? "Authenticator 2" : iLabel);

                        //   Display = "Authenticator 2";
                        sprintf(Message1[k], "%-25s : %s", Display.toAscii().trimmed().data(), split.at(1).toAscii().trimmed().data());
                        k++;
                    }
                    else
                    {
                        Display.clear();
                        Display = QString::fromUtf8(Statement[i].rfuText6Desc);
                        sprintf(Message1[k], "%-25s : %s", "Authenticator 2", Statement[i].rfuText6Val);
                        k++;
                    }
                }
                if( !(strcmp(Statement[i].rfuText7Val,"")==0) && !(strcmp(Statement[i].rfuText7Val,"null")==0))
                {
                    QString q1 = QString::fromUtf8(Statement[i].rfuText7Val);

                    bool check = q1.contains(",");
                    if(check == true)
                    {

                        QStringList split = q1.split(",");
                        QString label = split.at(0);

                        Display.clear();
                        QString iLabel = labelDB(label);
                        Display = (((iLabel == "") || (iLabel == "null"))? "Authenticator 3" : iLabel);

                        //   Display = "Authenticator 3";

                        ///   Display = ((label == "")? "Authenticator 3" : label, split[1]);
                        sprintf(Message1[k], "%-25s : %s", Display.toAscii().trimmed().data(), split.at(1).toAscii().trimmed().data());
                        k++;
                    }
                    else
                    {
                        Display.clear();
                        Display = QString::fromUtf8(Statement[i].rfuText7Desc);
                        sprintf(Message1[k], "%-25s : %s", "Authenticator 3", Statement[i].rfuText7Val);
                        k++;
                    }
                }
            }
            else if(TransType.contains("AEPS") == true)
            {
#if DEBUG
            qDebug()<<"AEPS k == "<<k;
#endif

                if(strcmp(Statement[i].transactionType,"CASHWAEPSACQ")==0)
                {
                    if(strcmp(Statement[i].debitCredit, "Cr") == 0)
                    {
                        Display.clear();
                        Display = "To A/C";
                        sprintf(Message1[k], "%-25s : %s", Display.toAscii().trimmed().data(), (((strcmp(Statement[i].accountNo, "null")==0) || (strcmp(Statement[i].accountNo, "")==0)) ? "" : (Statement[i].accountNo)));
                        k++;
                    }
                    else
                    {
                        Display.clear();
                        Display = "From A/C"; //  + QString::fromUtf8(Statement[i].rfuText1Val) + "  \n";
                        sprintf(Message1[k], "%-25s : %s", Display.toAscii().trimmed().data(), (((strcmp(Statement[i].accountNo, "null")==0) || (strcmp(Statement[i].accountNo, "")==0)) ? "" : (Statement[i].accountNo)));
                        k++;
                    }
                }
                if( !(strcmp(Statement[i].rfuText1Val,"")==0) && !(strcmp(Statement[i].rfuText1Val,"null")==0))
                {
                    char temp_uid[13];
                    memset(temp_uid,0,sizeof(temp_uid));
                    sprintf(temp_uid,"********%s", &Statement[i].rfuText1Val[strlen (Statement[i].rfuText1Val) - 4]);

                    Display.clear();
                    Display = "Aadhar No.";
                    sprintf(Message1[k], "%-25s : %s", Display.toAscii().trimmed().data(), temp_uid);
                    k++;

                }

                if( !(strcmp(Statement[i].rfuText3Val,"")==0) && !(strcmp(Statement[i].rfuText3Val,"null")==0))
                {
                    Display.clear();
                    Display = "Bank Name";
                    sprintf(Message1[k], "%-25s : %s", Display.toAscii().trimmed().data(), Statement[i].rfuText3Val);
                    k++;
                }

                if( !(strcmp(Statement[i].rfuText4Val,"")==0) && !(strcmp(Statement[i].rfuText4Val,"null")==0))
                {
                    Display.clear();
                    Display = "Mobile No";
                    sprintf(Message1[k], "%-25s : %s", Display.toAscii().trimmed().data(), Statement[i].rfuText4Val);
                    k++;
                }

            }
            else
            {
                if( !(strcmp(Statement[i].accountNo,"")==0) && !(strcmp(Statement[i].accountNo,"null")==0))
                {
                    Display.clear();
                    Display = "\nFrom A/C"; //  + QString::fromUtf8(Statement[i].accountNo) + "  \n";
                    sprintf(Message1[k], "%-25s : %s", Display.toAscii().trimmed().data(), Statement[i].accountNo);
                    k++;

                }
                if( !(strcmp(Statement[i].rfuText1Val,"")==0) && !(strcmp(Statement[i].rfuText1Val,"null")==0))
                {
                    Display.clear();
                    Display = "To A/C"; //  + QString::fromUtf8(Statement[i].rfuText1Val) + "  \n";
                    sprintf(Message1[k], "%-25s : %s", Display.toAscii().trimmed().data(), Statement[i].rfuText1Val);
                    k++;

                }

                if( !(strcmp(Statement[i].rfuNumeric3Desc,"")==0) && !(strcmp(Statement[i].rfuNumeric3Desc,"")==0) && !(strcmp(Statement[i].rfuNumeric3Val,"")==0) && !(strcmp(Statement[i].rfuNumeric3Val,"null")==0))
                {
                    Display.clear();
                    Display = QString::fromUtf8(Statement[i].rfuNumeric3Desc); // + " : "  + QString::fromUtf8(Statement[i].rfuNumeric3Val)) + "  \n";
                    sprintf(Message1[k], "%-25s : %s", Display.toAscii().trimmed().data(), Statement[i].rfuNumeric3Val);
                    k++;

                }
                if( !(strcmp(Statement[i].rfuNumeric4Desc,"")==0) && !(strcmp(Statement[i].rfuNumeric4Desc,"")==0) && !(strcmp(Statement[i].rfuNumeric4Val,"")==0) && !(strcmp(Statement[i].rfuNumeric4Val,"null")==0))
                {
                    Display.clear();
                    Display = QString::fromUtf8(Statement[i].rfuNumeric4Desc); // + " : "  + QString::fromUtf8(Statement[i].rfuNumeric4Val)) + "  \n";
                    sprintf(Message1[k], "%-25s : %s", Display.toAscii().trimmed().data(), Statement[i].rfuNumeric4Val);
                    k++;

                }
                if( !(strcmp(Statement[i].rfuText1Desc,"")==0) && !(strcmp(Statement[i].rfuText1Desc,"")==0) && !(strcmp(Statement[i].rfuText1Val,"")==0) && !(strcmp(Statement[i].rfuText1Val,"null")==0))
                {
                    Display.clear();
                    Display = QString::fromUtf8(Statement[i].rfuText1Desc); // + " : "  + QString::fromUtf8(Statement[i].rfuText1Val)) + "  \n";
                    sprintf(Message1[k], "%-25s : %s", Display.toAscii().trimmed().data(), Statement[i].rfuText1Val);
                    k++;

                }
                if( !(strcmp(Statement[i].rfuText2Desc,"")==0) && !(strcmp(Statement[i].rfuText2Desc,"")==0) && !(strcmp(Statement[i].rfuText2Val,"")==0) && !(strcmp(Statement[i].rfuText2Val,"null")==0))
                {
                    Display.clear();
                    Display = QString::fromUtf8(Statement[i].rfuText2Desc); // + " : "  + QString::fromUtf8(Statement[i].rfuText2Val)) + "  \n";
                    sprintf(Message1[k], "%-25s : %s", Display.toAscii().trimmed().data(), Statement[i].rfuText2Val);
                    k++;

                }

                if( !(strcmp(Statement[i].rfuText3Desc,"")==0) && !(strcmp(Statement[i].rfuText3Desc,"")==0) && !(strcmp(Statement[i].rfuText3Val,"")==0) && !(strcmp(Statement[i].rfuText3Val,"null")==0))
                {
                    Display.clear();
                    Display = QString::fromUtf8(Statement[i].rfuText3Desc); // + " : "  + QString::fromUtf8(Statement[i].rfuText3Val)) + "  \n";
                    sprintf(Message1[k], "%-25s : %s", Display.toAscii().trimmed().data(), Statement[i].rfuText3Val);
                    k++;

                }

                if( !(strcmp(Statement[i].rfuText4Desc,"")==0) && !(strcmp(Statement[i].rfuText4Desc,"")==0) && !(strcmp(Statement[i].rfuText4Val,"")==0) && !(strcmp(Statement[i].rfuText4Val,"null")==0))
                {
                    Display.clear();
                    Display = QString::fromUtf8(Statement[i].rfuText4Desc); // + " : "  + (QString::fromUtf8(Statement[i].rfuText4Val))) + "  \n";
                    sprintf(Message1[k], "%-25s : %s", Display.toAscii().trimmed().data(), Statement[i].rfuText4Val);
                    k++;

                }

                if( !(strcmp(Statement[i].rfuText5Desc,"")==0) && !(strcmp(Statement[i].rfuText5Desc,"")==0) && !(strcmp(Statement[i].rfuText5Val,"")==0) && !(strcmp(Statement[i].rfuText5Val,"null")==0))
                {
                    Display.clear();
                    Display = QString::fromUtf8(Statement[i].rfuText5Desc); // + " : "  + (QString::fromUtf8(Statement[i].rfuText5Val))) + "  \n";
                    sprintf(Message1[k], "%-25s : %s", Display.toAscii().trimmed().data(), Statement[i].rfuText5Val);
                    k++;

                }

                if( !(strcmp(Statement[i].rfuText8Desc,"")==0) && !(strcmp(Statement[i].rfuText8Desc,"")==0) && !(strcmp(Statement[i].rfuText8Val,"")==0) && !(strcmp(Statement[i].rfuText8Val,"null")==0))
                {
                    Display.clear();
                    Display = QString::fromUtf8(Statement[i].rfuText8Desc); // + " : "  + (QString::fromUtf8(Statement[i].rfuText8Val))) + "  \n";
                    sprintf(Message1[k], "%-25s : %s", Display.toAscii().trimmed().data(), Statement[i].rfuText8Val);
                    k++;

                }
                if( !(strcmp(Statement[i].rfuText9Desc,"")==0) && !(strcmp(Statement[i].rfuText9Desc,"")==0) && !(strcmp(Statement[i].rfuText9Val,"")==0) && !(strcmp(Statement[i].rfuText9Val,"null")==0))
                {
                    Display.clear();
                    Display = QString::fromUtf8(Statement[i].rfuText9Desc); //+ " : "  + (QString::fromUtf8(Statement[i].rfuText9Val))) + "  \n";
                    sprintf(Message1[k], "%-25s : %s", Display.toAscii().trimmed().data(), Statement[i].rfuText9Val);
                    k++;

                }

            }

            if( !(strcmp(Statement[i].transactionDesc,"")==0) && !(strcmp(Statement[i].transactionDesc,"null")==0))
            {
                Display.clear();
                Display = "Remarks";//  + QString::fromUtf8(Statement[i].transactionDesc) + "  \n";
                sprintf(Message1[k], "%-25s : %s", Display.toAscii().trimmed().data(), Statement[i].transactionDesc);
                k++;

            }
            if( !(strcmp(Statement[i].transactionAmount,"")==0) && !(strcmp(Statement[i].transactionAmount,"null")==0))
            {
                Display.clear();
                Display = "Amount";//  + QString::fromUtf8(Statement[i].debitAmount) + "  \n";
                sprintf(Message1[k], "%-25s : %s", Display.toAscii().trimmed().data(), Statement[i].transactionAmount);
                k++;

            }

            if( !(strcmp(Statement[i].payoutAmount,"")==0) && !(strcmp(Statement[i].payoutAmount,"null")==0))
            {
                Display.clear();
                Display = "Payout";
                sprintf(Message1[k], "%-25s : %s", Display.toAscii().trimmed().data(), Statement[i].payoutAmount);
                k++;
            }
            if( !(strcmp(Statement[i].tdsAmount,"")==0) && !(strcmp(Statement[i].tdsAmount,"null")==0))
            {
                Display.clear();
                Display = "Tds";
                sprintf(Message1[k], "%-25s : %s", Display.toAscii().trimmed().data(), Statement[i].tdsAmount);
                k++;
            }
            if( !(strcmp(Statement[i].commissionAmount,"")==0) && !(strcmp(Statement[i].commissionAmount,"null")==0))
            {
                Display.clear();
                Display = "Commission";
                sprintf(Message1[k], "%-25s : %s", Display.toAscii().trimmed().data(), Statement[i].commissionAmount);
                k++;
            }

            //            if( !(strcmp(Statement[i].chargeAmount,"")==0) && !(strcmp(Statement[i].chargeAmount,"null")==0))
            //            {
            //                Display.clear();
            //                Display = "Charges"; //  + QString::fromUtf8(Statement[i].chargeAmount) + "  \n";
            //                sprintf(Message1[k], "%-25s : %s", Display.toAscii().trimmed().data(), Statement[i].chargeAmount);
            //                k++;

            //            }

#if Debug
            qDebug()<<"k == "<<k;
#endif

            Display.clear();
            Display = "---------------------------------------------";
            sprintf(Message1[k], "%s", Display.toAscii().trimmed().data());
            k++;
            for(int q=0;q<k;q++)
                ui->TransactionStatus_listWidget_PG10->addItem(Message1[q]);

        }
        ui->stackedWidget->setCurrentIndex(9);
    }
    return;
}

void FPB::on_TransactionStatus__exit_button_PG10_clicked() // TS09072019
{
    ui->TransactionStatus_listWidget_PG10->clear();
    uidmessage::instance()->hide_Message();
    if(Flag_TS_Page == 0)
        ui->stackedWidget->setCurrentIndex(41);
    else
        ui->stackedWidget->setCurrentIndex(10);
}

int FPB::TransactionStatus()
{
    char* jsondata;
    int ret = 0, iRet = 0;
    Json1::Value mainreq, RequestData;
    char buff1[100];
    memset(buff1,0,sizeof(buff1));
    iGenerateRequestID((char *)username,(unsigned char *)buff1);

    mainreq["RequestId"] =  buff1;
    mainreq["MethodId"] = MID_TRANSACTIONSTATUS;
    mainreq["TellerID"]= username;
    mainreq["SessionId"] = (char *)configLoginLeg2.ucSessionId;
    mainreq["TokenId"]= "";
    mainreq["IsEncrypt"]= false;
    mainreq["X_Auth_Token"] ="";
    mainreq["SessionExpiryTime"]= (char *)configLoginLeg2.ucSessionExpiryTime;
    mainreq["ChannelID"]= CHANNEL_ID_IMPS;
    mainreq["ProductCode"] = mainreq.null;
    mainreq["Amount"] = mainreq.null;

    RequestData["accountno"] = "";
    RequestData["txnseqno"] = "";
    RequestData["referenceno"] = "";
    RequestData["prttxnid"] = (char *)Trans_Status;

    buff_tmp.clear();
    buff_tmp = serialize1(Convertstr(RequestData.toStyledString()));

    mainreq["RequestData"] = buff_tmp.toAscii().data();


    jsondata = Convertstr(mainreq.toStyledString());
#if DEBUG
    qDebug()<<"------------->"<<jsondata;
#endif
    memset(gSendData,0,sizeof(gSendData));
    strncpy((char*)gSendData, jsondata, strlen(jsondata));


    /****************************Encryption function*********************************/
    //    if(!strcmp(ISENCEYPT,"true"))
    //    {
    //        int gSendData_len =strlen((char *)gSendData);
    //        RequestDetachEncryptAttach((char *)gSendData,2,gSendData_len);
    //    }
    /***************************************************************/

    memset(str_url,0,sizeof(str_url));
#ifdef PRODUCTION
    strcpy(str_url,URL_CREATION(TRANSACTION_STATUS_PROD));
#else
    strcpy(str_url,URL_CREATION((char *)TRANSACTION_STATUS));
#endif


    ret = NetworkConnect(0,str_url, 2);
    if(ret != FUNCTION_SUCCESS)
    {
        uidmessage::instance()->hide_Message();
        if(ret == RESPONSE_TIMEOUT)
        {
#if DEBUG
            qDebug()<<"Response Timeout";
#endif
        }
        return FUNCTION_FAILED;
    }


    /****************************Decryption function*********************************/

    //    if(!strcmp(ISENCEYPT,"true"))
    //    {
    //        int oBuff_len = strlen(oBuff);
    //#if DEBUG
    //        qDebug()<<"Length of buffer"<<oBuff_len;
    //        printf("Buffer%s\n",oBuff);
    //#endif
    //        ResponseDetachEncryptAttach(oBuff,2,oBuff_len);//TEMPORARY

    //        system("rm /usr/FINO/FPB/RESP/OUTPUT.TXT");
    //        FILE *fp2 = NULL;
    //        fp2 =fopen("/usr/FINO/FPB/RESP/OUTPUT.TXT","a");
    //        fwrite(oBuff,strlen((char *)oBuff),1,fp2);
    //        sleep(1);
    //        fclose(fp2);
    //    }

    /*******************************************************************************/

    iRet = TranStatus_Parsing();
    return iRet;


}



int FPB::TranStatus_Parsing(void) // TS09072019
{
    //  struct TxnStatement iZero = {0};
    //    Statement = (TxnStatement*)malloc(sizeof *Statement);
    //    *Statement = iZero;

    system("sed 's/\"ResponseData\":\"{/\"ResponseData\":{/g' /usr/FINO/FPB/RESP/OUTPUT.TXT > /tmp/1.txt");
    system("sed 's/}\",\"MessageId\"/},\"MessageId\"/g' /tmp/1.txt > /tmp/2.txt  ");
    system(" sed 's/\\\\//g' /tmp/2.txt > /usr/FINO/FPB/RESP/OUTPUT.TXT");

    system("cd /tmp/");
    system("rm /tmp/1.txt");
    system("rm /tmp/2.txt");


    Json1::Value root;
    root.clear();
    QSqlQuery query;
    Json1::Reader reader;
    std::ifstream ReadData("/usr/FINO/FPB/RESP/OUTPUT.TXT");
    bool parsedSuccess = reader.parse(ReadData,root,false);

    ReadData.close();

    if(not parsedSuccess)
    {
        qDebug()<<"FpsMonthlyPdsData File not Found";
        return FUNCTION_FAILED;
    }
    else
    {
        memset(&Merdetail,0,sizeof(Merdetail));
#if DEBUG
        qDebug()<<"RequestId   : "<<Convertstr(root["RequestId"].asString());
        qDebug()<<"DisplayMessage : "<<Convertstr(root["DisplayMessage"].asString());
        qDebug()<<"ResponseMessage    : "<<Convertstr(root["ResponseMessage"].asString());
        qDebug()<<"ResponseCode : "<<Convertstr(root["ResponseCode"].asString());
        qDebug()<<"MessageType : "<<Convertstr(root["MessageType"].asString());
        qDebug()<<"RespocountNo : "<<Convertstr(root["RespocountNo"].asString());
#endif
        strcpy(Merdetail.ucRequestId,Convertstr(root["RequestId"].asString()));
        strcpy(Merdetail.ucDisplayMessage,Convertstr(root["DisplayMessage"].asString()));
        strcpy(Merdetail.ucResponseMessage,Convertstr(root["ResponseMessage"].asString()));
        strcpy(Merdetail.ucResponseCode,Convertstr(root["ResponseCode"].asString()));
        if(strcmp(Merdetail.ucResponseCode,"0")==0)
        {
            query.clear();
            query.exec("delete from DETAILED_STATEMENT");
            query.clear();

            Json1::Value ResponseData;
            ResponseData.clear();
            ResponseData = root.get("ResponseData",0);
#if DEBUG
            qDebug()<<"Size of the ResponseData"<<ResponseData.size();
            printf("\nSize : %d\n",ResponseData.size());
#endif
            if(ResponseData.size()==0)
            {
#if DEBUG
                qDebug("\n1:Key Register Data Not Found\n");
#endif
                return FUNCTION_FAILED;
            }
            else
            {
                Json1::Value lstOfDashboardTxnList;
                lstOfDashboardTxnList.clear();
                lstOfDashboardTxnList = ResponseData.get("txnDetails",0);

                iTransStatus.iTxnSCount = 0;
                iTransStatus.iTxnSCount = lstOfDashboardTxnList.size();
                //  Statement =(TxnStatement*) malloc(sizeof(TxnStatement)*Statement->iTxnSCount);
#if DEBUG
                printf("iTransStatus.iTxnSCount = %d\n", iTransStatus.iTxnSCount);
#endif
                for(int i=0;i<lstOfDashboardTxnList.size();i++)
                {
#if DEBUG
                    // strcpy(Statement[i].product,Convertstr(lstOfDashboardTxnList[i]["product"].asString()));
                    qDebug()<<"Statement[i].product"<<Convertstr(lstOfDashboardTxnList[i]["product"].asString());
                    // strcpy(Statement[i].transactionType,Convertstr(lstOfDashboardTxnList[i]["transactionType"].asString()));
                    qDebug()<<"Statement[i].transactionType"<<Convertstr(lstOfDashboardTxnList[i]["transactionType"].asString());
                    // strcpy(Statement[i].channel,Convertstr(lstOfDashboardTxnList[i]["channel"].asString()));
                    qDebug()<<"Statement[i].channel"<<Convertstr(lstOfDashboardTxnList[i]["channel"].asString());
                    // strcpy(Statement[i].transactionDate,Convertstr(lstOfDashboardTxnList[i]["transactionDate"].asString()));
                    qDebug()<<"Statement[i].transactionDate"<<Convertstr(lstOfDashboardTxnList[i]["transactionDate"].asString());
                    // strcpy(Statement[i].customerNo,Convertstr(lstOfDashboardTxnList[i]["customerNo"].asString()));
                    qDebug()<<"Statement[i].customerNo"<<Convertstr(lstOfDashboardTxnList[i]["customerNo"].asString());
                    // strcpy(Statement[i].accountNo,Convertstr(lstOfDashboardTxnList[i]["accountNo"].asString()));
                    qDebug()<<"Statement[i].accountNo"<<Convertstr(lstOfDashboardTxnList[i]["accountNo"].asString());
                    // strcpy(Statement[i].transactionAmount,Convertstr(lstOfDashboardTxnList[i]["transactionAmount"].asString()));
                    qDebug()<<"Statement[i].transactionAmount"<<Convertstr(lstOfDashboardTxnList[i]["transactionAmount"].asString());
                    // strcpy(Statement[i].branchCode,Convertstr(lstOfDashboardTxnList[i]["branchCode"].asString()));
                    qDebug()<<"Statement[i].branchCode"<<Convertstr(lstOfDashboardTxnList[i]["branchCode"].asString());
                    // strcpy(Statement[i].userId,Convertstr(lstOfDashboardTxnList[i]["userId"].asString()));
                    qDebug()<<"Statement[i].userId"<<Convertstr(lstOfDashboardTxnList[i]["userId"].asString());
                    // strcpy(Statement[i].sequenceNumber,Convertstr(lstOfDashboardTxnList[i]["sequenceNumber"].asString()));
                    qDebug()<<"Statement[i].sequenceNumber"<<Convertstr(lstOfDashboardTxnList[i]["sequenceNumber"].asString());
                    // strcpy(Statement[i].referenceId,Convertstr(lstOfDashboardTxnList[i]["referenceId"].asString()));
                    qDebug()<<"Statement[i].referenceId"<<Convertstr(lstOfDashboardTxnList[i]["referenceId"].asString());
                    // strcpy(Statement[i].status,Convertstr(lstOfDashboardTxnList[i]["status"].asString()));
                    qDebug()<<"Statement[i].status"<<Convertstr(lstOfDashboardTxnList[i]["status"].asString());
                    // strcpy(Statement[i].reversalFlag,Convertstr(lstOfDashboardTxnList[i]["reversalFlag"].asString()));
                    qDebug()<<"Statement[i].reversalFlag"<<Convertstr(lstOfDashboardTxnList[i]["reversalFlag"].asString());
                    // strcpy(Statement[i].transactionDesc,Convertstr(lstOfDashboardTxnList[i]["transactionDesc"].asString()));
                    qDebug()<<"Statement[i].transactionDesc"<<Convertstr(lstOfDashboardTxnList[i]["transactionDesc"].asString());
                    // strcpy(Statement[i].time,Convertstr(lstOfDashboardTxnList[i]["time"].asString()));
                    qDebug()<<"Statement[i].time"<<Convertstr(lstOfDashboardTxnList[i]["time"].asString());
                    // strcpy(Statement[i].customerName,Convertstr(lstOfDashboardTxnList[i]["customerName"].asString()));
                    qDebug()<<"Statement[i].customerName"<<Convertstr(lstOfDashboardTxnList[i]["customerName"].asString());
                    // strcpy(Statement[i].customerMobile,Convertstr(lstOfDashboardTxnList[i]["customerMobile"].asString()));
                    qDebug()<<"Statement[i].customerMobile"<<Convertstr(lstOfDashboardTxnList[i]["customerMobile"].asString());
                    // strcpy(Statement[i].splitSequence,Convertstr(lstOfDashboardTxnList[i]["splitSequence"].asString()));
                    qDebug()<<"Statement[i].splitSequence"<<Convertstr(lstOfDashboardTxnList[i]["splitSequence"].asString());
                    // strcpy(Statement[i].debitCredit,Convertstr(lstOfDashboardTxnList[i]["debitCredit"].asString()));
                    qDebug()<<"Statement[i].debitCredit"<<Convertstr(lstOfDashboardTxnList[i]["debitCredit"].asString());
                    // strcpy(Statement[i].chargeAmount,Convertstr(lstOfDashboardTxnList[i]["chargeAmount"].asString()));
                    qDebug()<<"Statement[i].chargeAmount"<<Convertstr(lstOfDashboardTxnList[i]["chargeAmount"].asString());
                    // strcpy(Statement[i].payoutAmount,Convertstr(lstOfDashboardTxnList[i]["payoutAmount"].asString()));
                    qDebug()<<"Statement[i].payoutAmount"<<Convertstr(lstOfDashboardTxnList[i]["payoutAmount"].asString());
                    // strcpy(Statement[i].commissionAmount,Convertstr(lstOfDashboardTxnList[i]["commissionAmount"].asString()));
                    qDebug()<<"Statement[i].commissionAmount"<<Convertstr(lstOfDashboardTxnList[i]["commissionAmount"].asString());
                    // strcpy(Statement[i].tdsAmount,Convertstr(lstOfDashboardTxnList[i]["tdsAmount"].asString()));
                    qDebug()<<"Statement[i].tdsAmount"<<Convertstr(lstOfDashboardTxnList[i]["tdsAmount"].asString());
                    // strcpy(Statement[i].originalTxnSequence,Convertstr(lstOfDashboardTxnList[i]["originalTxnSequence"].asString()));
                    qDebug()<<"Statement[i].originalTxnSequence"<<Convertstr(lstOfDashboardTxnList[i]["originalTxnSequence"].asString());
                    // strcpy(Statement[i].parentTxnId,Convertstr(lstOfDashboardTxnList[i]["parentTxnId"].asString()));
                    qDebug()<<"Statement[i].parentTxnId"<<Convertstr(lstOfDashboardTxnList[i]["parentTxnId"].asString());
                    // strcpy(Statement[i].parentTxnAmount,Convertstr(lstOfDashboardTxnList[i]["parentTxnAmount"].asString()));
                    qDebug()<<"Statement[i].parentTxnAmount"<<Convertstr(lstOfDashboardTxnList[i]["parentTxnAmount"].asString());
                    // strcpy(Statement[i].parentTxnAmountCurrent,Convertstr(lstOfDashboardTxnList[i]["parentTxnAmountCurrent"].asString()));
                    qDebug()<<"Statement[i].parentTxnAmountCurrent"<<Convertstr(lstOfDashboardTxnList[i]["parentTxnAmountCurrent"].asString());
                    // strcpy(Statement[i].chargeAmountCurrent,Convertstr(lstOfDashboardTxnList[i]["chargeAmountCurrent"].asString()));
                    qDebug()<<"Statement[i].chargeAmountCurrent"<<Convertstr(lstOfDashboardTxnList[i]["chargeAmountCurrent"].asString());
                    // strcpy(Statement[i].transactionTypeName,Convertstr(lstOfDashboardTxnList[i]["transactionTypeName"].asString()));
                    qDebug()<<"Statement[i].transactionTypeName"<<Convertstr(lstOfDashboardTxnList[i]["transactionTypeName"].asString());
                    // strcpy(Statement[i].TransactionTypeProduct,Convertstr(lstOfDashboardTxnList[i]["TransactionTypeProduct"].asString()));
                    qDebug()<<"Statement[i].TransactionTypeProduct"<<Convertstr(lstOfDashboardTxnList[i]["TransactionTypeProduct"].asString());
                    // strcpy(Statement[i].TransactionTypeDescription,Convertstr(lstOfDashboardTxnList[i]["TransactionTypeDescription"].asString()));
                    qDebug()<<"Statement[i].TransactionTypeDescription"<<Convertstr(lstOfDashboardTxnList[i]["TransactionTypeDescription"].asString());
                    // strcpy(Statement[i].Status,Convertstr(lstOfDashboardTxnList[i]["Status"].asString()));
                    qDebug()<<"Statement[i].Status"<<Convertstr(lstOfDashboardTxnList[i]["Status"].asString());
                    // strcpy(Statement[i].StatusDesc,Convertstr(lstOfDashboardTxnList[i]["StatusDesc"].asString()));
                    qDebug()<<"Statement[i].StatusDesc"<<Convertstr(lstOfDashboardTxnList[i]["StatusDesc"].asString());
                    // strcpy(Statement[i].rfuDateDesc,Convertstr(lstOfDashboardTxnList[i]["rfuDateDesc"].asString()));
                    qDebug()<<"Statement[i].rfuDateDesc"<<Convertstr(lstOfDashboardTxnList[i]["rfuDateDesc"].asString());
                    // strcpy(Statement[i].rfuDateVal,Convertstr(lstOfDashboardTxnList[i]["rfuDateVal"].asString()));
                    qDebug()<<"Statement[i].rfuDateVal"<<Convertstr(lstOfDashboardTxnList[i]["rfuDateVal"].asString());
                    // strcpy(Statement[i].rfuNumeric1Desc,Convertstr(lstOfDashboardTxnList[i]["rfuNumeric1Desc"].asString()));
                    qDebug()<<"Statement[i].rfuNumeric1Desc"<<Convertstr(lstOfDashboardTxnList[i]["rfuNumeric1Desc"].asString());
                    // strcpy(Statement[i].rfuNumeric1Val,Convertstr(lstOfDashboardTxnList[i]["rfuNumeric1Val"].asString()));
                    qDebug()<<"Statement[i].rfuNumeric1Val"<<Convertstr(lstOfDashboardTxnList[i]["rfuNumeric1Val"].asString());
                    // strcpy(Statement[i].rfuNumeric2Desc,Convertstr(lstOfDashboardTxnList[i]["rfuNumeric2Desc"].asString()));
                    qDebug()<<"Statement[i].rfuNumeric2Desc"<<Convertstr(lstOfDashboardTxnList[i]["rfuNumeric2Desc"].asString());
                    // strcpy(Statement[i].rfuNumeric2Val,Convertstr(lstOfDashboardTxnList[i]["rfuNumeric2Val"].asString()));
                    qDebug()<<"Statement[i].rfuNumeric2Val"<<Convertstr(lstOfDashboardTxnList[i]["rfuNumeric2Val"].asString());
                    // strcpy(Statement[i].rfuNumeric3Desc,Convertstr(lstOfDashboardTxnList[i]["rfuNumeric3Desc"].asString()));
                    qDebug()<<"Statement[i].rfuNumeric3Desc"<<Convertstr(lstOfDashboardTxnList[i]["rfuNumeric3Desc"].asString());
                    // strcpy(Statement[i].rfuNumeric3Val,Convertstr(lstOfDashboardTxnList[i]["rfuNumeric3Val"].asString()));
                    qDebug()<<"Statement[i].rfuNumeric3Val"<<Convertstr(lstOfDashboardTxnList[i]["rfuNumeric3Val"].asString());
                    // strcpy(Statement[i].rfuNumeric4Desc,Convertstr(lstOfDashboardTxnList[i]["rfuNumeric4Desc"].asString()));
                    qDebug()<<"Statement[i].rfuNumeric4Desc"<<Convertstr(lstOfDashboardTxnList[i]["rfuNumeric4Desc"].asString());
                    // strcpy(Statement[i].rfuNumeric4Val,Convertstr(lstOfDashboardTxnList[i]["rfuNumeric4Val"].asString()));
                    qDebug()<<"Statement[i].rfuNumeric4Val"<<Convertstr(lstOfDashboardTxnList[i]["rfuNumeric4Val"].asString());
                    // strcpy(Statement[i].rfuText1Desc,Convertstr(lstOfDashboardTxnList[i]["rfuText1Desc"].asString()));
                    qDebug()<<"Statement[i].rfuText1Desc"<<Convertstr(lstOfDashboardTxnList[i]["rfuText1Desc"].asString());
                    // strcpy(Statement[i].rfuText1Val,Convertstr(lstOfDashboardTxnList[i]["rfuText1Val"].asString()));
                    qDebug()<<"Statement[i].rfuText1Val"<<Convertstr(lstOfDashboardTxnList[i]["rfuText1Val"].asString());
                    // strcpy(Statement[i].rfuText2Desc,Convertstr(lstOfDashboardTxnList[i]["rfuText2Desc"].asString()));
                    qDebug()<<"Statement[i].rfuText2Desc"<<Convertstr(lstOfDashboardTxnList[i]["rfuText2Desc"].asString());
                    // strcpy(Statement[i].rfuText2Val,Convertstr(lstOfDashboardTxnList[i]["rfuText2Val"].asString()));
                    qDebug()<<"Statement[i].rfuText2Val"<<Convertstr(lstOfDashboardTxnList[i]["rfuText2Val"].asString());
                    // strcpy(Statement[i].rfuText3Desc,Convertstr(lstOfDashboardTxnList[i]["rfuText3Desc"].asString()));
                    qDebug()<<"Statement[i].rfuText3Desc"<<Convertstr(lstOfDashboardTxnList[i]["rfuText3Desc"].asString());
                    // strcpy(Statement[i].rfuText3Val,Convertstr(lstOfDashboardTxnList[i]["rfuText3Val"].asString()));
                    qDebug()<<"Statement[i].rfuText3Val"<<Convertstr(lstOfDashboardTxnList[i]["rfuText3Val"].asString());
                    // strcpy(Statement[i].rfuText4Desc,Convertstr(lstOfDashboardTxnList[i]["rfuText4Desc"].asString()));
                    qDebug()<<"Statement[i].rfuText4Desc"<<Convertstr(lstOfDashboardTxnList[i]["rfuText4Desc"].asString());
                    // strcpy(Statement[i].rfuText4Val,Convertstr(lstOfDashboardTxnList[i]["rfuText4Val"].asString()));
                    qDebug()<<"Statement[i].rfuText4Val"<<Convertstr(lstOfDashboardTxnList[i]["rfuText4Val"].asString());
                    // strcpy(Statement[i].rfuText5Desc,Convertstr(lstOfDashboardTxnList[i]["rfuText5Desc"].asString()));
                    qDebug()<<"Statement[i].rfuText5Desc"<<Convertstr(lstOfDashboardTxnList[i]["rfuText5Desc"].asString());
                    // strcpy(Statement[i].rfuText5Val,Convertstr(lstOfDashboardTxnList[i]["rfuText5Val"].asString()));
                    qDebug()<<"Statement[i].rfuText5Val"<<Convertstr(lstOfDashboardTxnList[i]["rfuText5Val"].asString());
                    // strcpy(Statement[i].rfuText6Desc,Convertstr(lstOfDashboardTxnList[i]["rfuText6Desc"].asString()));
                    qDebug()<<"Statement[i].rfuText6Desc"<<Convertstr(lstOfDashboardTxnList[i]["rfuText6Desc"].asString());
                    // strcpy(Statement[i].rfuText6Val,Convertstr(lstOfDashboardTxnList[i]["rfuText6Val"].asString()));
                    qDebug()<<"Statement[i].rfuText6Val"<<Convertstr(lstOfDashboardTxnList[i]["rfuText6Val"].asString());
                    // strcpy(Statement[i].rfuText7Desc,Convertstr(lstOfDashboardTxnList[i]["rfuText7Desc"].asString()));
                    qDebug()<<"Statement[i].rfuText7Desc"<<Convertstr(lstOfDashboardTxnList[i]["rfuText7Desc"].asString());
                    // strcpy(Statement[i].rfuText7Val,Convertstr(lstOfDashboardTxnList[i]["rfuText7Val"].asString()));
                    qDebug()<<"Statement[i].rfuText7Val"<<Convertstr(lstOfDashboardTxnList[i]["rfuText7Val"].asString());
                    // strcpy(Statement[i].rfuText8Desc,Convertstr(lstOfDashboardTxnList[i]["rfuText8Desc"].asString()));
                    qDebug()<<"Statement[i].rfuText8Desc"<<Convertstr(lstOfDashboardTxnList[i]["rfuText8Desc"].asString());
                    // strcpy(Statement[i].rfuText8Val,Convertstr(lstOfDashboardTxnList[i]["rfuText8Val"].asString()));
                    qDebug()<<"Statement[i].rfuText8Val"<<Convertstr(lstOfDashboardTxnList[i]["rfuText8Val"].asString());
                    // strcpy(Statement[i].rfuText9Desc,Convertstr(lstOfDashboardTxnList[i]["rfuText9Desc"].asString()));
                    qDebug()<<"Statement[i].rfuText9Desc"<<Convertstr(lstOfDashboardTxnList[i]["rfuText9Desc"].asString());
                    // strcpy(Statement[i].rfuText9Val,Convertstr(lstOfDashboardTxnList[i]["rfuText9Val"].asString()));
                    qDebug()<<"Statement[i].rfuText9Val"<<Convertstr(lstOfDashboardTxnList[i]["rfuText9Val"].asString());
                    // strcpy(Statement[i].rfuText10Desc,Convertstr(lstOfDashboardTxnList[i]["rfuText10Desc"].asString()));
                    qDebug()<<"Statement[i].rfuText10Desc"<<Convertstr(lstOfDashboardTxnList[i]["rfuText10Desc"].asString());
                    // strcpy(Statement[i].rfuText10Val,Convertstr(lstOfDashboardTxnList[i]["rfuText10Val"].asString()));
                    qDebug()<<"Statement[i].rfuText10Val"<<Convertstr(lstOfDashboardTxnList[i]["rfuText10Val"].asString());
                    // strcpy(Statement[i].debitAmount,Convertstr(lstOfDashboardTxnList[i]["debitAmount"].asString()));
                    qDebug()<<"Statement[i].debitAmount"<<Convertstr(lstOfDashboardTxnList[i]["debitAmount"].asString());
                    // strcpy(Statement[i].creditAmount,Convertstr(lstOfDashboardTxnList[i]["creditAmount"].asString()));
                    qDebug()<<"Statement[i].creditAmount"<<Convertstr(lstOfDashboardTxnList[i]["creditAmount"].asString());
                    // strcpy(Statement[i].cGST,Convertstr(lstOfDashboardTxnList[i]["cGST"].asString()));
                    qDebug()<<"Statement[i].cGST"<<Convertstr(lstOfDashboardTxnList[i]["cGST"].asString());
                    // strcpy(Statement[i].sGST,Convertstr(lstOfDashboardTxnList[i]["sGST"].asString()));
                    qDebug()<<"Statement[i].sGST"<<Convertstr(lstOfDashboardTxnList[i]["sGST"].asString());
                    // strcpy(Statement[i].iGST,Convertstr(lstOfDashboardTxnList[i]["iGST"].asString()));
                    qDebug()<<"Statement[i].iGST"<<Convertstr(lstOfDashboardTxnList[i]["iGST"].asString());
                    // strcpy(Statement[i].uGST,Convertstr(lstOfDashboardTxnList[i]["uGST"].asString()));
                    qDebug()<<"Statement[i].uGST"<<Convertstr(lstOfDashboardTxnList[i]["uGST"].asString());
                    qDebug()<<"Statement[i].status"<<Convertstr(lstOfDashboardTxnList[i]["status"].asString()); // TS09072019
                    qDebug()<<"______________________________________________________";
#endif
                    query.clear();
                    query.prepare("INSERT INTO DETAILED_STATEMENT (TRANTYPE,CHANNEL ,DATE ,CUSTOMER_NO ,ACCOUNT_NO ,AMOUNT ,BRANCH ,USER_ID ,SEQUENCE_NO ,REFERENCE_NO ,REVERSAL_FLAG ,NARRATIVE ,TIME ,CUSTOMER_NAME ,CUSTOMER_MOBILE ,SPLIT_SEQUENCE ,DEBIT_CREDIT ,CHARGE_AMOUNT ,PAYOUT_AMOUNT ,COMMISSION_AMOUNT ,TDS_AMOUNT ,ORIGINAL_SEQUENCE_NO ,PARENT_TXN_ID ,PARENT_TXN_AMOUNT ,PARENT_TXN_AMOUNT_CUR ,CHARGE_AMOUNT_CUR  ,TRANSACTION_TYPE_NAME ,TRANSACTION_TYPE_PRODUCT ,TRANSACTION_TYPE_DESCRIPTION ,STATUS ,STATUS_DESC ,RFU_DATE_DESC ,RFU_DATE_VAL ,RFU_NUMERIC1_DESC ,RFU_NUMERIC1_VAL ,RFU_NUMERIC2_DESC ,RFU_NUMERIC2_VAL ,RFU_NUMERIC3_DESC ,RFU_NUMERIC3_VAL ,RFU_NUMERIC4_DESC ,RFU_NUMERIC4_VAL  ,RFU_TEXT1_DESC ,RFU_TEXT1_VAL ,RFU_TEXT2_DESC ,RFU_TEXT2_VAL ,RFU_TEXT3_DESC ,RFU_TEXT3_VAL, RFU_TEXT4_DESC ,RFU_TEXT4_VAL, RFU_TEXT5_DESC ,RFU_TEXT5_VAL, RFU_TEXT6_DESC ,RFU_TEXT6_VAL, RFU_TEXT7_DESC ,RFU_TEXT7_VAL, RFU_TEXT8_DESC ,RFU_TEXT8_VAL, RFU_TEXT9_DESC ,RFU_TEXT9_VAL, RFU_TEXT10_DESC ,RFU_TEXT10_VAL ,DEBIT_AMOUNT ,CREDIT_AMOUNT ,CLSTATUS) values (:TRANTYPE,:CHANNEL ,:DATE ,:CUSTOMER_NO ,:ACCOUNT_NO ,:AMOUNT ,:BRANCH ,:USER_ID ,:SEQUENCE_NO ,:REFERENCE_NO ,:REVERSAL_FLAG ,:NARRATIVE ,:TIME ,:CUSTOMER_NAME ,:CUSTOMER_MOBILE ,:SPLIT_SEQUENCE ,:DEBIT_CREDIT ,:CHARGE_AMOUNT ,:PAYOUT_AMOUNT ,:COMMISSION_AMOUNT ,:TDS_AMOUNT ,:ORIGINAL_SEQUENCE_NO ,:PARENT_TXN_ID ,:PARENT_TXN_AMOUNT ,:PARENT_TXN_AMOUNT_CUR ,:CHARGE_AMOUNT_CUR  ,:TRANSACTION_TYPE_NAME ,:TRANSACTION_TYPE_PRODUCT ,:TRANSACTION_TYPE_DESCRIPTION ,:STATUS ,:STATUS_DESC ,:RFU_DATE_DESC ,:RFU_DATE_VAL ,:RFU_NUMERIC1_DESC ,:RFU_NUMERIC1_VAL ,:RFU_NUMERIC2_DESC ,:RFU_NUMERIC2_VAL ,:RFU_NUMERIC3_DESC ,:RFU_NUMERIC3_VAL ,:RFU_NUMERIC4_DESC ,:RFU_NUMERIC4_VAL  ,:RFU_TEXT1_DESC ,:RFU_TEXT1_VAL ,:RFU_TEXT2_DESC ,:RFU_TEXT2_VAL ,:RFU_TEXT3_DESC ,:RFU_TEXT3_VAL, :RFU_TEXT4_DESC ,:RFU_TEXT4_VAL, :RFU_TEXT5_DESC ,:RFU_TEXT5_VAL, :RFU_TEXT6_DESC ,:RFU_TEXT6_VAL, :RFU_TEXT7_DESC ,:RFU_TEXT7_VAL, :RFU_TEXT8_DESC ,:RFU_TEXT8_VAL, :RFU_TEXT9_DESC ,:RFU_TEXT9_VAL, :RFU_TEXT10_DESC ,:RFU_TEXT10_VAL ,:DEBIT_AMOUNT ,:CREDIT_AMOUNT ,:CLSTATUS )"); //,:RFU_5_DESC ,:RFU_5_VAL ,:RFU_6_DESC ,:RFU_6_VAL ,:RFU_7_DESC ,:RFU_7_VAL ,:RFU_8_DESC ,:RFU_8_VAL ,:RFU_9_DESC ,:RFU_9_VAL ,:RFU_10_DESC ,:RFU_10_VAL)"); // TS09072019

                    query.bindValue(":TRANTYPE",Convertstr(lstOfDashboardTxnList[i]["transactionType"].asString()));
                    query.bindValue(":CHANNEL",Convertstr(lstOfDashboardTxnList[i]["channel"].asString()));
                    query.bindValue(":DATE",Convertstr(lstOfDashboardTxnList[i]["transactionDate"].asString()));
                    query.bindValue(":CUSTOMER_NO",Convertstr(lstOfDashboardTxnList[i]["customerNo"].asString()));
                    query.bindValue(":ACCOUNT_NO",Convertstr(lstOfDashboardTxnList[i]["accountNo"].asString()));
                    query.bindValue(":AMOUNT",Convertstr(lstOfDashboardTxnList[i]["transactionAmount"].asString()));
                    query.bindValue(":BRANCH",Convertstr(lstOfDashboardTxnList[i]["branchCode"].asString()));
                    query.bindValue(":USER_ID",Convertstr(lstOfDashboardTxnList[i]["userId"].asString()));
                    query.bindValue(":SEQUENCE_NO",Convertstr(lstOfDashboardTxnList[i]["sequenceNumber"].asString()));
                    query.bindValue(":REFERENCE_NO",Convertstr(lstOfDashboardTxnList[i]["referenceId"].asString()));
                    query.bindValue(":REVERSAL_FLAG",Convertstr(lstOfDashboardTxnList[i]["reversalFlag"].asString()));
                    query.bindValue(":NARRATIVE",Convertstr(lstOfDashboardTxnList[i]["transactionDesc"].asString()));
                    query.bindValue(":TIME",Convertstr(lstOfDashboardTxnList[i]["time"].asString()));
                    query.bindValue(":CUSTOMER_NAME",Convertstr(lstOfDashboardTxnList[i]["customerName"].asString()));

                    query.bindValue(":CUSTOMER_MOBILE",Convertstr(lstOfDashboardTxnList[i]["customerMobile"].asString()));
                    query.bindValue(":SPLIT_SEQUENCE",Convertstr(lstOfDashboardTxnList[i]["splitSequence"].asString()));
                    query.bindValue(":DEBIT_CREDIT",Convertstr(lstOfDashboardTxnList[i]["debitCredit"].asString()));
                    query.bindValue(":CHARGE_AMOUNT",Convertstr(lstOfDashboardTxnList[i]["chargeAmount"].asString()));
                    query.bindValue(":PAYOUT_AMOUNT",Convertstr(lstOfDashboardTxnList[i]["payoutAmount"].asString()));
                    query.bindValue(":COMMISSION_AMOUNT",Convertstr(lstOfDashboardTxnList[i]["commissionAmount"].asString()));
                    query.bindValue(":TDS_AMOUNT",Convertstr(lstOfDashboardTxnList[i]["tdsAmount"].asString()));
                    query.bindValue(":ORIGINAL_SEQUENCE_NO",Convertstr(lstOfDashboardTxnList[i]["originalTxnSequence"].asString()));
                    query.bindValue(":PARENT_TXN_ID",Convertstr(lstOfDashboardTxnList[i]["parentTxnId"].asString()));
                    query.bindValue(":PARENT_TXN_AMOUNT",Convertstr(lstOfDashboardTxnList[i]["parentTxnAmount"].asString()));
                    query.bindValue(":PARENT_TXN_AMOUNT_CUR",Convertstr(lstOfDashboardTxnList[i]["parentTxnAmountCurrent"].asString()));
                    query.bindValue(":CHARGE_AMOUNT_CUR",Convertstr(lstOfDashboardTxnList[i]["chargeAmountCurrent"].asString()));
                    query.bindValue(":TRANSACTION_TYPE_NAME",Convertstr(lstOfDashboardTxnList[i]["transactionTypeName"].asString()));
                    query.bindValue(":TRANSACTION_TYPE_PRODUCT",Convertstr(lstOfDashboardTxnList[i]["TransactionTypeProduct"].asString()));
                    query.bindValue(":TRANSACTION_TYPE_DESCRIPTION",Convertstr(lstOfDashboardTxnList[i]["TransactionTypeDescription"].asString()));
                    //     query.bindValue(":TRANSACTION_TYPE",Convertstr(lstOfDashboardTxnList[i]["NBINStatus"].asString())); ////

                    query.bindValue(":STATUS",Convertstr(lstOfDashboardTxnList[i]["Status"].asString()));
                    query.bindValue(":STATUS_DESC",Convertstr(lstOfDashboardTxnList[i]["StatusDesc"].asString()));
                    query.bindValue(":RFU_DATE_DESC",Convertstr(lstOfDashboardTxnList[i]["rfuDateDesc"].asString()));
                    query.bindValue(":RFU_DATE_VAL",Convertstr(lstOfDashboardTxnList[i]["rfuDateVal"].asString()));
                    query.bindValue(":RFU_NUMERIC1_DESC",Convertstr(lstOfDashboardTxnList[i]["rfuNumeric1Desc"].asString()));
                    query.bindValue(":RFU_NUMERIC1_VAL",Convertstr(lstOfDashboardTxnList[i]["rfuNumeric1Val"].asString()));
                    query.bindValue(":RFU_NUMERIC2_DESC",Convertstr(lstOfDashboardTxnList[i]["rfuNumeric2Desc"].asString()));
                    query.bindValue(":RFU_NUMERIC2_VAL",Convertstr(lstOfDashboardTxnList[i]["rfuNumeric2Val"].asString()));
                    query.bindValue(":RFU_NUMERIC3_DESC",Convertstr(lstOfDashboardTxnList[i]["rfuNumeric3Desc"].asString()));
                    query.bindValue(":RFU_NUMERIC3_VAL",Convertstr(lstOfDashboardTxnList[i]["rfuNumeric3Val"].asString()));
                    query.bindValue(":RFU_NUMERIC4_DESC",Convertstr(lstOfDashboardTxnList[i]["rfuNumeric4Desc"].asString()));
                    query.bindValue(":RFU_NUMERIC4_VAL",Convertstr(lstOfDashboardTxnList[i]["rfuNumeric4Val"].asString()));
                    query.bindValue(":RFU_TEXT1_DESC",Convertstr(lstOfDashboardTxnList[i]["rfuText1Desc"].asString()));
                    query.bindValue(":RFU_TEXT1_VAL",Convertstr(lstOfDashboardTxnList[i]["rfuText1Val"].asString()));
                    query.bindValue(":RFU_TEXT2_DESC",Convertstr(lstOfDashboardTxnList[i]["rfuText2Desc"].asString()));
                    query.bindValue(":RFU_TEXT2_VAL",Convertstr(lstOfDashboardTxnList[i]["rfuText2Val"].asString()));
                    query.bindValue(":RFU_TEXT3_DESC",Convertstr(lstOfDashboardTxnList[i]["rfuText3Desc"].asString()));
                    query.bindValue(":RFU_TEXT3_VAL",Convertstr(lstOfDashboardTxnList[i]["rfuText3Val"].asString()));

                    query.bindValue(":RFU_TEXT4_DESC",Convertstr(lstOfDashboardTxnList[i]["rfuText4Desc"].asString()));
                    query.bindValue(":RFU_TEXT4_VAL",Convertstr(lstOfDashboardTxnList[i]["rfuText4Val"].asString()));
                    query.bindValue(":RFU_TEXT5_DESC",Convertstr(lstOfDashboardTxnList[i]["rfuText5Desc"].asString()));
                    query.bindValue(":RFU_TEXT5_VAL",Convertstr(lstOfDashboardTxnList[i]["rfuText5Val"].asString()));
                    query.bindValue(":RFU_TEXT6_DESC",Convertstr(lstOfDashboardTxnList[i]["rfuText6Desc"].asString()));
                    query.bindValue(":RFU_TEXT6_VAL",Convertstr(lstOfDashboardTxnList[i]["rfuText6Val"].asString()));
                    query.bindValue(":RFU_TEXT7_DESC",Convertstr(lstOfDashboardTxnList[i]["rfuText7Desc"].asString()));
                    query.bindValue(":RFU_TEXT7_VAL",Convertstr(lstOfDashboardTxnList[i]["rfuText7Val"].asString()));
                    query.bindValue(":RFU_TEXT8_DESC",Convertstr(lstOfDashboardTxnList[i]["rfuText8Desc"].asString()));
                    query.bindValue(":RFU_TEXT8_VAL",Convertstr(lstOfDashboardTxnList[i]["rfuText8Val"].asString()));
                    query.bindValue(":RFU_TEXT9_DESC",Convertstr(lstOfDashboardTxnList[i]["rfuText9Desc"].asString()));
                    query.bindValue(":RFU_TEXT9_VAL",Convertstr(lstOfDashboardTxnList[i]["rfuText9Val"].asString()));
                    query.bindValue(":RFU_TEXT10_DESC",Convertstr(lstOfDashboardTxnList[i]["rfuText10Desc"].asString()));
                    query.bindValue(":RFU_TEXT10_VAL",Convertstr(lstOfDashboardTxnList[i]["rfuText10Val"].asString()));
                    query.bindValue(":DEBIT_AMOUNT",Convertstr(lstOfDashboardTxnList[i]["debitAmount"].asString()));
                    query.bindValue(":CREDIT_AMOUNT",Convertstr(lstOfDashboardTxnList[i]["creditAmount"].asString()));
                    query.bindValue(":CLSTATUS",Convertstr(lstOfDashboardTxnList[i]["status"].asString())); // TS09072019

                    //                    query.bindValue(":RFU1","");
                    //                    query.bindValue(":RFU2","");
                    //                    query.bindValue(":RFU3","");



                    if(!query.exec())
                    {
#if DEBUG
                        qDebug() << "In Insert_BM_Txn_DB = " << query.lastError();
#endif
                        query.clear();
                        query.finish();
                        return FUNC_ERROR;
                    }
                    query.clear();

                }
            }
            return FUNC_OK;
        }
        else
        {
            CMisc miscObj;
            char ErrorMsg[250];
            memset(ErrorMsg, 0, sizeof(ErrorMsg));
            if(strcmp((char *)Merdetail.ucResponseMessage,"Session Expired.")==0)
            {
                uidmessage::instance()->hide_Message();
                sprintf(ErrorMsg,"%s",(char *)Merdetail.ucResponseMessage);
                miscObj.DisplayMsgBox(ErrorMsg);
                uidmessage::instance()->hide_Message();
                return SESSION_EXPIRED_FAILED;
            }
            ui->TransactionID_TransStWdRRN_lineEdit_pg41->setFocus();
            sprintf(ErrorMsg,"%s  ",(char *)Merdetail.ucResponseMessage);
            miscObj.DisplayMsgBox(ErrorMsg);
            uidmessage::instance()->hide_Message();
            return FUNC_ERROR;
        }
    }


}









