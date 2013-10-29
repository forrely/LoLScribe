#-------------------------------------------------
#
# Project created by QtCreator 2013-10-17T18:20:13
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = LoLScribe
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    dialog.cpp \
    apitest/apitest/player.cpp \
    apitest/apitest/match.cpp \
    apitest/apitest/champion.cpp

HEADERS  += mainwindow.h \
    dialog.h \
    apitest/apitest/player.h \
    apitest/apitest/match.h \
    apitest/apitest/champion.h

FORMS    += mainwindow.ui \
    dialog.ui


#DEFINES += CURL_STATICLIB
#INCLUDEPATH += "C:\CompSci\curl-7.32.0-devel-mingw32\curl-7.32.0-devel-mingw32\include"
#win32:LIBS += -L"C:\CompSci\curl-7.32.0-rtmp-ssh2-ssl-sspi-zlib-idn-static-bin-w32" -lcurl
INCLUDEPATH += ./curl-7.32.0-devel-mingw32/curl-7.32.0-devel-mingw32/include
win32:LIBS += -L./curl-7.32.0-rtmp-ssh2-ssl-sspi-zlib-idn-static-bin-w32 -lcurl


OTHER_FILES += \
    user settings.TXT \
    Resources/draven icon.png \
    apitest/apitest/matchIDs.txt \
    apitest/apitest/matchesraw.txt \
    apitest/apitest/matches.txt \
    apitest/apitest/itemsraw.txt \
    apitest/apitest/items.txt \
    apitest/apitest/champsraw.txt \
    apitest/apitest/champs.txt

