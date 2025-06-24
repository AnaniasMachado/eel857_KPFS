using MAT

# Open file
matfile = matopen("../../solutions/ILS/scenario1/correlated_sc1/300/grb_ils_kpfs_1_i1")

# List variables
println("Variáveis no arquivo: ", keys(matfile))

# Read a specific variable
dados = read(matfile, "sol")

# Close file
close(matfile)

# Read a key of variable
println(dados["included"])