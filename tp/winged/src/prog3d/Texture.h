#ifndef INC_TEXTURE_H
#define INC_TEXTURE_H

#include "glsupport.h"
#include <string>
#include "Vector2.h"
#include "Matrix4.h"
#include <QImage>
#include "Color.h"

/**
@file
@author F. Aubert
@brief Encapsulates common texture operations
*/


namespace prog3d {
/**
@class Texture
@brief Encapsulates common texture operations
*/
class Texture {
	GLuint _width,_height;
	GLuint _texId;
	GLuint _filter;
	bool _generateMipMap;
	bool _setLuminance;
	unsigned int _border;
	unsigned int _level;
  QImage *_img;
	void init();  
  void toGL();


public:
  virtual ~Texture();
  /// ctor
  Texture();
  /// ctor from image file
  Texture(const std::string &filename);
  /// empty texture of size w x h
	void init(int w,int h);
	/// read and build mip map texture
	void mipMap(const std::string &filename);
	/// read image file
	void read(const std::string &filename);
	/// read image file for the given mip map level
	void read(const std::string &filename,int level);

	/// get width
	inline unsigned int width() const {return _width;}
	/// get height
	inline unsigned int height() const {return _height;}
	/// get data from the texture (caller should delete the result when not needed)
	GLubyte *imageCopy();
	/// copy img in the texture
	void copyFrom(GLubyte *img,unsigned int width=0,unsigned int height=0);

	/// tells if the texture has border (should be called before read).
	void setBorder(unsigned int b);

  /// this is the current opengl texture for the current texture unit
  void bind() const;
  /// set wrap (wr = GL_CLAMP, GL_REPEAT, etc)
  void wrap(GLenum wr);
  /// spherical mapping
	void sphere() const;
	/// linear mapping in local coordinates
	void linearObject() const;
	void linearObject(const prog3d::Matrix4 &mat) const;
	/// linear mapping in eye coordinates
	void linearEye() const;
	void linearEye(const prog3d::Matrix4 &mat) const;
	/// texture mode
	void envMode(GLenum m);
	/// enable
	void enable() const;
	/// disable
	void disable() const;
	/// draw the texture on screen (debug)
	void draw(); // all screen
	void draw(int x,int y,int w,int h); // part of the screen


	/// draw the texture on the unit square
	void drawUnitSquare();

	/// copy frame in texture
	void copyFrame(int x0,int y0,int width,int height);
	/// get the texture id
    inline GLuint id() const {return _texId;}

  /// old uses
  void parameter(unsigned int para,unsigned int mode);

  void generateMipmap(bool ok);
  void filterMipmap();
  void filterLinear();
  void transparency(bool active);
  void setLuminance(bool ok);
  void setAlpha(const prog3d::Color &c, double v,double radius);
  void setAlpha(double v);
};


}

#endif

