

function add_item_ls_fi(inst::Instance, sol::Solution)
    improved = 0
    while (true)
        old_obj_val = compute_objetive_value(inst, sol)
        arr = 1:inst.nI
        shuffle(arr)
        for i in 1:inst.nI
            idx = arr[i]
            if (sol.solution[idx] == 0)
                add_item(inst, sol, idx)
                obj_val = compute_objetive_value(inst, sol)
                if (obj_val > old_obj_val) && (is_feasible(inst, sol))
                    improved = 1
                    break
                else
                    remove_item(inst, sol, idx)
                end
            end
        end
        break
    end
    return improved
end

function remove_item_ls_fi(inst::Instance, sol::Solution)
    improved = 0
    while (true)
        old_obj_val = compute_objetive_value(inst, sol)
        arr = 1:inst.nI
        shuffle(arr)
        for i in 1:inst.nI
            idx = arr[i]
            if (sol.solution[idx] == 1)
                remove_item(inst, sol, idx)
                obj_val = compute_objetive_value(inst, sol)
                if (obj_val > old_obj_val) && (is_feasible(inst, sol))
                    improved_once = 1
                    improved = 1
                    break
                else
                    add_item(inst, sol, idx)
                end
            end
        end
        break
    end
    return improved
end

function swap_item_ls_fi(inst::Instance, sol::Solution)
    improved = 0
    while (true)
        old_obj_val = compute_objetive_value(inst, sol)
        arr = 1:inst.nI
        shuffle(arr)
        for i in 1:inst.nI
            idx_i = arr[i]
            if (sol.solution[idx_i] == 0)
                for j in (i+1):inst.nI
                    idx_j = arr[j]
                    if (sol.solution[idx_j] == 1)
                        add_item(inst, sol, idx_i)
                        remove_item(inst, sol, idx_j)
                        obj_val = compute_objetive_value(inst, sol)
                        if (obj_val > old_obj_val) && (is_feasible(inst, sol))
                            improved = 1
                            break
                        else
                            remove_item(inst, sol, idx_i)
                            add_item(inst, sol, idx_j)
                        end
                    end
                end
            end
        end
        break
    end
    return improved
end