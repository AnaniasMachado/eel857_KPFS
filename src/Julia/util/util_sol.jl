
function create_solution(inst::Instance)
    sol = Solution(
        zeros(inst.nI),
        Int[],
        1:inst.nI,
        0,
        0,
        0
    )
    return sol
end

function add_item(inst::Instance, sol::Solution, val::Int)
    sol.solution[val] = 1
    push!(sol.included, val)
    filter!(x -> x != val, sol.not_included)
    update_penalty(inst, sol)
end

function remove_item(inst::Instance, sol::Solution, val::Int)
    sol.solution[val] = 0
    push!(sol.not_included, val)
    filter!(x -> x != val, sol.included)
    update_penalty(inst, sol)
end

function is_feasible(inst::Instance, sol::Solution)
    return inst.weights' * sol.solution <= inst.kS
end

function compute_objetive_value(inst::Instance, sol::Solution)
    return inst.values' * sol.solution - sol.total_penalty
end

function update_penalty(inst::Instance, sol::Solution)
    t = inst.forfeit_matrix * sol.solution
    p = max.(t - inst.a, 0)
    sol.total_penalty = inst.d' * p
end

function build_initial_solution(inst::Instance)
    sol = create_solution(inst)
    arr = 1:inst.nI
    arr = shuffle(arr)
    upper_lim = round(Int, inst.nI * 0.7)
    lower_lim = round(Int, inst.nI * 0.1)
    num_items = rand(lower_lim:upper_lim)
    for i in 1:num_items
        old_obj_val = compute_objetive_value(inst, sol)
        idx = arr[i]
        add_item(inst, sol, idx)
        obj_val = compute_objetive_value(inst, sol)
        if (obj_val < old_obj_val)
            remove_item(inst, sol, idx)
        end
    end
    return sol
end

function build_initial_solution_grasp(inst::Instance)
    sol = create_solution(inst)
    capacity_threshold = 0.6
    while (inst.weights' * sol.solution <= capacity_threshold)
        best_obj = compute_objetive_value(inst, sol)
        best_idx = -1
        for idx in sol.not_included
            add_item(inst, sol, idx)
            if (compute_objetive_value(inst, sol) > best_idx) && (is_feasible(inst, sol))
                best_obj = compute_objetive_value(inst, sol)
                best_idx = idx
            end
            remove_item(inst, sol, idx)
        end
        if (best_idx != -1)
            return sol
        else
            add_item(inst, sol, best_idx)
        end
    end
    return sol
end