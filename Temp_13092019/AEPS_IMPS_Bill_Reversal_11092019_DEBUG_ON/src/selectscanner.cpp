#include "selectscanner.h"

SelectScanner::SelectScanner(QWidget *parent) :
    QWidget(parent)
{
}

///////////////////////////////////////////////////////////////////
//
// This below function will Select Scanner Type (Optical/Capacitive)
//
//  Input : Display Message
//  Type  : Qstring
//  Output:
//
//  Date  :                           Author:
///////////////////////////////////////////////////////////////////

int SelectScanner::scanner_status_opt1()
{
    FILE *fp;
    char *buf;
    const char *ptr;
    struct stat sb;


    system("cat /proc/bus/usb/devices > /mnt/jffs2/finger_print_opt");
    fp=fopen("/mnt/jffs2/finger_print_opt","r");
    if(fp==NULL)
    {
        qDebug("Fail to open the file");
        return -1;
    }
    else
    {
        stat("/mnt/jffs2/finger_print_opt",&sb);
        int size=sb.st_size;
        printf("size=%d\n",size);
        buf=(char *)malloc(sb.st_size);
        if(buf==NULL)
            perror("malloc\n");
        int ret=fread(buf,1,size,fp);
        ptr=strstr(buf,"SAGEM");
        if(ptr!=NULL)
        {
            free(buf);
            fclose(fp);
            return 1;   //OPTICAL;
        }
        ptr=strstr(buf,"UPEK");
        if(ptr!=NULL)
        {
            free(buf);
            fclose(fp);
            return 2;   //CAPACITIVE;
        }
        else
        {
            free(buf);
            fclose(fp);
            return -1;
        }
    }
}


