#include "impscommon.h"
#include "ui_FPB.h"
#include "json.h"
#include "Login.h"
#include "displaymessage.h"
#include "print.h"
#include <QSqlDatabase>

CashOutOTPGen CashoutOTPGen;
extern Login_leg2 configLoginLeg2;


int FPB::Cashoutotp()
{

    HostComm hComm;
    CMisc miscObj;
    int WALK_len = 0, response_timeout1 = 120, ret = -3, res = -3, iRet = -3;
    char WALK_Buff[10000];
    char ErrorMsg[100];

      memset(ErrorMsg,0,sizeof(ErrorMsg));
      memset(CashoutOTPGen.Commdata_COG.ucMethodID_req,0, sizeof(CashoutOTPGen.Commdata_COG.ucMethodID_req));
      memset(CashoutOTPGen.Commdata_COG.ucTellerID_req,0, sizeof(CashoutOTPGen.Commdata_COG.ucTellerID_req));
      memset(CashoutOTPGen.Commdata_COG.ucSessionID_req,0, sizeof(CashoutOTPGen.Commdata_COG.ucSessionID_req));
      memset(CashoutOTPGen.Commdata_COG.ucIsEncrypt_req,0, sizeof(CashoutOTPGen.Commdata_COG.ucIsEncrypt_req));
       memset(CashoutOTPGen.Commdata_COG.ucChannelID_req,0, sizeof(CashoutOTPGen.Commdata_COG.ucChannelID_req));
       memset(CashoutOTPGen.uccustMobNo_COG,0, sizeof(CashoutOTPGen.uccustMobNo_COG));
      memset(CashoutOTPGen.Commdata_COG.ucSessionExpiryTime_req,0, sizeof(CashoutOTPGen.Commdata_COG.ucSessionExpiryTime_req));

       memset(CashoutOTPGen.ucMessageID_COG,0, sizeof(CashoutOTPGen.ucMessageID_COG));
        memset(CashoutOTPGen.ucAmount_COG,0, sizeof(CashoutOTPGen.ucAmount_COG));

       iGenerateRequestID((char *)username,(unsigned char *)&CashoutOTPGen.Commdata_COG.ucRequestID_req);
      memcpy(CashoutOTPGen.Commdata_COG.ucMethodID_req,"1",1);
      memcpy((char *)CashoutOTPGen.Commdata_COG.ucSessionID_req,configLoginLeg2.ucSessionId,strlen((char *)configLoginLeg2.ucSessionId));
      memcpy(CashoutOTPGen.Commdata_COG.ucTellerID_req,username,strlen((char *)username));
      memcpy(CashoutOTPGen.uccustMobNo_COG,"8898334504",10);//hardcoded
      memcpy(CashoutOTPGen.ucMessageID_COG,"459",3);//hardcoded
      memcpy(CashoutOTPGen.ucAmount_COG,"60",2);//hardcoded
      // memcpy(CashoutOTPGen.ucCustomerMob,"100020442",9);//hardcoded
      memcpy(CashoutOTPGen.Commdata_COG.ucIsEncrypt_req,"false",5);
      memcpy((char *)CashoutOTPGen.Commdata_COG.ucSessionExpiryTime_req,(char *)configLoginLeg2.ucSessionExpiryTime,strlen((char *)configLoginLeg2.ucSessionExpiryTime));

      ret	= makeJSON(17);
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
      memset(WALK_Buff,0,sizeof(WALK_Buff));
      if((hComm.host_2Sendrecv((unsigned char *)gSendData,len_gSendData,( unsigned char*)WALK_Buff,(int *)&WALK_len,(int)response_timeout1,0))!=SUCCESS)
      {
          printf("No Response From Server\n");
          hComm.close_connection();
          miscObj.DisplayMsgBox((char *)"Response Time-Out");
          uidmessage::instance()->hide_Message();
          qDebug()<<"***   Response Time-Out   ***";
          ui->stackedWidget->setCurrentIndex(1);
      }
      hComm.close_connection();


      QString WALKBuff = QString::fromLocal8Bit(WALK_Buff);
      bool check = WALKBuff.contains("ResponseCode");
      if(check == false)
      {
          miscObj.DisplayMsgBox((char *)"INVALID DATA ");
          uidmessage::instance()->hide_Message();
          return FUNCTION_FAILED;
      }


      WALKBuff.remove("\\");
      WALKBuff.remove("{");
      WALKBuff.remove("}");
      WALKBuff.remove('"');
      WALKBuff.remove("[");
      WALKBuff.remove("]");
      qDebug()<<"WALKBuff"<<WALKBuff;
      QStringList query = WALKBuff.split(",");
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
//       memcpy(WALK.ucRDRequestId,value.trimmed().toAscii().data(), strlen(value.trimmed().toAscii().data()));
//       qDebug()<<"WALK.ucRDRequestId"<<WALK.ucRDRequestId;
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



int FPB::iSetCASHOUTOTP(int tagID, unsigned char *value)
{

    switch(tagID)
    {
    case  1:
        strcpy((char *)value,(char *)CashoutOTPGen.Commdata_COG.ucRequestID_req);
        break;
    case  2:
        strcpy((char *)value,(char *)CashoutOTPGen.Commdata_COG.ucMethodID_req);//Method id
        break;
    case  3:
        strcpy((char *)value,(char *)CashoutOTPGen.Commdata_COG.ucSessionID_req);//teller id
        break;
    case  4:
        strcpy((char *)value,(char *)CashoutOTPGen.Commdata_COG.ucTellerID_req);//session key
        break;
    case  5:
        strcpy((char *)value,CashoutOTPGen.Commdata_COG.ucIsEncrypt_req);//token id
        break;
    case  6:
        strcpy((char *)value,(char *)CashoutOTPGen.uccustMobNo_COG);//is encrypt
        break;
    case  7:
        strcpy((char *)value,(char *)CashoutOTPGen.ucMessageID_COG);//start date
        break;
    case  8:
        strcpy((char *)value,(char *)CashoutOTPGen.ucAmount_COG);//start date
        break;
    case  9:
        strcpy((char *)value,(char *)CashoutOTPGen.Commdata_COG.ucSessionExpiryTime_req);//start date
        break;
    default:
        return FUNC_ERROR;
    }
    return FUNC_OK;

}



