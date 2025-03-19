#include "texttovoice.h"

#include <QtTextToSpeech/QTextToSpeech>
#include <QFile>

TextToVoice::TextToVoice(QObject *parent) : QObject(parent)
{
}

void TextToVoice::convertTextToSpeech(const QString &text, const QString &outputFilePath)
{
    QTextToSpeech *speech = new QTextToSpeech(this);
    speech->say(text);


//    QFile file(outputFilePath);
//    if (file.open(QIODevice::WriteOnly)) {
//        file.write(text.toUtf8());
//        file.close();
//    } else {

//    }
}
