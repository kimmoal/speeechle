#include "speechrecognition.h"
#include <QTimer>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

SpeechRecognition::SpeechRecognition(QObject* parent) : QObject(parent)
{

    m_network = new QNetworkAccessManager(this);
    listening = false;

    connect(m_network, &QNetworkAccessManager::finished,
                this, &SpeechRecognition::networkReplyFinished);
}

void SpeechRecognition::start()
{
    if (!listening) {
        listen();
    }
}

bool SpeechRecognition::isListening()
{
    return listening;
}

void SpeechRecognition::listen()
{

    QAudioFormat m_audioFormat;
    // Set up the desired format, for example:
    m_audioFormat.setSampleRate(16000);
    m_audioFormat.setChannelCount(1);
    m_audioFormat.setSampleSize(16);
    m_audioFormat.setCodec("audio/pcm");
    m_audioFormat.setByteOrder(QAudioFormat::LittleEndian);
    m_audioFormat.setSampleType(QAudioFormat::SignedInt);

    QAudioDeviceInfo info = QAudioDeviceInfo::defaultInputDevice();
    qDebug() << info.deviceName();
    if (!info.isFormatSupported(m_audioFormat)) {
        qWarning() << "Default format not supported, trying to use the nearest.";
        m_audioFormat = info.nearestFormat(m_audioFormat);
    }
    qDebug() << "Listening";
    qDebug() << "Samples: " << m_audioFormat.sampleRate();
    qDebug() << "Channels: " << m_audioFormat.channelCount();
    m_audioInput = new QAudioInput(m_audioFormat, this);
    connect(m_audioInput, &QAudioInput::stateChanged, this, &SpeechRecognition::handleStateChanged);

    m_audioData = new QByteArray;
    m_audioBuffer = new QBuffer(m_audioData);
    m_audioBuffer->open(QIODevice::WriteOnly);

    // Records audio for 3000ms
    /*
    QTimer m_timer(this);
    m_timer.setInterval(3000);
    m_timer.setSingleShot(true);
    connect(&m_timer, &QTimer::timeout, this, &SpeechRecognition::stop);
    m_timer.start();
    */

    listening = true;
    m_audioInput->start(m_audioBuffer);
    emit listeningStarted();
    qDebug() << m_audioInput->error();
    qDebug() << m_audioInput->state();
}

void SpeechRecognition::stop()
{
    if (!isListening()) {
        return;
    }
    qDebug() << "Stopped";
    qDebug() << m_audioInput->state();
    m_audioInput->stop();
    qDebug() << "Buffer size: " << m_audioBuffer->size();
    m_audioBuffer->close();

    listening = false;

    m_audioBuffer->open(QIODevice::ReadOnly);

    /*
    QFile outputFile("/tmp/saatana.raw");
    outputFile.open(QIODevice::WriteOnly);
    outputFile.write(m_audioBuffer->data());
    outputFile.close();
    */

    QString b64_audio = m_audioBuffer->data().toBase64();

    QString json = QString("{ 'config': { 'encoding': 'LINEAR16', 'sampleRateHertz': 16000, 'languageCode': 'en-US', 'enableWordTimeOffsets': false }, 'audio': { 'content': '%1' } }").arg(b64_audio);

    // json = "{ 'config': { 'encoding': 'FLAC', 'sampleRateHertz': 16000, 'languageCode': 'en-US', 'enableWordTimeOffsets': false }, 'audio': { 'uri': 'gs://cloud-samples-tests/speech/brooklyn.flac'  } }";

    QString token = "derp";
    // send audio
    const QUrl url("https://speech.googleapis.com/v1/speech:recognize");
    QNetworkRequest req(url);
    QString a = "Bearer ";
    req.setRawHeader(QByteArray("Authorization") , QByteArray(a.append(token).toUtf8()));

    req.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    req.setAttribute(QNetworkRequest::DoNotBufferUploadDataAttribute, false);
    req.setAttribute(QNetworkRequest::CacheLoadControlAttribute,
                     QNetworkRequest::AlwaysNetwork);
    m_reply = m_network->post(req, json.toLocal8Bit());
}

void SpeechRecognition::networkReplyFinished(QNetworkReply *reply)
{

    int errorCode = reply->error();
    if (errorCode != 0){
        qDebug() << "Error: " << reply->readAll();
    } else{
        m_speechText = reply->readAll();

        QJsonDocument json = QJsonDocument::fromJson(m_speechText.toUtf8());
        QJsonArray array = json["results"].toArray();
        QJsonObject first = array[0].toObject();
        qDebug() << first;
        QJsonArray alternatives = first["alternatives"].toArray();
        qDebug() << alternatives;
        QJsonObject wat = alternatives[0].toObject();
        m_speechText = wat["transcript"].toString();

        qDebug() << m_speechText;
        emit speechTextReady();
    }

    reply->deleteLater();
}

void SpeechRecognition::handleStateChanged(QAudio::State newState)
{
    switch (newState) {
        case QAudio::StoppedState:
            if (m_audioInput->error() != QAudio::NoError) {
                qDebug() << m_audioInput->error(); // Error handling
            } else {
                qDebug() << "Finished recording";
                // Finished recording
            }
            break;

        case QAudio::ActiveState:
            // Started recording - read from IO device
            qDebug() << "Started recording - read from IO device";
            break;

        default:
            qDebug() << "wut";
            // ... other cases as appropriate
            break;
    }
}


