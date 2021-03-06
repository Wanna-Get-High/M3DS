CONFIG += debug_and_release

QT += core gui opengl

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

CONFIG(debug,debug|release) {
  TARGET=$$join(TARGET,,,"_d")
}

DESTDIR = $$_PRO_FILE_PWD_/bin

INCLUDEPATH += src/main/ src/prog3d/ 
DEPENDPATH += src/main/ src/prog3d/ 

SOURCES += \
	src/main/main.cpp \
	src/main/MainWindow.cpp \
	src/main/GLView.cpp \
	src/prog3d/UtilGL.cpp \
	src/prog3d/Color.cpp \
	src/prog3d/Quaternion.cpp \
	src/prog3d/Error.cpp \
	src/prog3d/Matrix4.cpp \
	src/prog3d/Matrix3.cpp \
	src/prog3d/Vector4.cpp \
	src/prog3d/Plane.cpp \
	src/prog3d/Line.cpp \
	src/prog3d/Vector3.cpp \
	src/prog3d/Vector2.cpp \
	src/prog3d/Texture.cpp \
	src/prog3d/mediaDirectory.cpp \
	src/prog3d/Trackball.cpp \
	src/main/Engine.cpp \
	src/main/ParticleList.cpp \
	src/main/Particle.cpp \
	src/prog3d/Camera.cpp \
	src/prog3d/Movable.cpp
HEADERS += \
	src/main/GLView.h \
	src/prog3d/Color.h \
	src/prog3d/Error.h \
	src/prog3d/Quaternion.h \
	src/prog3d/Matrix3.h \
	src/prog3d/Vector4.h \
	src/prog3d/Matrix4.h \
	src/prog3d/Plane.h \
	src/prog3d/Line.h \
	src/prog3d/Vector3.h \
	src/prog3d/Vector2.h \
	src/prog3d/UtilGL.h \
	src/prog3d/mediaDirectory.h \
	src/prog3d/Texture.h \
	src/prog3d/Trackball.h \
	src/main/Engine.h \
	src/main/ParticleList.h \
	src/main/Particle.h \
	src/prog3d/Camera.h \
	src/prog3d/Movable.h \
	src/prog3d/glsupport.h \
	src/main/MainWindow.h
OTHER_FILES +=\
	media/lightmap.png\
	media/mosaic_pierre.jpg
