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

DESTDIR = $$_PRO_FILE_PWD_/bin

INCLUDEPATH += src/main/ src/prog3d/ 
DEPENDPATH += src/main/ src/prog3d/ 

SOURCES += \
	src/main/main.cpp \
	src/main/MainWindow.cpp \
	src/main/GLView.cpp \
	src/prog3d/Error.cpp \
	src/prog3d/Texture.cpp \
	src/prog3d/Matrix4.cpp \
	src/prog3d/Matrix3.cpp \
	src/prog3d/Quaternion.cpp \
	src/prog3d/Vector3.cpp \
	src/prog3d/Vector4.cpp \
	src/prog3d/Vector2.cpp \
	src/main/Square.cpp \
	src/prog3d/Shader.cpp \
	src/main/Entity.cpp \
	src/prog3d/MeshObj.cpp \
	src/prog3d/UtilGL.cpp \
	src/prog3d/Color.cpp \
	src/prog3d/Trackball.cpp
HEADERS += \
	src/prog3d/Error.h \
	src/main/GLView.h \
	src/prog3d/Texture.h \
	src/prog3d/Matrix3.h \
	src/prog3d/Quaternion.h \
	src/prog3d/Vector3.h \
	src/prog3d/Vector4.h \
	src/prog3d/Matrix4.h \
	src/prog3d/Vector2.h \
	src/main/Square.h \
	src/prog3d/Shader.h \
	src/main/Entity.h \
	src/prog3d/MeshObj.h \
	src/prog3d/UtilGL.h \
	src/prog3d/Color.h \
	src/prog3d/Trackball.h \
	src/prog3d/glsupport.h \
	src/main/MainWindow.h
OTHER_FILES +=\
	media/univ_lille1pair.jpg\
	media/lightmap.png\
	media/briques02.jpg\
	media/shader/simple.vert\
	media/shader/simple.frag\
	media/shader/outline.vert\
	media/shader/outline.frag\
	media/shader/light.vert\
	media/shader/light.frag\
	media/shader/gooch.vert\
	media/shader/gooch.frag\
	media/shader/cel.vert\
	media/shader/cel.frag\
	media/cow.obj
