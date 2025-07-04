using MAT
using Base.GC
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

scenarios = [2]
# types = ["correlated_sc", "fully_correlated_sc", "not_correlated_sc"]
types = ["not_correlated_sc", "correlated_sc", "fully_correlated_sc"]
sizes = [300, 500, 700, 800, 1000]

max_iter = 15
no_change_limit = 3
type_improvement = "fi"

for scenario in scenarios
    for type in types
        for size in sizes
            instances_folder = "../../instances/scenario$(scenario)/$(type)$(scenario)/$(size)/"
            solutions_folder = "../../solutions/GRASP/scenario$(scenario)/$(type)$(scenario)/$(size)/"
            for idx in 1:10
                try
                    inst_file = "kpfs_$(idx).txt"
                    path = joinpath(instances_folder, inst_file)
                    println("Solving: $(path)")
                    data = read_instance(path)
                    elapsed_time = @elapsed begin
                        sol = greedy_randomized_adaptive_search_procedure(data, max_iter, no_change_limit, type_improvement)
                    end
                    println("Time: $(elapsed_time)")
                    solution_filename = "grb_grasp_kpfs_$(idx)"
                    solution_filepath = joinpath(solutions_folder, solution_filename)
                    matwrite(solution_filepath, Dict("sol" => sol, "time" => elapsed_time))
                    GC.gc()
                catch e
                    println("Error: ", e)
                end
            end
        end
    end
end