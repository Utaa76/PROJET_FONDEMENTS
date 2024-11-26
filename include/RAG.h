#ifndef RAG_H
#define RAG_H
#endif 

typedef struct RAG * Rag;

extern Rag create_RAG(image, int, int);
extern void uncreate_RAG(Rag, int, int);
extern double RAG_give_closest_region(Rag,int*,int*);