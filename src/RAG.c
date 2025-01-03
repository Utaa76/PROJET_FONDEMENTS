#include <stdio.h>
#include <image.h>
#include <moments.h>
#include <RAG.h>

extern Rag create_RAG(image img, int n, int m) {
    Rag rag;
    int    M0;
    double M1[3];
    double M2[3];
    unsigned int i;
    unsigned int j;
    unsigned int nbNeighbours;

    rag = malloc(sizeof(struct RAG));

    rag->size = n*m;
    rag->img  = img;

    rag->m          = malloc(sizeof(struct moments) * n * m);
    rag->father     = malloc(sizeof(int)*n*m);
    rag->neighbours = malloc(sizeof(struct cellule) * n * m);

    for (i = 1 ; i <= (n*m) ; i++) {
        rag->father[i-1] = i;
        give_moments(img, i, n, m, &M0, M1, M2);
        rag->m[i-1].M0 = M0;

        /* Copy each moments calculated before to the array of moments */
        for (j = 0 ; j < 3 ; j++) {
            rag->m[i-1].M1[j] = M1[j];
            rag->m[i-1].M2[j] = M2[j];
        }

        nbNeighbours = 0;
        
        /* We do not add the upper neighbour neither the left neighbour because, per numerical order, i has theses blocs as parents */

        /* Right neighbour */
        if (i % n != 0) {
            rag->neighbours[i-1].block = i+1;
            nbNeighbours++;
        }

        /* Lower neighbour */
        if ((i <= (n*(m-1))) && (nbNeighbours == 1)) {  /* If there is already a right neighbour and he has a below neighbour */

            rag->neighbours[i-1].next = malloc(sizeof(struct cellule));
            rag->neighbours[i-1].next->block = i+n;
            rag->neighbours[i-1].next->next = malloc(sizeof(struct cellule));
            rag->neighbours[i-1].next->next = NULL;

        } else if(i <= (n*(m-1))) { /* If there is no right neighbours but a below neighbour exists */
            rag->neighbours[i-1].next = malloc(sizeof(struct cellule));
            rag->neighbours[i-1].block = i+n;
            rag->neighbours[i-1].next = NULL;

        } else if(nbNeighbours == 1) { /* If there is only a right neighbour */
            rag->neighbours[i-1].next = NULL;

        } else { /* If there is no neighbours */
            rag->neighbours[i-1].block = -1;
            rag->neighbours[i-1].next = NULL;
        }
        
    }
        
    return rag;
}

extern double absoluteValue(double val){
    if(val < 0) {
        val = -val;
    }
    return val;
}

extern double RAG_give_closest_region(Rag rag, int* indBlock1, int* indBlock2) {
    double quadraticError;
    double temp;
    int i;
    int tempind2;
    cellule cursor;
    quadraticError = 1E20; /*TODO useful?*/

    for(i=0; i< rag->size;i++){

        cursor = &rag->neighbours[i];
        while (cursor != NULL) {
            
            if (cursor->block != -1) {
                cursor->block = rag->father[(cursor->block) - 1];
            }
            cursor = cursor->next;
        }
        
        if(rag->father[i] == i+1) {

            /* If the current block has a neighbour */
            if(rag->neighbours[i].block != -1) {
                tempind2 = rag->neighbours[i].block - 1;

                temp = errorCalcul(rag->m[i].M0, rag->m[i].M1, rag->m[tempind2].M0, rag->m[tempind2].M1);
                if(temp < quadraticError) {
                    quadraticError = temp;
                    *indBlock1 = i;
                    *indBlock2 = tempind2;
                }

                /* If the current block's neighbour has also a neighbour */
                if(rag->neighbours[i].next != NULL) {
                    cursor = rag->neighbours[i].next;
                    while (cursor->next != NULL) {
                        tempind2 = cursor->next->block - 1;
                        temp = errorCalcul(rag->m[i].M0, rag->m[i].M1, rag->m[tempind2].M0, rag->m[tempind2].M1);
                        if(temp < quadraticError) {
                            quadraticError = temp;
                            *indBlock1 = i;
                            *indBlock2 = tempind2;
                        }   
                        cursor = cursor->next;
                    }
                }
            } 
        }
    }
    return quadraticError;
}

double errorCalcul(double M0ind1, double M1ind1[3], double M0ind2, double M1ind2[3]) {
    double error;
    double uR;
    double uG;
    double uB;

    error = (M0ind1 * M0ind2) / ( M0ind1 + M0ind2);
    uR = ((M1ind1[0] / M0ind1) - (M1ind2[0] / M0ind2));
    uG = ((M1ind1[1] / M0ind1) - (M1ind2[1] / M0ind2));
    uB = ((M1ind1[2] / M0ind1) - (M1ind2[2] / M0ind2));

    error = absoluteValue(uR*uR + uG*uG + uB*uB);

    return error;
}

extern void RAG_merge_region(Rag rag, int indBlock1, int indBlock2) {
    int tmp;
    int i;
    struct cellule *last2;
    struct cellule *neighbour1;
    struct cellule *celTmp;
    struct cellule *celTmp2;
    
    /* We make sure that indBlock1 is the smallest value */
    if (indBlock1 > indBlock2) {
        tmp = indBlock1;
        indBlock1 = indBlock2;
        indBlock2 = tmp;
    }

    /* Updating the father array */
    rag->father[indBlock1] = indBlock2+1;

    /* We change the father of the blocks that have indBlock1 as father */
    /* TODO A retirer au besoin*/
    /*for (i = 0 ; i < indBlock1 ; i++) {
        if (rag->father[i] == indBlock1+1) {
            rag->father[i] = indBlock2+1;
        }
    }*/

    /* Updating the moments array */
    rag->m[indBlock2].M0 += rag->m[indBlock1].M0;

    for (i = 0 ; i < 3 ; i++) {
        rag->m[indBlock2].M1[i] += rag->m[indBlock1].M1[i];
        rag->m[indBlock2].M2[i] += rag->m[indBlock1].M2[i];
    }

    /* Updating the neighbours array */
    /* TODO Mixx neighbours in order */
    last2 = &rag->neighbours[indBlock2];
    while (last2->next != NULL) {
        last2 = last2->next;
    }

    /* Adding the neighbour only if it's NOT the block2*/
    neighbour1 = &rag->neighbours[indBlock1];
    celTmp = NULL;
    while (neighbour1 != NULL) {
        if (neighbour1->block <= indBlock2+1) { /*Duplicate case*/
            if (celTmp != NULL) {
                celTmp->next = neighbour1->next;
                celTmp = NULL;
                celTmp2 = neighbour1->next;
                free(neighbour1);
                neighbour1 = celTmp2;
                continue;
            }
        } else {
            if (neighbour1 == &rag->neighbours[indBlock1]) {
                last2->next = malloc(sizeof(struct cellule));
                last2->next->block = neighbour1->block;
                last2->next->next = neighbour1->next;
            } else {
                last2->next = neighbour1;
                last2 = last2->next;
            }
        }

        celTmp = neighbour1;
        neighbour1 = neighbour1->next;
    }

    /* indBlock1 has now 0 neighbour */
    rag->neighbours[indBlock1].block = -1;
    rag->neighbours[indBlock1].next  = NULL;
}

/* Faire la question */
extern void RAG_normalize_parents(Rag rag) {
    int i;
    for( i=(rag->size)-1; i>=0; i--) {
        rag->father[i] = rag->father[rag->father[i] - 1];

    }
}

void free_neighbours_next(cellule next) {
    if (next->next == NULL || next->block == -1) {
        free(next);
        return;
    }
    free_neighbours_next(next->next);
}

extern void uncreate_RAG(Rag rag, int n, int m) {
    int i;
    free(rag->m);
    free(rag->father);

    for (i = 0 ; i < n*m ; i++) {
        if ((rag->neighbours[i].next != NULL) && (rag->neighbours[i].block != -1)) {
            free_neighbours_next(rag->neighbours[i].next);
        }
    }

    free(rag->neighbours);
}

extern void RAG_give_mean_color(Rag rag, int indBlock, int* avgColor) {
    int fatherBlock;
    int i;
    int totalPixels;
    double localM1[3];
    fatherBlock = rag->father[indBlock]-1;
    totalPixels = rag->m[fatherBlock].M0;
    localM1[0] = rag->m[fatherBlock].M1[0];
    localM1[1] = rag->m[fatherBlock].M1[1];
    localM1[2] = rag->m[fatherBlock].M1[2];
    for (i=0; i<3; i++) {
        avgColor[i] = localM1[i] / totalPixels;
    }
}