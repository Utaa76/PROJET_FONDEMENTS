#ifndef STDIO_H
    #define STDIO_H
    #include <stdio.h>
#endif
#ifndef STDLIB_H
    #define STDLIB_H
    #include <stdlib.h>
#endif
#include <RAG.h>
#include <quadError.h>

extern double RAG_give_closest_region(Rag rag, int* indBlock1, int* indBlock2) {
    double quadraticError;
    double temp;
    int i;
    int tempind1;
    int tempind2;
    double uR;
    double uG;
    double uB;
    quadraticError = 1E10;

    for(i=0; i< rag->size;i++){
        tempind1 = i;
        if(rag->father[i] == i) {

            if(rag->neighbours[i-1].block != 0) {
                tempind2 = rag->neighbours[i-1].block;
                temp = ((rag->moments[tempind1].M0 * rag->moments.M0[tempind2]) / (rag->moments.M0[tempind1]+rag->moments.M0[tempind2]));
                uR = ((rag->moments[tempind1].M1[0] / rag->moments[tempind1].M0) - (rag->moments[tempind2].M1[0] / rag->moments[tempind2].M0))*((rag->moments[tempind1].M1[0] / rag->moments[tempind1].M0) - (rag->moments[tempind2].M1[0] / rag->moments[tempind2].M0));
                uG = ((rag->moments[tempind1].M1[1] / rag->moments[tempind1].M0) - (rag->moments[tempind2].M1[1] / rag->moments[tempind2].M0))*((rag->moments[tempind1].M1[1] / rag->moments[tempind1].M0) - (rag->moments[tempind2].M1[1] / rag->moments[tempind2].M0));
                uB = ((rag->moments[tempind1].M1[2] / rag->moments[tempind1].M0) - (rag->moments[tempind2].M1[2] / rag->moments[tempind2].M0))*((rag->moments[tempind1].M1[2] / rag->moments[tempind1].M0) - (rag->moments[tempind2].M1[2] / rag->moments[tempind2].M0));
                temp *= (uR + uG + uB);

                if(temp < quadraticError) {
                    quadraticError = temp;
                    *indBlock1 = tempind1;
                    *indBlock2 = tempind2;
                }
            }

            if(rag->neighbours[i-1].next.block != 0) {
                tempind2 = rag->neighbours[i-1].next.block;
                temp = ((rag->moments[tempind1].M0 * rag->moments.M0[tempind2]) / (rag->moments.M0[tempind1]+rag->moments.M0[tempind2]));
                uR = ((rag->moments[tempind1].M1[0] / rag->moments[tempind1].M0) - (rag->moments[tempind2].M1[0] / rag->moments[tempind2].M0))*((rag->moments[tempind1].M1[0] / rag->moments[tempind1].M0) - (rag->moments[tempind2].M1[0] / rag->moments[tempind2].M0));
                uG = ((rag->moments[tempind1].M1[1] / rag->moments[tempind1].M0) - (rag->moments[tempind2].M1[1] / rag->moments[tempind2].M0))*((rag->moments[tempind1].M1[1] / rag->moments[tempind1].M0) - (rag->moments[tempind2].M1[1] / rag->moments[tempind2].M0));
                uB = ((rag->moments[tempind1].M1[2] / rag->moments[tempind1].M0) - (rag->moments[tempind2].M1[2] / rag->moments[tempind2].M0))*((rag->moments[tempind1].M1[2] / rag->moments[tempind1].M0) - (rag->moments[tempind2].M1[2] / rag->moments[tempind2].M0));
                temp *= (uR + uG + uB);

                if(temp < quadraticError) {
                    quadraticError = temp;
                    *indBlock1 = tempind1;
                    *indBlock2 = tempind2;
                }
            }
        }
    }

    return quadraticError;
}