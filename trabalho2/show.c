#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>
#include <limits.h>

#include "show.h"

void show_info_graph(int** matriz, int n) {
    int m;
    FILE *out = fopen("out.txt", "w");
    if (!out) {
        fprintf(stderr, "Erro ao abrir o arquivo de saída.\n");
        return;
    }

    int *grau = malloc(n * sizeof(int));
    if (!grau) {
        fprintf(stderr, "Erro de alocação de memória.\n");
        fclose(out);
        return;
    }

    int grau_total = 0;

    for (int i = 0; i < n; i++) {
        int aux = 0;
        for (int j = 0; j < n; j++) {
            aux += matriz[i][j]; 
        }
        grau[i] = aux;
        grau_total += aux;
    }

    m = grau_total / 2;

    fprintf(out, "# n = %d\n", n);
    fprintf(out, "# m = %d\n", m);

    for (int i = 0; i < n; i++) {
        fprintf(out, "%d %d\n", i + 1, grau[i]);
    }

    fclose(out);
    free(grau); // Libera a memória alocada para o vetor grau
}

void show_graph_as_list(int **matriz, int n) {
    FILE *out = fopen("lista.txt", "w");
    if (!out) {
        fprintf(stderr, "Erro ao abrir o arquivo lista.txt.\n");
        return;
    }

    fprintf(out, "Lista de adjacência:\n");
    for (int i = 0; i < n; i++) {
        fprintf(out, "%d ->", i + 1);
        for (int j = 0; j < n; j++) {
            if (matriz[i][j] == 1) {
                fprintf(out, " %d", j + 1);
            }
        }
        fprintf(out, "\n");
    }

    fclose(out);
}


void show_graph_as_matrix(int **matriz, int n) {
    FILE *out = fopen("matriz.txt", "w");
    if (!out) {
        fprintf(stderr, "Erro ao abrir o arquivo matriz.txt.\n");
        return;
    }

    fprintf(out, "Matriz de adjacência:\n");
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            fprintf(out, "%d ", matriz[i][j]);
        }
        fprintf(out, "\n");
    }

    fclose(out);
}

