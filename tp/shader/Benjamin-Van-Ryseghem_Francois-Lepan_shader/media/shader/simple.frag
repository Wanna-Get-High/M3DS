#version 120

varying vec4 couleur;

varying vec2 texCoord;

// uniform veut dire que la variable image est alloué dans le programme OpenGl -> GLView::drawSquare()
uniform sampler2D image;

uniform sampler2D image1;

uniform sampler2D image2;

void main() {
    // rouge
    //gl_FragColor=vec4(1.0,0.0,0.0,0.0);

    // vert
    //gl_FragColor=vec4(0.0,1.0,0.0,0.0);

    // la couleur rouge vec4(1.0,0,0,0) est multiplié par un coéfficient dépendant du x des coordonnées couleur du pixel
    // c'est pour cela que cette couleur varie en fonction de sa position
    //gl_FragColor=gl_FragCoord.x/512.0*vec4(1.0,0,0,0); // du rouge;

    // on récupère la valeur pondérée des couleurs affectées aux sommets pour ce pixel
    //gl_FragColor=couleur;

    // on affecte a gl_FragColor les coordonnées de texture
    //gl_FragColor = vec4(texCoord.s,texCoord.t,0,0);

    // on affecte a gl_FragColor la couleur de l'image aux coordonnées de texture
    //gl_FragColor = texture2D(image,texCoord);

    // on affect les trois textures pour les images de brique
    // image = brique, image1 = blanc noir, image2 = logo univ
    // on veut que la texture entre le logo et la brique soit == en quantité
    // alors que pour le cercle blanc on veut que lorsque se soit 0 -> on met 0 pareil pour 1.
    float lembda = 0.7;
    vec4 text1 = (1-lembda)*texture2D(image,texCoord) + lembda*texture2D(image2,texCoord);
    gl_FragColor = text1;

}
