#-------------------------------------------------
#
# Project created by QtCreator 2011-08-17T15:40:55
#
#-------------------------------------------------


QT       += core gui opengl

TEMPLATE = app

unix:!macx {
    CONFIG += link_pkgconfig
    PKGCONFIG += glew glu
}

macx {
    INCLUDEPATH += /opt/local/include
    CONFIG += link_pkgconfig
    LIBS += -lglew -framework OpenGL
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
    src/main/VertexBSP.h \
    src/main/ObjetBSP.h \
    src/main/MainWindow.h \
    src/main/GLView.h \
    src/main/FaceBSP.h \
    src/main/ArbreBSP.h \
    src/prog3d/Vector4.h \
    src/prog3d/Vector3.h \
    src/prog3d/Vector2.h \
    src/prog3d/UtilGL.h \
    src/prog3d/Trackball.h \
    src/prog3d/Quaternion.h \
    src/prog3d/MeshObj.h \
    src/prog3d/Matrix4.h \
    src/prog3d/Matrix3.h \
    src/prog3d/Error.h \
    src/prog3d/glsupport.h \
    src/prog3d/Color.h

SOURCES += \
    src/main/VertexBSP.cpp \
    src/main/ObjetBSP.cpp \
    src/main/MainWindow.cpp \
    src/main/main.cpp \
    src/main/GLView.cpp \
    src/main/FaceBSP.cpp \
    src/main/ArbreBSP.cpp \
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
    src/prog3d/Color.cpp

