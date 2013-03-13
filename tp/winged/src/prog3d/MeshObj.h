#ifndef MESHOBJ_H
#define MESHOBJ_H

#include <vector>
#include <string>
#include "Vector3.h"

namespace prog3d {
typedef std::vector<unsigned int> MeshFace;

static inline int cycle(int i,int nb) {
    return i-nb*(i/nb-(i<0?1:0));
}


class MeshObj
{
public:
    MeshObj();
    virtual ~MeshObj();

    void read(const std::string &filename);
    void scaleInBox(double left, double right, double top, double bottom,double znear,double zfar);
    void scaleInBoxMin(double left, double right, double bottom, double top, double znear, double zfar);

    void check();
    void computeNormal();
    void computeNormalFace(unsigned int i);
    void triangulate();

    inline const Vector3 &vertex(unsigned int i) {return _vertex[i];}
    inline const Vector3 &normalVertex(unsigned int i) {return _normalVertex[i];}
    inline const Vector3 &normalFace(unsigned int i) {return _normalFace[i];}
    inline int indexVertex(unsigned int i,unsigned int j) {return _face[i][j];}

    inline const Vector3 &vertexCycle(unsigned int i) {return _vertex[cycle(i,nbVertex())];}
    inline const Vector3 &normalVertexCycle(unsigned int i) {return _normalVertex[cycle(i,nbVertex())];}
    inline const Vector3 &normalFaceCycle(unsigned int i) {return _normalFace[cycle(i,nbFace())];}
    inline int indexVertexCycle(unsigned int i,unsigned int j) {return _face[cycle(i,nbFace())][cycle(j,nbVertexFace(i))];}



    inline unsigned int nbVertex() {return _vertex.size();}
    inline unsigned int nbVertexFace(unsigned int i) {return _face[i].size();}
    inline unsigned int nbFace() {return _face.size();}


protected: // protected only for easier coding of the answers (more visibility of the internal structure)
    std::vector<prog3d::Vector3> _vertex; //! x,y,z of a vertex
    std::vector<prog3d::Vector3> _normalVertex; //! x,y,z,of a normal (one normal per vertex)
    std::vector<prog3d::Vector3> _normalFace; //! x,y,z of a normal (one normal per face)
    std::vector<MeshFace> _face; //! _face[i][j] returns the index (relative to the array _vertex) of the j-ieme vertex of the i-ieme face
};
}

#endif // MESHOBJ_H

