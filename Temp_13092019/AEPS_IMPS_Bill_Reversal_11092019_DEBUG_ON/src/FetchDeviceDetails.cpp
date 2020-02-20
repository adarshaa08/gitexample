#include "FPB.h"
#include "HostComm.h"
#include "stdint.h"
#include "impscommon.h"
extern INT sock_fd;
extern SUPPORTDATA_DEVICEINFO sd_deviceinfo;

int FPB::get_mac_id(char *macid, char *IP_Address)
{
    int fd,i;
    char ethtype[8]="eth0";
    HostComm hComm;
    struct ifreq ifr;
    sock_fd=socket(AF_INET,SOCK_DGRAM,0);
    ifr.ifr_addr.sa_family = AF_INET;
    strncpy(ifr.ifr_name, ethtype, IFNAMSIZ-1);
    ioctl(sock_fd,SIOCGIFHWADDR,&ifr); // MAC ID

    ioctl(sock_fd, SIOCGIFADDR, &ifr); // IP Address

    hComm.close_connection();
    // sprintf(macid,"%02X:%02X:%02X:%02X:%02X:%02X",(unsigned char)ifr.ifr_hwaddr.sa_data[0],&nbsp,(unsigne char)ifr.ifr_hwaddr.sa_data[1],(unsigned char)ifr.ifr_hwaddr.sa_data[2],(unsigned char)ifr.ifr_hwaddr.sa_data[3],(unsigned char)ifr.ifr_hwaddr.sa_data[4],(unsigned char)ifr.ifr_hwaddr.sa_data[5]);
    sprintf(macid, "%02X:%02X:%02X:%02X:%02X:%02X", (unsigned char)ifr.ifr_hwaddr.sa_data[0],(unsigned char)ifr.ifr_hwaddr.sa_data[1],(unsigned char)ifr.ifr_hwaddr.sa_data[2],(unsigned char)ifr.ifr_hwaddr.sa_data[3],(unsigned char)ifr.ifr_hwaddr.sa_data[4],(unsigned char)ifr.ifr_hwaddr.sa_data[5]);
    for(i=0;i<(sizeof(macid));i++)
    {
        if(macid[i]==' ')
            macid[i]='0';
    }
    sprintf(IP_Address,"%s",inet_ntoa(((struct sockaddr_in *)&ifr.ifr_addr)->sin_addr));

    return 0;
}

char *FPB::get_Lat_long()
{
    FILE *fp2 = NULL;
    char buff_geo[100];
    memset(buff_geo, 0, sizeof(buff_geo));
    system("cd /var/");
    fp2 = fopen("/var/.geo_cell.txt", "rb"); // open
    if(fp2 == NULL)
    {
        memcpy(buff_geo,"Latitude:00.00,Longitude:00.00",22);

    }
    else
    {
        fseek(fp2, 0, SEEK_END);
        int len_geo = ftell(fp2);
        fseek(fp2, 0, SEEK_SET);

        fread(buff_geo,sizeof(unsigned char ),len_geo,fp2);
        fclose(fp2);
    }

    return buff_geo;

}


int FPB::DeviceInfo(void)
{
    Clear_DeviceInfo();

    char arr[20], arr_ip[16];
    char itimedate[20], ilatlong[100];

    QSqlQuery query;
    memset(arr, 0, sizeof(arr));
    memset(arr_ip, 0, sizeof(arr_ip));
    memset(itimedate, 0, sizeof(itimedate));
    memset(ilatlong, 0, sizeof(ilatlong));

    // ---------------------   MAC ID   ---------------------------

    int imac = get_mac_id(arr, arr_ip);
    QString qMac = QString::fromUtf8((const char*)arr);
    qMac = qMac.remove(":");

    // --------------------   TIME STAMP   ------------------------

    strcpy(itimedate,iTimestamp());
#if DEBUG
    printf("Time Stamp == %s\n",itimedate);
#endif
    // ----------------   LATITUDE AND LONGITUDE   ----------------

    strcpy(ilatlong,get_Lat_long());
#if DEBUG
    printf("ilatlong == %s\n",ilatlong);
#endif

    QString strToFind;

    QString Latitude = QString::fromLocal8Bit(ilatlong);
    QStringList ilatquery = Latitude.split(",");
#if DEBUG
    for(int i=0 ; i < ilatquery.length() ; i++)
        qDebug()<<"Query Response"<<ilatquery.at(i);
#endif

    strToFind.clear();
    QString str = ilatquery.at(0);
    strToFind = ":";
    QString value_latitude = str.mid(str.indexOf(strToFind)+strToFind.length());
#if DEBUG
    qDebug()<<"value_latitude = "<<value_latitude;
#endif
    str.clear();

    strToFind.clear();
    str = ilatquery.at(1);
    strToFind = ":";
    QString value_longitude = str.mid(str.indexOf(strToFind)+strToFind.length());
#if DEBUG
    qDebug()<<"value_longitude = "<<value_longitude;
#endif
    // --------------------   PREVIOUS RRN AND DATE   ----------------

    query.clear();
    char last_rrn[20], last_date[15];
    memset(last_rrn,0,sizeof(last_rrn));
    memset(last_date,0,sizeof(last_date));
    query.prepare("SELECT LAST_RRN, LAST_DATE from LastWithdrawalData");
    if(!query.exec())
    {
        qDebug() << query.lastError();
        query.clear();
        query.finish();
//        ui->stackedWidget->setCurrentIndex(2);
        return FUNCTION_FAILED;
    }
    else
    {
        while(query.next())
        {
            memset(last_rrn,0,sizeof(last_rrn));
            strcpy((char*)last_rrn, query.value(0).toString().trimmed().toAscii().data());
            //   printf("last_rrn = %s\n",last_rrn);
            memset(last_date,0,sizeof(last_date));
            strcpy((char*)last_date, query.value(1).toString().trimmed().toAscii().data());
            //  printf("last_rrn = %s\n",last_date);
        }
    }
    query.clear();

        if(!memcmp((char *)value_latitude.trimmed().toAscii().data(),"00.00",5))
        {
            memcpy(sd_deviceinfo.ucSupportData_CELLIDLatitude_req,"null",4);
            memcpy(sd_deviceinfo.ucSupportData_CELLIDLongitude_req,"null",4);
        }
        else
        {
            memcpy((char *)sd_deviceinfo.ucSupportData_CELLIDLatitude_req, value_latitude.trimmed().toAscii().data(), strlen(value_latitude.trimmed().toAscii().data()));   // 46 CHGAMT Charges (AEPS - 0)
            memcpy((char *)sd_deviceinfo.ucSupportData_CELLIDLongitude_req, value_longitude.trimmed().toAscii().data(), strlen(value_longitude.trimmed().toAscii().data()));   // 46 CHGAMT Charges (AEPS - 0)
        }

        memcpy((char *)sd_deviceinfo.ucSupportData_PVTXNID_req, last_rrn, strlen(last_rrn));  // 36 PVTXNID Previous Successful Txn ID ..length
        memcpy((char *)sd_deviceinfo.ucSupportData_PVTXNDT_req, last_date, strlen(last_date));  // 37 PVTXNDT Previous Successful Date 03\/03\/19
        memcpy((char *)sd_deviceinfo.ucSupportData_IPADDR_req, arr_ip, strlen(arr_ip));  // 38 IPADDR len 13
        memcpy((char *)sd_deviceinfo.ucSupportData_DEVICEID_req, qMac.trimmed().toAscii().data(), strlen(qMac.trimmed().toAscii().data())); // 39 DEVICEID Device Serial Number (Mac ID) 980CA5F2BEF1
        memcpy((char *)sd_deviceinfo.ucSupportData_MCCCellID_req, "0", 1); // 40 Cell ID
        memcpy((char *)sd_deviceinfo.ucSupportData_MCCLAC_req, "0", 1);   //  41 LAC
        memcpy((char *)sd_deviceinfo.ucSupportData_MCCMCC_req, "0", 1);   // 42 MCC
        memcpy((char *)sd_deviceinfo.ucSupportData_MCCMNC_req, "0", 1);   // 43 MNC
        memcpy((char *)sd_deviceinfo.ucSupportData_PRTXNID_req, itimedate, strlen(itimedate)); // 44 PRTXNID  Timestamps (Substring of RequestID) 030319050252729
        memcpy((char *)sd_deviceinfo.ucSupportData_SPLTSEQ_req, "0", 1);  // 45 SPLTSEQ  1 - Txn Amount>PerTransactionLimit (Total) | 0: Txn Amount<=PerTransactionLimit (Total)
        memcpy((char *)sd_deviceinfo.ucSupportData_CHGAMT_req, "0", 1);   // 46 CHGAMT Charges (AEPS - 0)

        sprintf((char *)sd_deviceinfo.ucSupportData_CELLID_req, "%s,%s", (char *)sd_deviceinfo.ucSupportData_CELLIDLatitude_req, (char *)sd_deviceinfo.ucSupportData_CELLIDLongitude_req);
        sprintf((char *)sd_deviceinfo.ucSupportData_MCC_req, "%s,%s,%s,%s", (char *)sd_deviceinfo.ucSupportData_MCCCellID_req, (char *)sd_deviceinfo.ucSupportData_MCCLAC_req, (char *)sd_deviceinfo.ucSupportData_MCCMCC_req, (char *)sd_deviceinfo.ucSupportData_MCCMNC_req);

    return FUNCTION_SUCCESS;
}














