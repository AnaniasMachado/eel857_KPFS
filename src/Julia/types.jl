struct Instance
    nI::Int                     # Number of items
    nS::Int                     # Number of forfeit sets
    kS::Int                     # Capacity
    values::Vector{Int}         # Values of items
    weights::Vector{Int}        # Weight of items
    forfeit_matrix::Matrix{Int} # Forfeit set matrix
    a::Vector{Int}              # Vector of allowances
    d::Vector{Int}              # Vector of penalties
    c::Vector{Int}              # Vector of forfeit sets sizes
end

mutable struct Solution
    solution::Vector{Int}
    included::Vector{Int}
    not_included::Vector{Int}
    total_value::Int
    total_weight::Int
    total_penalty::Int
end