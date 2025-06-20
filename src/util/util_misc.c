#include <stdio.h>
#include <stdlib.h>

// Shuffle an array
void shuffle(int *array, int n) {
    for (int i = n - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        int temp = array[i];
        array[i] = array[j];
        array[j] = temp;
    }
}

// Initialize an array of indeces
int* init_indices_array(int n) {
    int *array = malloc(n * sizeof(int));
    for (int i = 0; i < n; i++) {
        array[i] = i;
    }
    return array;
}