#include<fstream>
#include <QDebug>
#include "FPB.h"
QStringList tagnames;

//Widget::Widget(QWidget *parent) :
//    QWidget(parent),
//    ui(new Ui::Widget)
//{
//   // jsonpars1();
//    //Login_Data();
// //   findcust();
//  //  masterfile();
//    commanfun("District");
//   // ProdFeatureID();
//   // spiltfunction();
//    ui->setupUi(this);
//}

//Widget::~Widget()
//{
//    delete ui;
//}
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
    if(strcmp(d,"District") == 0)
    {
    tagnames.push_back("StateCode");
    tagnames.push_back("District");
    tagnames.push_back("CBSStateCode");
    tagnames.push_back("OrderID");
    tagnames.push_back("Lom_CityNumericCode");
    }
    if(strcmp(d,"Occupation") == 0)
    {
    tagnames.push_back("OCC");
    tagnames.push_back("OccupationName");

    }

}

void FPB::commanfun(char *d)
{
    QString data ;
    QStringList finaldata;
    TagNames("Occupation");

//    system(" sed 's/\\\\//g' /home/thirupathi/Desktop/fino/master.txt > /home/thirupathi/Desktop/fino/master.json");
      string line;
    ifstream myfile ("/home/thirupathi/Desktop/fino/mstOccupation.txt");
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
        system("sed 's/ResponseData\": \"{/ResponseData\":[{/g' /home/thirupathi/Desktop/fino/mstOccupation.txt > /home/thirupathi/Desktop/fino/1.txt");
        system("sed 's/\"}\",/\"}],/g' /home/thirupathi/Desktop/fino/1.txt > /home/thirupathi/Desktop/fino/mstOccupation.json");
    }

    Json::Value root;
        root.clear();
        //QSqlQuery query;
        Json::Reader reader;
        std::ifstream ReadData("/home/thirupathi/Desktop/fino/mstOccupation.json");
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

            Json::Value ResponseData;
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

                 Json::Value ResponseData1;
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
                      TmpStr = "insert into table values(";
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

//void FPB::ProdFeatureID() {
// string line;
//    system(" sed 's/\\\\//g' /home/thirupathi/Desktop/fino/MasterData_resp.txt > /home/thirupathi/Desktop/fino/master.json");
//    ifstream myfile ("/home/thirupathi/Desktop/fino/master.json");
//    if (myfile.is_open())
//    {
//        while ( getline (myfile,line) )
//        {
//            //cout << line << endl;
//        }
//        myfile.close();
//    }


//    size_t found = line.find("\"ResponseData\": \"{");
//    if (found != string::npos)
//    {
//            qDebug() << "First occurrence is " << found;
//            //system("cp /tmp/loginres.txt /mnt/jffs2/ITDA/tmp/loginResp.txt");
//    }
//    else
//    {
//        qDebug() << "First occurrence is-------> " << found;
//        system("sed 's/ResponseData\": \"{/ResponseData\":[{/g' /home/thirupathi/Desktop/fino/master.json > /home/thirupathi/Desktop/fino/1.txt");

//        system("sed 's/\"}\",/\"}],/g' /home/thirupathi/Desktop/fino/1.txt > /home/thirupathi/Desktop/fino/masterfinal.json");
//        system("sed 's/\":\"{/\":[{/g' /home/thirupathi/Desktop/fino/masterfinal.json > /home/thirupathi/Desktop/fino/1.txt ");
//        system("sed 's/\"{\"/[{\"/g' /home/thirupathi/Desktop/fino/1.txt > /home/thirupathi/Desktop/fino/masterfinal.json");
//        system("sed 's/}]}}\"/}]}}]/g' /home/thirupathi/Desktop/fino/masterfinal.json > /home/thirupathi/Desktop/fino/1.txt ");
//                system("sed 's/} }\"/} }]/g' /home/thirupathi/Desktop/fino/1.txt > /home/thirupathi/Desktop/fino/masterfinal.json");
//        system("sed 's/}}\"/}}]/g' /home/thirupathi/Desktop/fino/masterfinal.json > /home/thirupathi/Desktop/fino/1.json ");


//        /*  }]}}" }]}}]  }}"  } }" */


//        Json::Value root;
//            root.clear();
//            //QSqlQuery query;
//            Json::Reader reader;
//            std::ifstream ReadData("/home/thirupathi/Desktop/fino/1.json");
//            bool parsedSuccess = reader.parse(ReadData,root,false);
//            ReadData.close();
//            if(not parsedSuccess)
//            {
//                qDebug()<<" File not Found";

//                //return -1;
//            }
//            else
//            {
//                qDebug()<<"RequestId   : "<<Convertstr(root["RequestId"].asString());
//                qDebug()<<"respMessage : "<<Convertstr(root["DisplayMessage"].asString());
//                qDebug()<<"respCode    : "<<Convertstr(root["ResponseMessage"].asString());
//                Json::Value ResponseData;
//                ResponseData.clear();
//                ResponseData = root.get("ResponseData",0);
//                printf("\nSize : %d\n",ResponseData.size());
//                if(ResponseData.size()==0)
//                {
//                    qDebug("\n1:Key Register Data Not Found\n");
//                    return;
//                }
//                else
//                {
//                    for(int i=0;i<ResponseData.size();i++)
//                    {
//                        Json::Value ResponseData1;
//                        ResponseData1 = ResponseData[i].get("CacheMaster",0);
//                        qDebug()<<"------->"<<ResponseData1.size();
//                        for(int j=0;j<ResponseData1.size();j++)
//                        {
//                            qDebug()<<"ProdFeatureID :"<<Convertstr(ResponseData1[j]["ProdFeatureID"].asString());
//                            Json::Value ResponseData2;
//                            ResponseData2 = ResponseData1[j].get("ServiceCharge",0);
//                            qDebug()<<"------->"<<ResponseData2.size();
//                            for(int k=0;k<ResponseData2.size();k++)
//                            {
//                                qDebug()<<"SMSCharges Monthly:"<<Convertstr(ResponseData2[k]["SMSCharges"]["Monthly"].asString());
//                                Json::Value ResponseData3;
//                                ResponseData3 = ResponseData2[k].get("StatementCharges",0);
//                                qDebug()<<"------->"<<ResponseData3.size();
//                                for(int m=0;m<ResponseData3.size();m++)
//                                {

//                                    qDebug()<<"EStatementCharge weekly:"<<Convertstr(ResponseData3[m]["EStatementCharge"]["weekly"].asString());
//                                    qDebug()<<"EStatementCharge PhysicalStatement weekly:"<<Convertstr(ResponseData3[m]["PhysicalStatement"]["weekly"].asString());

//                                    //SMSCharges
//                                }
//                            }
//                        }
//                    }
//                }
//            }
//    }
//}

//void FPB::masterfile()
//{


//    system(" sed 's/\\\\//g' /home/thirupathi/Desktop/fino/master.txt > /home/thirupathi/Desktop/fino/master.json");
//      string line;
//    ifstream myfile ("/home/thirupathi/Desktop/fino/master.json");
//    if (myfile.is_open())
//    {
//        while ( getline (myfile,line) )
//        {
//            //cout << line << endl;
//        }
//        myfile.close();
//    }


//    size_t found = line.find("\"ResponseData\": \"{");
//    if (found != string::npos)
//    {
//            qDebug() << "First occurrence is " << found;
//            //system("cp /tmp/loginres.txt /mnt/jffs2/ITDA/tmp/loginResp.txt");
//    }
//    else
//    {
//        qDebug() << "First occurrence is-------> " << found;
//        system("sed 's/ResponseData\": \"{/ResponseData\":[{/g' /home/thirupathi/Desktop/fino/master.json > /home/thirupathi/Desktop/fino/1.txt");
//        system("sed 's/\"}\",/\"}],/g' /home/thirupathi/Desktop/fino/1.txt > /home/thirupathi/Desktop/fino/masterfinal.json");
//    }

//    Json::Value root;
//        root.clear();
//        //QSqlQuery query;
//        Json::Reader reader;
//        std::ifstream ReadData("/home/thirupathi/Desktop/fino/masterfinal.json");
//        bool parsedSuccess = reader.parse(ReadData,root,false);
//        ReadData.close();
//        if(not parsedSuccess)
//        {
//            qDebug()<<" File not Found";

//            //return -1;
//        }
//        else
//        {
//            qDebug()<<"RequestId   : "<<Convertstr(root["RequestId"].asString());
//            qDebug()<<"respMessage : "<<Convertstr(root["DisplayMessage"].asString());
//            qDebug()<<"respCode    : "<<Convertstr(root["ResponseMessage"].asString());
//            qDebug()<<"respMessage : "<<Convertstr(root["ResponseCode"].asString());
//          //  qDebug()<<"ResponseData : "<<Convertstr(root["ResponseData"].asString());
//           /////////////////////////////////////////////////all same /////////////////////

//            Json::Value ResponseData;
//            ResponseData.clear();
//            ResponseData = root.get("ResponseData",0);
//            printf("\nSize : %d\n",ResponseData.size());
//            if(ResponseData.size()==0)
//            {
//                qDebug("\n1:Key Register Data Not Found\n");
//                return;
//            }
//            else
//            {
//             for(int i=0;i<ResponseData.size();i++)
//                {
//                 Json::Value ResponseData1;
//                  ResponseData1 = ResponseData[i].get("CacheMaster",0);
//                   qDebug()<<"------->"<<ResponseData1.size();
//                  for(int j=0;j<ResponseData1.size();j++)
//                  {
//                      qDebug()<<"code :"<<Convertstr(ResponseData1[j]["code"].asString());
//                      qDebug()<<"description:"<<Convertstr(ResponseData1[j]["description"].asString());
//                      qDebug()<<"countryCode:"<<Convertstr(ResponseData1[j]["countryCode"].asString());
//                      qDebug()<<"GSTStateCode:"<<Convertstr(ResponseData1[j]["GSTStateCode"].asString());
//                      qDebug()<<"Lom_State:"<<Convertstr(ResponseData1[j]["Lom_State"].asString());
//                      qDebug()<<"Lom_StateNumericID:"<<Convertstr(ResponseData1[j]["Lom_StateNumericID"].asString());

//                    //  insert into tables  values (Convertstr(ResponseData1[j]["code"].asString(),)

//                  }
//                }

//            }

//        }
//}

//void FPB::findcust()
//{
//    system(" sed 's/\\\\//g' /home/thirupathi/Desktop/FC_RESP.json > /home/thirupathi/Desktop/FC_RESP1.json");

//    string line;
//    ifstream myfile ("/home/thirupathi/Desktop/FC_RESP1.json");
//    if (myfile.is_open())
//    {
//        while ( getline (myfile,line) )
//        {
//            //cout << line << endl;
//        }
//        myfile.close();
//    }


//    size_t found = line.find("\"ResponseData\":\"{");
//    if (found != string::npos)
//    {
//            qDebug() << "First occurrence is " << found;
//            //system("cp /tmp/loginres.txt /mnt/jffs2/ITDA/tmp/loginResp.txt");
//    }
//    else
//    {

//        system("sed 's/ResponseData\":\"{/ResponseData\":[{/g' /home/thirupathi/Desktop/FC_RESP1.json > /home/1.txt");
//        system("sed 's/]}}\",\"/]}}],\"/g' /home/1.txt > /home/thirupathi/Desktop/FC_RESP1.json");
//    }


//    //"ResponseData":"{
//    //]}}","
//    Json::Value root;
//        root.clear();
//        //QSqlQuery query;
//        Json::Reader reader;
//        std::ifstream ReadData("/home/thirupathi/Desktop/FC_RESP1.json");
//        bool parsedSuccess = reader.parse(ReadData,root,false);
//        ReadData.close();
//        if(not parsedSuccess)
//        {
//            qDebug()<<" File not Found";

//            //return -1;
//        }
//        else
//        {
//            qDebug()<<"RequestId   : "<<Convertstr(root["RequestId"].asString());
//            qDebug()<<"respMessage : "<<Convertstr(root["DisplayMessage"].asString());
//            qDebug()<<"respCode    : "<<Convertstr(root["ResponseMessage"].asString());
//            qDebug()<<"respMessage : "<<Convertstr(root["ResponseCode"].asString());
//            qDebug()<<"respMessage : "<<Convertstr(root["MessageType"].asString());


//        }

//            Json::Value ResponseData;
//            ResponseData.clear();
//            ResponseData = root.get("ResponseData",0);
//            printf("\nSize : %d\n",ResponseData.size());
//            if(ResponseData.size()==0)
//            {
//                qDebug("\n1:Key Register Data Not Found\n");
//                return;
//            }
//            else
//            {
//             for(int i=0;i<ResponseData.size();i++)
//                {
//                    qDebug()<<"returnCode:"<<Convertstr(ResponseData[i]["returnCode"].asString());

//                    Json::Value ResponseData1;
//                     ResponseData1 = ResponseData[i].get("customerList",0);
//                      qDebug()<<"------->"<<ResponseData1.size();
//                     for(int j=0;j<ResponseData1.size();j++)
//                     {

//                         qDebug()<<"fullName:"<<Convertstr(ResponseData1[j]["fullName"].asString());
//                         Json::Value ResponseData2;
//                          ResponseData2 = ResponseData1[j].get("accounts",0);
//                           qDebug()<<"------->"<<ResponseData2.size();
//                           for(int k=0;k<ResponseData2.size();k++)
//                           {
//                               qDebug()<<"clas Code:"<<Convertstr(ResponseData2[k]["clas"]["code"].asString());
//                               qDebug()<<"availableBalance:"<<Convertstr(ResponseData2[k]["availableBalance"].asString());

//                           }
//                     }

//                }

//            }
//        }




//void FPB::jsonpars1()
//{

//    Json::Value root;
//        root.clear();
//        //QSqlQuery query;
//        Json::Reader reader;
//        std::ifstream ReadData("/home/thirupathi/Desktop/1.json");

//        bool parsedSuccess = reader.parse(ReadData,root,false);
//        ReadData.close();
//        if(not parsedSuccess)
//        {
//            qDebug()<<"FpsMonthlyPdsData File not Found";

//            //return -1;
//        }
//        else
//        {
//            /*qDebug()<<"RequestId:"<<Convertstr(root["RequestId"].asString());
//            qDebug()<<"respMessage : "<<Convertstr(root["DisplayMessage"].asString());
//            qDebug()<<"respCode    : "<<Convertstr(root["ResponseMessage"].asString());
//            qDebug()<<"respMessage : "<<Convertstr(root["ResponseCode"].asString());
//            qDebug()<<"respMessage : "<<Convertstr(root["MessageType"].asString());*/

//            Json::Value ResponseData;
//            ResponseData.clear();
//            ResponseData = root.get("customerList",0);
//            printf("\nSize : %d\n",ResponseData.size());
//            if(ResponseData.size()==0)
//            {
//                qDebug("\n1:Key Register Data Not Found\n");
//                return;
//            }
//            else
//            {
//                for(int i=0;i<ResponseData.size();i++)
//                {
//                    Json::Value ResponseData1;
//                     ResponseData1 = ResponseData[i].get("accounts",0);
//                      qDebug()<<"------->"<<ResponseData1.size();
//                     for(int j=0;j<ResponseData1.size();j++)
//                     {
//                         qDebug()<<"class name:"<<Convertstr(ResponseData1[j]["name"].asString());
//                         qDebug()<<"class description:"<<Convertstr(ResponseData1[j]["clas"]["description"].asString());

//                     }

//                }

//            }
//        }

//}

//void FPB::Login_Data()
//{

//    Json::Value root;
//        root.clear();
//        //QSqlQuery query;
//        Json::Reader reader;
//        std::ifstream ReadData("/home/thirupathi/Desktop/2.json");

//        bool parsedSuccess = reader.parse(ReadData,root,false);
//        ReadData.close();
//        if(not parsedSuccess)
//        {
//            qDebug()<<"FpsMonthlyPdsData File not Found";

//            //return -1;
//        }
//        else
//        {
//            /*qDebug()<<"RequestId:"<<Convertstr(root["RequestId"].asString());
//            qDebug()<<"respMessage : "<<Convertstr(root["DisplayMessage"].asString());
//            qDebug()<<"respCode    : "<<Convertstr(root["ResponseMessage"].asString());
//            qDebug()<<"respMessage : "<<Convertstr(root["ResponseCode"].asString());
//            qDebug()<<"respMessage : "<<Convertstr(root["MessageType"].asString());*/

//            Json::Value ResponseData;
//            ResponseData.clear();
//            ResponseData = root.get("ResponseData",0);
//            printf("\nSize : %d\n",ResponseData.size());
//            if(ResponseData.size()==0)
//            {
//                qDebug("\n1:Key Register Data Not Found\n");
//                return;
//            }
//            else
//            {
//                for(int i=0;i<ResponseData.size();i++)
//                {

//                    qDebug()<<"ipAddress2:"<<Convertstr(ResponseData[i]["Login_Data"]["ipAddress2"].asString());

//                    qDebug()<<"identifier:"<<Convertstr(ResponseData[i]["Login_Data"]["userDetails"]["identifier"].asString());


//                    qDebug()<<"identifier:"<<Convertstr(ResponseData[i]["Login_Data"]["userDetails"]["userClass"]["code"].asString());

//                    qDebug()<<"identifier:"<<Convertstr(ResponseData[i]["Login_Data"]["userDetails"]["branchAssociated"]["description"].asString());

//                    qDebug()<<"expires_in:"<<Convertstr(ResponseData[i]["Login_Data"]["expires_in"].asString());

//            //
//                }

//            }
//        }
//  /*  char *RequestId,*returnCode,*questionId;
//    Json::Value root,root1;
//    Json::Reader reader;
//    std::ifstream ReadData("/home/thirupathi/Desktop/2.json");
//    bool parsedSuccess = reader.parse(ReadData,root,false);
//    if(not parsedSuccess) {
//        qDebug("File not found");
//        return ;
//    }
//    else
//    {
//        RequestId = Convertstr(root["RequestId"].asString());
//        qDebug()<<"Req Id------->"<<RequestId;

//        Json::Value TxnComdtydata,TxnComdtydata1;
//        TxnComdtydata.clear();
//        TxnComdtydata1.clear();
//        TxnComdtydata = root.get("ResponseData",0);
//        printf("\nSize : %d\n",TxnComdtydata.size());
//        if(TxnComdtydata.size()==0)
//        {
//            qDebug("\nTxnComdtydata Failed\n");
//            return ;
//        }
//        else
//        {
//                for(int i=0;i<TxnComdtydata.size();i++)
//                {
//                    //TxnComdtydata1 = TxnComdtydata.get("Login_Data",0);
//                          printf("LastLogin:%s\n",Convertstr(TxnComdtydata[i]["LastLogin"].asString()));
////                        printf("usertypeID:%s\n",Convertstr(TxnComdtydata[i]["usertypeID"].asString()));
////                        printf("CertificateExpiryDate:%s\n",Convertstr(TxnComdtydata[i]["CertificateExpiryDate"].asString()));
////                        printf("CertificateExpiryDate:%s\n",Convertstr(TxnComdtydata[i]["MastersVersion"].asString()));
//                         TxnComdtydata1 = TxnComdtydata.get("Login_Data",0);
//                         qDebug()<<"------------>"<<TxnComdtydata1.size();
//                         for(int j=0;j<TxnComdtydata1.size();j++)
//                        {
//                             //printf("LastLogin:%s\n",Convertstr(TxnComdtydata[i],TxnComdtydata1[j]["ipAddress2"].asString()));

//                         }

//                }

//       }
//    }*/

//}

