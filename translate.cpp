#include "translate.h"

Translate::Translate(QObject *parent) : QObject(parent),
    hostPath("https://api.microsofttranslator.com/V2/Http.svc/Translate")
{
    m_network = new QNetworkAccessManager(this);
    connect(m_network, &QNetworkAccessManager::finished,
                this, &Translate::networkReplyFinished);
}

void Translate::translate(QString text, QString fromLang, QString toLang)
{

    const QUrl url(hostPath + QString("?from=%2&to=%1&text=%3").arg(fromLang, toLang, text));
    QNetworkRequest req(url);
    req.setRawHeader(QByteArray("Ocp-Apim-Subscription-Key") , api_key.toUtf8());
    m_reply = m_network->get(req);
}

void Translate::networkReplyFinished(QNetworkReply *reply)
{

    int errorCode = reply->error();
    if (errorCode != 0){
        qDebug() << "Error: " << reply->readAll();
    } else{
        m_translate = reply->readAll();
        qDebug() << m_translate;
        emit translationTextReady();
    }

    reply->deleteLater();
}
