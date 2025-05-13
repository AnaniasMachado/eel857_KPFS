include("util.jl")
include("solver.jl")

instances_folder = "../../instances/scenario1/not_correlated_sc1/300/"
inst_folder = readdir(instances_folder)
inst_file = "kpfs_1.txt"

path = joinpath(instances_folder, inst_file)
data = read_instance(path)

info_matrix(data.forfeit_matrix)

x, v = gurobi_solver(data)

println("Objective value: $(objetive_value(data, x, v))")

println("Is viable: $(is_viable(data, x))")