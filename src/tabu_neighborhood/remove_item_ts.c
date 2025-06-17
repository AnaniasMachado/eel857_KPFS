#include <stdio.h>
#include <stdlib.h>

// Improvement: Remove item with first improvement
int try_remove_item_ts(const KnapsackInstance *instance, Solution *sol, int **tabu_list, int size) {
    // Initialize an array of indices
    int *indices_arr = init_indices_array(instance->item_count);
    // Create a permutation of the array of indices
    shuffle(indices_arr, instance->item_count);

    // Try to improve solution
    for (int i = 0; i < instance->item_count; i++) {
        int idx = indices_arr[i];
        if (sol->included[idx]) {
            // Try removing item
            // sol->included[idx] = 0;
            remove_item(sol, idx);

            // Recompute total weight and value
            int new_weight = sol->total_weight - instance->items[idx].weight;
            int new_value = sol->total_value - instance->items[idx].value;

            // Recompute penalties
            int new_penalty = compute_penalty(instance, sol);

            // Check feasibility
            if (new_weight <= instance->capacity) {
                double new_obj = new_value - new_penalty;
                // Check if solution is in tabu list
                int is_tabu_flag = is_tabu(instance, sol, tabu_list, size);
                if ((new_obj > (sol->total_value - sol->total_penalty)) && !is_tabu_flag) {
                    // Accept removal
                    sol->total_value = new_value;
                    sol->total_weight = new_weight;
                    sol->total_penalty = new_penalty;
                    free(indices_arr);
                    return 1; // Improvement
                }
            }
            // Revert removal
            // sol->included[idx] = 1;
            add_item(sol, idx);
        }
    }
    free(indices_arr);
    return 0; // No improvement
}