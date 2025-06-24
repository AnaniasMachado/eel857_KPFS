

function rvnd(inst::Instance, sol::Solution, type_improvement::String)
    local_search_funcs = [add_item_ls_fi, remove_item_ls_fi, swap_item_ls_fi]
    local_search_count = 3
    shuffle(local_search_funcs)
    while (true)
        improved = 0
        for i in 1:local_search_count
            ls_func = local_search_funcs[i]
            if (ls_func(inst, sol) == 1)
                improved = 1
                break
            end
        end
        if (improved == 0)
            break
        end
    end
end