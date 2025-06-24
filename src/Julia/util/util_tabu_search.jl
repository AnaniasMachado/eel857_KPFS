

function is_tabu(sol::Solution, tabu_list::Vector)
    return sol.solution in tabu_list
end

function add_item_tabu_ls(inst::Instance, sol::Solution, tabu_list::Vector)
    best_obj_val = compute_objetive_value(inst, sol)
    best_idx = -1
    for i in 1:inst.nI
        if (sol.solution[i] == 0)
            add_item(inst, sol, i)
            obj_val = compute_objetive_value(inst, sol)
            if (obj_val > best_obj_val) && (is_feasible(inst, sol)) && (!is_tabu(sol, tabu_list))
                best_obj_val = obj_val
                best_idx = i
            end
            remove_item(inst, sol, i)
        end
    end
    if (best_idx != -1)
        add_item(inst, sol, best_idx)
    end
end

function remove_item_tabu_ls(inst::Instance, sol::Solution, tabu_list::Vector)
    best_obj_val = compute_objetive_value(inst, sol)
    best_idx = -1
    for i in 1:inst.nI
        if (sol.solution[i] == 0)
            remove_item(inst, sol, i)
            obj_val = compute_objetive_value(inst, sol)
            if (obj_val > best_obj_val) && (is_feasible(inst, sol)) && (!is_tabu(sol, tabu_list))
                best_obj_val = obj_val
                best_idx = i
            end
            add_item(inst, sol, i)
        end
    end
    if (best_idx != -1)
        remove_item(inst, sol, best_idx)
    end
end

function swap_item_tabu_ls(inst::Instance, sol::Solution, tabu_list::Vector)
    best_obj_val = compute_objetive_value(inst, sol)
    best_idx_i = -1
    best_idx_j = -1
    for i in 1:inst.nI
        if (sol.solution[i] == 0)
            for j in (i+1):inst.nI
                if (sol.solution[j] == 1)
                    add_item(inst, sol, i)
                    remove_item(inst, sol, j)
                    obj_val = compute_objetive_value(inst, sol)
                    if (obj_val > best_obj_val) && (is_feasible(inst, sol)) && (!is_tabu(sol, tabu_list))
                        best_obj_val = obj_val
                        best_idx_i = i
                        best_idx_j = j
                    end
                    remove_item(inst, sol, i)
                    add_item(inst, sol, j)
                end
            end
        end
    end
    if (best_idx_i != -1)
        add_item(inst, sol, best_idx_i)
        remove_item(inst, sol, best_idx_j)
    end
end