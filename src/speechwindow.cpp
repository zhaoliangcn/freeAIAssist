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
    : QDialog(parent),  // 调用父类QDialog的构造函数
    ui(new Ui::SpeechWindow),  // 创建UI对象
    m_speech(0)  // 初始化语音引擎指针为nullptr
{
    // 设置UI界面
    ui->setupUi(this);
    // 设置日志过滤规则，启用语音相关日志
    QLoggingCategory::setFilterRules(QStringLiteral("qt.speech.tts=true \n qt.speech.tts.*=true"));

    // 连接音量滑块和数值框的信号
    connect(ui->volume, &QSlider::valueChanged, ui->spinBoxVolume, &QSpinBox::setValue);
    connect(ui->spinBoxVolume, QOverload<int>::of(&QSpinBox::valueChanged), ui->volume, &QSlider::setValue);
    
    // 连接语速滑块和数值框的信号
    connect(ui->rate, &QSlider::valueChanged, ui->spinBoxRate, &QSpinBox::setValue);
    connect(ui->spinBoxRate, QOverload<int>::of(&QSpinBox::valueChanged), ui->rate, &QSlider::setValue);
    
    // 连接音调滑块和数值框的信号
    connect(ui->pitch, &QSlider::valueChanged, ui->spinBoxPitch, &QSpinBox::setValue);
    connect(ui->spinBoxPitch, QOverload<int>::of(&QSpinBox::valueChanged), ui->pitch, &QSlider::setValue);

    // 初始化引擎选择列表
    ui->engine->addItem("Default", QString("default"));  // 添加默认引擎选项
    const auto engines = QTextToSpeech::availableEngines();  // 获取可用引擎
    for (const QString &engine : engines)
        ui->engine->addItem(engine, engine);  // 添加每个引擎到选择列表
    ui->engine->setCurrentIndex(0);  // 设置默认选中第一个引擎
    engineSelected(0);  // 初始化选中的引擎

    // 连接按钮和控件的信号到对应的槽函数
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
    // 获取当前选择的引擎名称
    QString engineName = ui->engine->itemData(index).toString();
    
    // 删除旧的语音引擎实例
    delete m_speech;
    
    // 根据选择的引擎创建新的语音引擎实例
    if (engineName == "default")
        m_speech = new QTextToSpeech(this);
    else
        m_speech = new QTextToSpeech(engineName, this);
    
    // 断开语言选择框的信号连接
    disconnect(ui->language, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged), this, &SpeechWindow::languageSelected);
    
    // 清空语言选择框
    ui->language->clear();
    
    // 获取当前语音引擎支持的所有语言
    const QVector<QLocale> locales = m_speech->availableLocales();
    QLocale current = m_speech->locale();
    
    // 将支持的语言添加到语言选择框中
    for (const QLocale &locale : locales) {
        QString name(QString("%1 (%2)")
                     .arg(QLocale::languageToString(locale.language()))
                     .arg(QLocale::countryToString(locale.country())));
        QVariant localeVariant(locale);
        ui->language->addItem(name, localeVariant);
        if (locale.name() == current.name())
            current = locale;
    }
    
    // 设置语音引擎的语速、音调和音量
    setRate(ui->rate->value());
    setPitch(ui->pitch->value());
    setVolume(ui->volume->value());
    
    // 连接控制按钮的信号
    connect(ui->stopButton, &QPushButton::clicked, m_speech, &QTextToSpeech::stop);
    connect(ui->pauseButton, &QPushButton::clicked, m_speech, &QTextToSpeech::pause);
    connect(ui->resumeButton, &QPushButton::clicked, m_speech, &QTextToSpeech::resume);

    // 连接语音引擎的状态和语言改变信号
    connect(m_speech, &QTextToSpeech::stateChanged, this, &SpeechWindow::stateChanged);
    connect(m_speech, &QTextToSpeech::localeChanged, this, &SpeechWindow::localeChanged);

    // 重新连接语言选择框的信号
    connect(ui->language, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged), this, &SpeechWindow::languageSelected);
    
    // 更新当前语言
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
    // 将当前语言转换为QVariant类型
    QVariant localeVariant(locale);
    // 在语言选择框中设置当前选择的语言
    ui->language->setCurrentIndex(ui->language->findData(localeVariant));

    // 断开语音选择框的信号连接
    disconnect(ui->voice, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged), this, &SpeechWindow::voiceSelected);
    // 清空语音选择框
    ui->voice->clear();

    // 获取当前语言下可用的所有语音
    m_voices = m_speech->availableVoices();
    // 获取当前使用的语音
    QVoice currentVoice = m_speech->voice();
    
    // 遍历所有可用语音，添加到语音选择框中
    for (const QVoice &voice : qAsConst(m_voices)) {
        // 格式化显示语音信息：名称 - 性别 - 年龄
        ui->voice->addItem(QString("%1 - %2 - %3").arg(voice.name())
                          .arg(QVoice::genderName(voice.gender()))
                          .arg(QVoice::ageName(voice.age())));
        // 如果当前语音匹配，设置为选中状态
        if (voice.name() == currentVoice.name())
            ui->voice->setCurrentIndex(ui->voice->count() - 1);
    }
    
    // 重新连接语音选择框的信号
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
