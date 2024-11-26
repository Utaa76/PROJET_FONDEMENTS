#include <image.h>
#include <moments.h>
#include <RAG.h>

int main(void) {
    image img;
    Rag rag;
    int a;
    int b;

    a = 3;
    b = 4;

    img = FAIRE_image();
    image_charger(img, "./IMAGES/zelda.ppm");
    
    rag = create_RAG(img, 5, 5);
    printf("%f\n", RAG_give_closest_region(rag, &a, &b));
    
    uncreate_RAG(rag, 5, 5);
    DEFAIRE_image(img);
    return 0;
}