#version 120

varying vec4 couleur;
varying vec4 vertexEye;

varying vec3 L;
varying vec3 N;


void main() {

    vec3 NN = normalize(N);
    vec3 LL = normalize(L);
    // de signe contraire car dirigé vers l'utilisateur
    vec3 V = normalize(- vertexEye.xyz);

    // N doit toujours être orienté vers l'observateur
    if (dot(V,NN)<0) NN = -NN;

    // R est donné par 2*(N.L)N−L.

    vec3 R = normalize(2*dot(NN,LL)*NN - LL);

    // Le coefficient de brillance s est donné par la constante prédéfinie gl_FrontMaterial.shininess
    // (affectée par l’application avec l’instruction glMaterial déjà faite dans le squelette)
    float S = gl_FrontMaterial.shininess;

    // la valeur du spéculaire a un sens si les vecteurs R et V vont dans la même direction (angle aigu) sinon l’éclairement est nul.
    // Donc lorsque les vecteurs R et V forment un angle obtus (V · R <0) il faut forcer un coefficient spec=0.
    // On peut obtenir cela en appliquant simplement en GLSL coef=max(dot(V,R),0.0;
    float coef = max(dot(V,R),0.0);

    // spec =(V · R)^s
    float spec = pow(coef,S);

    gl_FragColor = (couleur + spec*gl_FrontMaterial.specular)/2;
 //   gl_FragColor=vec4(1.0,0.0,0.0,0.0);

}

