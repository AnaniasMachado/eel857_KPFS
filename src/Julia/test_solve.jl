include("util.jl")
include("solver.jl")

instances_folder = "../../instances/scenario1/not_correlated_sc1/300/"
inst_folder = readdir(instances_folder)
inst_file = "kpfs_1.txt"

path = joinpath(instances_folder, inst_file)
data = read_instance(path)

info_matrix(data.forfeit_matrix)

elapsed_time = @elapsed begin
    x, v, duality_gap = gurobi_solver(data)
end

println("Objective value: $(eval_objective_value(data, x, v))")

println("Is viable: $(is_viable(data, x))")

println("Duality gap: $(duality_gap)")

println("Time: $(elapsed_time)")