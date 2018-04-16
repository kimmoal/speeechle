#ifndef TRANSLATE_H
#define TRANSLATE_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>

class Translate : public QObject
{
    Q_OBJECT
public:
    explicit Translate(QObject *parent = nullptr);

    void setApiKey(QString key) { api_key = key; }
    void translate(QString text, QString fromLang, QString toLang);

signals:
    void translationTextReady();

public slots:
    void networkReplyFinished(QNetworkReply *reply);

private:
    QNetworkAccessManager* m_network;
    QNetworkReply* m_reply;

    QString m_translate;

    QString api_key;
    QString hostPath;
};


#endif // TRANSLATE_H
