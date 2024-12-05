#include <stdio.h>
#include <stdlib.h>
#include <RAG.h>

void perform_merge(Rag rag, double threshold) {
    int* indblock1;
    int* indblock2;

    indblock1 = malloc(sizeof(int));
    indblock2 = malloc(sizeof(int));
    
    while(RAG_give_closest_region(rag, indblock1, indblock2) <= threshold ) {
        RAG_merge_region(rag, rag->father[*indblock1 - 1], rag->father[*indblock2 - 1]);
    }
    free(indblock1);
    free(indblock2);
}