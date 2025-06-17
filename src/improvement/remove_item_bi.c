#include <stdio.h>
#include <stdlib.h>

// Improvement: Remove item with best improvement
int try_remove_item_bi(const KnapsackInstance *instance, Solution *sol) {
    // Initialize best solution found variables
    int best_obj = sol->total_value - sol->total_penalty;
    int best_idx = -1;

    // Try to improve solution
    for (int i = 0; i < instance->item_count; i++) {
        if (sol->included[i]) {
            // Try removing item
            // sol->included[i] = 0;
            remove_item(sol, i);

            // Recompute total weight and value
            int new_weight = sol->total_weight - instance->items[i].weight;
            int new_value = sol->total_value - instance->items[i].value;

            // Recompute penalties
            int new_penalty = compute_penalty(instance, sol);

            // Check feasibility
            if (new_weight <= instance->capacity) {
                double new_obj = new_value - new_penalty;
                // Accept remove
                if (new_obj > best_obj) {
                    // Update best solution found variables
                    best_obj = new_obj;
                    best_idx = i;
                }
            }
            // Revert removal
            sol->included[i] = 1;
            add_item(sol, i);
        }
    }

    // After checking all options, do the best move
    if (best_idx != -1) {
        // Update solution with best move
        sol->included[best_idx] = 0;
        remove_item(sol, best_idx);
        sol->total_weight -= instance->items[best_idx].weight;
        sol->total_value -= instance->items[best_idx].value;
        sol->total_penalty = compute_penalty(instance, sol);
        return 1; // Improvement
    }

    return 0; // No improvement
}