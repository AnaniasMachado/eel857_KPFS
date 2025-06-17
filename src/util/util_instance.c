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

    // Read first line: nI, nS, kS
    if (fscanf(file, "%d %d %d", &nI, &nS, &kS) != 3) {
        fprintf(stderr, "Error: Could not read line 1.");
        fclose(file);
        return 1;
    }

    instance->item_count = nI;
    instance->forfeit_count = nS;
    instance->capacity = kS;

    // Allocate memory for items
    instance->items = (Item *) malloc(nI * sizeof(Item));
    if (!instance->items) {
        fprintf(stderr, "Error: Could not allocate memory for items.");
        fclose(file);
        return 1;
    }

    // Read value of items
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
        // Initialize item
        instance->items[i].id = i;
    }

    // Read weight of items
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
        // Attribute weight to item
        instance->items[i].weight = weights[i];
    }

    // Attribute value to items
    for (int i = 0; i < nI; i++) {
        instance->items[i].value = values[i];
    }

    free(values);
    free(weights);

    // Allocate memory for forfeit sets
    instance->forfeit_sets = (ForfeitSet *) malloc(nS * sizeof(ForfeitSet));
    if (!instance->forfeit_sets) {
        fprintf(stderr, "Error: Could not allocate memory for forfeit sets.");
        free(instance->items);
        fclose(file);
        return 1;
    }

    // Read forfeit sets data
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

        // Allocate memory for items of forfeit set
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

        // Read IDs of forfeit set items
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

    // // Allocate forfeit set size array
    // instance->forfeit_sizes = (int *) malloc(nS * sizeof(int));
    // if (!instance->forfeit_sizes) {
    //     fprintf(stderr, "Error: Could not create forfeit sizes array.\n");
    // }

    // // Allocate forfeit set matrix
    // instance->F = (int **) malloc(nS * sizeof(int *));
    // for (int i = 0; i < nS; i++) {
    //     instance->F[i] = (int *) calloc(nI, sizeof(int));
    //     if (!instance->F[i]) {
    //         fprintf(stderr, "Error: Could not create forfeit matrix row %d\n", i);
    //     }
    // }

    // // Read forfeit sets
    // for (int i = 0; i < nS; i++) {
    //     int nA, fC, nI_i;
    //     if (fscanf(file, "%d %d %d", &nA, &fC, &nI_i) != 3) {
    //         fprintf(stderr, "Error: Could not read items of forfeit set %d\n", i);
    //     }
    //     instance->forfeit_sizes[i] = nI_i;
    //     instance->thresholds[i] = nA;
    //     instance->penalties[i] = fC;
    //     for (int j = 0; j < nI_i; j++) {
    //         int item_id;
    //         if (fscanf(file, "%d", &item_id) != 1) {
    //             fprintf(stderr, "Error: Could not read an item of forfeit set %d\n", i);
    //         }
    //         if (item_id >= 0 && item_id < nI) {
    //             instance->F[i][item_id] = 1;
    //         }
    //     }
    // }

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

// void freeInstance(KnapsackInstance *instance) {
//     if (instance->items) {
//         free(instance->items);
//     }
//     for (int i = 0; i < instance->forfeit_count; i++) {
//         free(instance->F[i]);
//     }
//     free(instance->F);
//     free(instance->forfeit_sizes);
// }