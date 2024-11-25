#include <stdio.h>
#include <image.h>
#include <moments.h>
#include <RAG.h>
#include <stdlib.h>

int main(void) {
    image img;
    int M0;
    double M1[3];
    double M2[3];
    Rag rag;

    img = FAIRE_image();
    image_charger(img, "./IMAGES/zelda.ppm");
    
    /*give_moments(img,1,5,5,&M0, M1, M2);*/
    rag = create_RAG(img, 5, 5);

    uncreate_RAG(rag, 5, 5);
    DEFAIRE_image(img);
    return 0;
}