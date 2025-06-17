using MAT
using Base.GC

include("util.jl")
include("solver.jl")

# scenarios = [1, 2, 3, 4]
scenarios = [1, 3]
# types = ["correlated_sc", "fully_correlated_sc", "not_correlated_sc"]
types = ["correlated_sc"]
sizes = [300, 500, 700, 800, 1000]

opt_tol = 10^(-5)
# time_limit = 21600 # 6 hours
time_limit = 300 # 5 minutes

for scenario in scenarios
    for type in types
        for size in sizes
            instances_folder = "../../instances/scenario$(scenario)/$(type)$(scenario)/$(size)/"
            solutions_folder = "../../solutions/Gurobi/scenario$(scenario)/$(type)$(scenario)/$(size)/"
            if (scenario == 1) && (size == 300)
                continue
            elseif (scenario == 1) && (size == 500)
                for idx in 2:10
                    try
                        inst_file = "kpfs_$(idx).txt"
                        path = joinpath(instances_folder, inst_file)
                        println("Solving: $(path)")
                        data = read_instance(path)
                        elapsed_time = @elapsed begin
                            x, v, gap = gurobi_solver(data, opt_tol, time_limit)
                        end
                        println("Time: $(elapsed_time)")
                        solution_filename = "grb_sol_kpfs_$(idx)"
                        solution_filepath = joinpath(solutions_folder, solution_filename)
                        matwrite(solution_filepath, Dict("x" => x, "v" => v, "time" => elapsed_time, "gap" => gap))
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
                            x, v, gap = gurobi_solver(data, opt_tol, time_limit)
                        end
                        println("Time: $(elapsed_time)")
                        solution_filename = "grb_sol_kpfs_$(idx)"
                        solution_filepath = joinpath(solutions_folder, solution_filename)
                        matwrite(solution_filepath, Dict("x" => x, "v" => v, "time" => elapsed_time, "gap" => gap))
                        GC.gc()
                    catch e
                        println("Error: ", e)
                    end
                end
            end
        end
    end
end