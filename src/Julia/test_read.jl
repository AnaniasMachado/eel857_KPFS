using MAT

solutions_folder = "../../solutions/Gurobi/scenario1/not_correlated_sc1/300/"
solution_file = "grb_sol_kpfs_idx_1"
path = joinpath(solutions_folder, solution_file)
matfile = matread(path)

x = matfile["x"]
v = matfile["v"]
time = matfile["time"]

println("Solution x: $x")
println("Solution v: $v")
println("Time: $time")