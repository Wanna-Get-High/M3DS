#ifndef VIEWPORTUTIL_H_INCLUDED
#define VIEWPORTUTIL_H_INCLUDED

#include "Matrix4.h"
#include "Vector2.h"

/**
@file
@author F. Aubert
@brief converts a point in various OpenGL coordinate systems (local,eye,normalized,window)
*/
namespace prog3d {

/**
@class ViewportUtil
@brief converts a point in various coordinate systems (local,eye,normalized,window). setView should be called to setup the current OpenGL transformation
*/

class ViewportUtil {
  Matrix4 localWindow; // matrix M_Local_Window
  Matrix4 normalizedLocal;
  Matrix4 windowNormalized;
  Matrix4 normalizedWindow;
  Matrix4 eyeLocal;

public:
  /// ctor
  ViewportUtil();
  /// dtor
  virtual ~ViewportUtil();

  /// donne le point 2d (en coordonnées pixels) du point 3d p (donné dans le repère local)
  Vector2 windowCoordinate(const Vector3 &p) const;
  Vector2 applyWindowLocal(const Vector3 &p) const;
  /// donne le point 3d, exprimé dans le repère local courant, correspondant au point 2d (en pixels) mouse. Le point 3d donné sera tel que sa profondeur (i.e. z dans Eye) sera la même que le point référence
  Vector3 applyLocalWindow(const Vector2 &mouse,const Vector3 &reference);
  /// donne le point 3d, exprimé dans le repère local courant, correspondant au point 2d (en pixels). Le point 3d donné sera tel que sa profondeur normalisée (dans Eye) soit depth
  Vector3 applyLocalWindow(const Vector2 &mouse,double depth);
  /// donne le Vector3 en coordoonées normalisées
  Vector3 applyNormalizedLocal(const Vector3 &p) const;
  /// affecte les matrices de passages correspondantes au repère local et à la projection courants (à appeler dans le display pour mémoriser les matrices openGL courantes).
  void setView();
  /// donne le point 3D de l'observateur exprimé dans le repère local courant
  Vector3 cameraLocal();
  /// donne le point 3D en coordonnées observateur de p (supposé en coordonnées locales).
  Vector3 applyEyeLocal(const Vector3 &p) const;
  /// donne la matrice eye->local (i.e. la modelview au moment du setView() ).
  const Matrix4 &getEyeLocal() const;

};

}

#endif // VIEWPORTUTIL_H_INCLUDED
