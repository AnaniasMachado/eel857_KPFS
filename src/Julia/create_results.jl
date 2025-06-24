using MAT
using CSV
using DataFrames
using Statistics

include("util.jl")
include("util/util_sol.jl")

# Define parameters
algorithms = ["ILS", "Simulated_Annealing", "GRASP", "Tabu_Search"]
algorithms_acronym = ["ils", "sa", "grasp", "ts"]
scenarios = [1, 2, 3, 4]
types = ["correlated_sc", "fully_correlated_sc", "not_correlated_sc"]
types_acronym = ["C", "FC", "NC"]
sizes = [300, 500, 700, 800, 1000]
indices = [i for i in 1:10]

# Initialize dataframe to store results
results = DataFrame(
    algorithm=String[],
    scenario=Int[],
    type=String[],
    size=Int[],
    obj_value=Float64[],
    cv_obj_value=Float64[],
    runtime=Float64[],
    cv_runtime=Float64[]
)

# Base path to instances
base_instances_path = "../../instances"

# Base path to solutions
base_solutions_path = "../../solutions"

for i_alg in 1:4
    algorithm = algorithms[i_alg]
    alg = algorithms_acronym[i_alg]
    for scenario in scenarios
        for i_type in 1:3
            type = types[i_type]
            type_s = types_acronym[i_type]
            for size in sizes
                obj_val_list = []
                time_list = []
                for idx in indices
                    instances_folder = "../../instances/scenario$(scenario)/$(type)$(scenario)/$(size)/"
                    inst_folder = readdir(instances_folder)
                    inst_file = "kpfs_$(idx).txt"
                    path = joinpath(instances_folder, inst_file)
                    try
                        data = read_instance(path)
                        matfile = matopen("../../solutions/$(algorithm)/scenario$(scenario)/$(type)$(scenario)/$(size)/grb_$(alg)_kpfs_$(idx)")
                        sol = read(matfile, "sol")
                        time = read(matfile, "time")
                        obj_val = data.values' * sol["solution"] - sol["total_penalty"]
                        push!(obj_val_list, obj_val)
                        push!(time_list, time)
                    catch e
                        println("Error: ", e)
                    end
                end
                
                try
                    # Compute mean
                    mean_obj_val = mean(obj_val_list)
                    mean_time = mean(time_list)

                    # Compute standard deviation
                    std_dev_obj_val = std(obj_val_list) 
                    std_dev_time = std(time_list) 

                    # Compute coefficient of variation in percentage
                    cv_obj_val = (std_dev_obj_val / mean_obj_val) * 100
                    cv_time = (std_dev_time / mean_time) * 100

                    # Add result to dataframe
                    push!(results, (
                        algorithm=algorithm,
                        scenario=scenario,
                        type=type_s,
                        size=size,
                        obj_value=mean_obj_val,
                        cv_obj_value=cv_obj_val,
                        runtime=mean_time,
                        cv_runtime=cv_time,
                    ))
                catch e
                    println("Error: ", e)
                end
            end
        end
    end
end

# Save dataframe in a CSV file
CSV.write("results.csv", results)