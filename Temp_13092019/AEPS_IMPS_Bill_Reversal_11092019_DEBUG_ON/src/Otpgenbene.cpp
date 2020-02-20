#include "impscommon.h"
#include "ui_FPB.h"
#include "json.h"
#include "Login.h"
#include "displaymessage.h"
#include "print.h"
#include <QSqlDatabase>

OTPGenForBene OtpForBene;
extern Login_leg2 configLoginLeg2;


int FPB::Otpgenbene()
{

    HostComm hComm;
    CMisc miscObj;
    int Otpgen_len = 0, response_timeout1 = 120, ret = -3, res = -3, iRet = -3;
    char Otpgen_Buff[10000];
    char ErrorMsg[100];

      memset(ErrorMsg,0,sizeof(ErrorMsg));
      memset(OtpForBene.Commdata_OGB.ucMethodID_req,0, sizeof(OtpForBene.Commdata_OGB.ucMethodID_req));
      memset(OtpForBene.Commdata_OGB.ucTellerID_req,0, sizeof(OtpForBene.Commdata_OGB.ucTellerID_req));
      memset(OtpForBene.Commdata_OGB.ucSessionID_req,0, sizeof(OtpForBene.Commdata_OGB.ucSessionID_req));
      memset(OtpForBene.Commdata_OGB.ucIsEncrypt_req,0, sizeof(OtpForBene.Commdata_OGB.ucIsEncrypt_req));
       memset(OtpForBene.Commdata_OGB.ucChannelID_req,0, sizeof(OtpForBene.Commdata_OGB.ucChannelID_req));
       memset(OtpForBene.ucCustMobNo_OGB,0, sizeof(OtpForBene.ucCustMobNo_OGB));
       memset(OtpForBene.ucMessageID_OGB,0, sizeof(OtpForBene.ucMessageID_OGB));
       memset(OtpForBene.ucBenename_OGB,0, sizeof(OtpForBene.ucBenename_OGB));
       memset(OtpForBene.ucBeneAccNo_OGB,0, sizeof(OtpForBene.ucBeneAccNo_OGB));
       memset(OtpForBene.ucAccountNo_OGB,0, sizeof(OtpForBene.ucAccountNo_OGB));
      memset(OtpForBene.Commdata_OGB.ucSessionExpiryTime_req,0, sizeof(OtpForBene.Commdata_OGB.ucSessionExpiryTime_req));


      iGenerateRequestID((char *)username,(unsigned char *)&OtpForBene.Commdata_OGB.ucRequestID_req);
      memcpy(OtpForBene.Commdata_OGB.ucMethodID_req,"1",1);
      memcpy((char *)OtpForBene.Commdata_OGB.ucSessionID_req,configLoginLeg2.ucSessionId,strlen((char *)configLoginLeg2.ucSessionId));
      memcpy(OtpForBene.Commdata_OGB.ucTellerID_req,username,strlen((char *)username));
      memcpy(OtpForBene.ucCustMobNo_OGB,"8898334504",10);//hardcoded
      memcpy(OtpForBene.ucMessageID_OGB,"456",3);//hardcoded
      memcpy(OtpForBene.ucBenename_OGB,"dinesh",6);//hardcoded
      memcpy(OtpForBene.ucBeneAccNo_OGB,"96786787687",11);//hardcoded
      memcpy(OtpForBene.ucAccountNo_OGB,"20000110147",11);//hardcoded
      // memcpy(OtpForBene.ucCustomerMob,"100020442",9);//hardcoded
      memcpy(OtpForBene.Commdata_OGB.ucIsEncrypt_req,"false",5);
      memcpy((char *)OtpForBene.Commdata_OGB.ucSessionExpiryTime_req,(char *)configLoginLeg2.ucSessionExpiryTime,strlen((char *)configLoginLeg2.ucSessionExpiryTime));

      ret	= makeJSON(20);
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
      memset(Otpgen_Buff,0,sizeof(Otpgen_Buff));
      if((hComm.host_2Sendrecv((unsigned char *)gSendData,len_gSendData,( unsigned char*)Otpgen_Buff,(int *)&Otpgen_len,(int)response_timeout1,0))!=SUCCESS)
      {
          printf("No Response From Server\n");
          hComm.close_connection();
          miscObj.DisplayMsgBox((char *)"Response Time-Out");
          uidmessage::instance()->hide_Message();
          qDebug()<<"***   Response Time-Out   ***";
          ui->stackedWidget->setCurrentIndex(1);
      }
      hComm.close_connection();


      QString OtpgenBuff = QString::fromLocal8Bit(Otpgen_Buff);
      bool check = OtpgenBuff.contains("ResponseCode");
      if(check == false)
      {
          miscObj.DisplayMsgBox((char *)"Invalid Data");
          uidmessage::instance()->hide_Message();
          return FUNCTION_FAILED;
      }


      OtpgenBuff.remove("\\");
      OtpgenBuff.remove("{");
      OtpgenBuff.remove("}");
      OtpgenBuff.remove('"');
      OtpgenBuff.remove("[");
      OtpgenBuff.remove("]");
      qDebug()<<"OtpgenBuff"<<OtpgenBuff;
      QStringList query = OtpgenBuff.split(",");
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
//       memcpy(Otpgen.ucRDRequestId,value.trimmed().toAscii().data(), strlen(value.trimmed().toAscii().data()));
//       qDebug()<<"Otpgen.ucRDRequestId"<<Otpgen.ucRDRequestId;
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



int FPB::iSetOTPBENEGEN(int tagID, unsigned char *value)
{
    switch(tagID)
        {
        case  1:
            strcpy((char *)value,OtpForBene.Commdata_OGB.ucRequestID_req);
            break;
        case  2:
            strcpy((char *)value,OtpForBene.Commdata_OGB.ucMethodID_req);
            break;
        case  3:
            strcpy((char *)value,OtpForBene.Commdata_OGB.ucSessionID_req);
            break;
        case  4:
            strcpy((char *)value,OtpForBene.Commdata_OGB.ucTellerID_req);
            break;
        case  5:
            strcpy((char *)value,OtpForBene.Commdata_OGB.ucIsEncrypt_req);
            break;
        case  6:
            strcpy((char *)value,(char *)OtpForBene.ucCustMobNo_OGB);
            break;
        case  7:
            strcpy((char *)value,(char *)OtpForBene.ucMessageID_OGB);
            break;
        case  8:
            strcpy((char *)value,(char *)OtpForBene.ucBenename_OGB); //BankBin+adhaar no
            break;
        case  9:
            strcpy((char *)value,(char *)OtpForBene.ucBeneAccNo_OGB);
            break;
        case  10:
            strcpy((char *)value,(char *)OtpForBene.ucAccountNo_OGB);
            break;
        case  11:
            strcpy((char *)value,(char *)OtpForBene.Commdata_OGB.ucSessionExpiryTime_req);
            break;
            default:
            return FUNC_ERROR;
        }
    return FUNC_OK;
}



