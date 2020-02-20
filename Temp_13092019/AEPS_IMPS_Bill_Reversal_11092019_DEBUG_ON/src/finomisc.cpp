#include "finomisc.h"
#include "displaymessage.h"
//#include <QtGui>
//#include<stdlib.h>
#include <QPushButton>
char MER_FLAG = 0;

CMisc::CMisc()
{

}


///////////////////////////////////////////////////////////////////
//
// This below function Plays Audio file
//
//
//  Input :
//  Type  : char *
//  Output:
//
//  Date  :                           Author:
///////////////////////////////////////////////////////////////////

void CMisc::AudioTest(char *filename)
{
    char frame[128];
    int volCount = 192;



    int DBFlag = GetAudioFlag();

    memset(frame,0,sizeof(frame));
    sprintf(frame,"amixer cset numid=4 %d",volCount);
    system(frame);

    memset(frame,0,sizeof(frame));
    //sprintf(frame,"mplayer -autosync 30 -speed 3 /mnt/jffs2/MP3/%05d%s.mp3 &",DBFlag,filename);
    sprintf(frame,"mplayer -autosync 30 -speed 3 /mnt/jffs2/MP3/%05d%s.mp3",DBFlag,filename);
    //qDebug() << frame;
    system(frame);

   // if(DBFlag == 4)
   // sleep(3);

    /*    char p_id[16];
    system("pidof mplayer > /tmp/mpplay ");
    int fd9 = ::open("/tmp/mpplay",O_RDONLY);
    {
        int kee = read(fd9,p_id,6);
        qDebug() << "kee = " << kee;
        memset(p_id,0,strlen(p_id));
        p_id[kee] = '\0';
        ::close(fd9);
    }

    memset(frame,0,sizeof(frame));
    sprintf(frame,"kill -19 %s",p_id);
    system(frame);

    remove("/tmp/mpplay");*/

    // gl11_audio_power_off();

}

///////////////////////////////////////////////////////////////////
//
// This below function Gets audio flag
//
//
//  Input :
//  Type  :
//  Output:
//
//  Date  :                           Author:
///////////////////////////////////////////////////////////////////

int CMisc::GetAudioFlag()
{
    QString str,str1;
    int LangID = 0;

    QSqlQuery query;
    query.clear();
    query.prepare("select flagvalue from paramflag where headername='RFU'");
    if(!query.exec())
        qDebug() << query.lastError();
    else
    {
        while(query.next())
        {
            str = query.value(0).toString().at(1);
        }
    }

    if(str == "Y")
    {
        query.clear();
        query.prepare("select flagvalue from paramflag where headername='language_id'");
        if(!query.exec())
            qDebug() << query.lastError();
        else
        {
            while(query.next())
            {
                str1 = query.value(0).toString();
            }
        }
    }
    else
    {
        str1 = "0";
    }

    LangID = str1.toInt();
    qDebug()<<"LangID"<<LangID;
    return LangID;
}

///////////////////////////////////////////////////////////////////
//
// This below function is used to display message box
//
//
//  Input :
//  Type  :
//  Output:
//
//  Date  :                           Author:
///////////////////////////////////////////////////////////////////

void CMisc::DisplayMsgBox(char *str)
{
//    QMessageBox msg;

//    //msg.setGeometry(80,50,320,200);  // akshay j changes 100516



//    msg.setGeometry(0,0,320,200);  // akshay j changes 100516
//    msg.setStyleSheet("color: rgb(255, 255, 255); background-color: rgb(58, 114, 127)");
//    msg.setWindowFlags(Qt::FramelessWindowHint);

//    msg.setCursor(Qt::ArrowCursor);
//    msg.setStandardButtons(QMessageBox::Ok);

//    QLabel *qmsg = new QLabel();
//    qmsg->setGeometry(30, 60, 280, 100);
//    qmsg->setText(str);
//    qmsg->setWordWrap(true);
//    qmsg->setAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
//    msg.exec();
//    return;


    ///////


    QDialog  *confirmation = new QDialog();
    confirmation->setWindowFlags(Qt::FramelessWindowHint);
    confirmation->setAutoFillBackground(true);
    confirmation->setStyleSheet("color: rgb(85, 0, 127); background-color: rgb(255, 255, 255)");

    confirmation->setGeometry(0, 0, 320, 240);

    QLabel *msg = new QLabel(confirmation);
    msg->setGeometry(30, 70, 280, 100);
    msg->setText(str);
    msg->setWordWrap(true);
    msg->setAlignment(Qt::AlignHCenter|Qt::AlignVCenter);


    QPushButton *yesButton = new QPushButton(confirmation);
    yesButton->setGeometry(105,160,100,40);
    yesButton->setStyleSheet("color: rgb(255, 255, 255); background-color: rgb(85, 0, 127)");
    yesButton->setFont(QFont("Normal"));
    yesButton->setText("OK");

    QObject::connect(yesButton, SIGNAL(clicked()), confirmation, SLOT(accept()));

    int ret = confirmation->exec();

    qDebug()<<"Ret "<< ret;
    return;
}

int CMisc::chngepasswd()
{
    MER_FLAG = 'C';
    return TRUE;
}

int CMisc::updateversion()
{
    MER_FLAG = 'U';
    return TRUE;
}

int CMisc::logout()
{
    MER_FLAG = 'L';
    return TRUE;
}


int CMisc::DisplayMsgBox_MerchntDash(char *str, char* str2)
{
    QDialog  *confirmation = new QDialog();
    confirmation->setWindowFlags(Qt::FramelessWindowHint);
    confirmation->setAutoFillBackground(true);
    confirmation->setStyleSheet(" color: rgb(85, 0, 127); background-color:  rgb(255,255,255)");
    confirmation->setGeometry(0, 0, 320, 240);

    QLabel *msg = new QLabel(confirmation);
    msg->setStyleSheet("font-size: 16pt; font-weight: bold; color: rgb(255,255,255); background-color:  rgb(85, 0, 127)");
    msg->setGeometry(0,0,320,40);
    msg->setText(str);
    msg->setWordWrap(true);
    msg->setAlignment(Qt::AlignRight|Qt::AlignBottom);
    //msg->setPicture();

//    QLabel *msg1 = new QLabel(confirmation);
//    msg1->setStyleSheet("font-size: 16pt; font-weight: bold; color: rgb(255,255,255); background-color:  rgb(85, 0, 127)");
//    //msg1->setGeometry(0,0,320,40);
//    msg1->setText(str2);
//    msg1->setWordWrap(true);
//    msg1->setAlignment(Qt::AlignLeft|Qt::AlignBottom);

//    label1->setText("SHG TRANSACTION");

//    str = "font-size: 14pt; font-weight: bold; color:rgb(0,0,0);";
//    label_acc_number->setStyleSheet(str);
//    label_acc_number->setGeometry(10,40,300,15);
//    label_acc_number->clear();
//    label_acc_number->setText("SHG account number:");

    char *str1 = NULL;
    str1 = "font-size: 14pt; font-weight: bold; color: rgb(255,255,255); background-color:  rgb(85, 0, 127)";

    QPushButton *ChangePasswdButton = new QPushButton(confirmation);
    //str = "font-size: 14pt; font-weight: bold; color: rgb(255,255,255); background-color:  rgb(85, 0, 127)";
    ChangePasswdButton->setStyleSheet(str1);
    ChangePasswdButton->setGeometry(50,70,200,30);
    ChangePasswdButton->setText("Change Password");

    QObject::connect(ChangePasswdButton, SIGNAL(clicked()), confirmation, SLOT(chngepasswd()));
    QObject::connect(ChangePasswdButton, SIGNAL(clicked()), confirmation, SLOT(accept()));

    QPushButton *UpdateVerButton = new QPushButton(confirmation);
    UpdateVerButton->setStyleSheet(str1);
    UpdateVerButton->setGeometry(50,115,200,30);
    UpdateVerButton->setText("Update Version");

    QObject::connect(UpdateVerButton, SIGNAL(clicked()), confirmation, SLOT(updateversion()));
    QObject::connect(UpdateVerButton, SIGNAL(clicked()), confirmation, SLOT(accept()));

    QPushButton *LogOutButton = new QPushButton(confirmation);
    LogOutButton->setStyleSheet(str1);
    LogOutButton->setGeometry(50,160,200,30);
    LogOutButton->setText("Logout");

    QObject::connect(LogOutButton, SIGNAL(clicked()), confirmation, SLOT(logout()));
    QObject::connect(LogOutButton, SIGNAL(clicked()), confirmation, SLOT(accept()));

    QPushButton *BackButton = new QPushButton(confirmation);
    BackButton->setStyleSheet(str1);
    BackButton->setGeometry(150,200,130,30);
    BackButton->setText("BACK");

    QObject::connect(BackButton, SIGNAL(clicked()), confirmation, SLOT(reject()));

    int ret = confirmation->exec();
    qDebug()<<"Ret "<< ret;
    return ret;
}

void CMisc::rupay_DisplayMsgBox(char *str)
{
    QMessageBox msg;
    //msg.setGeometry(30,40,320,180);
   // msg.setStyleSheet("background-color:rgb(17,255,240)");
    msg.setStyleSheet("color: rgb(255, 255, 255); background-color: rgb(58, 114, 127)");
    msg.setWindowFlags(Qt::FramelessWindowHint);
    msg.setStandardButtons(QMessageBox::Ok);
    msg.setText(str);
    msg.exec();
    return;
}


int CMisc::DisplayMsgBox_SHG(char *str)
{
    QDialog  *confirmation = new QDialog();
    confirmation->setWindowFlags(Qt::FramelessWindowHint);
    confirmation->setAutoFillBackground(true);
    confirmation->setStyleSheet("color: rgb(85, 0, 127); background-color: rgb(255, 255, 255)");

    confirmation->setGeometry(0, 0, 320, 240);

    QLabel *msg = new QLabel(confirmation);
    msg->setGeometry(30, 60, 280, 100);
    msg->setText(str);
    msg->setWordWrap(true);
    msg->setAlignment(Qt::AlignHCenter|Qt::AlignVCenter);


    QPushButton *yesButton = new QPushButton(confirmation);
    yesButton->setGeometry(45,160,100,40);
    yesButton->setStyleSheet("color: rgb(255, 255, 255); background-color: rgb(85, 0, 127)");
    yesButton->setFont(QFont("Normal"));
    yesButton->setText("OK");

    QObject::connect(yesButton, SIGNAL(clicked()), confirmation, SLOT(accept()));

    QPushButton *NoButton = new QPushButton(confirmation);
    NoButton->setGeometry(195,160,100,40);
    NoButton->setStyleSheet("color: rgb(255, 255, 255); background-color: rgb(85, 0, 127)");
    NoButton->setFont(QFont("Normal"));
    NoButton->setText("CANCEL");

    QObject::connect(NoButton, SIGNAL(clicked()), confirmation, SLOT(reject()));

    int ret = confirmation->exec();

    qDebug()<<"Ret "<< ret;
    return ret;
}

///////////////////////////////////////////////////////////////////
//
// This below function is used to get Machine ID of the device
//
//
//  Input :
//  Type  :
//  Output:
//
//  Date  :                           Author:
///////////////////////////////////////////////////////////////////


int CMisc::GetMachineId(char *machineid)
{
    FILE *fp;
    char filename[]="/mnt/jffs2/fmachineid";
    char buff[20]="";
    char *p, *q;

    system("fw_printenv hwid > /mnt/jffs2/fmachineid");
    memset(machineid,0,sizeof(machineid));

    fp=fopen(filename,"r");
    if(fp != NULL)
    {
        memset(buff, 0, sizeof(buff));
        get_line_init(buff,fp);
        fprintf(stdout, "buff = %s##\n", buff);
        fclose(fp);

        if(strlen(buff) > 0)
        {
            p = strchr(buff, '=');
            if(p == NULL)
                return -1;

            q = p+1;
        }

        strcpy(machineid, q);
        fprintf(stdout, "machine id = %s###\n", machineid);
    }
    else
    {
        strcpy(machineid, "Machine ID Not Found");
        return -1;
    }

    return 0;
}

int CMisc::get_line_init(char *buff, FILE  *fd)
{
    char ch=' ';
    int i=-1;

    ch=fgetc(fd);
    while(!feof(fd))
    {
        i++;
        if(i>1023)
            return -1;
        buff[i]=ch;
        if(ch=='\n')break;
        ch=fgetc(fd);
        buff[i+1]='\0';
    }
    buff[strlen(buff)-1] = '\0';
    return i;
}

///////////////////////////////////////////////////////////////////
//
// This below function is used to play numbers
//
//
//  Input :
//  Type  :
//  Output:
//
//  Date  :                           Author:
///////////////////////////////////////////////////////////////////


int CMisc::play(char *str)
{
    //    QString QAudioVal;
    //    QSqlQuery query;
    //    query.prepare("SELECT RFU FROM paramdownloads");
    //    if(!query.exec())
    //        qDebug() << query.lastError();
    //    else
    //    {
    //        while(query.next())
    //        {
    //            QAudioVal = query.value(0).toString().at(1);
    //        }
    //    }

    //    if((QAudioVal == "N") || (QAudioVal == "n"))
    //        return -1;


    int i;
    int a;
    int m=1;
    int res[10];
    int res2[10];
    int cnt=0;
    int cnt2=0;
    int j=0;
    char wave[20][20];
    int flag = 1;
    memset(res,0,sizeof(res));
    memset(res2,0,sizeof(res2));

    i=atoi(str);
    if(i>=0 && i<=100)//If you have 0 to 100 direct mp3 files
        //if(i>=0 && i<=20)
    {
        //sprintf(wave[0],"%d.mp3",i);
        sprintf(wave[0],"%d",i);
        sound_test(wave[0]);
        //sound_test("RUPEES.mp3");
        sound_test("RUPEES");
        return 0;
    }
    //---------- split the number -----
    while(i)
    {
        a=i%10;
        i=i/10;
        a=a*m;
        m=m*10;
        res[cnt++] = a;
        j++;
    }
    cnt2 = 0;
    cnt =0;
    while(cnt<j)
    {
        cnt++;
    }
    cnt =0;

    while(cnt<j)
    {
        switch(cnt)
        {
        case 0:
            //printf("case 0\n");
            if(res[cnt+1] ==0)
            {
                res2[cnt2] = res[cnt];
            }
            break;

        case 1:
            //printf("case 1\n");
            if(res[cnt]>0)
            {
                res2[cnt2] = res[cnt] + res[cnt-1];
            }
            break;
        case 2:
            //printf("case 2\n");
            if(res[cnt]>0)
            {
                res2[cnt2] = 100;
                cnt2++;
                res2[cnt2] = res[cnt]/100;
            }
            break;
        case 3:
            //printf("case 3\n");
            if(res[cnt]>0)
            {
                flag = 0;
                res2[cnt2] = 1000;
            }
            if(res[cnt+1]==0)
            {
                cnt2++;
                res2[cnt2] = res[cnt]/1000;
            }
            break;
        case 4:
            //printf("case 4\n");
            /*if(res[cnt]<10)
                {
                    res2[cnt2] = res[cnt]/1000;
                    //cnt2++;
                }
                else*/
            if(res[cnt]>0)
            {
                if(flag == 1)
                {
                    res2[cnt2] = 1000;
                    cnt2++;
                }
                res2[cnt2] = (res[cnt] + res[cnt-1])/1000;
            }
            break;
        case 5:
            //printf("case 5\n");
            /*				if(res[cnt]>0)
                {
                    res2[cnt2] = 100000;
                    cnt2++;
                    res2[cnt2] = res[cnt]/100000;
                }
*/
            if(res[cnt]>0)
            {
                res2[cnt2] = 100000;
                //cnt2++;
                //res2[cnt2] = res[cnt]/100000;
            }
            if(res[cnt+1] >= 1000000)
            {
                res2[cnt2] = 100000;
                cnt2++;
                res2[cnt2] = (res[cnt] + res[cnt+1])/100000;
            }
            else
            {
                cnt2++;
                res2[cnt2] = res[cnt]/100000;
            }

            break;


        }
        if(cnt)
            cnt2++;
        cnt++;
    }
    j=cnt2;
    cnt2 = 0;
    cnt = 0;
    memset(res,0,sizeof(res));
    while(cnt2<j )
    {
        if(res2[cnt2] !=0)
        {
            res[cnt] = res2[cnt2];
            cnt++;
        }
        cnt2++;
    }

    i = 0;
    while(cnt)
    {
        cnt--;
        //sprintf(wave[i],"%d.mp3",res[cnt]);
        sprintf(wave[i],"%d",res[cnt]);
        i++;
    }

    i = 0;
    while(res[i]!='\0')
    {
        sound_test(wave[i]);
        //		sleep(4);
        i++;
    }
    //sound_test("RUPEES.mp3");
    sound_test("RUPEES");
    return 0;

}

int CMisc::sound_test (char * filename)
{
    //char buff[50]="/mnt/jffs2/";
    char buff[50];
    //int ret = 0;
    int n=0, p=0;

    //    if(strcmp(filename, "RUPEES") == 0)
    //        return 0;

    if(atoi(filename) > 20 && atoi(filename) <100)
    {
        n = atoi(filename);
        p = (n/10)*10;
        printf("file name b/w 20 to 100 -> %s\n", filename);

        memset(buff, 0, sizeof(buff));
        sprintf(buff, "%d", p);

        mp3play(buff);

        p = n%10;

        if(p > 0)
        {
            memset(buff, 0, sizeof(buff));
            sprintf(buff, "%d", p);

            mp3play(buff);
        }
    }
    else if((atoi(filename) >0 && atoi(filename) <=20 ) || atoi(filename) >=100)
    {
        printf("in else %s\n", filename);

        memset(buff, 0, sizeof(buff));
        sprintf(buff, "%s", filename);

        mp3play(buff);

    }
    else
        mp3play("999");

    return 0;
}

int CMisc::mp3play(char *buff)
{
    char frame[128];
    int volCount = 192;
    //    int DBFlag = 1;
    int num = 0;
    char mp3File[16];

    memset(mp3File, 0, sizeof(mp3File));
    num = atoi(buff);

    qDebug()<<"Number = "<<num;

    if(get_audio_file(num, mp3File) < 0)
    {
        return -1;
    }

    qDebug()<<mp3File<<" Playing....";
    qDebug()<<"-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*";

    //return 0;
    //gl11_audio_power_on();

    int DBFlag = GetAudioFlag();

    memset(frame,0,sizeof(frame));
    sprintf(frame,"amixer cset numid=4 %d",volCount);
    system(frame);

    memset(frame,0,sizeof(frame));
    sprintf(frame,"mplayer -autosync 30 -speed 3 /mnt/jffs2/MP3/%05d%s.mp3",DBFlag, mp3File);
    system(frame);
    usleep(1000);

    //    gl11_audio_power_off();
    return 0;
}


int CMisc::get_audio_file(int num, char *buff)
{
    switch(num)
    {
    case 0:
        //strcpy(buff,"00001000.mp3");
        strcpy(buff,"000");
        break;
    case 1:
        strcpy(buff,"001");
        break;
    case 2:
        strcpy(buff,"002");
        break;
    case 3:
        strcpy(buff,"003");
        break;
    case 4:
        strcpy(buff,"004");
        break;
    case 5:
        strcpy(buff,"005");
        break;
    case 6:
        strcpy(buff,"006");
        break;
    case 7:
        strcpy(buff,"007");
        break;
    case 8:
        strcpy(buff,"008");
        break;
    case 9:
        strcpy(buff,"009");
        break;
    case 10:
        strcpy(buff,"010");
        break;
    case 11:
        strcpy(buff,"011");
        break;
    case 12:
        strcpy(buff,"012");
        break;
    case 13:
        strcpy(buff,"013");
        break;
    case 14:
        strcpy(buff,"014");
        break;
    case 15:
        strcpy(buff,"015");
        break;
    case 16:
        strcpy(buff,"016");
        break;
    case 17:
        strcpy(buff,"017");
        break;
    case 18:
        strcpy(buff,"018");
        break;
    case 19:
        strcpy(buff,"019");
        break;
    case 20:
        strcpy(buff,"020");
        break;
    case 30:
        strcpy(buff,"030");
        break;
    case 40:
        strcpy(buff,"040");
        break;
    case 50:
        strcpy(buff,"050");
        break;
    case 60:
        strcpy(buff,"060");
        break;
    case 70:
        strcpy(buff,"070");
        break;
    case 80:
        strcpy(buff,"080");
        break;
    case 90:
        strcpy(buff,"090");
        break;
    case 100:
        strcpy(buff,"666");//Hundred
        break;
    case 1000:
        strcpy(buff,"777");//Thousond
        break;
    case 10000:
        strcpy(buff,"888");//Ten Thousond
        break;
    case 100000:
        strcpy(buff,"888");//Lackh
        break;
        //    case 1:
        //      strcpy(buff,"555");//AND
        //        break;

    default:
        strcpy(buff,"999");//RUPEES
        break;
    }

    return 0;
}

///////////////////////////////////////////////////////////////////
//
// This below function is used to remove spaces from left side
//
//
//  Input :
//  Type  :
//  Output:
//
//  Date  :                           Author:
///////////////////////////////////////////////////////////////////

void CMisc::ltrim( char *string, char *trim )
{
    while ( string[0] != '\0' && strchr( trim, string[0] ) != NULL )
    {
        memmove( &string[0], &string[1], strlen(string) );
    }
}

///////////////////////////////////////////////////////////////////
//
// This below function is used to remove spaces from right side
//
//
//  Input :
//  Type  :
//  Output:
//
//  Date  :                           Author:
///////////////////////////////////////////////////////////////////

void CMisc::rtrim(char *string, char *trim)
{
    int i=0,j=0;

    //    while (string[i] != '\0')
    //    {
    //        if (!(string[i] == ' ' && string[i+1] == ' '))
    //        {
    //            trim[j] = string[i];
    //            j++;
    //        }
    //        i++;
    //    }
    //    trim[j] = '\0';

    //int i;
    for( i = strlen (string) - 1; i >= 0 && strchr (trim, string[i] ) != NULL; i-- )
        string[i] = '\0';
}

///////////////////////////////////////////////////////////////////
//
// This below function is used to disconnect GPRS
//
//
//  Input :
//  Type  :
//  Output:
//
//  Date  :                           Author:
///////////////////////////////////////////////////////////////////

int CMisc::DisConnectGPRS()
{

//    uidmessage::instance()->Display_BusyMessage("Please Wait...");
//    ppp_close();
//    sleep(1);

//     uidmessage::instance()->hide_Message();
    return 0;
}

int CMisc::DisConnectGPRS_new()
{

    uidmessage::instance()->Display_BusyMessage("Please Wait...");
    ppp_close();
    sleep(1);

     uidmessage::instance()->hide_Message();
    return 0;
}

///////////////////////////////////////////////////////////////////
//
// This below function is used to connect GPRS
//
//
//  Input :
//  Type  :
//  Output: Return 0(Success) OR 1 (Failure)
//
//  Date  :                           Author: Vaibahv Lamane
///////////////////////////////////////////////////////////////////

int CMisc::ConnectGPRS()
{
//    qDebug() << "In ConnectGPRS()";

//    uidmessage::instance()->Display_BusyMessage("Connecting GPRS");

//    QSqlQuery query;
//    char cGprsAPN[64], gprsbuff[64];

//    query.prepare("SELECT gprs_apn FROM paramflag_temp");
//    if(!query.exec())
//        qDebug() << query.lastError();
//    else
//    {
//        while(query.next())
//        {
//            memset(gprsbuff, 0, sizeof(gprsbuff));
//            strcpy(gprsbuff, query.value(0).toString().trimmed().toAscii().data());
//            qDebug()<<"GPRS APN EXTRACTED from DATABASE"<<query.value(0).toString().toAscii().data();
//            qDebug()<<"GPRS APN STRORED TO BUFF:"<<gprsbuff;
//        }
//    }

//    memset(cGprsAPN, 0, sizeof(cGprsAPN));
//    strcpy(cGprsAPN,gprsbuff);          // akshay J changes 20 Nov 15
//    rtrim(cGprsAPN, " ");    // trim the remaining space characters

//    ltrim(cGprsAPN," ");


//    ppp_options("1234","1234");  // gprs connection launch
//    pppdial_gprs("*99***1#");
//    ppp_dial_update("*99***1#",cGprsAPN);

//    if(ppp_open() != 0)
//    {
//        ppp_close();

//        gl11_lcdbklt_on();
//        CMisc miscObj;
//        uidmessage::instance()->hide_Message();
//        miscObj.DisplayMsgBox((char *)"GPRS Not Connected");
//        return -1;
//    }
//     uidmessage::instance()->hide_Message();
//    uidmessage::instance()->Display_BusyMessage("GPRS Connected");
//    sleep(2);
//     uidmessage::instance()->hide_Message();
    return 0;

}

int CMisc::ConnectGPRS_new()
{
    qDebug() << "In ConnectGPRS()";

    uidmessage::instance()->Display_BusyMessage("Connecting GPRS");

    QSqlQuery query;
    char cGprsAPN[64], gprsbuff[64];

//    query.prepare("SELECT gprs_apn FROM paramflag_temp");
//    if(!query.exec())
//        qDebug() << query.lastError();
//    else
//    {
//        while(query.next())
//        {
//            memset(gprsbuff, 0, sizeof(gprsbuff));
//            strcpy(gprsbuff, query.value(0).toString().trimmed().toAscii().data());
//            qDebug()<<"GPRS APN EXTRACTED from DATABASE"<<query.value(0).toString().toAscii().data();
//            qDebug()<<"GPRS APN STRORED TO BUFF:"<<gprsbuff;
//        }
//    }

    memset(cGprsAPN, 0, sizeof(cGprsAPN));
    strcpy(cGprsAPN,"fino2g");          // akshay J changes 20 Nov 15
    rtrim(cGprsAPN, " ");    // trim the remaining space characters

    ltrim(cGprsAPN," ");


    ppp_options("1234","1234");  // gprs connection launch
    pppdial_gprs("*99***1#");
    ppp_dial_update("*99***1#",cGprsAPN);

    if(ppp_open() != 0)
    {
        ppp_close();

//        gl11_lcdbklt_on(); // FPB
        CMisc miscObj;
        uidmessage::instance()->hide_Message();
        miscObj.DisplayMsgBox((char *)"GPRS Not Connected");
        return -1;
    }
     uidmessage::instance()->hide_Message();
    uidmessage::instance()->Display_BusyMessage("GPRS Connected");
    sleep(2);
     uidmessage::instance()->hide_Message();
    return 0;

}

///////////////////////////////////////////////////////////////////
//
// This below function is used to check IP address is valid or not
//
//
//  Input : ip address
//  Type  : char *
//  Output:
//
//  Date  :                           Author:
///////////////////////////////////////////////////////////////////

int CMisc::CHECK_IPADDRESS(char *Buff)
{
    register int i=0;
    int DOT=0;
    int res=0;
    int digit=0;
    int len;
    char buff[3];
    int num;


    len=strlen(Buff);
    //err_printf("length: %d",len);
    //DO not accept dots before numerics
    if(Buff[0]=='.')return 1;
    for(i=0;i<len;i++)
    {
        //checking for two consecutive dots
        if((Buff[i]=='.') && (Buff[i+1]=='.'))return 1;


        //check for dot occuring
        if(Buff[i]=='.'){DOT++;digit=0;}
        else if(DOT>=4){res=1;goto end;}


        //err_printf("%c",Buff[i]);
        //checking for numerics otherthan alpha,special chars.
        if(Buff[i]>='0' && Buff[i] <='9')
        {
            buff[digit]=Buff[i];
            digit++;if(digit<1 || digit>3){res=1;goto end;}
        }
        else if(!(Buff[i]>='0' && Buff[i]<='9') && !(Buff[i]=='.')){res=1;goto end;}


        //checking IP Address ranging from 0-255 only, if not simply thrown away
        if(digit>0)
        {
            buff[digit]='\0';
            num=atoi(buff);
            if(num<0 || num>255){res=1;goto end;}
        }
    }
    //Dots must be 3, nor lessthan 3 or greaterthan 3
    if(Buff[len-1]=='.' || DOT<3)return 1;
    //err_printf("%d\n",num);
    //err_printf("\nDot:%d\n",DOT);


end:
    return res;
}



///////////////////////////////////////////////////////////////////
//
// This below function is used to check GPRS Connectivity
//
//
//  Input :
//  Type  :
//  Output:
//
//  Date  :                           Author:
///////////////////////////////////////////////////////////////////



bool CMisc::GPRS_isAvailable()
{

    FILE *fp;
    int status;
    char chBuffer[11];
    memset(chBuffer, 0, sizeof(chBuffer));
    fp = popen("/sbin/ifconfig ppp0| wc -w", "r");

    while (fgets(chBuffer,6, fp) != NULL)
    {
        qDebug() << atoi(chBuffer);
    }

    status = pclose(fp);
    if (status == -1)
    {
        qDebug() << "Error closing pipe";
    }

    if(atoi(chBuffer)<=0)
    {

        qDebug() << "GPRS Disconnected";
        return false;

    }

    if(atoi(chBuffer)>0)
    {
         qDebug()<<"GPRS already connected"<<chBuffer;

         return true;
    }


}
