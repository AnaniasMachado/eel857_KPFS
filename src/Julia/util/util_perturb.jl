

function add_item_perturb(inst::Instance, sol::Solution)
    upper_lim = round(Int, inst.nI * 0.05)
    lower_lim = 1
    num_items = rand(lower_lim:upper_lim)
    shuffle(sol.not_included)
    for i in 1:num_items
        idx = sol.not_included[i]
        add_item(inst, sol, idx)
        if (!is_feasible(inst, sol))
            remove_item(inst, sol, idx)
        end
    end
end

function remove_item_perturb(inst::Instance, sol::Solution)
    upper_lim = round(Int, inst.nI * 0.05)
    lower_lim = 1
    num_items = rand(lower_lim:upper_lim)
    shuffle(sol.not_included)
    for i in 1:num_items
        idx = sol.not_included[i]
        remove_item(inst, sol, idx)
    end
end

function flip_item_perturb(inst::Instance, sol::Solution)
    arr = 1:inst.nI
    upper_lim = round(Int, inst.nI * 0.05)
    lower_lim = 1
    num_items = rand(lower_lim:upper_lim)
    shuffle(arr)
    for i in 1:num_items
        idx = arr[i]
        if (sol.solution[idx] == 1)
            remove_item(inst, sol, idx)
        else
            add_item(inst, sol, idx)
            if (!is_feasible(inst, sol))
                remove_item(inst, sol, idx)
            end
        end
    end
end