#ifndef SQUARE_H
#define SQUARE_H

#include "glsupport.h"
#include <vector>


class Square {
  public:
    Square();

    void texture(unsigned int unite,GLuint id);

    void initBuffer();
    void drawBuffer();

    virtual ~Square();
  protected:
  private:

  GLuint _bufferVertex;
 GLuint _bufferColor;

  std::vector<GLuint> _idTexture;  // les identifiants de texture disponibles
  std::vector<GLuint> _bufferTexCoord; // les coordonnées de textures pour chaque unité de texture

};

#endif // SQUARE_H

