#ifndef CURLTXNS_H
#define CURLTXNS_H

#include <QDebug>

class CCurlTxns
{
public:
    CCurlTxns();

    int GET_file_clear(char *, char *, char *content, int UserPwdFlag);
    int GET_file_clear1(char *http_url, char *filename,int UserPwdFlag);
    int GET_file_upload(char *,char *,char *,int);
};

#endif // CURLTXNS_H
