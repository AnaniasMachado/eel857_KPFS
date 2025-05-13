include("types.jl")

epsilon = 10^(-5)

function read_instance(instance_path::String)
    # Open file to read
    open(instance_path, "r") do f
        # Read first line with nI, nS, kS
        first_line = readline(f)
        nI, nS, kS = parse.(Int, split(first_line))
        
        # Read value of items
        values_line = readline(f)
        values = parse.(Float64, split(values_line))
        
        # Read weight of items
        weights_line = readline(f)
        weights = parse.(Float64, split(weights_line))
        
        # Initialize forfeit sets matrix: nS x nI
        forfeit_matrix = zeros(Int, nS, nI)

        # Initialize vector of allowances
        a = zeros(nS)

        # Initialize vector of forfeit costs
        d = zeros(nS)

        # Initialize vector of sizes of each forfeit set
        c = zeros(nS)
        
        # For each forfeit set, reads information
        for i in 1:nS
            # Line with nA_i, fC_i, nI_i
            info_line = readline(f)
            nA_i, fC_i, nI_i = parse.(Int, split(info_line))

            a[i] = nA_i
            d[i] = fC_i
            c[i] = nI_i
            
            # Line with id of items in forfeit set
            ids_line = readline(f)
            ids = parse.(Int, split(ids_line))
            
            # Fill matrix: for each id, fill with 1 in line i
            for id in ids
                # Check if id is in expected interval
                if 0 <= id < nI
                    forfeit_matrix[i, id+1] = 1 # Plus 1 since Julia is 1-based
                else
                    error("ID of item $id outside of allowed interval 0..$(nI-1)")
                end
            end
        end

        # Create struct with data
        inst = Instance(
            nI,
            nS,
            kS,
            values,
            weights,
            forfeit_matrix,
            a,
            d,
            c
        )
        return inst
    end
end

function info_matrix(M)
    dims = size(M)
    total = length(M)
    nonzeros = count(!=(0), M)
    density = nonzeros / total
    println("Dimensions: ", dims)
    println("Density: ", density)
end

function vector_norm_0(x::Vector{Int})
    norm_0 = 0
    for a in x
        if abs(a) > epsilon
            norm_0 += 1
        end
    end
    return norm_0
end

function objetive_value(data::Instance, x::Vector{Int}, v::Vector{Int})
    return data.values' * x - data.d' * v
end

function is_viable(data::Instance, x::Vector{Int})
    return data.weights' * x <= data.kS
end