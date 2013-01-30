#ifndef CAM_H
#define CAM_H

#include "Vector3.h"
#include "Quaternion.h"

namespace prog3d {

class Camera {
  public:
    Camera();
    virtual ~Camera();

    const Quaternion &orientation() const;
    const Vector3 &position() const;

    void orientation(const Quaternion &q);
    void position(const Vector3 &p);

    void set(const Camera &c);

    void applyGL();

    void lookAt(const Vector3 &at,const Vector3 &up);
    void interpolate(const Camera &cam1,const Camera &cam2,double t);
  protected:
  private:
    Quaternion _orientation;
    Vector3 _position;
};
}
#endif // CAM_H
