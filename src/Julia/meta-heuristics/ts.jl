

function tabu_search(inst::Instance, max_iter::Int, tabu_size::Int, no_change_limit::Int, type_improvement::String)
    init_sol = build_initial_solution(inst)
    sol_star = create_solution(inst)
    old_obj_val = compute_objetive_value(inst, init_sol)
    no_change = 0
    tabu_list = []
    push!(tabu_list, init_sol->solution)
    tabu_ls_funcs = [add_item_tabu_ls, remove_item_tabu_ls, swap_item_tabu_ls]
    tabu_ls_count = 3
    rvnd(inst, init_sol, type_improvement)
    for k in 1:max_iter
        r = rand(1:tabu_ls_count)
        tabu_ls_funcs[r](inst, init_sol, tabu_list)
        if (compute_objetive_value(inst, init_sol) > compute_objetive_value(inst, sol_star))
            sol_star = deepcopy(init_sol)
        end
        push!(tabu_list, init_sol->solution)
        if (length(tabu_list) > tabu_size)
            popfirst!(tabu_list)
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