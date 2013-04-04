#ifndef CSGTREE_H_INCLUDED
#define CSGTREE_H_INCLUDED

/*!
*
* @file
*
* @brief opérations sur une grille de nbU x nbV points
* @author F. Aubert
*
*/

class IntersectionArray;
class Primitive;

#include "Line.h"
#include "Matrix4.h"
#include "Material.h"
#include "Ray.h"
#include "Matrix3.h"


/**
@class CsgTree
@brief opérations sur les csg
*/
class CsgTree {

public:
  typedef enum {INTERSECTION,UNION,DIFFERENCE,LEAF} EOperation;
    /// constructeur par défaut
  CsgTree();
  /// destructeur
  ~CsgTree();

    /// constructeur par champ pour un noeud interne
  CsgTree(const prog3d::Matrix4 &trans,CsgTree *left,CsgTree *right,EOperation op);
  /// constructeur par champ pour une feuille
    CsgTree(const prog3d::Matrix4 &trans,Primitive *p);

    /// Calcul de la normale au point P d'une feuille
    prog3d::Vector3 computeNormalLeaf(const prog3d::Vector3 &p);


    inline const prog3d::Matrix4 &matrix() const {return _parent2Node;}


    void pointNode2Root(prog3d::Vector3 *p);
    void directionNode2Root(prog3d::Vector3 *p);
    void pointRoot2Node(prog3d::Vector3 *p);
    void directionRoot2Node(prog3d::Vector3 *p);
    void normalNode2Root(prog3d::Vector3 *p);

    Primitive *primitive() const;


    bool isLeaf() const;

    /// prepare le csg (les feuilles contiendront le passage de racine à feuille et inverse
    void cumulateMatrix();
    void composeMatrix(const prog3d::Matrix4 &root2Node,const prog3d::Matrix4 &node2Root);


    static void cumulateMatrix(CsgTree *t,const prog3d::Matrix4 &root2Parent,const prog3d::Matrix4 &parent2Root);

    void root2Node(const prog3d::Matrix4 &m) {_root2Node.set(m);}
    void node2Root(const prog3d::Matrix4 &m) {_node2Root.set(m);}
    void normalNode2Root(const prog3d::Matrix3 &m) {_normalNode2Root.set(m);}

    inline const prog3d::Matrix4 &parent2Node() const {return _parent2Node;}
    inline const prog3d::Matrix4 &node2Parent() const {return _node2Parent;}

    inline const prog3d::Matrix4 &root2Node() const {return _root2Node;}
    inline const prog3d::Matrix4 &node2Root() const {return _node2Root;}

    CsgTree* createNode(const prog3d::Matrix4 &trans,CsgTree *left,CsgTree *right,EOperation op);
    CsgTree* createLeaf(const prog3d::Matrix4 &trans,Primitive *p);

    inline CsgTree *left() const {return _left;}
    inline CsgTree *right() const {return _right;}
    inline EOperation operation() const {return _operation;}

    void setNode(const prog3d::Matrix4 &trans,CsgTree *left,CsgTree *right,EOperation op);
    void setLeaf(const prog3d::Matrix4 &trans,Primitive *p);

    inline void left(CsgTree *left) {_left=left;}
    inline void right(CsgTree *right) {_right=right;}
    inline void primitive(Primitive *p) {_primitive=p;_operation=LEAF;}
    inline void operation(EOperation op) {_operation=op;}
    inline void matrix(const prog3d::Matrix4 &f) {_parent2Node=f;_node2Parent.invert(f);}

    void print();


    static void draw(CsgTree *t);
    void drawLocal();
    void drawParent();

    void intersection(const Ray &ray, IntersectionArray *res);
private:
    prog3d::Matrix4 _parent2Node;
    prog3d::Matrix4 _root2Node;

    prog3d::Matrix4 _node2Parent;
    prog3d::Matrix4 _node2Root;

    prog3d::Matrix3 _normalNode2Root;

    CsgTree *_right,*_left;
    Primitive *_primitive;
    CsgTree::EOperation _operation;

};


#endif // CSGTREE_H_INCLUDED

