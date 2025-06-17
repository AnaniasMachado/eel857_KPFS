#include <stdlib.h>

// Combine the given solution with a random viable solution
void hybrid_perturb_solution(const KnapsackInstance *instance, const Solution *sol, Solution *sol_prime) {
    // Create a random viable solution
    Solution *rand_sol = build_initial_solution_2(instance);

    // Create a copy of sol
    copy_solution(instance, sol, sol_prime);

    // For each item, decides if keep it or change it based on the random solution
    for (int i = 0; i < instance->item_count; i++) {
        int rand_decision = rand() % 2; // 0 or 1
        if (rand_decision == 0) {
            // Try to keep item from sol
            if (rand_sol->included[i]) {
                // Include item in final solution
                if (sol_prime->total_weight + instance->items[i].weight <= instance->capacity) {
                    // Include it if possible
                    if (!sol_prime->included[i]) {
                        // sol_prime->included[i] = 1;
                        add_item(sol_prime, i);
                        sol_prime->total_weight += instance->items[i].weight;
                        sol_prime->total_value += instance->items[i].value;
                    }
                } else {
                    // If it is not possible to include it, remove it
                    if (sol_prime->included[i]) {
                        // sol_prime->included[i] = 0;
                        remove_item(sol_prime, i);
                        sol_prime->total_weight -= instance->items[i].weight;
                        sol_prime->total_value -= instance->items[i].value;
                    }
                }
            }
            // If item is not in random sol, then keep it
        } else {
            // Try to keep item from random sol
            if (rand_sol->included[i]) {
                if (sol_prime->total_weight + instance->items[i].weight <= instance->capacity) {
                    if (!sol_prime->included[i]) {
                        // sol_prime->included[i] = 1;
                        add_item(sol_prime, i);
                        sol_prime->total_weight += instance->items[i].weight;
                        sol_prime->total_value += instance->items[i].value;
                    }
                }
            } else {
                // If item is not in random sol, then keep it
            }
        }
    }

    // Recompute penalties after combination
    sol_prime->total_penalty = compute_penalty(instance, sol_prime);

    // Free rand_sol memory
    free_solution(rand_sol);
}