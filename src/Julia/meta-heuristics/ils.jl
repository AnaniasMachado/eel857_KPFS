

function iterated_local_search(inst::Instance, max_iter::Int, no_change_limit::Int, type_improvement::String)
    init_sol = build_initial_solution(inst)
    sol_star = create_solution(inst)
    old_obj_val = compute_objetive_value(inst, init_sol)
    no_change = 0
    perturbation_funcs = [add_item_perturb, remove_item_perturb, flip_item_perturb]
    perturbation_count = 3
    rvnd(inst, init_sol, type_improvement)
    for k in 1:max_iter
        r = rand(1:perturbation_count)
        perturbation_funcs[r](inst, init_sol)
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