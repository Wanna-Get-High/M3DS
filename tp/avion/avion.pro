#-------------------------------------------------
#
# Project created by QtCreator 2011-08-17T15:40:55
#
#-------------------------------------------------


QT       += core gui opengl

TEMPLATE = app

unix {
    CONFIG += link_pkgconfig
    PKGCONFIG += glew glu
}

macx {
    INCLUDEPATH += /opt/local/include
    CONFIG += link_pkgconfig
    LIBS += -lglew -framework OpenGL -framework GLUT
}

win32 {
  CONFIG += console
  DEFINES+=GLEW_STATIC _STDCALL_SUPPORTED
  LIBS+= -lglew32s -lopengl32 -lglu32
}

DESTDIR = $$PWD
INCLUDEPATH += src/main src/prog3d
DEPENDPATH += src/main src/prog3d

HEADERS += \
    src/prog3d/Vector4.h \
    src/prog3d/Vector3.h \
    src/prog3d/Vector2.h \
    src/prog3d/UtilGL.h \
    src/prog3d/Trackball.h \
    src/prog3d/Quaternion.h \
    src/prog3d/MeshObj.h \
    src/prog3d/Matrix4.h \
    src/prog3d/Matrix3.h \
    src/prog3d/glsupport.h \
    src/prog3d/Error.h \
    src/prog3d/Color.h \
    src/main/MainWindow.h \
    src/main/GLView.h \
    src/main/Camera.h \
    src/main/Avion.h

SOURCES += \
    src/prog3d/Vector4.cpp \
    src/prog3d/Vector3.cpp \
    src/prog3d/Vector2.cpp \
    src/prog3d/UtilGL.cpp \
    src/prog3d/Trackball.cpp \
    src/prog3d/Quaternion.cpp \
    src/prog3d/MeshObj.cpp \
    src/prog3d/Matrix4.cpp \
    src/prog3d/Matrix3.cpp \
    src/prog3d/Error.cpp \
    src/prog3d/Color.cpp \
    src/main/MainWindow.cpp \
    src/main/main.cpp \
    src/main/GLView.cpp \
    src/main/Camera.cpp \
    src/main/Avion.cpp




