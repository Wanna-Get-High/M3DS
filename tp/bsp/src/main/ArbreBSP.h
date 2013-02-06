#ifndef INC_ARBREBSP_H
#define INC_ARBREBSP_H

#include "FaceBSP.h"
#include <string>

/**
@file
@author F. Aubert
@brief opérations sur les bsp
*/

/**
@class ArbreBSP
@brief Représentation d'un arbre BSP
*/
class ArbreBSP {
  /// les sous arbres gauche et droit
  ArbreBSP *_left,*_right;
  /// la facette pivot
  FaceBSP *_face;
public:
	/// destructeur
	~ArbreBSP();
	/// constructeur par défaut
	ArbreBSP();
	/// constructeur en affectant chacun des champs
	ArbreBSP(FaceBSP *f,ArbreBSP *g,ArbreBSP *d);

	/// affectation de chacun des champs
	void set(FaceBSP *f,ArbreBSP *g,ArbreBSP *d);
	/// affecte la facette pivot avec f
	void node(FaceBSP *f);
	/// affecte le noeud gauche
	void left(ArbreBSP *g);
	/// affecte le noeud droit
	void right(ArbreBSP *d);
	/// donne la facette du noeud
	FaceBSP *node();
	/// donne le sous arbre gauche
	ArbreBSP *left();
	/// donne le sous arbre droit
	ArbreBSP *right();
	/// trace le BSP selon technique du peintre
	void draw(const prog3d::Vector3 &obs);
	/// compte le nombre de faces
	int nbFace();
};



#endif
