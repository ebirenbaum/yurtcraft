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
    src/sampleapp.cpp \
    src/world.cpp \
    src/voxelsystem.cpp \
    src/triangle.cpp \
    src/skybox.cpp \
    src/player.cpp \
    src/obj.cpp \
    src/noise.cpp \
    src/mcworld.cpp \
    src/mcchunkfactory.cpp \
    src/mcchunk.cpp \
    src/graphics.cpp \
    src/entity.cpp \
    src/ellipsoid.cpp \
    src/cylinder.cpp \
    src/collisions.cpp \
    src/chunkfactory.cpp \
    src/chunk.cpp \
    src/aab.cpp \
    src/particlefireball.cpp \
    src/particleemitter.cpp \
    src/fireball.cpp \
    src/vrdata.cpp \
    src/biomemanager.cpp \
    src/biometile.cpp \
    src/terrainfeature.cpp \
    src/biome.cpp \
    src/mountainbiome.cpp \
    src/mountainforestbiome.cpp \
    src/tundrabiome.cpp \
    src/junglebiome.cpp \
    src/boulderfieldbiome.cpp \
    src/forestbiome.cpp \
    src/desertbiome.cpp \
    src/grasslandsbiome.cpp \
    src/terrainfeature.cpp \
    src/treefeature.cpp \
    src/boulderfeature.cpp \
    src/mountainpeakfeature.cpp \
    src/cactusfeature.cpp \
    src/monorail.cpp

HEADERS += \
    src/vrfrontend.h \
    src/vrcamera.h \
    src/includes.h \
    src/sampleapp.h \
    src/world.h \
    src/voxelsystem.h \
    src/vector.h \
    src/triangle.h \
    src/skybox.h \
    src/player.h \
    src/obj.h \
    src/noise.h \
    src/mcworld.h \
    src/mcchunkfactory.h \
    src/mcchunk.h \
    src/matrix.h \
    src/graphics.h \
    src/entity.h \
    src/ellipsoid.h \
    src/cylinder.h \
    src/collisions.h \
    src/chunkfactory.h \
    src/chunk.h \
    src/blocktype.h \
    src/blockdef.h \
    src/aab.h \
    src/particlefireball.h \
    src/particleemitter.h \
    src/fireball.h \
    src/vrdata.h \
    src/biomemanager.h \
    src/biometile.h \
    src/biome.h \
    src/mountainbiome.h \
    src/mountainforest.h \
    src/forestbiome.h \
    src/tundrabiome.h \
    src/grasslandsbiome.h \
    src/junglebiome.h \
    src/boulderfieldbiome.h \
    src/desertbiome.h \
    src/terrainfeature.h \
    src/treefeature.h \
    src/boulderfeature.h \
    src/mountainpeakfeature.h \
    src/cactusfeature.h \
    src/monorail.h

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

unix:!macx:!symbian: LIBS += -L$$PWD/lib/ -lSOIL

INCLUDEPATH += $$PWD/lib/include
DEPENDPATH += $$PWD/lib/include

unix:!macx:!symbian: PRE_TARGETDEPS += $$PWD/lib/libSOIL.a
