QT += core gui network
QT += core gui widgets network texttospeech

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = chat-ai-ui
TEMPLATE = app

# 源文件
SOURCES += src/main.cpp \
           src/mainwindow.cpp \
           src/httpconfigdialog.cpp \
           src/aiassistantconfigdialog.cpp \
           src/filedig.cpp \
           src/speechwindow.cpp \
           src/chat_history.cpp \
           src/OperationLogger.cpp \
           src/commonsystempromptdialog.cpp \
           src/textsegment.cpp \
           src/knowledgebase.cpp \
           src/KnowledgeBaseManager.cpp \
           src/chat_window.cpp \
           src/aiconfig.cpp

# 头文件
HEADERS += include/mainwindow.h \
           include/httpconfigdialog.h \
           include/aiassistantconfigdialog.h \
           include/filedig.h \
           include/speechwindow.h \
           include/chat_history.h \
           include/OperationLogger.h \
           include/commonsystempromptdialog.h \
           include/textsegment.h \
           include/knowledgebase.h \
           incldue/KnowledgeBaseManager.h \
           include/chat_window.h \
           include/aiconfig.h

# UI 文件
FORMS += ui/mainwindow.ui \
         ui/textsegment.ui \
         ui/speechwindow.ui \
         ui/httpconfigdialog.ui \
         ui/aiassistantconfigdialog.ui \
         ui/chat_history.ui \
         ui/commonsystempromptdialog.ui \
         ui/KnowledgeBase.ui \
         ui/chat_window.ui

INCLUDEPATH += include

win32{
LIBS += user32.lib
}

RESOURCES += ai.qrc
