#include <stdio.h>
#include <stdlib.h>

// Improvement: Remove item with first improvement
void try_remove_item_neighboord(const KnapsackInstance *instance, const Solution * sol_prime, Solution *sol_double_prime) {
    // Initialize an array of indices
    int *indices_arr = init_indices_array(instance->item_count);
    // Create a permutation of the array of indices
    shuffle(indices_arr, instance->item_count);
    // Copy sol_prime to sol_double_prime
    copy_solution(instance, sol_prime, sol_double_prime);

    // Try to improve solution
    for (int i = 0; i < instance->item_count; i++) {
        int idx = indices_arr[i];
        if (sol_double_prime->included[idx]) {
            // Try removing item
            sol_double_prime->included[idx] = 0;

            // Recompute total weight and value
            int new_weight = sol_double_prime->total_weight - instance->items[idx].weight;
            int new_value = sol_double_prime->total_value - instance->items[idx].value;

            // Recompute penalties
            int new_penalty = compute_penalty(instance, sol_double_prime);

            // Check feasibility
            if (new_weight <= instance->capacity) {
                // Accept removal
                sol_double_prime->total_value = new_value;
                sol_double_prime->total_weight = new_weight;
                sol_double_prime->total_penalty = new_penalty;
                free(indices_arr);
                return;
            }
            // Revert removal
            sol_double_prime->included[idx] = 1;
        }
    }
    free(indices_arr);
}