using Gurobi
using JuMP
using MathOptInterface
const MOI = MathOptInterface

function gurobi_solver(data::Instance, opt_tol::Float64=10^(-5), time_limit::Int64=7200)
    model = Model(Gurobi.Optimizer)

    @variable(model, x[1:data.nI], Bin)
    @variable(model, v[1:data.nS], Int)

    @objective(model, Max, data.values' * x - data.d' * v)

    @constraint(model, data.weights' * x .<= data.kS, base_name = "Capacity_")
    @constraint(model, data.forfeit_matrix * x - v .<= data.a, base_name = "Allowance_")
    @constraint(model, v .>= 0, base_name = "v_lowerlimit_")
    @constraint(model, v .<= data.c - data.a, base_name = "v_upperlimit_")

    set_attribute(model, "OptimalityTol", opt_tol)

    set_optimizer_attribute(model, "TimeLimit", time_limit)

    set_optimizer_attribute(model, "MemLimit", 8)

    set_optimizer_attribute(model, "LogToConsole", 0)

    # set_optimizer_attribute(inst.model, "LogFile", "gurobi_log.txt")

    optimize!(model)

    status = termination_status(model)
    if (status == MOI.OPTIMAL) || (status == MOI.MEMORY_LIMIT) || (status == MOI.TIME_LIMIT)
        x_star = [Int(round(value(x[i]))) for i in 1:data.nI]
        v_star = [Int(round(value(v[i]))) for i in 1:data.nS]
        obj_bound = MOI.get(model, MOI.ObjectiveBound())
        obj_value = MOI.get(model, MOI.ObjectiveValue())
        duality_gap = abs(obj_value - obj_bound)
        return x_star, v_star, duality_gap
    else
        throw(ErrorException("Model was not optimized successfully. Status Code: $status"))
    end
end