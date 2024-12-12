#include <image.h>
#ifndef RAG_H
#define RAG_H

typedef struct RAG * Rag;

typedef struct moments * moments;

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
    moments m;
    int *father;
    cellule neighbours;
};

extern Rag create_RAG(image, int, int);
extern void uncreate_RAG(Rag, int, int);
extern double RAG_give_closest_region(Rag,int*,int*);
extern double absoluteValue(double);
extern void RAG_merge_region(Rag, int, int);
extern void RAG_normalize_parents(Rag);
double errorCalcul(double, double[3], double, double[3]);
extern void RAG_give_mean_color(Rag, int, int*);
#endif