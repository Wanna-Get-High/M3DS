#ifndef Entity_H
#define Entity_H

#include "glsupport.h"
#include <string>
#include "MeshObj.h"

/**
@author F. Aubert
*/

class Entity : public prog3d::MeshObj
{
  public:
    Entity();
    virtual ~Entity();

    void read(std::string nom);

    void initBuffer();
    void drawBuffer();

protected:
  private:
    GLuint _vertexBuffer;
    GLuint _indiceBuffer;
    GLuint _normalBuffer;
};

#endif // OBJ3D_H

