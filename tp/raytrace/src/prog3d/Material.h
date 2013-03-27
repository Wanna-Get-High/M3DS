#ifndef MATERIAL_H_INCLUDED
#define MATERIAL_H_INCLUDED

#include <string>
#include "Texture.h"
#include "Color.h"

/***
@file
@author Fabrice Aubert
@brief représentation matériel simple (phong, refraction, reflexion)
**/

namespace prog3d {

class Material {
  std::string _name;
  Color _ka,_kd,_ks;  
  double _alpha;
  double _shininess;
  double _refractionCoefficient,_reflexionCoefficient;
  double _refractionIndex;
	Texture *_mapKd;
	public:
	/// constructeur par défaut
	Material();
	/// destructeur
	virtual ~Material();

	///
  inline void ambient(const Color &a) {_ka=a;}
  inline void diffuse(const Color &d) {_kd=d;}
  inline void specular(const Color &s) {_ks=s;}
  inline void shininess(double s) {_shininess=s;}
  inline void alpha(double a) {_alpha=a;}

	///
  inline const Color &ambient() const {return _ka;}
  inline const Color &diffuse() const {return _kd;}
  inline const Color &specular() const {return _ks;}
  inline double shininess() const {return _shininess;}
  inline double alpha() const {return _alpha;}

  std::string name() const {return _name;}
  void name(const std::string &name) {_name=name;}

	///
//	void setDescartes(double n);
  void reflectionCoefficient(double alpha) {_reflexionCoefficient=alpha;}
  void refractionCoefficient(double alpha) {_refractionCoefficient=alpha;}
  void refractionIndex(double m) {_refractionIndex=m;}

  double refractionIndex() const {return _refractionIndex;}

	///
  //double descartes() const;
  double reflectionCoefficient() const {return _reflexionCoefficient;}
  double refractionCoefficient() const {return _refractionCoefficient;}

	void setGL();
	void activeTexture();

	void mapDiffuse(Texture *t);
	Texture *mapDiffuse();

  friend std::ostream& operator <<(std::ostream &s,const Material &q);

};

}


#endif // MATERIAL_H_INCLUDED

