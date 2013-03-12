#version 120

// varying = créé un lien pour le fragment shader
// la couleur affecté au sommet est transmise par le biais de couleur puis interpolé (moyenne pondéré)
// puis récupéré par le fragment shader et il reste à affecter cette valeur au pixel.
varying vec4 couleur;


varying vec2 texCoord;

void main() {
    // on affecte à la variable couleur gl_Color qui est la couleur du sommet
    // traité par le shader
    couleur = gl_Color;

    // .st renvoi un vec2 correspondant aux coordonnées s et t
    texCoord = gl_MultiTexCoord0.st;

    gl_Position = gl_ProjectionMatrix*gl_ModelViewMatrix*gl_Vertex;
}
