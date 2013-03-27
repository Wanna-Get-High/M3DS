#include "Sphere.h"
#include <cmath>
#include <iostream>
#include "UtilGL.h"
#include "IntersectionArray.h"

/**
@file
@author Fabrice Aubert
*/

using namespace prog3d;
using namespace std;

Sphere::Sphere() : Primitive() {};


/**
  donne la liste croissante issus de l'intersection de ray=(A,u)  et de la sphere x^2+y^2=1
  (2 ou 0 intersection)
*/
void Sphere::intersection(const Ray &ray,IntersectionArray *result) {

    /**
   * A COMPLETER : il faut résoudre l'équation en lambda, puis créer les intersections correspondant aux lambda trouvés dans le résultat :
   * !! pour ajouter une intersection à la fin de la liste result, utilisez : result->addIntersection(lambda)
   * ray est déjà dans le repère local de la sphere unitaire centrée à l'origine :
   *   - ray.point() : donne l'origine du rayon (Vector3)
   *   - ray.direction() : donne le vecteur directeur (Vector3)
   * il faut impérativement retourner 0 ou 2 intersections pour assurer une cohérence dans l'intersection.
   * il faut que l'intersection la plus petite soit en premier (respect d'une contrainte de listes triées pour l'intersection avec le CSG)
   *
   *  rappel : a.dot(b) donne le produit scalaire de a par b
   */


    result->clear(); // initialisation : liste d'intersection vide

    Vector3 direction = ray.direction();
    Vector3 origin = ray.origin();

    // on récupère les a, b et c pour le calcule du lambda
    // facteur des carré
    float a = direction.dot(direction);
    float b = 2 * direction.dot(origin);
    float c = origin.dot(origin) - 1;

    // delta = b^2 - 4*ac
    float delta = pow(b,2)-4*(a*c);

    float lambda1;
    float lambda2;

    // On regarde le delta
    // -> 2 intersection ( > 0 )
    // -> tangeante ( = 0 )
    // -> pas d'intersection ( < 0 )
    if (delta > 0)
    {
        // On calcule les lambda
        lambda1=(-b+sqrt(delta))/(2*a);
        lambda2=(-b-sqrt(delta))/(2*a);

        // On les mets dans l'ordre croissant
        if (lambda1 < lambda2)
        {
            result->addIntersection(lambda1);
            result->addIntersection(lambda2);
        } else {
            result->addIntersection(lambda2);
            result->addIntersection(lambda1);
        }
    }

}


/** ****************************************************** **/
/** ****************************************************** **/
/** ****************************************************** **/
/** ****************************************************** **/

Vector3 Sphere::computeNormal(const Vector3 &p) {
    return p;
}


void Sphere::drawGL() {
    colorGL();
    UtilGL::drawSphere(20,20);
}

