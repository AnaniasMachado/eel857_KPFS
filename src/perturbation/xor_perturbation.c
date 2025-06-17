#include <stdlib.h>

// Combine the given solution with a random viable solution using XOR
void xor_perturb_solution(const KnapsackInstance *instance, const Solution *sol, Solution *sol_prime) {
    // Create a random viable solution
    Solution *rand_sol = build_initial_solution_2(instance);

    // Create a copy of sol
    copy_solution(instance, sol, sol_prime);

    // Apply XOR item by item
    for (int i = 0; i < instance->item_count; i++) {
        int new_value = sol->included[i] ^ rand_sol->included[i];

        if (new_value == 1) {
            // Try to include item in final solution
            if (sol_prime->total_weight + instance->items[i].weight <= instance->capacity) {
                if (!sol_prime->included[i]) {
                    // sol_prime->included[i] = 1;
                    add_item(sol_prime, i);
                    sol_prime->total_weight += instance->items[i].weight;
                    sol_prime->total_value += instance->items[i].value;
                }
            }
        } else {
            // Try to remove item in final solution
            if (sol_prime->included[i]) {
                // sol_prime->included[i] = 0;
                remove_item(sol_prime, i);
                sol_prime->total_weight -= instance->items[i].weight;
                sol_prime->total_value -= instance->items[i].value;
            }
        }
    }

    // Recompute penalties after combination
    sol_prime->total_penalty = compute_penalty(instance, sol_prime);

    // Free rand_sol memory
    free_solution(rand_sol);
}
