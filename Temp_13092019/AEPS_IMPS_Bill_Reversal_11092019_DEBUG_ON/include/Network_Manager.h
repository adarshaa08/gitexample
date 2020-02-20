#ifndef NETWORKMANAGER_H
#define NETWORKMANAGER_H

#include <QtNetwork/QNetworkAccessManager>


class NetworkManager : public QNetworkAccessManager {
public:
    NetworkManager(QObject *parent =0);

    virtual QNetworkReply *createRequest ( Operation op, const QNetworkRequest & req, QIODevice * outgoingData = 0 );

public slots:
    void sslErrors(QNetworkReply *reply, const QList<QSslError> &error);
};

#endif // NETWORKMANAGER_H
