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

scenarios = [1]
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
            solutions_folder = "../../solutions/ILS/scenario$(scenario)/$(type)$(scenario)/$(size)/"
            if (scenario == 1) && (type == "correlated_sc")
                continue
            elseif (scenario == 1) && (type == "fully_correlated_sc") && (size in [300, 500, 700, 800])
                continue
            elseif (scenario == 1) && (type == "fully_correlated_sc") && (size == 1000)
                for idx in 7:10
                    try
                        inst_file = "kpfs_$(idx).txt"
                        path = joinpath(instances_folder, inst_file)
                        println("Solving: $(path)")
                        data = read_instance(path)
                        elapsed_time = @elapsed begin
                            sol = iterated_local_search(data, max_iter, no_change_limit, type_improvement)
                        end
                        println("Time: $(elapsed_time)")
                        solution_filename = "grb_ils_kpfs_$(idx)"
                        solution_filepath = joinpath(solutions_folder, solution_filename)
                        matwrite(solution_filepath, Dict("sol" => sol, "time" => elapsed_time))
                        GC.gc()
                    catch e
                        println("Error: ", e)
                    end
                end
            else
                for idx in 1:10
                    try
                        inst_file = "kpfs_$(idx).txt"
                        path = joinpath(instances_folder, inst_file)
                        println("Solving: $(path)")
                        data = read_instance(path)
                        elapsed_time = @elapsed begin
                            sol = iterated_local_search(data, max_iter, no_change_limit, type_improvement)
                        end
                        println("Time: $(elapsed_time)")
                        solution_filename = "grb_ils_kpfs_$(idx)"
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
end