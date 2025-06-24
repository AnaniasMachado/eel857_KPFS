

function greedy_randomized_adaptive_search_procedure(inst::Instance, max_iter::Int, no_change_limit::Int, type_improvement::String)
    sol_star = create_solution(inst)
    old_obj_val = compute_objetive_value(inst, sol_star)
    no_change = 0
    for k in 1:max_iter
        init_sol = build_initial_solution_grasp(inst)
        rvnd(inst, init_sol, type_improvement)
        if (compute_objetive_value(inst, init_sol) > compute_objetive_value(inst, sol_star))
            sol_star = deepcopy(init_sol)
        end
        if (compute_objetive_value(inst, sol_star) == old_obj_val)
            no_change += 1
        else
            no_change = 0
        end
        old_obj_val = compute_objetive_value(inst, sol_star)
        if (no_change >= no_change_limit)
            return sol_star
        end
    end
    return sol_star
end