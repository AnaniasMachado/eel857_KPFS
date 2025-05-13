#include <stdlib.h>

// Overhaul the given solution
void overhaul_perturb_solution(const KnapsackInstance *instance, const Solution *sol, Solution *sol_prime) {
    // Create a random viable solution
    Solution *rand_sol = build_initial_solution_2(instance);

    // Create a copy of sol
    copy_solution(instance, rand_sol, sol_prime);

    // Free rand_sol memory
    free_solution(rand_sol);
}