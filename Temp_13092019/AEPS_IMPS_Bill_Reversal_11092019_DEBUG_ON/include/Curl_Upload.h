#ifndef CURL_UPLOAD_H
#define CURL_UPLOAD_H

#include <QDebug>

class Curl_Upload
{
public:
    Curl_Upload();
    int Upload_File(char *, char *);
    int Check_Exist_Remote_File(char *, int, int);

};


#endif // CURL_UPLOAD_H
