#ifndef STDIO_H
    #define STDIO_H
    #include <stdio.h>
#endif
#ifndef STDLIB_H
    #define STDLIB_H
    #include <stdlib.h>
#endif
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
    double uR;
    double uG;
    double uB;
    double a;
    int b;
    quadraticError = 1E20;

    for(i=0; i< rag->size;i++){
        if(rag->father[i] == i+1) {
            /* If the current block has a neighbour */
            if(rag->neighbours[i].block != -1) {
                rag->neighbours[i].block = rag->father[(rag->neighbours[i].block) - 1];
                tempind2 = rag->neighbours[i].block;
                a = (double)rag->m[i].M0 * (double)rag->m[tempind2].M0;
                b = (double)rag->m[i].M0 + (double)rag->m[tempind2].M0;
                temp = a / b;
                uR = ((rag->m[i].M1[0] / rag->m[i].M0) - (rag->m[tempind2].M1[0] / rag->m[tempind2].M0));
                uR *= uR;
                uG = ((rag->m[i].M1[1] / rag->m[i].M0) - (rag->m[tempind2].M1[1] / rag->m[tempind2].M0))*((rag->m[i].M1[1] / rag->m[i].M0) - (rag->m[tempind2].M1[1] / rag->m[tempind2].M0));
                uG *= uG;
                uB = ((rag->m[i].M1[2] / rag->m[i].M0) - (rag->m[tempind2].M1[2] / rag->m[tempind2].M0))*((rag->m[i].M1[2] / rag->m[i].M0) - (rag->m[tempind2].M1[2] / rag->m[tempind2].M0));
                temp *= absoluteValue(uR + uG + uB);
                if(temp < quadraticError) {
                    quadraticError = temp;
                    *indBlock1 = i + 1;
                    *indBlock2 = tempind2;
                }

                /* If the current block's neighbour has also a neighbour */
                if(rag->neighbours[i].next != NULL) {
                    rag->neighbours[i].block = rag->father[(rag->neighbours[i].block) - 1];
                    tempind2 = rag->neighbours[i].next->block;
                    a = (double)rag->m[i].M0 * (double)rag->m[tempind2].M0;
                    b = (double)rag->m[i].M0 + (double)rag->m[tempind2].M0;
                    temp = a / b;
                    uR = ((rag->m[i].M1[0] / rag->m[i].M0) - (rag->m[tempind2].M1[0] / rag->m[tempind2].M0))*((rag->m[i].M1[0] / rag->m[i].M0) - (rag->m[tempind2].M1[0] / rag->m[tempind2].M0));
                    uG = ((rag->m[i].M1[1] / rag->m[i].M0) - (rag->m[tempind2].M1[1] / rag->m[tempind2].M0))*((rag->m[i].M1[1] / rag->m[i].M0) - (rag->m[tempind2].M1[1] / rag->m[tempind2].M0));
                    uB = ((rag->m[i].M1[2] / rag->m[i].M0) - (rag->m[tempind2].M1[2] / rag->m[tempind2].M0))*((rag->m[i].M1[2] / rag->m[i].M0) - (rag->m[tempind2].M1[2] / rag->m[tempind2].M0));
                    temp *= absoluteValue(uR + uG + uB);

                    if(temp < quadraticError) {
                        quadraticError = temp;
                        *indBlock1 = i + 1;
                        *indBlock2 = tempind2;
                    }
                }
            } 
        }
    }
    return quadraticError;
}

extern void RAG_merge_region(Rag rag, int indBlock1, int indBlock2) {
    int tmp;
    int i;
    cellule last2;
    cellule neighbour;

    if (indBlock1 > indBlock2) {
        tmp = indBlock1;
        indBlock1 = indBlock2;
        indBlock2 = tmp;
    }

    printf("1 %d\n", rag->neighbours[indBlock1 - 1].block);
    printf("2 %d\n", rag->neighbours[indBlock2 - 1].block);

    /* Updating the father array */
    rag->father[indBlock1-1] = indBlock2;

    /* Updating the moments array */
    rag->m[indBlock2-1].M0 += rag->m[indBlock1-1].M0;

    for (i = 0 ; i < 3 ; i++) {
        rag->m[indBlock2-1].M1[i] += rag->m[indBlock1-1].M1[i];
        rag->m[indBlock2-1].M2[i] += rag->m[indBlock1-1].M2[i];
    }

    /* Updating the neighbours array */
    /* TODO Mixx neighbours in order */
    last2 = &rag->neighbours[indBlock2-1];
    while (last2->next != NULL) {
        last2 = last2->next;
    }

    neighbour = &rag->neighbours[indBlock1-1];
    last2->next = neighbour;


    rag->neighbours[indBlock1-1].block = -1;
    rag->neighbours[indBlock1-1].next  = NULL;
}

/* Faire la question */
extern void RAG_normalize_parents(Rag rag) {
    int i;
    for( i=(rag->size)-1; i>=0; i--) {
        rag->father[i] = rag->father[rag->father[i]];
    }
}

void free_neighbours_next(cellule next) {
    if (next->next == NULL || next->block != -1) {
        printf("free %d\n", next->block);
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
        printf("bloc %d\n", i+1);
        if ((rag->neighbours[i].next != NULL) && (rag->neighbours[i].block != -1)) {
            printf("free 2 ==> %d\n", rag->neighbours[i].next->block);s
            free_neighbours_next(rag->neighbours[i].next);
        }
    }

    free(rag->neighbours);
}