/****************************************************************************
**
** Copyright (C) 2017 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** BSD License Usage
** Alternatively, you may use this file under the terms of the BSD license
** as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of The Qt Company Ltd nor the names of its
**     contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/



#include "speechwindow.h"
#include <QLoggingCategory>
#include "ui_speechwindow.h"
#include <QDebug>
QT_BEGIN_NAMESPACE
namespace Ui { class SpeechWindow; }
QT_END_NAMESPACE
SpeechWindow::SpeechWindow(QWidget *parent)
    : QDialog(parent),
    ui(new Ui::SpeechWindow),
    m_speech(0)
{
    ui->setupUi(this);
    QLoggingCategory::setFilterRules(QStringLiteral("qt.speech.tts=true \n qt.speech.tts.*=true"));

    // 连接滑块和数值框的信号
    connect(ui->volume, &QSlider::valueChanged, ui->spinBoxVolume, &QSpinBox::setValue);
    connect(ui->spinBoxVolume, QOverload<int>::of(&QSpinBox::valueChanged), ui->volume, &QSlider::setValue);
    
    connect(ui->rate, &QSlider::valueChanged, ui->spinBoxRate, &QSpinBox::setValue);
    connect(ui->spinBoxRate, QOverload<int>::of(&QSpinBox::valueChanged), ui->rate, &QSlider::setValue);
    
    connect(ui->pitch, &QSlider::valueChanged, ui->spinBoxPitch, &QSpinBox::setValue);
    connect(ui->spinBoxPitch, QOverload<int>::of(&QSpinBox::valueChanged), ui->pitch, &QSlider::setValue);

    // Populate engine selection list
    ui->engine->addItem("Default", QString("default"));
    const auto engines = QTextToSpeech::availableEngines();
    for (const QString &engine : engines)
        ui->engine->addItem(engine, engine);
    ui->engine->setCurrentIndex(0);
    engineSelected(0);

    connect(ui->speakButton, &QPushButton::clicked, this, &SpeechWindow::speak);
    connect(ui->pitch, &QSlider::valueChanged, this, &SpeechWindow::setPitch);
    connect(ui->rate, &QSlider::valueChanged, this, &SpeechWindow::setRate);
    connect(ui->volume, &QSlider::valueChanged, this, &SpeechWindow::setVolume);
    connect(ui->engine, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged), this, &SpeechWindow::engineSelected);
}

SpeechWindow::~SpeechWindow()
{
    delete ui;
}

void SpeechWindow::SpeekText(QString text)
{
    ui->plainTextEdit->setPlainText(text);
    on_speakButton_clicked();
}

void SpeechWindow::speak()
{
    m_speech->say(ui->plainTextEdit->toPlainText());
}
void SpeechWindow::stop()
{
    m_speech->stop();
}

void SpeechWindow::setRate(int rate)
{
    m_speech->setRate(rate / 10.0);
}

void SpeechWindow::setPitch(int pitch)
{
    m_speech->setPitch(pitch / 10.0);
}

void SpeechWindow::setVolume(int volume)
{
    m_speech->setVolume(volume / 100.0);
}

void SpeechWindow::stateChanged(QTextToSpeech::State state)
{
    if (state == QTextToSpeech::Speaking) {
        ui->statusbar->showMessage(u8"正在朗读...");
        ui->speakButton->setEnabled(false);
        ui->pauseButton->setEnabled(true);
        ui->resumeButton->setEnabled(false);
        ui->stopButton->setEnabled(true);
    } else if (state == QTextToSpeech::Ready) {
        ui->statusbar->showMessage(u8"就绪");
        ui->speakButton->setEnabled(true);
        ui->pauseButton->setEnabled(false);
        ui->resumeButton->setEnabled(false);
        ui->stopButton->setEnabled(false);
    } else if (state == QTextToSpeech::Paused) {
        ui->statusbar->showMessage(u8"已暂停");
        ui->speakButton->setEnabled(false);
        ui->pauseButton->setEnabled(false);
        ui->resumeButton->setEnabled(true);
        ui->stopButton->setEnabled(true);
    } else if (state == QTextToSpeech::BackendError) {
        ui->statusbar->showMessage(u8"语音引擎错误");
        ui->speakButton->setEnabled(false);
        ui->pauseButton->setEnabled(false);
        ui->resumeButton->setEnabled(false);
        ui->stopButton->setEnabled(false);
    }
}

void SpeechWindow::engineSelected(int index)
{
    QString engineName = ui->engine->itemData(index).toString();
    delete m_speech;
    if (engineName == "default")
        m_speech = new QTextToSpeech(this);
    else
        m_speech = new QTextToSpeech(engineName, this);
    disconnect(ui->language, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged), this, &SpeechWindow::languageSelected);
    ui->language->clear();
    // Populate the languages combobox before connecting its signal.
    const QVector<QLocale> locales = m_speech->availableLocales();
    QLocale current = m_speech->locale();
    for (const QLocale &locale : locales) {
        QString name(QString("%1 (%2)")
                     .arg(QLocale::languageToString(locale.language()))
                     .arg(QLocale::countryToString(locale.country())));
        QVariant localeVariant(locale);
        ui->language->addItem(name, localeVariant);
        if (locale.name() == current.name())
            current = locale;
    }
    setRate(ui->rate->value());
    setPitch(ui->pitch->value());
    setVolume(ui->volume->value());
    connect(ui->stopButton, &QPushButton::clicked, m_speech, &QTextToSpeech::stop);
    connect(ui->pauseButton, &QPushButton::clicked, m_speech, &QTextToSpeech::pause);
    connect(ui->resumeButton, &QPushButton::clicked, m_speech, &QTextToSpeech::resume);

    connect(m_speech, &QTextToSpeech::stateChanged, this, &SpeechWindow::stateChanged);
    connect(m_speech, &QTextToSpeech::localeChanged, this, &SpeechWindow::localeChanged);

    connect(ui->language, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged), this, &SpeechWindow::languageSelected);
    localeChanged(current);
}

void SpeechWindow::languageSelected(int language)
{
    QLocale locale = ui->language->itemData(language).toLocale();
    m_speech->setLocale(locale);
}

void SpeechWindow::voiceSelected(int index)
{
    m_speech->setVoice(m_voices.at(index));
}

void SpeechWindow::localeChanged(const QLocale &locale)
{
    QVariant localeVariant(locale);
    ui->language->setCurrentIndex(ui->language->findData(localeVariant));

    disconnect(ui->voice, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged), this, &SpeechWindow::voiceSelected);
    ui->voice->clear();

    m_voices = m_speech->availableVoices();
    QVoice currentVoice = m_speech->voice();
    for (const QVoice &voice : qAsConst(m_voices)) {
        ui->voice->addItem(QString("%1 - %2 - %3").arg(voice.name())
                          .arg(QVoice::genderName(voice.gender()))
                          .arg(QVoice::ageName(voice.age())));
        if (voice.name() == currentVoice.name())
            ui->voice->setCurrentIndex(ui->voice->count() - 1);
    }
    connect(ui->voice, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged), this, &SpeechWindow::voiceSelected);
}

void SpeechWindow::volumeChanged(int volume)
{
    m_speech->setVolume(volume / 100.0);
}

void SpeechWindow::rateChanged(int rate)
{
    m_speech->setRate(rate / 10.0);
}

void SpeechWindow::pitchChanged(int pitch)
{
    m_speech->setPitch(pitch / 10.0);
}

void SpeechWindow::on_speakButton_clicked()
{
    m_speech->say(ui->plainTextEdit->toPlainText());
    ui->pauseButton->setEnabled(true);
    ui->resumeButton->setEnabled(false);
    ui->stopButton->setEnabled(true);
}

void SpeechWindow::on_pauseButton_clicked()
{
    m_speech->pause();
    ui->pauseButton->setEnabled(false);
    ui->resumeButton->setEnabled(true);
}

void SpeechWindow::on_resumeButton_clicked()
{
    m_speech->resume();
    ui->pauseButton->setEnabled(true);
    ui->resumeButton->setEnabled(false);
}

void SpeechWindow::on_stopButton_clicked()
{
    m_speech->stop();
    ui->pauseButton->setEnabled(false);
    ui->resumeButton->setEnabled(false);
    ui->stopButton->setEnabled(false);
}
