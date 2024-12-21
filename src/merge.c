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
        printf("i = %d, height = %d\n", i, height);
        printf("i*n/height = %d\n", (i*n)/height);
        indBlock = ((i*n)/height) * m;
        printf("indBlock %d\n", indBlock);
        image_debut(finalImg);
        for (k = 0 ; k < i ; k++) {
            image_pixel_dessous(finalImg);
        }

        for (j = 0 ; j < width ; j++) {
            image_ecrire_pixel(finalImg, avgColor);
            image_pixel_droite(finalImg);

            if (((j%(width/n)) == 0) && (j !=0)) {
                printf("avgColor %d %d %d\n", avgColor[0], avgColor[1], avgColor[2]);
                RAG_give_mean_color(rag, indBlock, avgColor);
                printf("indBlock dans if %d\n", indBlock);
                indBlock++;
            }
        }
    }

    /*while (indBlock < n*m) {
        printf("indblock debut while %d\n", indBlock);
        for (i=0; i<width; i++) {
            image_ecrire_pixel(finalImg, avgColor);
            cpt++;
            image_pixel_droite(finalImg);
            widthCpt++;
            if (widthCpt > (width/n)) {
                RAG_give_mean_color(rag, indBlock, avgColor);
                widthCpt = 0;
                printf("test %d et avgColor %d %d %d\n", indBlock, avgColor[0], avgColor[1], avgColor[2]);
                indBlock++;
            }
        }
        cpt = 0;
        image_pixel_dessous(finalImg);
        heightCpt++;
        cptBis++;
        if (heightCpt > (height/m)) {
            heightCpt = 0;
            indBlock += n;
        }

        for (i=0; i<width; i++) {
            image_pixel_gauche(finalImg);
        }
        printf("indBlock a la fin %d\n", indBlock);
        indBlock = indBlock-n;
    }
    printf("pixels parcourus : %d\nattendus : %d\n", cpt, width*height);
    printf("nb lignes parcourus %d\n", cptBis);*/
    
    image_sauvegarder(finalImg, "./coucou.ppm");
    printf("heightCpt %d\n", heightCpt);
    return finalImg;
}