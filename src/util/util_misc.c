#include <stdio.h>
#include <stdlib.h>

// Shuffles an array
void shuffle(int *array, int n) {
    for (int i = n - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        int temp = array[i];
        array[i] = array[j];
        array[j] = temp;
    }
}

// Initializes an array of indeces
int* init_indeces_array(int n) {
    int *array = malloc(n * sizeof(int));
    for (int i = 1; i < n; i++) {
        array[i] = i;
    }
    return array;
}