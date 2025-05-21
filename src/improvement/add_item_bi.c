#include <stdio.h>
#include <stdlib.h>

// Improvement: Add item with best improvement
int try_add_item_bi(const KnapsackInstance *instance, Solution *sol) {
    // Initialize best solution found variables
    int best_obj = sol->total_value - sol->total_penalty;
    int best_idx = -1;

    // Try to improve solution
    for (int i = 0; i < instance->item_count; i++) {
        if (!sol->included[i]) {
            // Try adding item
            sol->included[i] = 1;

            // Update total weight and value
            int new_weight = sol->total_weight + instance->items[i].weight;
            if (new_weight <= instance->capacity) {
                // Recompute total value and penalties
                int new_value = sol->total_value + instance->items[i].value;
                // Recompute penalties
                int new_penalty = compute_penalty(instance, sol);
                // Accept move
                int new_obj = new_value - new_penalty;
                if (new_obj > best_obj) {
                    // Update best solution found variables
                    best_obj = new_obj;
                    best_idx = i;
                }
            }
            // Revert addition
            sol->included[i] = 0;
        }
    }

    // After checking all options, do the best move
    if (best_idx != -1) {
        // Update solution with best move
        sol->included[best_idx] = 1;
        sol->total_weight += instance->items[best_idx].weight;
        sol->total_value += instance->items[best_idx].value;
        sol->total_penalty = compute_penalty(instance, sol);
        return 1; // Improvement
    }

    return 0; // No improvement
}