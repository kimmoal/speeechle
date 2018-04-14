#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QLoggingCategory>
#include <QtTextToSpeech>

#include "speechrecognition.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QLoggingCategory::setFilterRules(QStringLiteral("qt.speech.tts=true \n qt.speech.tts.*=true"));

    QTextToSpeech* speech = new QTextToSpeech;
    qRegisterMetaType<QTextToSpeech::State>("State");

    qmlRegisterType<SpeechRecognition>("stupidhack", 1, 0, "SpeechRecognition");

    SpeechRecognition* recognition = new SpeechRecognition;

    QQmlApplicationEngine engine;
    engine.rootContext()->setContextProperty("speech", speech);
    engine.rootContext()->setContextProperty("recognition", recognition);

    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}
