#include "FPB.h"
#include "ui_FPB.h"
#include "json.h"
#include "Login.h"
#include "findcust.h"
#include "displaymessage.h"
#include "impscommon.h"
#include<fstream>
#include <QDebug>
QStringList tagnames;
extern Login_leg2 configLoginLeg2;
extern sCustDetails CustDet;

FINDCUSTOMER FINDCUST;
char FC_CustomerNumber[15];
char FC_MobileNumber[12];
char FC_AcceptorLocation[100];
char CUST_TYPE = 0;
int disp_acc_count = 0;
char Flag_IMPS_Walkin = '\0'; // Walkin created through bill payments but customer details not added using IMPS

void FPB::spiltfunction()
{
    QString data = "mstCurrency#2~mstGender#15~mstStates#29~mstMaritalStatus#1~mstEducationCodes#2~mstIncomeCode#2~mstSbuCode#2~mstRiskCode#1";
    QStringList PidData;
    PidData = data.split("~");
    qDebug()<<"data--->"<<PidData;
    QString data1 = PidData.at(1);
    QStringList PidData1 =  data1.split("#");
    qDebug()<<"real data--->"<<PidData1.at(0);
    qDebug()<<"real data--->"<<PidData1.at(1);
}

char * Convertstr(string str)
{
    char* cstr;
    string fname = str + '\0';
    cstr = new char [fname.size()+1];
    strcpy( cstr, fname.c_str());

    return cstr;
}

void FPB::TagNames(char *d)
{
    //    if(strcmp(d,"District") == 0)
    //    {
    //        tagnames.push_back("StateCode");
    //        tagnames.push_back("District");
    //        tagnames.push_back("CBSStateCode");
    //        tagnames.push_back("OrderID");
    //        tagnames.push_back("Lom_CityNumericCode");
    //    }
    //    if(strcmp(d,"Occupation") == 0)
    //    {
    //        tagnames.push_back("OCC");
    //        tagnames.push_back("OccupationName");
    //    }


    //"IINMasterMob", "ProductTransMap751", "mstRouteConfigDetails"

    printf("In TagNames = %s\n",d);

    if(strcmp(d,"IINMasterMob") == 0)
    {
        qDebug()<<"1";
        tagnames.push_back("BankCode");
        tagnames.push_back("BankName");
        tagnames.push_back("TypeOfBank");
        tagnames.push_back("IFSC");
        tagnames.push_back("MICR");
        tagnames.push_back("IIN");
    }
    //    if(strcmp(d,"ProductTransMap751") == 0)
    //    {
    //        qDebug()<<"2";
    //        tagnames.push_back("pTy");
    //        tagnames.push_back("TID");
    //        tagnames.push_back("Ty");
    //        tagnames.push_back("Nm");
    //        tagnames.push_back("OCC");
    //        tagnames.push_back("OccupationName");
    //    }

}

void FPB::commanfun(char *ims, char *table)
{
    qDebug()<<"ims";
    QString data ;
    QStringList finaldata;
    // TagNames("Occupation");
    TagNames(ims);

    int iRet;
    FILE *fp1;
    int ret=-3, contents_len=0;
    unsigned char  *contents;

    char ucResCode[4],ucResMessage[50];
    memset(ucResCode,    0, sizeof(ucResCode));
    memset(ucResMessage, 0, sizeof(ucResMessage));
    system("cd /usr/FINO/FPB/REQ/");
    fp1 = fopen("/usr/FINO/FPB/REQ/GPABM.TXT", "rb"); // open
    if (fp1 == NULL)
    {
        qDebug()<<"Can't OpenARBP";
        //return FUNCTION_FAILED;
        return ;
    }
    fseek(fp1, 0, SEEK_END);
    contents_len = ftell(fp1);
    fseek(fp1, 0, SEEK_SET);
    contents = (unsigned char *)malloc(contents_len);
    if (contents == NULL)
    {
        free(contents);
        fclose(fp1);
        printf("\n in testbio memeory alloc failed");
        return ;
    }
    ret=fread(contents,sizeof(unsigned char ),contents_len,fp1);
    if(ret==0)
        fclose(fp1);
    qDebug()<<"contents"<<contents;
    qDebug()<<"contents_len"<<contents_len;


    iRet = json->iIsJsonValid((char *)contents,(char *)ucResCode,(char *)ucResMessage,(int *)BANKMASTER);
    qDebug()<<"After Json valid";
    if ((memcmp(ucResCode,"0",1)!=0) || (iRet != FUNC_OK))
    {
        qDebug()<<"error";
        free(contents);
        return ;
    }
    memset(JSONTagBuff,0,sizeof(JSONTagBuff));

    system("rm /usr/FINO/FPB/REQ/GPABM.TXT");
    int gRecv_len = strlen((char *)contents);
    FILE *fp2 = NULL;
    fp2 = fopen("/usr/FINO/FPB/REQ/GPABM.TXT","a");
    qDebug()<<"fILE"<<fp2;
    if(fp2 == NULL)
    {
        return;
    }

    fwrite( contents, 1, gRecv_len, fp2 );
    fclose(fp2);

    free(contents);
    qDebug()<<"After the free content";
    //    system(" sed 's/\\\\//g' /home/thirupathi/Desktop/fino/master.txt > /home/thirupathi/Desktop/fino/master.json");

    system(" sed 's/\\\\//g' /usr/FINO/FPB/REQ/GPABM.TXT > /tmp/master.json");
    string line;
    ifstream myfile ("/tmp/master.json");
    if (myfile.is_open())
    {
        while ( getline (myfile,line) )
        {
            //cout << line << endl;
        }
        myfile.close();
    }


    size_t found = line.find("\"ResponseData\": [{\"");
    qDebug()<<"found--------->"<<found;
    if (found != string::npos)
    {
        qDebug() << "First occurrence is " << found;
        //system("cp /tmp/loginres.txt /mnt/jffs2/ITDA/tmp/loginResp.txt");
    }
    else
    {
        qDebug() << "First occurrence is-------> " << found;
        system("sed 's/ResponseData\": \"{/ResponseData\":[{/g' /tmp/master.json > /tmp/1.json");
        system("sed 's/\"}\",/\"}],/g' /tmp/1.json > /usr/FINO/FPB/RESP/GPABM.json");
    }

    Json1::Value root;
    root.clear();
    //QSqlQuery query;
    Json1::Reader reader;
    std::ifstream ReadData("/usr/FINO/FPB/RESP/GPABM.json");
    bool parsedSuccess = reader.parse(ReadData,root,false);
    ReadData.close();
    if(not parsedSuccess)
    {
        qDebug()<<" File not Found";

        //return -1;
    }
    else
    {
        qDebug()<<"RequestId   : "<<Convertstr(root["RequestId"].asString());
        qDebug()<<"respMessage : "<<Convertstr(root["DisplayMessage"].asString());
        qDebug()<<"respCode    : "<<Convertstr(root["ResponseMessage"].asString());
        qDebug()<<"respMessage : "<<Convertstr(root["ResponseCode"].asString());

        //
        /////////////////////////////////////////////////all same /////////////////////

        Json1::Value ResponseData;
        ResponseData.clear();
        ResponseData = root.get("ResponseData",0);
        printf("\nSize : %d\n",ResponseData.size());
        if(ResponseData.size()==0)
        {
            qDebug("\n1:Key Register Data Not Found\n");
            return;
        }
        else
        {
            for(int i=0;i<ResponseData.size();i++)
            {
                qDebug()<<"versionID : "<<Convertstr(ResponseData[i]["versionID"].asString());
                qDebug()<<"MastersVersion : "<<Convertstr(ResponseData[i]["MastersVersion"].asString());

                Json1::Value ResponseData1;
                ResponseData1 = ResponseData[i].get("CacheMaster",0);
                qDebug()<<"------->"<<ResponseData1.size();

                for(int j=0;j<ResponseData1.size();j++)
                {




                    int k;
                    for(k =0 ;k<tagnames.length();k++)
                    {
                        data = tagnames.at(k);
                        finaldata.push_back(Convertstr(ResponseData1[j][data.toAscii().data()].asString()));

                    }
                    QString TmpStr;
                    TmpStr.clear();
                    TmpStr = "insert into";
                    TmpStr = QString::fromLocal8Bit(table);
                    TmpStr = " values(";
                    for(int ij=0,ik=0;ik<finaldata.size();ij+=k+1,ik+=k+1)
                    {

                        for(int n = ij;n<finaldata.size();n++)
                        {

                            TmpStr+="'"+finaldata.at(n)+"'"+",";
                        }
                        TmpStr = TmpStr.left(TmpStr.length() - 1);
                        TmpStr+=")";
                        qDebug()<<"-------------->"<<TmpStr;
                    }
                    finaldata.clear();
                }

            }

        }

    }
}


int FPB::findcust(int fc_input/*char *buff_FC, int bufflen_FC*/)
{
    /*    int bufflen_FC1 = strlen(buff_FC);
    printf("bufflen_FC1 = %d , buff_FC = %s\n",bufflen_FC1,buff_FC);
    system("rm /usr/FINO/FPB/RESP/FINDCUST.TXT");
    FILE *fp2 = NULL;
    fp2 = fopen("/usr/FINO/FPB/RESP/FINDCUST.TXT","a");
    qDebug()<<"fILE"<<fp2;
    if(fp2 == NULL)
    {
        return;
    }

    fwrite( buff_FC, 1, bufflen_FC1, fp2 );
    fclose(fp2);                qDebug()<<"Inside the json Vt";

*/
    CMisc miscObj;
    char ErrorMsg[250];
    char cResCode[10+1];
    memset(cResCode, 0, sizeof(cResCode));
    memset(ErrorMsg, 0, sizeof(ErrorMsg));
    system("rm /usr/FINO/FPB/RESP/FINDCUST1.json");
    system("sed 's/\"ResponseData\":\"{/\"ResponseData\":{/g' /usr/FINO/FPB/RESP/FINDCUST.TXT > /tmp/1.txt");
    system("sed 's/}\",\"MessageId\"/},\"MessageId\"/g' /tmp/1.txt > /tmp/2.txt  ");
    system(" sed 's/\\\\//g' /tmp/2.txt > /usr/FINO/FPB/RESP/FINDCUST1.json");

    string line;
    //ifstream myfile ("/home/thirupathi/Desktop/FC_RESP1.json");
    ifstream myfile ("/usr/FINO/FPB/RESP/FINDCUST1.json");
    if (myfile.is_open())
    {
        while ( getline (myfile,line) )
        {
            //cout << line << endl;
        }
        myfile.close();
    }


    //    size_t found = line.find("\"ResponseData\":\"{");
    //    if (found != string::npos)
    //    {
    //        qDebug() << "First occurrence is " << found;
    //        //system("cp /tmp/loginres.txt /mnt/jffs2/ITDA/tmp/loginResp.txt");
    //    }
    //    else
    {
        system("sed 's/\"{\"/{\"/g' /usr/FINO/FPB/RESP/FINDCUST1.json > /tmp/fc.json");
        system("sed 's/\"}\"/\"}/g' /tmp/fc.json > /tmp/fc1.json");

        system("sed 's/\"}}\"/\"}}/g' /tmp/fc1.json > /usr/FINO/FPB/RESP/FINDCUST1.json");

        //system("sed 's/ResponseData\":\"{/ResponseData\":[{/g' /home/thirupathi/Desktop/FC_RESP1.json > /home/1.txt");
        //system("sed 's/]}}\",\"/]}}],\"/g' /home/1.txt > /home/thirupathi/Desktop/FC_RESP1.json");
    }


    //"ResponseData":"{
    //]}}","
    Json1::Value root;
    root.clear();
    //QSqlQuery query;
    Json1::Reader reader;
    //std::ifstream ReadData("/home/thirupathi/Desktop/FC_RESP1.json");
    std::ifstream ReadData("/usr/FINO/FPB/RESP/FINDCUST1.json");
    bool parsedSuccess = reader.parse(ReadData,root,false);
    printf("parsedSuccess == %c\n",parsedSuccess);
    ReadData.close();
    if(not parsedSuccess)
    {
        qDebug()<<" File not Found";

        return FUNCTION_FAILED;
    }
    else
    {
#if DEBUG
        qDebug()<<"RequestId   : "<<Convertstr(root["RequestId"].asString());
        qDebug()<<"DisplayMessage : "<<Convertstr(root["DisplayMessage"].asString());
        qDebug()<<"respCode    : "<<Convertstr(root["ResponseCode"].asString());
        qDebug()<<"respMessage : "<<Convertstr(root["ResponseMessage"].asString());
        qDebug()<<"MessageType : "<<Convertstr(root["MessageType"].asString());
 #endif
        strcpy(cResCode,Convertstr(root["ResponseCode"].asString()));

    }


    if(!strcmp((const char*)Convertstr(root["ResponseCode"].asString()),"1"))
    {
#if DEBUG
        qDebug()<<"Walking Customer Response Code = "<<Convertstr(root["ResponseCode"].asString());
#endif
       // if(strcmp((const char*)Convertstr(root["ResponseMessage"].asString()),"Record Not found") == 0) // After Discusssion with Dinesh 12062019
        if(strcmp((const char*)Convertstr(root["ResponseData"]["returnCode"].asString()),"100") == 0) // After Discusssion with Dinesh 12062019
        {

            qDebug()<<"*********      In Record Not Found !      *********";

            memset(FC_MobileNumber, 0, sizeof(FC_MobileNumber));
            memset(FC_CustomerNumber, 0, sizeof(FC_CustomerNumber));

            memcpy(FC_MobileNumber, (char *)ui->Search_lineEdit->text().trimmed().toAscii().data(), strlen((const char*)ui->Search_lineEdit->text().trimmed().toAscii().data()));
#if DEBUG
            printf("FC_MobileNumber = %s\n",FC_MobileNumber);
#endif
            memcpy(FC_CustomerNumber,(char*)"999999999999", 12);
#if DEBUG
            printf("FC_CustomerNumber = %s\n",FC_CustomerNumber);
#endif

            //            memcpy((char*)FINDCUST.ucRDCLmobilePhoneNumber, (char *)ui->Search_lineEdit->text().trimmed().toAscii().data(), strlen(ui->Search_lineEdit->text().trimmed().toAscii().data()));
            //            printf("FINDCUST.ucRDCLmobilePhoneNumber = %s\n",FINDCUST.ucRDCLmobilePhoneNumber);
            disp_acc_count = 0;
            CUST_TYPE = 'W';
            return FUNC_NEW_CUST;
        }
        else
            CUST_TYPE = '\0';

    }
    else
    {
#if DEBUG
        qDebug()<<"CASA Customer Response Code = "<<Convertstr(root["ResponseCode"].asString());
#endif
        CUST_TYPE = 'C';
        //        return CASA;
    }

    CUST_TYPE = '\0';

    if(memcmp(cResCode,"0",1)==0)
    {
        Json1::Value ResponseData,fpRequestData,optReqData;
        char BuffCardAdr[100], Buff_Final[200];
        ResponseData.clear();
        ResponseData = root.get("ResponseData",0);
        printf("\nSize : %d\n",ResponseData.size());
        if(ResponseData.size()==0)
        {
            qDebug("\n1:Key Register Data Not Found\n");
            uidmessage::instance()->hide_Message();

            if(strcmp((const char*)Convertstr(root["ResponseMessage"].asString()),"")!=0)
            {

                memset(ErrorMsg, 0, sizeof(ErrorMsg));
                sprintf(ErrorMsg,"%s",(char *)Convertstr(root["ResponseMessage"].asString()));
                miscObj.DisplayMsgBox(ErrorMsg);
                if(strcmp((char *)Convertstr(root["ResponseMessage"].asString()),"Session Expired.")==0)
                {
                    uidmessage::instance()->hide_Message();
                    return SESSION_EXPIRED_FAILED;
                }
            }
            return FUNCTION_FAILED;
        }
        else
        {
            // for(int i=0;i<ResponseData.size();i++)
            {
 #if DEBUG
                qDebug()<<"returnCode:"<<Convertstr(ResponseData["returnCode"].asString());
#endif
                Json1::Value ResponseData1;
                ResponseData1 = ResponseData.get("customerList",0);
 #if DEBUG
                qDebug()<<"------->"<<ResponseData1.size();
 #endif

                Json1::Value ArrayList(Json1::arrayValue);
                for(int j=0;j<ResponseData1.size();j++)
                {
                    memset(FINDCUST.ucRDCLfullName_resp, 0, sizeof(FINDCUST.ucRDCLfullName_resp));
                    memset(FINDCUST.ucRDCLnumber_resp, 0, sizeof(FINDCUST.ucRDCLnumber_resp));
                    memset(FINDCUST.ucRDCMAaddressLine1, 0, sizeof(FINDCUST.ucRDCMAaddressLine1));
                    memset(FINDCUST.ucRDCMAaddressLine2, 0, sizeof(FINDCUST.ucRDCMAaddressLine2));
                    memset(FINDCUST.ucRDCMAcity, 0, sizeof(FINDCUST.ucRDCMAcity));
                    memset(FINDCUST.ucRDCMAstate, 0, sizeof(FINDCUST.ucRDCMAstate));
                    memset(FINDCUST.ucRDCMAcountry, 0, sizeof(FINDCUST.ucRDCMAcountry));
                    memset(FINDCUST.ucRDCLmobilePhoneNumber, 0, sizeof(FINDCUST.ucRDCLmobilePhoneNumber));



                    strcpy((char*)FINDCUST.ucRDCLfullName_resp,(const char*)Convertstr(ResponseData1[j]["fullName"].asString()));
                    strcpy((char*)FINDCUST.ucRDCLnumber_resp,(const char*)Convertstr(ResponseData1[j]["number"].asString()));





                    strcpy((char*)FINDCUST.ucRDCMAaddressLine1,(const char*)Convertstr(ResponseData1[j]["mailingAddress"]["addressLine1"].asString()));
                    strcpy((char*)FINDCUST.ucRDCMAaddressLine2,(const char*)Convertstr(ResponseData1[j]["mailingAddress"]["addressLine2"].asString()));
                    strcpy((char*)FINDCUST.ucRDCMAcity,(const char*)Convertstr(ResponseData1[j]["mailingAddress"]["city"].asString()));
                    strcpy((char*)FINDCUST.ucRDCMAstate,(const char*)Convertstr(ResponseData1[j]["mailingAddress"]["state"].asString()));
                    strcpy((char*)FINDCUST.ucRDCMAcountry,(const char*)Convertstr(ResponseData1[j]["mailingAddress"]["country"].asString()));

                    memcpy((char*)FINDCUST.ucRDCLmobilePhoneNumber,(const char*)Convertstr(ResponseData1[j]["mobilePhoneNumber"].asString()),strlen((const char*)Convertstr(ResponseData1[j]["mobilePhoneNumber"].asString())));
                    strcpy((char*)FINDCUST.ucRDCLnumber,(const char*)Convertstr(ResponseData1[j]["number"].asString()));

                    strcpy((char*)FINDCUST.ucRDCLaadharNumber,(const char*)Convertstr(ResponseData1[j]["aadharNumber"].asString())); // 02052019

                   // memclear((char *)FINDCUST.ucRDCLtaxIdNumber);
                    memset(FINDCUST.ucRDCLtaxIdNumber, 0, sizeof((char *)FINDCUST.ucRDCLtaxIdNumber));

                    memcpy((char*)FINDCUST.ucRDCLtaxIdNumber,(const char*)Convertstr(ResponseData1[j]["taxIdNumber"].asString()), strlen((const char*)Convertstr(ResponseData1[j]["taxIdNumber"].asString())));

                    Json1::Value ResponseData2;
                    ResponseData2 = ResponseData1[j].get("accounts",0);
                    disp_acc_count = 0;



#if DEBUG

                    printf("FINDCUST.ucRDCLnumber_resp = %s\n",FINDCUST.ucRDCLnumber_resp);
                    printf("FINDCUST.ucRDCMAaddressLine1 = %s\n",FINDCUST.ucRDCMAaddressLine1);
                    printf("FINDCUST.ucRDCMAaddressLine2 = %s\n",FINDCUST.ucRDCMAaddressLine2);
                    printf("FINDCUST.ucRDCMAcity = %s\n",FINDCUST.ucRDCMAcity);
                    printf("FINDCUST.ucRDCMAstate = %s\n",FINDCUST.ucRDCMAstate);
                    printf("FINDCUST.ucRDCMAcountry = %s\n",FINDCUST.ucRDCMAcountry);
                    printf("FINDCUST.ucRDCLmobilePhoneNumber = %s\n",FINDCUST.ucRDCLmobilePhoneNumber);
                    printf("FINDCUST.ucRDCLnumber = %s\n",FINDCUST.ucRDCLnumber);
                    printf("FINDCUST.ucRDCLaadharNumber = %s\n",FINDCUST.ucRDCLaadharNumber); // 02052019
                    printf("FINDCUST.ucRDCLtaxIdNumber = %s\n",FINDCUST.ucRDCLtaxIdNumber);
                    qDebug()<<"fullName:"<<Convertstr(ResponseData1[j]["fullName"].asString());
                    qDebug()<<"addressLine1:"<<Convertstr(ResponseData1[j]["mailingAddress"]["addressLine1"].asString());

                    qDebug()<<"taxIdNumber:"<<Convertstr(ResponseData1[j]["taxIdNumber"].asString());
                    qDebug()<<"primaryCustomerNumber:"<<Convertstr(ResponseData2[j]["primaryCustomerNumber"].asString());

#endif

                    memset((char *)FINDCUST.ucRDCLdescription, 0, sizeof((char *)FINDCUST.ucRDCLdescription));
                    memset((char *)FINDCUST.ucRDCLunclearedBalance, 0, sizeof((char *)FINDCUST.ucRDCLunclearedBalance));
                    memset((char *)FINDCUST.ucRDCLACCnumber, 0, sizeof((char *)FINDCUST.ucRDCLACCnumber));
                    memset((char *)FINDCUST.ucRDCLACavailableBalance, 0, sizeof((char *)FINDCUST.ucRDCLACavailableBalance));

                    for(int j=0;j<ResponseData2.size();j++)
                    {
                        char temp_description[50];
                        memset(temp_description, 0, sizeof(temp_description));
                        memcpy(temp_description, (const char*)Convertstr(ResponseData2[j]["description"].asString()), strlen((const char*)Convertstr(ResponseData2[j]["description"].asString())));
#if DEBUG
                        qDebug()<<"description:"<<Convertstr(ResponseData2[j]["description"].asString());
                        printf("\n\ntemp_description ===== %s\n",temp_description);
#endif
                        if(strcmp((char*)temp_description, "BC Merchant Account")==0)
                        {
                            disp_acc_count++;
 #if DEBUG
                            qDebug()<<"name:"<<Convertstr(ResponseData2[j]["name"].asString());
#endif
                            memcpy((char*)FINDCUST.ucRDCLdescription[j+1],(const char*)Convertstr(ResponseData2[j]["description"].asString()), strlen((const char*)Convertstr(ResponseData2[j]["description"].asString())));
                            memcpy((char*)FINDCUST.ucRDCLunclearedBalance[j+1],(const char*)Convertstr(ResponseData2[j]["unclearedBalance"].asString()), strlen((const char*)Convertstr(ResponseData2[j]["unclearedBalance"].asString())));

                            memcpy((char*)FINDCUST.ucRDCLACCnumber[j+1],(const char*)Convertstr(ResponseData2[j]["number"].asString()), strlen((const char*)Convertstr(ResponseData2[j]["number"].asString())));
                            memcpy((char*)FINDCUST.ucRDCLACavailableBalance[j+1],(const char*)Convertstr(ResponseData2[j]["availableBalance"].asString()), strlen((const char*)Convertstr(ResponseData2[j]["availableBalance"].asString())));



#if DEBUG
                            printf("j = %d  ,  FINDCUST.ucRDCLdescription = %s\n", j, FINDCUST.ucRDCLdescription[j+1]);
                            qDebug()<<"description:"<<Convertstr(ResponseData2[j]["description"].asString());
                            printf("FINDCUST.ucRDCLunclearedBalance = %s\n",FINDCUST.ucRDCLunclearedBalance[j+1]);
                            qDebug()<<"unclearedBalance:"<<Convertstr(ResponseData2[j]["unclearedBalance"].asString());
                            printf("FINDCUST.ucRDCLACCnumber = %s\n",FINDCUST.ucRDCLACCnumber[j+1]);
                            qDebug()<<"number:"<<Convertstr(ResponseData2[j]["number"].asString());
                            qDebug()<<"availableBalance:"<<Convertstr(ResponseData2[j]["availableBalance"].asString());
                            printf("FINDCUST.ucRDCLACavailableBalance = %s\n",FINDCUST.ucRDCLACavailableBalance[j+1]);
  #endif
                        }
                    }

                    //                    if(ResponseData2.size() != 0)
                    //                    {


                    //                    }
                    //                    else
                    //                    {
                    //                        strcpy((char*)FINDCUST.ucRDCLdescription,(const char*)"Walkin");
                    //                        strcpy((char*)FINDCUST.ucRDCLunclearedBalance,(const char*)"0.00");

                    //                    }
                    // strcpy(FC_CustomerNumber,Convertstr(ResponseData2[j]["primaryCustomerNumber"].asString()));


                    memset(FC_MobileNumber,       0, sizeof(FC_MobileNumber));
                    memset(FC_CustomerNumber,     0, sizeof(FC_CustomerNumber));
                    memset(CustDet.cMobileNumber, 0, sizeof(CustDet.cMobileNumber));
                    memset(CustDet.cCustName,     0, sizeof(CustDet.cCustName));

                    memcpy(FC_MobileNumber,(char*)FINDCUST.ucRDCLmobilePhoneNumber, strlen((char*)FINDCUST.ucRDCLmobilePhoneNumber));
                    memcpy(CustDet.cMobileNumber, (char*)FINDCUST.ucRDCLmobilePhoneNumber, strlen((char*)FINDCUST.ucRDCLmobilePhoneNumber));
                    memcpy(CustDet.cCustName, (char*)FINDCUST.ucRDCLfullName_resp, strlen((char*)FINDCUST.ucRDCLfullName_resp));

                    memcpy(FC_CustomerNumber,(char*)FINDCUST.ucRDCLnumber_resp, strlen((char*)FINDCUST.ucRDCLnumber_resp));

 #if DEBUG
                    printf("FC_MobileNumber = %s\n",FC_MobileNumber);
                    printf("FC_CustomerNumber = %s\n",FC_CustomerNumber);
#endif
                    for(int k=0;k<ResponseData2.size();k++)
                    {
 #if DEBUG
                        qDebug()<<"clas Code:"<<Convertstr(ResponseData2[k]["clas"]["code"].asString());
                        qDebug()<<"availableBalance:"<<Convertstr(ResponseData2[k]["availableBalance"].asString());
#endif
                    }
                }
            }
        }

        if(fc_input == 1)
        {
            memset(BuffCardAdr, 0, sizeof(BuffCardAdr));
            memset(Buff_Final, 0, sizeof(Buff_Final));
            sprintf((char *)BuffCardAdr,"%s%s",FINDCUST.ucRDCMAaddressLine1,FINDCUST.ucRDCMAaddressLine2);
            sprintf((char *)Buff_Final,"%.19s %s %s%s",BuffCardAdr,FINDCUST.ucRDCMAcity, FINDCUST.ucRDCMAstate, FINDCUST.ucRDCMAcountry);
            int q_len = strlen((const char*)Buff_Final);
            QString temp = QString::fromUtf8((const char*)Buff_Final);

            if(q_len > 40)
            {
                temp = temp.mid(q_len-40).trimmed();
                sprintf((char *)configLoginLeg2.ucFCCardAcceptorLocation,"%s",(char*)temp.toAscii().data());
            }
            else
            {
                temp = temp.trimmed();
                sprintf((char *)configLoginLeg2.ucFCCardAcceptorLocation,"%s",(char*)temp.toAscii().data());
            }
#if DEBUG
            printf("****  Card Acceptor Location = %s  ****\n",configLoginLeg2.ucFCCardAcceptorLocation);
#endif
        }

    }
    else
    {
        CMisc miscObj;
        uidmessage::instance()->hide_Message();

        if(strcmp((char*)Convertstr(root["ResponseMessage"].asString()),"")!=0)
        {
            memset(ErrorMsg, 0, sizeof(ErrorMsg));
            sprintf(ErrorMsg,"%s",(char *)Convertstr(root["ResponseMessage"].asString()));
            miscObj.DisplayMsgBox(ErrorMsg);
            if(strcmp((char *)Convertstr(root["ResponseMessage"].asString()),"Session Expired.")==0)
            {
                uidmessage::instance()->hide_Message();
                return SESSION_EXPIRED_FAILED;
            }
        }
        uidmessage::instance()->hide_Message();
        return FUNCTION_FAILED;
    }

    return FUNCTION_SUCCESS;
}

typedef struct
{
    unsigned char ucRequestId_ABENEREQ[30];
}ADDBENEFICIARY;



void FPB::jsonpars1()
{

    Json1::Value root;
    root.clear();
    //QSqlQuery query;
    Json1::Reader reader;
    std::ifstream ReadData("/home/thirupathi/Desktop/1.json");

    bool parsedSuccess = reader.parse(ReadData,root,false);
    ReadData.close();
    if(not parsedSuccess)
    {
        qDebug()<<"FpsMonthlyPdsData File not Found";

        //return -1;
    }
    else
    {
        /*qDebug()<<"RequestId:"<<Convertstr(root["RequestId"].asString());
           qDebug()<<"respMessage : "<<Convertstr(root["DisplayMessage"].asString());
           qDebug()<<"respCode    : "<<Convertstr(root["ResponseMessage"].asString());
           qDebug()<<"respMessage : "<<Convertstr(root["ResponseCode"].asString());
           qDebug()<<"respMessage : "<<Convertstr(root["MessageType"].asString());*/

        Json1::Value ResponseData,fpRequestData,optReqData;
        ResponseData.clear();
        ResponseData = root.get("ResponseData",0);
        printf("\nSize : %d\n",ResponseData.size());
        if(ResponseData.size()==0)
        {
            qDebug("\n1:Key Register Data Not Found\n");
            return ;
        }
        else
        {
            for(int i=0;i<ResponseData.size();i++)
            {
                Json1::Value ResponseData1;
                ResponseData1 = ResponseData[i].get("accounts",0);
                qDebug()<<"------->"<<ResponseData1.size();
                for(int j=0;j<ResponseData1.size();j++)
                {
 #if DEBUG
                    qDebug()<<"class name:"<<Convertstr(ResponseData1[j]["name"].asString());
                    qDebug()<<"class description:"<<Convertstr(ResponseData1[j]["clas"]["description"].asString());
#endif
                }
            }
        }
    }
}

int FPB::Login_Data()
{

    system("sed 's/\"ResponseData\":\"{/\"ResponseData\":{/g' /usr/FINO/FPB/RESP/LEG2_RESP.TXT > /tmp/1.txt");
    system("sed 's/}\",\"MessageId\"/},\"MessageId\"/g' /tmp/1.txt > /tmp/2.txt  ");
    system(" sed 's/\\\\//g' /tmp/2.txt > /usr/FINO/FPB/RESP/LEG2_RESP.TXT");

    system("cd /tmp/");
    system("rm /tmp/1.txt");
    system("rm /tmp/2.txt");
    //    system("sed 's/\"ResponseData\":\"{/\"ResponseData\":{/g' /usr/FINO/FPB/RESP/LEG2_RESP.TXT > /tmp/1.txt");
    //   // system("sed 's/\"}\"/\"}/g' /tmp/1.txt > /tmp/2.txt  ");
    //    qDebug()<<"1--------";
    //    system("sed 's/}\",\"MessageId\"/},\"MessageId\"/g' /tmp/1.txt > /tmp/2.txt ");
    ////    qDebug()<<"2-----------";
    ////    system("sed 's/\[//g' /tmp/2.txt > /tmp/3.txt");
    ////    qDebug()<<"3---------";
    ////    system("sed 's/\]//g' /tmp/3.txt > /tmp/4.txt");
    ////    qDebug()<<"4-----------";
    //    system("sed 's/\\//g' /tmp/2.txt > /usr/FINO/FPB/RESP/LEG2_RESP.TXT");
    //    qDebug()<<"5-----------";


    //------------------------THIRUPATHI--------------------------------------------------------------------------------
    //    system("sed 's/\"ResponseData\":\"{/\"ResponseData\":{/g' /usr/FINO/FPB/RESP/LEG2_RESP.TXT > /tmp/1.txt");
    //    system("sed 's/\"}\"/\"}/g' /tmp/1.txt > /tmp/2.txt  ");
    //    system(" sed 's/\\\\//g' /tmp/2.txt > /usr/FINO/FPB/RESP/login.json");
    //------------------------------------------------------------------------------------------------------------------

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
        qDebug()<<"RequestId   : "<<Convertstr(root["RequestId"].asString());
        qDebug()<<"DisplayMessage : "<<Convertstr(root["DisplayMessage"].asString());
        qDebug()<<"ResponseMessage    : "<<Convertstr(root["ResponseMessage"].asString());
        qDebug()<<"ResponseCode : "<<Convertstr(root["ResponseCode"].asString());
#endif
        strcpy(configLoginLeg2.ucRequestId,Convertstr(root["RequestId"].asString()));
        strcpy(configLoginLeg2.ucDisplayMessage,Convertstr(root["DisplayMessage"].asString()));
        strcpy(configLoginLeg2.ucResponseMessage,Convertstr(root["ResponseMessage"].asString()));
        strcpy(configLoginLeg2.ucResponseCode,Convertstr(root["ResponseCode"].asString()));
        if(strcmp(configLoginLeg2.ucResponseCode,"0")==0)
        {
            Json1::Value ResponseData;
            ResponseData.clear();
            ResponseData = root.get("ResponseData",0);
            printf("\nSize : %d\n",ResponseData.size());
            if(ResponseData.size()==0)
            {
                qDebug("\n1:Key Register Data Not Found\n");
                return FUNCAL_FAILED;
            }
            else
            {
                strcpy(configLoginLeg2.ucipaddress,Convertstr(ResponseData["Login_Data"]["ipAddress2"].asString()));
                strcpy(configLoginLeg2.ucid_token,Convertstr(ResponseData["Login_Data"]["id_token"].asString()));
                strcpy(configLoginLeg2.ucdate2,Convertstr(ResponseData["Login_Data"]["date2"].asString()));
                strcpy(configLoginLeg2.uctoken_type,Convertstr(ResponseData["Login_Data"]["token_type"].asString()));
                strcpy(configLoginLeg2.ucidentifier,Convertstr(ResponseData["Login_Data"]["userDetails"]["identifier"].asString()));
                strcpy(configLoginLeg2.ucname,Convertstr(ResponseData["Login_Data"]["userDetails"]["name"].asString()));
                strcpy(configLoginLeg2.ucallowedToChangePostingDate,Convertstr(ResponseData["Login_Data"]["userDetails"]["allowedToChangePostingDate"].asString()));
                strcpy(configLoginLeg2.uccurrencyEnvironment,Convertstr(ResponseData["Login_Data"]["userDetails"]["currencyEnvironment"].asString()));
                strcpy(configLoginLeg2.ucprimaryCashLimit,Convertstr(ResponseData["Login_Data"]["userDetails"]["primaryCashLimit"].asString()));
                strcpy(configLoginLeg2.uctellerIdentifier,Convertstr(ResponseData["Login_Data"]["userDetails"]["fosIdentifier"].asString()));

                strcpy(configLoginLeg2.ucADagentStatus,Convertstr(ResponseData["Login_Data"]["userDetails"]["agencyDetails"]["agentStatus"].asString()));
                strcpy(configLoginLeg2.ucADdistributorFlag,Convertstr(ResponseData["Login_Data"]["userDetails"]["agencyDetails"]["distributorFlag"].asString()));

                strcpy(configLoginLeg2.ucpasswordExpired,Convertstr(ResponseData["Login_Data"]["userDetails"]["passwordExpired"].asString()));
                strcpy(configLoginLeg2.ucfPIndex,Convertstr(ResponseData["Login_Data"]["userDetails"]["fPIndex"].asString()));
                strcpy(configLoginLeg2.ucmobileNumber,Convertstr(ResponseData["Login_Data"]["userDetails"]["mobileNumber"].asString()));
                strcpy(configLoginLeg2.ucmerchantAccount,Convertstr(ResponseData["Login_Data"]["userDetails"]["merchantAccount"].asString()));

#if DEBUG
                printf("\n\nconfigLoginLeg2.ucmerchantAccount = %s\n\n", configLoginLeg2.ucmerchantAccount);
 #endif
                strcpy(configLoginLeg2.ucstatus,Convertstr(ResponseData["Login_Data"]["userDetails"]["status"].asString()));
                strcpy(configLoginLeg2.ucrevoked,Convertstr(ResponseData["Login_Data"]["userDetails"]["revoked"].asString()));
                strcpy(configLoginLeg2.ucsupervisorId,Convertstr(ResponseData["Login_Data"]["userDetails"]["supervisorId"].asString()));
                strcpy(configLoginLeg2.ucBAcode,Convertstr(ResponseData["Login_Data"]["userDetails"]["branchAssociated"]["code"].asString()));
                strcpy(configLoginLeg2.ucBAdescription,Convertstr(ResponseData["Login_Data"]["userDetails"]["branchAssociated"]["description"].asString()));
                strcpy(configLoginLeg2.ucifsc,Convertstr(ResponseData["Login_Data"]["userDetails"]["branchAssociated"]["ifsc"].asString()));
                strcpy(configLoginLeg2.ucmicr,Convertstr(ResponseData["Login_Data"]["userDetails"]["branchAssociated"]["micr"].asString()));
                strcpy(configLoginLeg2.ucgrid,Convertstr(ResponseData["Login_Data"]["userDetails"]["branchAssociated"]["grid"].asString()));
                strcpy(configLoginLeg2.ucdefaultCostCenter,Convertstr(ResponseData["Login_Data"]["userDetails"]["branchAssociated"]["defaultCostCenter"].asString()));
                strcpy(configLoginLeg2.uccashOpen,Convertstr(ResponseData["Login_Data"]["userDetails"]["branchAssociated"]["cashOpen"].asString()));
                strcpy(configLoginLeg2.uccts,Convertstr(ResponseData["Login_Data"]["userDetails"]["branchAssociated"]["cts"].asString()));
                strcpy(configLoginLeg2.ucgridBranch,Convertstr(ResponseData["Login_Data"]["userDetails"]["branchAssociated"]["gridBranch"].asString()));
                strcpy(configLoginLeg2.ucBranchtype,Convertstr(ResponseData["Login_Data"]["userDetails"]["branchAssociated"]["branchType"]["type"].asString()));
                strcpy(configLoginLeg2.ucBTdescription,Convertstr(ResponseData["Login_Data"]["userDetails"]["branchAssociated"]["branchType"]["description"].asString()));
                strcpy(configLoginLeg2.ucsubType,Convertstr(ResponseData["Login_Data"]["userDetails"]["branchAssociated"]["branchType"]["subType"].asString()));
                strcpy(configLoginLeg2.ucsubtypedescription,Convertstr(ResponseData["Login_Data"]["userDetails"]["branchAssociated"]["branchType"]["subtypedescription"].asString()));

                strcpy(configLoginLeg2.ucaddressLine1,Convertstr(ResponseData["Login_Data"]["userDetails"]["branchAssociated"]["address"]["addressLine1"].asString()));

                strcpy(configLoginLeg2.ucaddressLine2,Convertstr(ResponseData["Login_Data"]["userDetails"]["branchAssociated"]["address"]["addressLine2"].asString()));
                strcpy(configLoginLeg2.ucaddressLine3,Convertstr(ResponseData["Login_Data"]["userDetails"]["branchAssociated"]["address"]["addressLine3"].asString()));
                strcpy(configLoginLeg2.uccity,Convertstr(ResponseData["Login_Data"]["userDetails"]["branchAssociated"]["address"]["city"].asString()));
                strcpy(configLoginLeg2.ucstate,Convertstr(ResponseData["Login_Data"]["userDetails"]["branchAssociated"]["address"]["state"].asString()));
                strcpy(configLoginLeg2.ucstateDescription,Convertstr(ResponseData["Login_Data"]["userDetails"]["branchAssociated"]["address"]["stateDescription"].asString()));
                strcpy(configLoginLeg2.ucpinCode,Convertstr(ResponseData["Login_Data"]["userDetails"]["branchAssociated"]["address"]["pinCode"].asString()));
                strcpy(configLoginLeg2.uccountry,Convertstr(ResponseData["Login_Data"]["userDetails"]["branchAssociated"]["address"]["country"].asString()));



                //            strcpy(configLoginLeg2.ucsucceeded_schemes,Convertstr(ResponseData["Login_Data"]["succeeded_schemes"].asString()));
                strcpy(configLoginLeg2.ucaccess_token,Convertstr(ResponseData["Login_Data"]["access_token"].asString()));
                strcpy(configLoginLeg2.ucreturnCode,Convertstr(ResponseData["Login_Data"]["returnCode"].asString()));
                strcpy(configLoginLeg2.ucfirstLoginKilled,Convertstr(ResponseData["Login_Data"]["firstLoginKilled"].asString()));
                strcpy(configLoginLeg2.ucrefresh_expires_in,Convertstr(ResponseData["Login_Data"]["refresh_expires_in"].asString()));


                Json1::Value ResponseData1;
                ResponseData1.clear();
                ResponseData1 = ResponseData.get("Login_Data",0);

                Json1::Value balancesList;
                balancesList.clear();
                balancesList = ResponseData1.get("balancesList",0);

                for(int i=0;i<balancesList.size();i++)
                {
                    strcpy(configLoginLeg2.ucBLaccountNo,Convertstr(balancesList[i]["accountNo"].asString()));
                    strcpy(configLoginLeg2.ucledgerBalance,Convertstr(balancesList[i]["ledgerBalance"].asString()));
                    strcpy(configLoginLeg2.ucavailableBalance,Convertstr(balancesList[i]["availableBalance"].asString()));
                    strcpy(configLoginLeg2.uccurrency,Convertstr(balancesList[i]["currency"].asString()));
                    //                    qDebug()<<"accountNo:"<<Convertstr(balancesList[i]["accountNo"].asString());
                    //                    qDebug()<<"availableBalance:"<<Convertstr(balancesList[i]["availableBalance"].asString());
                }

                //                strcpy(configLoginLeg2.ucBLaccountNo,Convertstr(ResponseData["Login_Data"]["balancesList"]["accountNo"].asString()));
                //                strcpy(configLoginLeg2.ucledgerBalance,Convertstr(ResponseData["Login_Data"]["balancesList"]["ledgerBalance"].asString()));
                //                strcpy(configLoginLeg2.ucavailableBalance,Convertstr(ResponseData["Login_Data"]["balancesList"]["availableBalance"].asString()));
                //                strcpy(configLoginLeg2.uccurrency,Convertstr(ResponseData["Login_Data"]["balancesList"]["currency"].asString()));

                //                strcpy(configLoginLeg2.ucavailableBalance,"12845.05");

#if DEBUG
                qDebug()<<"configLoginLeg2.ucBLaccountNo"<<configLoginLeg2.ucBLaccountNo;
                qDebug()<<"configLoginLeg2.ucledgerBalance"<<configLoginLeg2.ucledgerBalance;
                qDebug()<<"configLoginLeg2.ucavailableBalance"<<configLoginLeg2.ucavailableBalance;
                qDebug()<<"configLoginLeg2.uccurrency"<<configLoginLeg2.uccurrency;
                qDebug()<<"=======================9=";
#endif
                strcpy(configLoginLeg2.ucLDresponseMessage,Convertstr(ResponseData["Login_Data"]["responseMessage"].asString()));
#if DEBUG
                qDebug()<<"=======================10=";
                qDebug()<<"configLoginLeg2.ucLDresponseMessage"<<configLoginLeg2.ucLDresponseMessage;
                qDebug()<<"configLoginLeg2.refresh_expires_in"<<configLoginLeg2.ucrefresh_expires_in;
                qDebug()<<"configLoginLeg2.expires_in"<<configLoginLeg2.ucrefresh_expires_in;
                qDebug()<<"=======================11=";
#endif
                strcpy(configLoginLeg2.ucexpires_in,Convertstr(ResponseData["Login_Data"]["expires_in"].asString()));
                //  strcpy(configLoginLeg2.ucerrors,Convertstr(ResponseData["Login_Data"]["errors"].asString()));
                strcpy(configLoginLeg2.ucforce_change_password,Convertstr(ResponseData["Login_Data"]["force_change_password"].asString()));
                strcpy(configLoginLeg2.uctellerProofList,Convertstr(ResponseData["Login_Data"]["tellerProofList"].asString()));
                strcpy(configLoginLeg2.ucUCcode,Convertstr(ResponseData["Login_Data"]["userDetails"]["userClass"]["code"].asString()));
                strcpy(configLoginLeg2.ucUCdescription,Convertstr(ResponseData["Login_Data"]["userDetails"]["userClass"]["description"].asString()));
                strcpy(configLoginLeg2.ucmaxDaysBackdatedAllowed,Convertstr(ResponseData["Login_Data"]["userDetails"]["userClass"]["maxDaysBackdatedAllowed"].asString()));
                strcpy(configLoginLeg2.ucmaxDaysFutureDatedAllowed,Convertstr(ResponseData["Login_Data"]["userDetails"]["userClass"]["maxDaysFutureDatedAllowed"].asString()));
                strcpy(configLoginLeg2.ucuserId,Convertstr(ResponseData["User ID"].asString()));
                strcpy(configLoginLeg2.ucLastLogin,Convertstr(ResponseData["LastLogin"].asString()));
                strcpy(configLoginLeg2.ucusertypeID,Convertstr(ResponseData["usertypeID"].asString()));
                strcpy(configLoginLeg2.ucuserRole,Convertstr(ResponseData["userRole"].asString()));
                strcpy(configLoginLeg2.uceAgreement,Convertstr(ResponseData["eAgreement"].asString()));
                strcpy(configLoginLeg2.uceSurvey,Convertstr(ResponseData["eSurvey"].asString()));
                strcpy(configLoginLeg2.CertificateExpiryDate,Convertstr(ResponseData["CertificateExpiryDate"].asString()));
                strcpy(configLoginLeg2.ucMandatoryVersion,Convertstr(ResponseData["MandatoryVersion"].asString()));
                strcpy(configLoginLeg2.ucCurrentVersion,Convertstr(ResponseData["CurrentVersion"].asString()));
#if DEBUG
                qDebug()<<"configLoginLeg2.ucCurrentVersion"<<configLoginLeg2.ucCurrentVersion;
                qDebug()<<"configLoginLeg2.ucMandatoryVersion"<<configLoginLeg2.ucMandatoryVersion;
#endif


                strcpy(configLoginLeg2.ucev_mandat,Convertstr(ResponseData["ev_mandat"].asString()));
                strcpy(configLoginLeg2.ucev_current,Convertstr(ResponseData["ev_current"].asString()));
                strcpy(configLoginLeg2.ucmorp_mandat,Convertstr(ResponseData["morp_mandat"].asString()));
                strcpy(configLoginLeg2.ucmorp_current,Convertstr(ResponseData["morp_current"].asString()));

                strcpy(configLoginLeg2.uczeroizationDateTime,Convertstr(ResponseData["zeroizationDateTime"].asString()));
                strcpy(configLoginLeg2.ucchannelID,Convertstr(ResponseData["channelID"].asString()));
                strcpy(configLoginLeg2.ucMastersVersion,Convertstr(ResponseData["MastersVersion"].asString()));

                strcpy(configLoginLeg2.ucMessageId,Convertstr(root["MessageId"].asString()));
                strcpy(configLoginLeg2.ucSessionExpiryTime,Convertstr(root["SessionExpiryTime"].asString()));
                strcpy(configLoginLeg2.ucSessionId,Convertstr(root["SessionId"].asString()));
                strcpy(configLoginLeg2.ucRouteID,Convertstr(root["RouteID"].asString()));
                strcpy(configLoginLeg2.ucErrorCode,Convertstr(root["ErrorCode"].asString()));
                strcpy(configLoginLeg2.ucXMLData,Convertstr(root["XMLData"].asString()));
                strcpy(configLoginLeg2.ucAuthmanFlag,Convertstr(root["AuthmanFlag"].asString()));
                strcpy(configLoginLeg2.ucServiceID,Convertstr(root["ServiceID"].asString()));



#if DEBUG
                qDebug()<<"configLoginLeg2.ucCurrentVersion"<<configLoginLeg2.ucCurrentVersion;
                qDebug()<<"configLoginLeg2.ucMandatoryVersion"<<configLoginLeg2.ucMandatoryVersion;
                qDebug()<<"configLoginLeg2.ucCurrentVersion"<<configLoginLeg2.ucCurrentVersion;
                qDebug()<<"configLoginLeg2.ucforce_change_password"<<configLoginLeg2.ucforce_change_password;
                qDebug()<<"%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%";
#endif
                return FUNC_OK;
            }
        }
        else
        {
            return FUNC_ERROR;
        }
    }

}

