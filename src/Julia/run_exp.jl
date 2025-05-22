using MAT
using Base.GC

include("util.jl")
include("solver.jl")

scenarios = [1, 2, 3, 4]
types = ["correlated_sc", "fully_correlated_sc", "not_correlated_sc"]
sizes = [300, 500, 700, 800, 1000]

for scenario in scenarios
    for type in types
        for size in sizes
            instances_folder = "../../instances/scenario$(scenario)/$(type)$(scenario)/$(size)/"
            solutions_folder = "../../solutions/Gurobi/scenario$(scenario)/$(type)$(scenario)/$(size)/"
            for idx in 1:10
                inst_file = "kpfs_$(idx).txt"
                path = joinpath(instances_folder, inst_file)
                data = read_instance(path)
                time = @elapsed begin
                    x, v = gurobi_solver(data)
                end
                solution_filename = "grb_sol_kpfs_idx_$(idx)"
                solution_filepath = joinpath(solutions_folder, solution_filename)
                matwrite(solution_filepath, Dict("x" => x, "v" => v, "time" => time))
                GC.gc()
            end
        end
    end
end