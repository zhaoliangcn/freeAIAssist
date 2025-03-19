#ifndef TEXTTOSPEECH_H
#define TEXTTOSPEECH_H

#include <QObject>
#include <QString>

class TextToVoice : public QObject
{
    Q_OBJECT
public:
    explicit TextToVoice(QObject *parent = nullptr);
    void convertTextToSpeech(const QString &text, const QString &outputFilePath);



private:
    // 私有成员函数和变量
};

#endif // TEXTTOSPEECH_H
