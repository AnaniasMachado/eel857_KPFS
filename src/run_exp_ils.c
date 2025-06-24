// #include <stdio.h>
// #include <stdlib.h>
// #include <time.h>
// #include <string.h>

// #include "types/item.h"
// #include "types/forfeit_set.h"
// #include "types/knapsack_instance.h"
// #include "types/solution.h"
// #include "util/util_instance.c"
// #include "util/util_sol.c"
// #include "./meta-heuristics/ils.c"

// int main() {
//     // Definições dos conjuntos S, CT, Sizes
//     // Para exemplo, vamos usar alguns valores fixos para esses conjuntos
//     int S[] = {1, 2, 3, 4}; // Como um único cenário, pode modificar para mais
//     int num_S = sizeof(S) / sizeof(S[0]);

//     // Tipos de correlação
//     const char* CT[] = {"correlated", "not_correlated", "uncorrelated"};
//     int num_CT = sizeof(CT) / sizeof(CT[0]);

//     // Sizes de 1 a 10
//     int Sizes[] = {300, 500, 700, 800, 1000};
//     int num_Sizes = sizeof(Sizes) / sizeof(Sizes[0]);

//     // Arquivo para salvar resultados
//     FILE *f = fopen("results.txt", "w");
//     if (!f) {
//         printf("Erro ao abrir arquivo para escrita.\n");
//         return 1;
//     }

//     // Cabeçalho do arquivo
//     fprintf(f, "Scenario,CorrelationType,Size,InstIdx,ObjectiveValue,ElapsedTime");

//     // Loop principal
//     for (int s_idx = 0; s_idx < num_S; s_idx++) {
//         int scenario_id = S[s_idx];

//         for (int ct_idx = 0; ct_idx < num_CT; ct_idx++) {
//             const char* corr_type = CT[ct_idx];

//             for (int sz_idx = 0; sz_idx < num_Sizes; sz_idx++) {
//                 int size = Sizes[sz_idx];

//                 char filename[256];
//                 snprintf(filename, sizeof(filename), "../instances/scenario%d/%s/%d/idx_.txt", scenario_id, corr_type, size, inst_idx);

//                 KnapsackInstance instance;
//                 if (!loadInstance(filename, &instance)) {
//                     printf("Falha ao carregar a instância: %s\n", filename);
//                     return 1;
//                 }

//                 for (int inst_idx = 0; inst_idx < 10; inst_idx++) {
//                     printf("Executando cenário %d, tipo %s, size %d, idx %d\n", scenario_id, corr_type, size, inst_idx);

//                     int max_iterations = 100;
//                     int no_change_limit = 10;
//                     char type_improvement[3];
//                     strcpy(type_improvement, "fi");
//                     srand(time(NULL));

//                     struct timespec start, end;
//                     double elapsed;

//                     // Executar a heurística uma vez (ou mais, se desejar)
//                     clock_gettime(CLOCK_MONOTONIC, &start);
//                     Solution* sol = iterated_local_search(&instance, max_iterations, no_change_limit, type_improvement);
//                     clock_gettime(CLOCK_MONOTONIC, &end);

//                     elapsed = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;

//                     int obj_value = objective_value(sol);

//                     // Salvar resultados
//                     fprintf(f, "%d,%s,%d,%d,%f\n", scenario_id, corr_type, size, inst_idx, obj_value, elapsed);

//                     // Liberar memória
//                     free_solution(sol);
//                 }
//                 freeInstance(&instance);
//             }
//         }
//     }

//     fclose(f);
//     printf("Experimento concluído. Resultados salvos em results.txt");

//     return 0;
// }

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#include "types/item.h"
#include "types/forfeit_set.h"
#include "types/knapsack_instance.h"
#include "types/solution.h"
#include "types/data_instance.h"
#include "util/util_instance.c"
#include "util/util_sol.c"
#include "./meta-heuristics/ils.c"
#include "run_ils.c"

void write_line(FILE *f, int scenario_id, const char *corr_type, int size, int inst_idx, DataInst *data_inst, int i) {
    fprintf(f, "%d,%s,%d,%d,%d, %.6f", scenario_id, corr_type, size, inst_idx, data_inst->obj_vals[i], data_inst->elapseds[i]);
}

int main() {
    // Definições dos conjuntos S, CT, Sizes
    // Para exemplo, vamos usar alguns valores fixos para esses conjuntos
    int S[] = {1, 2, 3, 4}; // Como um único cenário, pode modificar para mais
    int num_S = sizeof(S) / sizeof(S[0]);

    // Tipos de correlação
    const char* CT[] = {"correlated", "not_correlated", "uncorrelated"};
    int num_CT = sizeof(CT) / sizeof(CT[0]);

    // Sizes de 1 a 10
    int Sizes[] = {300, 500, 700, 800, 1000};
    int num_Sizes = sizeof(Sizes) / sizeof(Sizes[0]);

    // Arquivo para salvar resultados
    FILE *f = fopen("results.txt", "w");
    if (!f) {
        printf("Erro ao abrir arquivo para escrita.\n");
        return 1;
    }

    // Cabeçalho do arquivo
    fprintf(f, "Scenario,CorrelationType,Size,InstIdx,ObjectiveValue,ElapsedTime\n");

    // Loop principal
    for (int s_idx = 0; s_idx < num_S; s_idx++) {
        int scenario_id = S[s_idx];

        for (int ct_idx = 0; ct_idx < num_CT; ct_idx++) {
            const char* corr_type = CT[ct_idx];

            for (int sz_idx = 0; sz_idx < num_Sizes; sz_idx++) {
                int size = Sizes[sz_idx];

                for (int inst_idx = 1; inst_idx <= 10; inst_idx++) {

                    // printf("Executando cenário %d, tipo %s, size %d, idx %d\n", scenario_id, corr_type, size, inst_idx);

                    int max_iterations = 100;
                    int no_change_limit = 10;
                    char type_improvement[3];
                    strcpy(type_improvement, "fi");
                    srand(time(NULL));

                    DataInst *data_inst = run_ils(f, scenario_id, corr_type, size, inst_idx);

                    // printf("Point N0\n");

                    // Salvar resultados
                    // for (int i = 0; i < 2; i++) {
                    //     printf("Obj val: %d\n", data_inst->obj_vals[i]);
                    //     printf("Elapsed: %f\n", data_inst->elapseds[i]);
                    //     // fprintf(f, "%d,%s,%d,%d,%d, %.6f\n", scenario_id, corr_type, size, inst_idx, data_inst->obj_vals[i], data_inst->elapseds[i]);
                    //     write_line(f, scenario_id, corr_type, size, inst_idx, data_inst, i);
                    // }

                    char buffer[200];

                    // Usa snprintf para evitar estouro de buffer
                    // for (int i = 0; i < 2; i++) {
                    //     // printf("Obj val: %d\n", data_inst->obj_vals[i]);
                    //     // printf("Elapsed: %f\n", data_inst->elapseds[i]);
                    //     snprintf(buffer, sizeof(buffer), "%d,%s,%d,%d,%d, %.6f\n", scenario_id, corr_type, size, inst_idx, data_inst->obj_vals[i], data_inst->elapseds[i]);
                    //     printf("%s", buffer);
                    // }

                    // printf("Point N1\n");

                    // Liberar memória
                    // free_solution(sol);
                    free(data_inst->obj_vals);
                    // printf("Point N3\n");
                    free(data_inst->elapseds);
                    // printf("Point N4\n");
                    free(data_inst);
                    // printf("Point N5\n");
                    fclose(f);
                    return 0;
                }
            }
        }
    }

    fclose(f);
    // printf("Experimento concluído. Resultados salvos em results.txt");

    return 0;
}