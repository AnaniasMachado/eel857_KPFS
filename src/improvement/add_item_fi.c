#include <stdio.h>
#include <stdlib.h>

// Improvement: Add item with first improvement
int try_add_item_fi(const KnapsackInstance *instance, Solution *sol) {
    // Initialize an array of indices
    int *indices_arr = init_indeces_array(instance->item_count);
    // Create a permutation of the array of indices
    shuffle(indices_arr, instance->item_count);

    // Try to improve solution
    for (int i = 0; i < instance->item_count; i++) {
        int idx = indices_arr[i];
        if (!sol->included[idx]) {
            // Try adding item
            sol->included[idx] = 1;

            // Update total weight and value
            int new_weight = sol->total_weight + instance->items[idx].weight;
            if (new_weight <= instance->capacity) {
                // Recompute total value and penalties
                int new_value = sol->total_value + instance->items[idx].value;
                // Recompute penalties
                int new_penalty = compute_penalty(instance, sol);
                // Accept move
                int new_obj = new_value - new_penalty;
                if (new_obj > (sol->total_value - sol->total_penalty)) {
                    // Update solution
                    sol->total_value = new_value;
                    sol->total_weight = new_weight;
                    sol->total_penalty = new_penalty;
                    free(indices_arr);
                    return 1; // Improvement
                } else {
                    // Revert addition
                    sol->included[idx] = 0;
                }
            } else {
                // Capacity exceeded, revert
                sol->included[idx] = 0;
            }
        }
    }
    free(indices_arr);
    return 0; // No improvement
}