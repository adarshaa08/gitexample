#ifndef XMLFORMATCREATION_H
#define XMLFORMATCREATION_H
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkAccessManager>
#include <QSqlQuery>
#include <QSqlError>

class xmlformatRequestcreation: public QNetworkRequest
{
protected:
    QString xmlreqbody;
    QUrl xmlrequrl;

public:
    xmlformatRequestcreation();
    ~xmlformatRequestcreation();
    QString requestBody();
    // QString mou_leg1(QString Content_data);

};

#endif // XMLFORMATCREATION_H
