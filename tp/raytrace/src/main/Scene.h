#ifndef SCENE_H
#define SCENE_H

#include "Vector3.h"
#include <string>
#include "Camera.h"
#include "CsgTree.h"
#include "Ray.h"
#include "Intersection.h"
#include "Material.h"

class TiXmlElement;

class Scene {
  public:
    Scene();
    virtual ~Scene();

    const std::string &name() {return _name;}
    void name(const std::string &name) {_name=name;}

    void read(const std::string &name);

    inline void csg(CsgTree *csg) {_csg=csg;}
    inline CsgTree *csg() {return _csg;}

    const prog3d::Vector3 &lightPosition(unsigned int i) const;
    void lightPosition(unsigned int i,const prog3d::Vector3 &p);
    void addLight(const prog3d::Vector3 &p,double intensity);
    double lightIntensity(unsigned int i);

    inline unsigned int nbLight() {return _light.size();}

    inline const prog3d::Camera &camera() {return _camera;}
    inline void camera(const prog3d::Camera &cam) {_camera.set(cam);}

    Intersection *intersection(const Ray &ray,double seuil);

    void drawGL();

    void prepareCsg();

    void trackball(const prog3d::Quaternion &q) {_trackball=q;}
    const prog3d::Quaternion &trackball() {return _trackball;}



  protected:
    CsgTree *readNode(TiXmlElement *elem,prog3d::Material currentMat);
  private:
    CsgTree *_csg;
    std::vector<prog3d::Vector3> _light;
    std::vector<double> _intensity;

    prog3d::Quaternion _trackball;
    prog3d::Camera _camera;
    std::string _name;
};

#endif // SCENE_H

