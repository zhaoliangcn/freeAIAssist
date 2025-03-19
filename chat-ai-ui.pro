QT += core gui network
QT += texttospeech

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = chat-ai-ui
TEMPLATE = app

# 源文件
SOURCES += src/main.cpp \
           src/mainwindow.cpp \
           src/httpconfigdialog.cpp \
           src/aiassistantconfigdialog.cpp \
           src/filedig.cpp \
           src/speechwindow.cpp 

# 头文件
HEADERS += include/mainwindow.h \
           include/httpconfigdialog.h \
           include/aiassistantconfigdialog.h \
           include/filedig.h \
           include/speechwindow.h 

# UI 文件
FORMS += ui/mainwindow.ui \
         ui/speechwindow.ui \
         ui/httpconfigdialog.ui \
         ui/aiassistantconfigdialog.ui

INCLUDEPATH += include

LIBS += ../chat-ai-ui/thirdparty/lib/libdoc2text.lib \
        user32.lib

RESOURCES += ai.qrc
