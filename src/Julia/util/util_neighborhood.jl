

function add_item_neighborhood(inst::Instance, sol::Solution)
    m = length(sol.not_included)
    if (m != 0)
        shuffle(sol.not_included)
        for i in 1:m
            idx = sol.not_included[i]
            add_item(inst, sol, idx)
            if (!is_feasible(inst, sol))
                remove_item(inst, sol, idx)
            else
                return
            end
        end
    end
end

function remove_item_neighborhood(inst::Instance, sol::Solution)
    m = length(sol.included)
    if (m != 0)
        shuffle(sol.included)
        idx = sol.included[1]
        remove_item(inst, sol, idx)
    end
end

function flip_item_neighborhood(inst::Instance, sol::Solution)
    arr = 1:inst.nI
    shuffle(arr)
    for i in 1:inst.nI
        idx = arr[i]
        if (sol.solution[idx] == 1)
            remove_item(inst, sol, idx)
            return
        else
            add_item(inst, sol, idx)
            if (!is_feasible(inst, sol))
                remove_item(inst, sol, idx)
            else
                return
            end
        end
    end
end