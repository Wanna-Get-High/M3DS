#ifndef RAYTRACE_H_INCLUDED
#define RAYTRACE_H_INCLUDED

#include "Color.h"
#include "Vector3.h"
#include "Ray.h"
#include "CsgTree.h"
#include "Intersection.h"
#include "Scene.h"
#include <QThread>
#include <QImage>




class Raytrace : public QThread {
private:
    Scene *_scene;
    QImage *_image;
    unsigned int _width,_height;
    bool _stopRequest;

    // boucle principale (calcul pour chaque pixel)
    void computeImage();

    // calcul de la couleur portée par un rayon (récursif si objets réfléchissant ou réfractant)
    prog3d::Color computeRayColor(const Ray &ray,int profondeur,double alpha);

    // calcule de la couleur par le modèle de Phong en un point d'intersection
    prog3d::Color computePhongColor(const Intersection &inter);

    // transformation pixel => repère caméra
    prog3d::Vector3 applyEyeWindow(int x,int y,float fovy);

public:
    Raytrace(unsigned int width,unsigned int height);
    ~Raytrace();

    void close();
    void run();
    inline void scene(Scene *s) {_scene=s;}

    QImage *image() {return _image;}

 };


#endif // RAYTRACE_H_INCLUDED

