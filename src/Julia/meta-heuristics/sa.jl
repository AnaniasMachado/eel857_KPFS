

function simulated_annealing(inst::Instance, max_iter::Int, T::Float64, t_0::Float64, alpha::Float64, type_improvement::String)
    init_sol = build_initial_solution(inst)
    sol_prime = create_solution(inst)
    sol_star = create_solution(inst)
    neighborhood_funcs = [add_item_neighborhood, remove_item_neighborhood, flip_item_neighborhood]
    neighborhood_count = 3
    iter = 0
    t = t_0
    while (t > T)
        while (iter < max_iter)
            iter += 1
            sol_prime = deepcopy(init_sol)
            old_obj_val = compute_objetive_value(inst, init_sol)
            r = rand(1:neighborhood_count)
            neighborhood_funcs[r](inst, init_sol)
            delta = compute_objetive_value(inst, init_sol) - old_obj_val
            if (delta >= 0)
                if (compute_objetive_value(inst, sol_prime) > compute_objetive_value(inst, sol_star))
                    rvnd(inst, sol_prime, type_improvement)
                    sol_star = deepcopy(sol_prime)
                end
            elseif rand(0:1) < exp(-delta / t)
            else
                init_sol = deepcopy(sol_prime)
            end
        end
        t = alpha * t
        iter = 0
    end
    return sol_star
end