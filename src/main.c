#include <image.h>
#include <moments.h>
#include <merge.h>

int main(void) {
    image img;
    Rag rag;
    int* indBlock1;
    int* indBlock2;
    int n;
    int m;
    double error;

    indBlock1 = malloc(sizeof(int));
    indBlock2 = malloc(sizeof(int));

    n = 5;
    m = 5;

    error = 1E10;

    img = FAIRE_image();
    image_charger(img, "./IMAGES/papillon.ppm");
    
    rag = create_RAG(img, n, m);

    RAG_merge_region(rag, 21, 22);
    printf("%f\n", RAG_give_closest_region(rag, indBlock1, indBlock2));
    
    perform_merge(rag, error);

    uncreate_RAG(rag, n, m);
    DEFAIRE_image(img);
    
    free(indBlock1);
    free(indBlock2);
    
    return 0;
}

/*

1   2   3   4   5
6   7   8   9   10
11  12  13  14  15
16  17  18  19  20
21  22  23  24  25

 */

 /*./obj/image.o ./obj/main.o ./obj/merge.o ./obj/moments.o ./obj/RAG.o */