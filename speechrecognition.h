#ifndef SPEECHRECOGNITION_H
#define SPEECHRECOGNITION_H

#include <QObject>
#include <QAudioInput>
#include <QAudio>
#include <QBuffer>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QFile>
#include <QTimer>

class SpeechRecognition : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool listening READ isListening NOTIFY listeningStarted)
    Q_PROPERTY(QString speechText READ speechText NOTIFY speechTextReady)

public:
    SpeechRecognition(QObject *parent = 0);

    Q_INVOKABLE void start();
    Q_INVOKABLE void stop();

    bool isListening();
    QString speechText() { return m_speechText; }
    void listen();

signals:
    void speechTextReady();
    void listeningStarted();
    void listeningStopped();

private slots:
    void networkReplyFinished(QNetworkReply* reply);
    void handleStateChanged(QAudio::State newState);

private:
    QNetworkAccessManager* m_network;
    QNetworkReply* m_reply;
    QFile outputFile;
    QTimer m_timer;
    QAudioFormat m_audioFormat;
    QAudioInput* m_audioInput;
    QByteArray* m_audioData;
    QBuffer* m_audioBuffer;
    bool listening;
    QString m_speechText;
};

#endif // SPEECHRECOGNITION_H
