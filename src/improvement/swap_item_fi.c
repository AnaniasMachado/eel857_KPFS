#include <stdio.h>
#include <stdlib.h>

// Improvement: Swap two items in the solution with first improvement
int try_swap_item_fi(const KnapsackInstance *instance, Solution *sol) {
    // Initialize an array of indices
    int *indices_arr = init_indeces_array(instance->item_count);
    // Create a permutation of the array of indices
    shuffle(indices_arr, instance->item_count);

    // Try to improve solution
    for (int i = 0; i < instance->item_count; i++) {
        int idx_i = indices_arr[i];
        if (sol->included[idx_i]) {
            for (int j = 0; j < instance->item_count; j++) {
                int idx_j = indices_arr[j];
                if (!sol->included[idx_j]) {
                    // Try swapping i with j
                    sol->included[idx_i] = 0;
                    sol->included[idx_j] = 1;

                    // Check scapacity
                    int new_weight = sol->total_weight - instance->items[idx_i].weight + instance->items[idx_j].weight;
                    if (new_weight <= instance->capacity) {
                        // Recompute value
                        int new_value = sol->total_value - instance->items[idx_i].value + instance->items[idx_j].value;

                        // Recompute penalties
                        int new_penalty = compute_penalty(instance, sol);

                        int new_obj = new_value - new_penalty;
                        if (new_obj > (sol->total_value - sol->total_penalty)) {
                            // Accept swap
                            sol->total_value = new_value;
                            sol->total_weight = new_weight;
                            sol->total_penalty = new_penalty;
                            free(indices_arr);
                            return 1;
                        }
                    }
                    // Revert swap
                    sol->included[idx_i] = 1;
                    sol->included[idx_j] = 0;
                }
            }
        }
    }
    free(indices_arr);
    return 0; // No improvement
}