#include <stdio.h>
#include <stdlib.h>
#include <RAG.h>

void perform_merge(Rag rag, double threshold) {
    int* indblock1;
    int* indblock2;

    indblock1 = malloc(sizeof(int));
    indblock2 = malloc(sizeof(int));
    
    while(RAG_give_closest_region(rag, indblock1, indblock2) <= threshold ) {
        RAG_merge_region(rag, rag->father[*indblock1] - 1, rag->father[*indblock2] - 1);
    }
    free(indblock1);
    free(indblock2);
}

image create_output_image(Rag rag, int n, int m) {
    int width;
    int height;
    int cpt;
    int i;
    int j;
    int k;
    int avgColor[3];
    int indBlock;
    int widthCpt;
    int heightCpt;
    int cptBis;
    image finalImg;

    widthCpt = 1;
    heightCpt = 1;
    indBlock = 0;

    cpt = 0;
    cptBis = .0;

    width  = image_give_largeur(rag->img);
    height = image_give_hauteur(rag->img);

    finalImg = FAIRE_image();
    image_initialize(finalImg, image_give_dim(rag->img), width, height);

    image_debut(finalImg);
    RAG_give_mean_color(rag, 0, avgColor);

    for (i = 0 ; i < height ; i++) {
        indBlock = ((i*n)/height) * m;
        image_debut(finalImg);
        for (k = 0 ; k < i ; k++) {
            image_pixel_dessous(finalImg);
        }

        for (j = 0 ; j < width ; j++) {
            /*       (width+n-1)/n sert à arrondir au supérieur la division entière*/
            if (((j%((width+n-1)/n)) == 0) && indBlock < ((k*m/height)+2)*n) {
                RAG_give_mean_color(rag, indBlock, avgColor);
                indBlock++;
            }
            image_ecrire_pixel(finalImg, avgColor);
            image_pixel_droite(finalImg);
        }
    }
    
    image_sauvegarder(finalImg, "./image_output.ppm");
    return finalImg;
}