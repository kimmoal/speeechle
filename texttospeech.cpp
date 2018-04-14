#include "texttospeech.h"

#include <QtTextToSpeech>

TextToSpeech::TextToSpeech()
{

}

void TextToSpeech::print()
{
    // Populate engine selection list
    foreach (QString engine, QTextToSpeech::availableEngines())
        qDebug() << engine;

}

