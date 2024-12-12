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
    int* avgColor;
    int indBlock;
    int widthCpt;
    int heightCpt;
    image finalImg;

    widthCpt = 1;
    heightCpt = 1;
    indBlock = 0;
    avgColor = malloc(sizeof(int) * 3);

    cpt = 0;

    width  = image_give_largeur(rag->img);
    height = image_give_hauteur(rag->img);

    finalImg = FAIRE_image();
    image_initialize(finalImg, image_give_dim(rag->img), width, height);

    image_debut(finalImg);
    while (cpt <= (width*height) && indBlock < n*m) {
        for (i=0; i<width; i++) {
            RAG_give_mean_color(rag, indBlock, avgColor);
            image_ecrire_pixel(finalImg, avgColor);
            cpt++;
            image_pixel_droite(finalImg);
            widthCpt++;
            if (widthCpt > (width/n)) {
                widthCpt = 1;
                indBlock++;
            }
        }
        image_pixel_dessous(finalImg);
        heightCpt++;
        if (heightCpt > (height/n)) {
            heightCpt = 1;
            indBlock++;
        }
        for (i=width-1; i>=0; i--) {
            image_pixel_gauche(finalImg);
        }
    }
    
    image_sauvegarder(finalImg, "./coucou.ppm");
    free(avgColor);
    return finalImg;
}