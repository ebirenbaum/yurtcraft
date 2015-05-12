#-------------------------------------------------
#
# Project created by QtCreator 2014-10-27T15:48:55
#
#-------------------------------------------------
QT       -= gui

TARGET = qt-yurtcraft

TEMPLATE = app

INCLUDEPATH += src
DEPENDPATH += src

SOURCES += \
    src/vrfrontend.cpp \
    src/main.cpp \
    src/vrcamera.cpp \
    src/sampleapp.cpp

HEADERS += \
    src/vrfrontend.h \
    src/vrcamera.h \
    src/includes.h \
    src/sampleapp.h

LIBS += -L/lib/x86_64-linux-gnu \
        -lGLU \
        -lGL \
        -lglut \
        -lpthread-2.13 \
        -lSDL \
#        -lavutil \
#        -lavformat \
#        -lavcodec \
        -lvrpn \
        -lpng \
        -ljpeg \
        -ldl \
        -lXrandr \
        -lX11 \
        -lfreeimage \
        -lXi \
        -lXmu

LIBS += -L$$(G)/install_linux/lib

INCLUDEPATH += $$(G)/install_linux/include
DEPENDPATH += $$(G)/install_linux/include

INCLUDEPATH += $$(G)/install_linux/include/ffmpeg
DEPENDPATH += $$(G)/install_linux/include/ffmpeg

unix:!macx:!symbian: LIBS += -L$$(G)/install_linux/lib/ -lGLG3D
unix:!macx:!symbian: PRE_TARGETDEPS += $$(G)/install_linux/lib/libGLG3D.a

unix:!macx:!symbian: LIBS += -L$$(G)/install_linux/lib/ -lG3D
unix:!macx:!symbian: PRE_TARGETDEPS += $$(G)/install_linux/lib/libG3D.a

unix:!macx:!symbian: LIBS += -L$$(G)/install_linux/lib/ -lvrg3d
unix:!macx:!symbian: PRE_TARGETDEPS += $$(G)/install_linux/lib/libvrg3d.a

unix:!macx:!symbian: LIBS += -L$$(G)/install_linux/lib/ -lzip
unix:!macx:!symbian: PRE_TARGETDEPS += $$(G)/install_linux/lib/libzip.a

unix:!macx:!symbian: LIBS += -L$$(G)/install_linux/lib/ -lavutil
unix:!macx:!symbian: PRE_TARGETDEPS += $$(G)/install_linux/lib/libavutil.a

unix:!macx:!symbian: LIBS += -L$$(G)/install_linux/lib/ -lavcodec
unix:!macx:!symbian: PRE_TARGETDEPS += $$(G)/install_linux/lib/libavcodec.a

unix:!macx:!symbian: LIBS += -L$$(G)/install_linux/lib/ -lavformat
unix:!macx:!symbian: PRE_TARGETDEPS += $$(G)/install_linux/lib/libavformat.a
