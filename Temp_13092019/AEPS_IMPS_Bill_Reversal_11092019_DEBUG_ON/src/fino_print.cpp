
//#include "print.h"
#include "fino_print.h"
//#include <QDebug>
//#include <QBitmap>

unsigned char bmpbuf[30000]="";

extern "C"
{
#include "gl11.h"
}
/*
void FinoPrint::PrintLogo()
{

       //creates hex array from bmp image
        createHexArray(1,"/mnt/jffs2/fino.bmp",true);

        //printing
        takePrint(1);

}
*/
int FinoPrint::PrintLogo(int iLogoType)//int iLogoType)
{
    char bmp_file[64];
    char file[128];
    //int iLogoType = 0;

   // iLogoType = getLogoTypeFromDB();
    qDebug() << "LogoTYpe = " << iLogoType;


    memset(file, 0, sizeof(file));
    sprintf(file, "/mnt/jffs2/Logo/%d.bmp", iLogoType);

    QFile ChkFile(file);
    if(!ChkFile.exists())
    {
        qDebug() << "Logo_Filename Not Exists in Logo folder";
        return -1;
    }

    //creates hex array from bmp image
    //createHexArray(1,"/mnt/jffs2/fino.bmp",true);
    createHexArray(1, file ,true);

    qDebug()<<"logo 1";
    //printing
    int ret = takePrint(1);

    system("rm -f *.bmp");
    system("rm -f *.hex");
    system("rm -f *.h");

    return ret;
}

int FinoPrint::getLogoTypeFromDB()
{
    QSqlQuery query;
    int iLogoId = 0;
    char straddfield[100],logid[3];

    query.prepare("SELECT flagvalue from paramflag where headername='logo_id'"); // akshay J changes 23 NOV 15
    if(!query.exec())
        qDebug() << query.lastError();
    else
    {
        while(query.next())
        {
            memset(straddfield,0,sizeof(straddfield));
            strcpy(straddfield, query.value(0).toString().trimmed().toAscii().data());
        }
    }

//    memset(logid, 0, sizeof(logid));  // akshay j commented
//    strncpy(&logid[0],&straddfield[4],2);
//    logid[3] = '\0';


    qDebug()<< "Logo id "<<atoi(straddfield); // akshay j changes

    //return atoi(logid);
    return atoi(straddfield);  // akshay j changes
}

void FinoPrint::get_bmp_file_name(int iLogoType, char *szLogoFile)
{
    switch(iLogoType)
    {
    case LOGO_ANDHRA:       strcpy(szLogoFile, "Andhra_Bank.bmp");      break;
    case LOGO_BARODA:       strcpy(szLogoFile, "Bank_of_Baroda.bmp");   break;
    case LOGO_DHANALAKSHMI: strcpy(szLogoFile, "Dhanlakshmi_Bank.bmp"); break;
    case LOGO_ICICI:        strcpy(szLogoFile, "ICICI.bmp");            break;
    case LOGO_JANALAKSHMI:  strcpy(szLogoFile, "Janalakshmi.bmp");      break;
    case LOGO_KOTAK:        strcpy(szLogoFile, "Kotak.bmp");            break;
    case LOGO_PUNJAB:       strcpy(szLogoFile, "Punjab.bmp");           break;
    case LOGO_UNION:        strcpy(szLogoFile, "Union.bmp");            break;
    case LOGO_APGOVT:       strcpy(szLogoFile, "AP.bmp");               break;
    case LOGO_CORP:         strcpy(szLogoFile, "Corporation_Bank.bmp"); break;
    case LOGO_FINO:         strcpy(szLogoFile, "Fino.bmp");             break;
    case LOGO_INDIAN:       strcpy(szLogoFile, "Indian_Bank.bmp");      break;
    case LOGO_JK:           strcpy(szLogoFile, "jk.bmp");               break;
    case LOGO_KRISHNA:      strcpy(szLogoFile, "Krishna_Bank.bmp");     break;
    case LOGO_PURVANCHAL:   strcpy(szLogoFile, "Purvanchal.bmp");       break;
    case LOGO_UNITED:       strcpy(szLogoFile, "United_Bank.bmp");      break;
    case LOGO_AXIS:         strcpy(szLogoFile, "Axis.bmp");             break;
    case LOGO_DECCAN:       strcpy(szLogoFile, "Deccan.bmp");           break;
    case LOGO_GURGOAN:      strcpy(szLogoFile, "Gurgoan_Bank.bmp");     break;
    case LOGO_ING:          strcpy(szLogoFile, "ING_Vysya.bmp");        break;
    case LOGO_JK_GRAMIN:    strcpy(szLogoFile, "J_&_K.bmp");            break;
    case LOGO_NMB:          strcpy(szLogoFile, "NMB_Bank.bmp");         break;
    case LOGO_UCO:          strcpy(szLogoFile, "UCO_Bank.bmp");         break;

    default: strcpy(szLogoFile, "No_File");                        break;
    }

    return;
}

void FinoPrint::createHexArray(int fileno,char * filepath,bool bargCheck )
{
    FILE *fp,*fo,*fs;

    unsigned char   c;
    unsigned int    count;
    unsigned long     i;
    // ***Header***
    unsigned char   signature[2];
    unsigned long   filesize;
    unsigned long   reserved;
    unsigned long   dataoffset;

    // ***InfoHeader***
    unsigned long   size;
    unsigned long   width;
    unsigned long   height;
    unsigned short  planes;
    unsigned short  bitcount;
    unsigned long   compression;
    unsigned long   imagesize;
    unsigned long   xpixelsperm;
    unsigned long   ypixelsperm;
    unsigned long   colorsused;
    unsigned long   colorsimportant;

    unsigned char   res;

    char catCmd[1024] = "";

    char bmp_filename[60] = "";
    char hex_filename[60] = "";
    char hexArr_filename[255] = "";

    //bmp file path
    sprintf(bmp_filename,"%s",filepath);

    sprintf(hex_filename,"%s%d%s","bmpToHex_",fileno,".hex");
    sprintf(hexArr_filename,"%s%d%s","bmpHexArr_",fileno,".h");

    fp=fopen(bmp_filename,"rb");
    if(fp==NULL)
    {
        puts("cannot open input file");
        return;
    }

    fo=fopen(hex_filename,"wb");
    if(fo==NULL)
    {
        puts("cannot create output file");
        return;
    }

    //printf("\n\nBITMAP FILE HEADER:\n");

    for(i=0;i<2;i++){
        fread(&c,sizeof(c),1,fp);
        signature[i]=c;
    }
    /*
        printf("\n      Signature:");
        for(i=0;i<2;i++){
                printf("%c",signature[i]);
        }
    */

    fread(&filesize,sizeof(filesize),1,fp);
    //printf("\n      FileSize:%lu",filesize);

    fread(&reserved,sizeof(reserved),1,fp);
    //printf("\n      Reserved:%lu",reserved);

    fread(&dataoffset,sizeof(dataoffset),1,fp);
    //printf("\n      DataOffset:%lu",dataoffset);

    //printf("\n\nBITMAP INFOHEADER:\n");

    fread(&size,sizeof(size),1,fp);
    //printf("\n      Size:%lu",size);

    fread(&width,sizeof(width),1,fp);
    //printf("\n      Width:%lu",width);

    fread(&height,sizeof(height),1,fp);
    //printf("\n      Height:%lu",height);

    fread(&planes,sizeof(planes),1,fp);
    //printf("\n      Planes:%u",planes);

    fread(&bitcount,sizeof(bitcount),1,fp);
    //printf("\n      BitCount:%u",bitcount);

    fread(&compression,sizeof(compression),1,fp);
    //printf("\n      Compression:%lu",compression);

    fread(&imagesize,sizeof(imagesize),1,fp);
    //printf("\n      ImageSize:%lu",imagesize);

    fread(&xpixelsperm,sizeof(xpixelsperm),1,fp);
    //printf("\n      XpixelsPerM:%lu",xpixelsperm);

    fread(&ypixelsperm,sizeof(ypixelsperm),1,fp);
    //printf("\n      YpixelsPerM:%lu",ypixelsperm);

    fread(&colorsused,sizeof(colorsused),1,fp);
    //printf("\n      ColorsUsed:%lu",colorsused);

    fread(&colorsimportant,sizeof(colorsimportant),1,fp);
    //printf("\n      ColorsImportant:%lu",colorsimportant);

    count=0;

    for(i=0;i<=imagesize;i++){
        fseek(fp,-i,SEEK_END);
        fread(&res,sizeof(res),1,fp);
        //read from 1st byte not from 0th byte
        if(i!=0){

            if(bargCheck == true)
            {
                //res =~res;
                fprintf(fo,"0x%x,",res);//Logo Printing From BMP
            }
            else
            {
                res =~res;
                fprintf(fo,"0x%x,",res);//Signature & MultiLingual
            }

            count++;
        }
        if(count>=16){
            fprintf(fo,"\n");
            count=0;
        }
    }

    fclose(fp);
    fclose(fo);

    sprintf(catCmd,"%s%s%s%s","cat ",hex_filename," >> ",hexArr_filename);

    fs=fopen(hexArr_filename,"w");
    if(fs==NULL)
    {
        puts("cannot create array buffer file - 1");
        return;
    }

    fprintf(fs,"unsigned char shot_bmp[]=\n{\n");

    fclose(fs);

    //printf("cat command is : %s\n", catCmd);
    system(catCmd);

    fs=fopen(hexArr_filename,"a");
    if(fs==NULL)
    {
        puts("cannot open array buffer file - 2");
        return;
    }

    fprintf(fs,"};");

    fclose(fs);

}

int FinoPrint::takePrint(int fileno)
{

    char bmp[30000]="",hexArr_filename[255]="";
    int ret,len,ret_bmp;

    sprintf(hexArr_filename,"%s%d%s","bmpHexArr_",fileno,".h");

    Check_bitmap(hexArr_filename);
    len=Get_bitmap(bmp,hexArr_filename);
    //printf("Lenght of the buff %d\n",len);

    ret_bmp = prepare_bmp_image((unsigned char*)bmp,len);
    //printf("Lenght of the ret buff %d\n",ret_bmp);
    ret = prn_write_bmp(((unsigned char*)bmpbuf),ret_bmp);
//    printf("Return of the prn_write_bmp %d\n",ret);

    return ret;
}

int FinoPrint::Get_bitmap(char *buf,char *file_name)
{
        char ch;
        int val=0,i=0;
        FILE *fp;

        fp=fopen(file_name,"r");
        if(fp==NULL)
                return FILEERROR;

        while(!feof(fp)){
                ch=fgetc(fp);
                if(ch=='x')
                {
                        fscanf(fp,"%x",&val);
                        buf[i++]=val;
                }

        }
        fclose(fp);
        //fprintf(stdout,"Bitmap size: %d\n",i);
        return i;
}

int FinoPrint::prepare_bmp_image(unsigned char *data,int len)
{
        int i,j;
        for(i=0,j=0; i<len; i++){
                if(j >29800){
                        return ERROR;
                }
                if(i%48 == 0)
                        j=j+(48*4);
                bmpbuf[j+(47-(i%48))] = ~data[i];
                bmpbuf[j+48+(47-(i%48))] = ~data[i];
                bmpbuf[j+96+(47-(i%48))] = ~data[i];
                bmpbuf[j+48+96+(47-(i%48))] = ~data[i];
        }
        return j;
}

int FinoPrint::Check_bitmap(char *file_name)
{
        char ch;
        char *n=NULL;
        int i=0;
        FILE *fp;
        char *buff;
        struct stat buf;

    //printf("file_name =%s\n",file_name);
        i = stat (file_name, &buf );
        if((i<0) || (buf.st_size == 0)){
                printf("empty file received\n");
                remove(file_name);
                printf("Bitmap is empty\n");
                return FILE_EMPTY_RCVD;
        }
        else {

                fp=fopen(file_name,"r");
                if(fp!=NULL){
                        buff= (char *)malloc(buf.st_size+1);
                        ch=fgetc(fp);
                        while(!feof(fp)){
                                buff[i]=ch;
                                i++;
                                buff[i]='\0';
                                ch=fgetc(fp);
                        }
                        buff[i]='\0';
                        fclose(fp);
                        n=strchr(buff,'}');
                        if(n!=NULL){
                                free(buff);
                                return SUCCESS;
                        }
                        else{
                                remove(file_name);
                                printf("Bitmap is invalid\n");
                                free(buff);
                                return INVALID_BITMAP;
                        }
                }
                else{
                        remove(file_name);
                        printf("Bitmap doesn't exists\n");
                        return FILENEXISTS;
                }
        }
        return SUCCESS;
}
