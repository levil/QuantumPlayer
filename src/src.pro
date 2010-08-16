TEMPLATE = app
TARGET = quantumplayer
QT += sql \
    phonon
INCLUDEPATH += /usr/include/KDE
SOURCES += main.cpp \
    quantumplayer.cpp \
    player.cpp
HEADERS += quantumplayer.h \
    player.h
