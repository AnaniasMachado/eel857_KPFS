#include <stdio.h>
#include <stdlib.h>

#include "types/item.h"
#include "types/forfeit_set.h"
#include "types/knacksack_instance.h"
#include "types/solution.h"
#include "util.c"

int main() {
    KnapsackInstance instance;
    const char *filename = "../instances/scenario1/not_correlated_sc1/300/kpfs_1.txt";
    
    if (!loadInstance(filename, &instance)) {
        printf("Instance loaded succesfully!!\n");
        printf("Number of items: %d\n", instance.item_count);
        printf("Capacity: %d\n", instance.capacity);
        printf("Number of forfeit sets: %d\n", instance.forfeit_count);

        int id = 0;

        printf("Forfeit set size: %d\n", instance.forfeit_sets[id].size);
        printf("Forfeit set threshold: %d\n", instance.forfeit_sets[id].threshold);
        printf("Forfeit set penalty: %d\n", instance.forfeit_sets[id].penalty);

        for (int i = 0; i < instance.forfeit_sets[id].size; i++) {
            printf("%d \n", instance.forfeit_sets[id].items[i]);
        }

        // Free memory
        freeInstance(&instance);
    } else {
        printf("Failed to load instance.\n");
    }
    return 0;
}