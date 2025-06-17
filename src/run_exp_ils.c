#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#include "types/item.h"
#include "types/forfeit_set.h"
#include "types/knapsack_instance.h"
#include "types/solution.h"
#include "util/util_instance.c"
#include "util/util_sol.c"
#include "./meta-heuristics/ils.c"

int main() {
    // Definições dos conjuntos S, CT, Sizes
    // Para exemplo, vamos usar alguns valores fixos para esses conjuntos
    int S[] = {1}; // Como um único cenário, pode modificar para mais
    int num_S = sizeof(S) / sizeof(S[0]);

    // Tipos de correlação
    const char* CT[] = {"correlated", "not_correlated", "uncorrelated"};
    int num_CT = sizeof(CT) / sizeof(CT[0]);

    // Sizes de 1 a 10
    int Sizes[10];
    for (int i = 0; i < 10; i++) {
        Sizes[i] = i + 1;
    }
    int num_Sizes = 10;

    // Arquivo para salvar resultados
    FILE *f = fopen("results.txt", "w");
    if (!f) {
        printf("Erro ao abrir arquivo para escrita.\n");
        return 1;
    }

    // Cabeçalho do arquivo
    fprintf(f, "Scenario,CorrelationType,Size,ObjectiveValue,ElapsedTime");

    // Loop principal
    for (int s_idx = 0; s_idx < num_S; s_idx++) {
        int scenario_id = S[s_idx];

        for (int ct_idx = 0; ct_idx < num_CT; ct_idx++) {
            const char* corr_type = CT[ct_idx];

            for (int sz_idx = 0; sz_idx < num_Sizes; sz_idx++) {
                int size = Sizes[sz_idx];

                // Montar o nome do arquivo de instância correspondente ao cenário, tamanho e tipo de correlação
                // Ajuste o caminho e nome conforme sua estrutura
                char filename[256];
                snprintf(filename, sizeof(filename), "../instances/scenario%d/%s/scenario%d_%d.txt", scenario_id, corr_type, scenario_id, size);

                KnapsackInstance instance;
                if (!loadInstance(filename, &instance)) {
                    printf("Falha ao carregar a instância: %s", filename);
                    continue; // Pula para o próximo caso
                }

                printf("Executando cenário %d, tipo %s, size %d", scenario_id, corr_type, size);

                int max_iterations = 100;
                int no_change_limit = 10;
                char type_improvement[3];
                strcpy(type_improvement, "fi");
                srand(time(NULL));

                struct timespec start, end;
                double elapsed;

                // Executar a heurística uma vez (ou mais, se desejar)
                clock_gettime(CLOCK_MONOTONIC, &start);
                Solution* sol = iterated_local_search(&instance, max_iterations, no_change_limit, type_improvement);
                clock_gettime(CLOCK_MONOTONIC, &end);

                elapsed = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;

                double obj_value = objective_value(sol);

                // Salvar resultados
                fprintf(f, "%d,%s,%d,%f,%f", scenario_id, corr_type, size, obj_value, elapsed);

                // Liberar memória
                free_solution(sol);
                freeInstance(&instance);
            }
        }
    }

    fclose(f);
    printf("Experimento concluído. Resultados salvos em results.txt");

    return 0;
}