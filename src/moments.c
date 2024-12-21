#ifndef STDIO_H
    #define STDIO_H
    #include <stdio.h>
#endif
#ifndef STDLIB_H
    #define STDLIB_H
    #include <stdlib.h>
#endif
#include <image.h>
#include "moments.h"

/**
 * Fills M0, M1 and M2
 * M0 = The amount of pixels in the block,
 * M1 = The sum of each pixel's RGB values in the block
 * M2 = The sum of each pixel's RGB squared values.
 * 
 * N.B.: Blocks starts from 1, not 0
 *
 * @param img The image
 * @param numBlock The number of the block to proceed in
 * @param n The amount of lines
 * @param m The amount of columns
 * @param M0 The array to fill with the amount of pixels
 * @param M1 The array to fill with the sum of each pixel's RGB values
 * @param M2 The array to fill with the sum of each pixel's RGB squared values
 */
extern void give_moments(image img, int numBlock, int n, int m, int* M0, double* M1, double* M2) {
    int widthBlock ;
    int heightBlock;
    int dimBlock;
    int *tabRGB;
    int X;
    int Y;
    int initialX;
    int initialY;
    int i;
    int j;
    
    /* Definition of the size of the image */
    dimBlock    = image_give_dim    (img);
    widthBlock  = image_give_largeur(img) / n;
    heightBlock = image_give_hauteur(img) / m;

    /* Calculation of M0 */
    *M0 = widthBlock*heightBlock;

    /* Calculation of M1 and M2*/
    image_debut(img);
    X = ((numBlock - 1) % n) * widthBlock ;
    Y = ((numBlock - 1) / n) * heightBlock;

    initialX = X;
    initialY = Y;

    for (i = 0 ; i < initialX ; i++) {
        image_pixel_droite(img);
    }

    for (j = 0 ; j < Y ; j++) {
        image_pixel_dessous(img);
    }

    M1[0] = 0.0;
    M1[1] = 0.0;
    M1[2] = 0.0;
    M2[0] = 0.0;
    M2[1] = 0.0;
    M2[2] = 0.0;

    /* Loop for each lines of the Block */
    while( (Y <= (initialY + heightBlock)) && (X <= (initialX + widthBlock)) ) {
        for (i = 0 ; i < widthBlock ; i++) {
            tabRGB = image_lire_pixel(img);
            printf("rouge %d\n", tabRGB[0]);
            M1[0] += tabRGB[0];
            M1[1] += tabRGB[1];
            M1[2] += tabRGB[2];
            M2[0] += tabRGB[0] * tabRGB[0];
            M2[1] += tabRGB[1] * tabRGB[1];
            M2[2] += tabRGB[2] * tabRGB[2];
            image_pixel_droite(img);
            X++;
        }
        Y++;
        image_pixel_dessous(img);

        if (Y <= (initialY + heightBlock)) {
            for(j = 0; j < widthBlock; j++) {
                tabRGB = image_lire_pixel(img);
                M1[0] += tabRGB[0];
                M1[1] += tabRGB[1];
                M1[2] += tabRGB[2];
                M2[0] += tabRGB[0] * tabRGB[0];
                M2[1] += tabRGB[1] * tabRGB[1];
                M2[2] += tabRGB[2] * tabRGB[2];
                image_pixel_gauche(img);
                X--;
            }
            Y++;
            image_pixel_dessous(img);
        }
        printf("M1[0] %f\n", M1[0]);
    }

    printf("X*Y %d\nM0 %d\n", X*Y, *M0);
    printf("M1/M0 %f\n", M1[0] / *M0);
}

