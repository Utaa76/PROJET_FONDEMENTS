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

struct moments {
    int    M0;
    double M1[3];
    double M2[3];
};

typedef struct cellule* cellule;

struct cellule {
    int block;
    cellule next;
};

struct RAG {
    int size;
    image img;
    struct moments *m;
    int *father;
    cellule neighbours;
};

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
        if ((i <= (n*(m-1))) && (nbNeighbours == 1)) {  /* If there is already a right neighbour and he has a down neighbour */

            rag->neighbours[i-1].next = malloc(sizeof(struct cellule));
            rag->neighbours[i-1].next->block = i+n;
            rag->neighbours[i-1].next->next = malloc(sizeof(struct cellule));
            rag->neighbours[i-1].next->next = NULL;
        }
        else if(i <= (n*(m-1))) { /* If there is no right neighbours but a down neighbour exists */
            rag->neighbours[i-1].next = malloc(sizeof(struct cellule));
            rag->neighbours[i-1].block = i+n;
            rag->neighbours[i-1].next = NULL;
        }
        else { /* If there is no neighbours */
            rag->neighbours[i-1].block = -1;
            rag->neighbours[i-1].next = NULL;
        }
        
    }
        
    return rag;
}

void free_neighbours_next(cellule next) {
    if (next->next == NULL) {
        free(next);
        return;
    }
    free_neighbours_next(next->next);
}

extern void uncreate_RAG(Rag rag, int n, int m) {
    int i;
    free(rag->m);
    free(rag->father);

    /** @todo free les neighbours next */
    for (i = 0 ; i < n*m ; i++) {
        if (rag->neighbours[i].next != NULL) {
            free_neighbours_next(rag->neighbours[i].next);
        }
    }

    free(rag->neighbours);
}