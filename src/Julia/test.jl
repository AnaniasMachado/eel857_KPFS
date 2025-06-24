using Random

include("util.jl")
include("solver.jl")
include("types.jl")
include("util/util_sol.jl")
include("util/util_perturb.jl")
include("util/util_neighborhood.jl")
include("util/util_tabu_search.jl")
include("util/util_local_search.jl")
include("util/util_rvnd.jl")
include("meta-heuristics/ils.jl")
include("meta-heuristics/grasp.jl")
include("meta-heuristics/sa.jl")
include("meta-heuristics/ts.jl")

instances_folder = "../../instances/scenario1/not_correlated_sc1/300/"
inst_folder = readdir(instances_folder)
inst_file = "kpfs_1.txt"

path = joinpath(instances_folder, inst_file)
data = read_instance(path)

info_matrix(data.forfeit_matrix)

# sol = build_initial_solution_grasp(data)

# println(sol.solution)
# println(sol.included)
# println(sol.not_included)
# println(sol.total_value)
# println(sol.total_weight)
# println(sol.total_penalty)

max_iter = 15
no_change_limit = 3
type_improvement = "fi"
T = 0.001;
t_0 = 150.0;
alpha = 0.50;
tabu_size = 3

sol = iterated_local_search(data, max_iter, no_change_limit, type_improvement)

println("ILS objetive function value: $(compute_objetive_value(data, sol))")
println("ILS is feasible: $(is_feasible(data, sol))")

sol = greedy_randomized_adaptive_search_procedure(data, max_iter, no_change_limit, type_improvement)

println("GRASP objetive function value: $(compute_objetive_value(data, sol))")
println("GRASP is feasible: $(is_feasible(data, sol))")

sol = simulated_annealing(data, max_iter, T, t_0, alpha, type_improvement)

println("SA objetive function value: $(compute_objetive_value(data, sol))")
println("SA is feasible: $(is_feasible(data, sol))")

sol = tabu_search(data, max_iter, tabu_size, no_change_limit, type_improvement)

println("TS objetive function value: $(compute_objetive_value(data, sol))")
println("TS is feasible: $(is_feasible(data, sol))")