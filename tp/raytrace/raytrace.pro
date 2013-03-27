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

INCLUDEPATH += src/main/ src/prog3d/ src/tinyxml/ 
DEPENDPATH += src/main/ src/prog3d/ src/tinyxml/ 

SOURCES += \
	src/main/main.cpp \
	src/main/MainWindow.cpp \
	src/prog3d/mediaDirectory.cpp \
	src/prog3d/Error.cpp \
	src/main/RaytraceView.cpp \
	src/main/Raytrace.cpp \
	src/prog3d/UtilGL.cpp \
	src/prog3d/Color.cpp \
	src/prog3d/Quaternion.cpp \
	src/prog3d/Matrix4.cpp \
	src/prog3d/Matrix3.cpp \
	src/prog3d/Vector4.cpp \
	src/prog3d/Plane.cpp \
	src/prog3d/Vector3.cpp \
	src/prog3d/Vector2.cpp \
	src/prog3d/Texture.cpp \
	src/main/Ray.cpp \
	src/prog3d/Line.cpp \
	src/main/Primitive.cpp \
	src/main/IntersectionArray.cpp \
	src/main/Intersection.cpp \
	src/main/CsgTree.cpp \
	src/prog3d/Material.cpp \
	src/main/Scene.cpp \
	src/main/Cylinder.cpp \
	src/main/Cone.cpp \
	src/main/Sphere.cpp \
	src/main/Cube.cpp \
	src/tinyxml/tinyxml.cpp \
	src/tinyxml/tinyxmlerror.cpp \
	src/tinyxml/tinyxmlparser.cpp \
	src/tinyxml/tinystr.cpp \
	src/prog3d/Camera.cpp \
	src/prog3d/Trackball.cpp \
	src/prog3d/Movable.cpp \
	src/main/GLView.cpp
HEADERS += \
	src/prog3d/Error.h \
	src/prog3d/mediaDirectory.h \
	src/prog3d/Color.h \
	src/prog3d/Quaternion.h \
	src/prog3d/Matrix3.h \
	src/prog3d/Vector4.h \
	src/prog3d/Matrix4.h \
	src/prog3d/Plane.h \
	src/prog3d/Vector3.h \
	src/prog3d/Vector2.h \
	src/prog3d/UtilGL.h \
	src/prog3d/Texture.h \
	src/prog3d/glsupport.h \
	src/main/Ray.h \
	src/prog3d/Line.h \
	src/main/Primitive.h \
	src/main/IntersectionArray.h \
	src/main/Intersection.h \
	src/main/CsgTree.h \
	src/prog3d/Material.h \
	src/main/Raytrace.h \
	src/main/Cylinder.h \
	src/main/Cone.h \
	src/main/Sphere.h \
	src/main/Cube.h \
	src/tinyxml/tinyxml.h \
	src/tinyxml/tinystr.h \
	src/main/Scene.h \
	src/prog3d/Trackball.h \
	src/prog3d/Camera.h \
	src/prog3d/Movable.h \
	src/main/RaytraceView.h \
	src/main/GLView.h \
	src/main/MainWindow.h
OTHER_FILES +=\
	media/scene3.csg\
	media/scene2.csg\
	media/scene1.csg\
	media/scene0.csg
