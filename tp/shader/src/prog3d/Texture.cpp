#include "Texture.h"
#include <iostream>
#include <cstdlib>

using namespace std;
using namespace prog3d;

#include <stdio.h>
#include <fstream>
#include "Error.h"
#include "UtilGL.h"


void Texture::init() {
    _texId=0;
    _border=0;
    _generateMipMap=false;
    _setLuminance=false;
}

Texture::Texture() {
    this->init();
}

void Texture::setBorder(unsigned int b) {
  _border=b;
}

void Texture::init(int w,int h) {
  if (_texId==0) {
    glGenTextures(1,&_texId);
  }
  _width=w;_height=h;
  glBindTexture(GL_TEXTURE_2D,_texId);

///////////////////////////////////////////////////////
//
  glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
  glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);

  glTexEnvf(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_REPLACE);
  this->_filter=GL_NEAREST;
  this->_level=0;
  ////////////////////////////////////////////////

  glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,_width,_height,_border,GL_RGB,GL_UNSIGNED_BYTE,0);
}

Texture::Texture(const string &nom) {
	this->init();
	this->read(nom);
}

void Texture::bind() const {
		glBindTexture(GL_TEXTURE_2D,_texId);
}


unsigned char *Texture::imageCopy() {
    GLubyte *result=new GLubyte[_width*_height*3];
    glGetTexImage(GL_TEXTURE_2D,0,GL_RGB,GL_UNSIGNED_BYTE,result);
    return result;
}

void Texture::copyFrom(GLubyte *img,unsigned int w,unsigned int h) {
    if (w==0) w=_width;
    if (h==0) h=_height;
    if ((h>_height) || (w>_width)) error("copyFrom size",__LINE__,__FILE__);


    glBindTexture(GL_TEXTURE_2D,_texId);
    glTexSubImage2D(GL_TEXTURE_2D,0,0,0,w,h,GL_RGB,GL_UNSIGNED_BYTE,img);
}

void Texture::read(const string &filename) {
  if (_texId==0) {
    glGenTextures(1,&_texId);
  }

  QImage img;
  img.load(QString::fromStdString(filename));

  if (img.isNull()) {
    throw Error("file not found : read(\""+filename+"\")",__LINE__,__FILE__);
  }

  _width=img.width();
  _height=img.height();

  glBindTexture(GL_TEXTURE_2D,_texId);

  if (_generateMipMap) {
    gluBuild2DMipmaps(GL_TEXTURE_2D,GL_RGB,_width,_height,GL_RGB,GL_UNSIGNED_BYTE,img.bits());
  } else
    glTexImage2D(GL_TEXTURE_2D,0,4,_width,_height,0,GL_BGRA,GL_UNSIGNED_BYTE,img.bits());

  glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
  glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);

  glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
  glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
  glTexEnvf(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_REPLACE);
  this->_filter=GL_NEAREST;
  this->_level=0;
}


void Texture::read(const string &filename,int level) {
  if (_texId==0) {
    throw Error("read with mipmap level only with an already created mipmap texture",__LINE__,__FILE__);
  }
  QImage img;
  img.load(QString::fromStdString(filename));

  if (img.isNull()) {
    throw Error("file not found : read(\""+filename+"\")",__LINE__,__FILE__);
  }

  int awidth=img.width();
  int aheight=img.height();
  int aformat=GL_RGB;
  int abpp=3;


  glBindTexture(GL_TEXTURE_2D,_texId);
  glTexImage2D(GL_TEXTURE_2D,level,abpp,awidth,aheight,0,aformat,GL_UNSIGNED_BYTE,img.bits());
}



void Texture::envMode(GLenum m) {
  glTexEnvf(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,m);
}

void Texture::wrap(GLenum wr) {
  glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,wr);
  glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,wr);
  glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_WRAP_R,wr);
}

void Texture::disable() const {
	glDisable(GL_TEXTURE_GEN_S);
	glDisable(GL_TEXTURE_GEN_T);
	glDisable(GL_TEXTURE_2D);
}

void Texture::sphere() const {
	glBindTexture(GL_TEXTURE_2D,_texId);

	glTexGeni(GL_S,GL_TEXTURE_GEN_MODE,GL_SPHERE_MAP);
	glEnable(GL_TEXTURE_GEN_S);

	glTexGeni(GL_T,GL_TEXTURE_GEN_MODE,GL_SPHERE_MAP);
	glEnable(GL_TEXTURE_GEN_T);

	glEnable(GL_TEXTURE_2D);
}

void Texture::linearObject() const {
  glBindTexture(GL_TEXTURE_2D,_texId);

 glTexGeni(GL_S,GL_TEXTURE_GEN_MODE,GL_OBJECT_LINEAR);
 glEnable(GL_TEXTURE_GEN_S);

 glTexGeni(GL_T,GL_TEXTURE_GEN_MODE,GL_OBJECT_LINEAR);
 glEnable(GL_TEXTURE_GEN_T);

 glTexGeni(GL_R,GL_TEXTURE_GEN_MODE,GL_OBJECT_LINEAR);
 glEnable(GL_TEXTURE_GEN_R);

 glTexGeni(GL_Q,GL_TEXTURE_GEN_MODE,GL_OBJECT_LINEAR);
 glEnable(GL_TEXTURE_GEN_Q);

 glEnable(GL_TEXTURE_2D);
}

void Texture::linearEye(const Matrix4 &mat) const {
 float m[4];
 glBindTexture(GL_TEXTURE_2D,_texId);

 glTexGeni(GL_S,GL_TEXTURE_GEN_MODE,GL_EYE_LINEAR);
 glEnable(GL_TEXTURE_GEN_S);
 m[0]=mat[0];m[1]=mat[4];m[2]=mat[8];m[3]=mat[12];
 glTexGenfv(GL_S,GL_EYE_PLANE,m);

 glTexGeni(GL_T,GL_TEXTURE_GEN_MODE,GL_EYE_LINEAR);
 glEnable(GL_TEXTURE_GEN_T);
 m[0]=mat[1];m[1]=mat[5];m[2]=mat[9];m[3]=mat[13];
 glTexGenfv(GL_T,GL_EYE_PLANE,m);

 glTexGeni(GL_R,GL_TEXTURE_GEN_MODE,GL_EYE_LINEAR);
 glEnable(GL_TEXTURE_GEN_R);
 m[0]=mat[2];m[1]=mat[6];m[2]=mat[10];m[3]=mat[14];
 glTexGenfv(GL_R,GL_EYE_PLANE,m);

 glTexGeni(GL_Q,GL_TEXTURE_GEN_MODE,GL_EYE_LINEAR);
 glEnable(GL_TEXTURE_GEN_Q);
 m[0]=mat[3];m[1]=mat[7];m[2]=mat[11];m[3]=mat[15];
 glTexGenfv(GL_Q,GL_EYE_PLANE,m);

 glEnable(GL_TEXTURE_2D);
}

void Texture::linearObject(const Matrix4 &mat) const {

 float m[4];
 glEnable(GL_TEXTURE_2D);
 glBindTexture(GL_TEXTURE_2D,_texId);

 glEnable(GL_TEXTURE_GEN_S);
 glTexGeni(GL_S,GL_TEXTURE_GEN_MODE,GL_OBJECT_LINEAR);
 m[0]=mat[0];m[1]=mat[4];m[2]=mat[8];m[3]=mat[12];
 glTexGenfv(GL_S,GL_OBJECT_PLANE,m);

 glEnable(GL_TEXTURE_GEN_T);
 glTexGeni(GL_T,GL_TEXTURE_GEN_MODE,GL_OBJECT_LINEAR);
 m[0]=mat[1];m[1]=mat[5];m[2]=mat[9];m[3]=mat[13];
 glTexGenfv(GL_T,GL_OBJECT_PLANE,m);


 glTexGeni(GL_R,GL_TEXTURE_GEN_MODE,GL_OBJECT_LINEAR);
 glEnable(GL_TEXTURE_GEN_R);
 m[0]=mat[2];m[1]=mat[6];m[2]=mat[10];m[3]=mat[14];
 glTexGenfv(GL_R,GL_OBJECT_PLANE,m);

 glTexGeni(GL_Q,GL_TEXTURE_GEN_MODE,GL_OBJECT_LINEAR);
 glEnable(GL_TEXTURE_GEN_Q);
 m[0]=mat[3];m[1]=mat[7];m[2]=mat[11];m[3]=mat[15];
 glTexGenfv(GL_Q,GL_OBJECT_PLANE,m);


}


void Texture::linearEye() const {
  glBindTexture(GL_TEXTURE_2D,_texId);

 glTexGeni(GL_S,GL_TEXTURE_GEN_MODE,GL_EYE_LINEAR);
 glEnable(GL_TEXTURE_GEN_S);

 glTexGeni(GL_T,GL_TEXTURE_GEN_MODE,GL_EYE_LINEAR);
 glEnable(GL_TEXTURE_GEN_T);

 glTexGeni(GL_R,GL_TEXTURE_GEN_MODE,GL_EYE_LINEAR);
 glEnable(GL_TEXTURE_GEN_R);

 glTexGeni(GL_Q,GL_TEXTURE_GEN_MODE,GL_EYE_LINEAR);
 glEnable(GL_TEXTURE_GEN_Q);

 glEnable(GL_TEXTURE_2D);
}


void Texture::enable() const {
	glBindTexture(GL_TEXTURE_2D,_texId);
	glEnable(GL_TEXTURE_2D);
}


void Texture::copyFrame(int x0,int y0,int width,int height) {
    bind();
    glCopyTexImage2D(GL_TEXTURE_2D,0,GL_RGB,x0,y0,width,height,0);
    disable();
}

void Texture::draw() {
    bind();
    glPushMatrix();
    glLoadIdentity();
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    enable();
    glOrtho(0,1,0,1,-0.01,1);
    glBegin(GL_QUADS);
    glTexCoord2f(0,0);
    glVertex2f(0,0);
    glTexCoord2f(1,0);
    glVertex2f(1,0);
    glTexCoord2f(1,1);
    glVertex2f(1,1);
    glTexCoord2f(0,1);
    glVertex2f(0,1);
    glEnd();
    disable();
    glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
    glLineWidth(2.0);
    glColor3f(1,0,0);
    glTranslatef(0,0,0.01);
    glBegin(GL_QUADS);
    glTexCoord2f(0,0);
    glVertex2f(0,0);
    glTexCoord2f(1,0);
    glVertex2f(1,0);
    glTexCoord2f(1,1);
    glVertex2f(1,1);
    glTexCoord2f(0,1);
    glVertex2f(0,1);
    glEnd();
    glLineWidth(1.0);
    glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
    disable();
}

void Texture::drawUnitSquare() {
    enable();
    glBegin(GL_QUADS);
    glTexCoord2f(0,0);
    glVertex2f(-1,-1);
    glTexCoord2f(1,0);
    glVertex2f(1,-1);
    glTexCoord2f(1,1);
    glVertex2f(1,1);
    glTexCoord2f(0,1);
    glVertex2f(-1,1);
    glEnd();
    disable();
}

void Texture::draw(int x,int y,int w,int h) {
    int view[4];
    glGetIntegerv(GL_VIEWPORT,view);
    glViewport(x,y,w,h);
    draw();
    glViewport(view[0],view[1],view[2],view[3]);
}

void Texture::generateMipmap(bool ok) {
  _generateMipMap=ok;
}

void Texture::parameter(unsigned int para,unsigned int mode) {
  glBindTexture(GL_TEXTURE_2D,_texId);
  glTexParameteri(GL_TEXTURE_2D,para,mode);
}

void Texture::filterMipmap() {
  bind();
  parameter(GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_LINEAR);
  parameter(GL_TEXTURE_MAG_FILTER,GL_LINEAR);
}

void Texture::filterLinear() {
  bind();
  parameter(GL_TEXTURE_MIN_FILTER,GL_LINEAR);
  parameter(GL_TEXTURE_MAG_FILTER,GL_LINEAR);
}

void Texture::setLuminance(bool ok) {
  _setLuminance=ok;
}

void Texture::transparency(bool active) {
  if (active) {
    glEnable(GL_BLEND);
    glBlendFunc(GL_ONE_MINUS_SRC_ALPHA,GL_SRC_ALPHA);
  }
  else
    glDisable(GL_BLEND);
}

