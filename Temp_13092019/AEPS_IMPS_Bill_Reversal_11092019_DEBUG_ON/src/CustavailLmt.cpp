#include "impscommon.h"
#include "ui_FPB.h"
#include "json.h"
#include "Login.h"
#include "displaymessage.h"
#include "print.h"
#include <QSqlDatabase>

CustAvailbleLimit CustAvailLimit;
extern Login_leg2 configLoginLeg2;


int FPB::CustAvailLmt()
{

    HostComm hComm;
    CMisc miscObj;
    int Custlmt_len = 0, response_timeout1 = 120, ret = -3, res = -3, iRet = -3;
    char Custlmt_Buff[10000];
    char ErrorMsg[100];
      memset(ErrorMsg,0,sizeof(ErrorMsg));
      memset(CustAvailLimit.Commdata_CAL.ucMethodID_req,0, sizeof(CustAvailLimit.Commdata_CAL.ucMethodID_req));
      memset(CustAvailLimit.Commdata_CAL.ucTellerID_req,0, sizeof(CustAvailLimit.Commdata_CAL.ucTellerID_req));
      memset(CustAvailLimit.Commdata_CAL.ucSessionID_req,0, sizeof(CustAvailLimit.Commdata_CAL.ucSessionID_req));
      memset(CustAvailLimit.Commdata_CAL.ucIsEncrypt_req,0, sizeof(CustAvailLimit.Commdata_CAL.ucIsEncrypt_req));
       memset(CustAvailLimit.Commdata_CAL.ucChannelID_req,0, sizeof(CustAvailLimit.Commdata_CAL.ucChannelID_req));
      memset(CustAvailLimit.Commdata_CAL.ucSessionExpiryTime_req,0, sizeof(CustAvailLimit.Commdata_CAL.ucSessionExpiryTime_req));
       memset(CustAvailLimit.ucAccoutNo_CAL,0, sizeof(CustAvailLimit.ucAccoutNo_CAL));



       iGenerateRequestID((char *)username,(unsigned char *)&CustAvailLimit.Commdata_CAL.ucRequestID_req);
      memcpy(CustAvailLimit.Commdata_CAL.ucMethodID_req,"87",2);
      memcpy((char *)CustAvailLimit.Commdata_CAL.ucSessionID_req,configLoginLeg2.ucSessionId,strlen((char *)configLoginLeg2.ucSessionId));
      memcpy(CustAvailLimit.Commdata_CAL.ucTellerID_req,"11",2);
      memcpy(CustAvailLimit.ucAccoutNo_CAL,"20000132672",11);//hardcoded
     // memcpy(CustAvailLimit.ucAmount_COG,"60",2);//hardcoded
    //  memcpy(CustAvailLimit.ucMessageID_COG,"459",3);//hardcoded
      // memcpy(CustAvailLimit.ucCustomerMob,"100020442",9);//hardcoded
      memcpy(CustAvailLimit.Commdata_CAL.ucIsEncrypt_req,"false",5);
      memcpy((char *)CustAvailLimit.Commdata_CAL.ucSessionExpiryTime_req,(char *)configLoginLeg2.ucSessionExpiryTime,strlen((char *)configLoginLeg2.ucSessionExpiryTime));

      ret	= makeJSON(18);
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
      memset(Custlmt_Buff,0,sizeof(Custlmt_Buff));
      if((hComm.host_2Sendrecv((unsigned char *)gSendData,len_gSendData,( unsigned char*)Custlmt_Buff,(int *)&Custlmt_len,(int)response_timeout1,0))!=SUCCESS)
      {
          printf("No Response From Server\n");
          hComm.close_connection();
          miscObj.DisplayMsgBox((char *)"Response Time-Out");
          uidmessage::instance()->hide_Message();
          qDebug()<<"***   Response Time-Out   ***";
          ui->stackedWidget->setCurrentIndex(1);
      }
      hComm.close_connection();


      QString CustlmtBuff = QString::fromLocal8Bit(Custlmt_Buff);
      bool check = CustlmtBuff.contains("ResponseCode");
      if(check == false)
      {
          miscObj.DisplayMsgBox((char *)"Invalid data");
          uidmessage::instance()->hide_Message();
          return FUNCTION_FAILED;
      }


      CustlmtBuff.remove("\\");
      CustlmtBuff.remove("{");
      CustlmtBuff.remove("}");
      CustlmtBuff.remove('"');
      CustlmtBuff.remove("[");
      CustlmtBuff.remove("]");
      qDebug()<<"CustlmtBuff"<<CustlmtBuff;
      QStringList query = CustlmtBuff.split(",");
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
//       memcpy(Custlmt.ucRDRequestId,value.trimmed().toAscii().data(), strlen(value.trimmed().toAscii().data()));
//       qDebug()<<"Custlmt.ucRDRequestId"<<Custlmt.ucRDRequestId;
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



int FPB::iSetCUSTLMT(int tagID, unsigned char *value)
{
    switch(tagID)
        {
        case  1:
            strcpy((char *)value,(char *)CustAvailLimit.Commdata_CAL.ucRequestID_req);
            break;
        case  2:
            strcpy((char *)value,(char *)CustAvailLimit.Commdata_CAL.ucMethodID_req);//Method id
            break;
        case  3:
            strcpy((char *)value,(char *)CustAvailLimit.Commdata_CAL.ucSessionID_req);//teller id
            break;
        case  4:
            strcpy((char *)value,(char *)CustAvailLimit.Commdata_CAL.ucTellerID_req);//session key
            break;
        case  5:
            strcpy((char *)value,(char *)CustAvailLimit.ucAccoutNo_CAL);//token id
            break;
        case  6:
            strcpy((char *)value,(char *)CustAvailLimit.Commdata_CAL.ucIsEncrypt_req);//is encrypt
            break;
        case  7:
            strcpy((char *)value,(char *)CustAvailLimit.Commdata_CAL.ucSessionExpiryTime_req);//start date
            break;
        default:
                return FUNC_ERROR;
        }
        return FUNC_OK;
}




