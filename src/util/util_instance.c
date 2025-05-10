#include <stdio.h>
#include <stdlib.h>

// Function to read a instance from a file
int loadInstance(const char *filename, KnapsackInstance *instance) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Error: Could not open file.");
        return 1;
    }

    int nI, nS, kS;

    // Reads first line: nI, nS, kS
    if (fscanf(file, "%d %d %d", &nI, &nS, &kS) != 3) {
        fprintf(stderr, "Error: Could not read line 1.");
        fclose(file);
        return 1;
    }

    instance->item_count = nI;
    instance->forfeit_count = nS;
    instance->capacity = kS;

    // Allocates memory for items
    instance->items = (Item *) malloc(nI * sizeof(Item));
    if (!instance->items) {
        fprintf(stderr, "Error: Could not allocate memory for items.");
        fclose(file);
        return 1;
    }

    // Reads value of items
    double *values = (double *) malloc(nI * sizeof(double));
    if (fscanf(file, "%lf", &values[0]) != 1) {
        fprintf(stderr, "Error: (T1) Could not read items values.");
        free(instance->items);
        fclose(file);
        return 1;
    }
    for (int i = 0; i < nI; i++) {
        if (i > 0) {
            if (fscanf(file, "%lf", &values[i]) != 1) {
                fprintf(stderr, "Error: (T2) Could not read items values.");
                free(instance->items);
                free(values);
                fclose(file);
                return 1;
            }
        }
        // Initializes item
        instance->items[i].id = i;
    }

    // Reads weight of items
    double *weights = (double *) malloc(nI * sizeof(double));
    if (fscanf(file, "%lf", &weights[0]) != 1) {
        fprintf(stderr, "Error: (T1) Could not read items weight.");
        free(instance->items);
        free(values);
        free(weights);
        fclose(file);
        return 1;
    }
    for (int i = 0; i < nI; i++) {
        if (i > 0) {
            if (fscanf(file, "%lf", &weights[i]) != 1) {
                fprintf(stderr, "Error: (T2) Could not read items values.");
                free(instance->items);
                free(values);
                free(weights);
                fclose(file);
                return 1;
            }
        }
        // Attributes weight to item
        instance->items[i].weight = weights[i];
    }

    // Attributes value to items
    for (int i = 0; i < nI; i++) {
        instance->items[i].value = values[i];
    }

    free(values);
    free(weights);

    // Allocates memory for forfeit sets
    instance->forfeit_sets = (ForfeitSet *) malloc(nS * sizeof(ForfeitSet));
    if (!instance->forfeit_sets) {
        fprintf(stderr, "Error: Could not allocate memory for forfeit sets.");
        free(instance->items);
        fclose(file);
        return 1;
    }

    // Reads forfeit sets data
    for (int i = 0; i < nS; i++) {
        int nA, fC, nI_i;

        // Line with nA, fC, nI
        if (fscanf(file, "%d %d %d", &nA, &fC, &nI_i) != 3) {
            fprintf(stderr, "Error: Could not read line of forfeit set %d", i);
            free(instance->items);
            for (int j = 0; j < i; j++) {
                free(instance->forfeit_sets[j].items);
            }
            free(instance->forfeit_sets);
            fclose(file);
            return 1;
        }

        // Allocates memory for items of forfeit set
        instance->forfeit_sets[i].items = (int *) malloc(nI_i * sizeof(int));
        if (!instance->forfeit_sets[i].items) {
            fprintf(stderr, "Error: Could not allocate memory for forfeit set %d", i);
            free(instance->items);
            for (int j = 0; j < i; j++) {
                free(instance->forfeit_sets[j].items);
            }
            free(instance->forfeit_sets);
            fclose(file);
            return 1;
        }

        instance->forfeit_sets[i].size = nI_i;
        instance->forfeit_sets[i].threshold = nA;
        instance->forfeit_sets[i].penalty = fC;

        // Reads IDs of forfeit set items
        for (int j = 0; j < nI_i; j++) {
            if (fscanf(file, "%d", &instance->forfeit_sets[i].items[j]) != 1) {
                fprintf(stderr, "Error: Could not read items of forfeit set %d", i);
                free(instance->items);
                for (int k = 0; k <= i; k++) {
                    free(instance->forfeit_sets[k].items);
                }
                free(instance->forfeit_sets);
                fclose(file);
                return 1;
            }
        }
    }

    fclose(file);
    return 0;
}

// Function to free instance memory
void freeInstance(KnapsackInstance *instance) {
    if (instance->items) {
        free(instance->items);
    }
    if (instance->forfeit_sets) {
        for (int i = 0; i < instance->forfeit_count; i++) {
            if (instance->forfeit_sets[i].items) {
                free(instance->forfeit_sets[i].items);
            }
        }
        free(instance->forfeit_sets);
    }
}