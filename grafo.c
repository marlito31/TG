#include "grafo.h"

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
    printf("Matriz de adjacência salva no arquivo matriz.txt.\n");
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
    printf("Lista de adjacência salva no arquivo lista.txt.\n");
}

void bfs(int **matriz, int inicio, int n) {
    if (inicio <= 0 || inicio > n) {
        fprintf(stderr, "Erro: índice inicial fora dos limites.\n");
        return;
    }

    inicio -= 1;
    int *visited = calloc(n, sizeof(int));
    int *parent = calloc(n, sizeof(int));
    int *level = calloc(n, sizeof(int));
    FILE *out = fopen("bfs_output.txt", "w");
    
    if (!visited || !parent || !level || !out) {
        fprintf(stderr, "Erro de alocação de memória ou ao abrir o arquivo.\n");
        free(visited);
        free(parent);
        free(level);
        fclose(out);
        return;
    }

    for (int i = 0; i < n; i++) {
        parent[i] = -1;
        level[i] = -1;
    }

    int *queue = malloc(n * sizeof(int));
    if (!queue) {
        fprintf(stderr, "Erro de alocação de memória.\n");
        free(visited);
        free(parent);
        free(level);
        fclose(out);
        return;
    }

    int front = 0, rear = 0;
    queue[rear++] = inicio;
    visited[inicio] = 1;
    level[inicio] = 0;

    while (front < rear) {
        int v = queue[front++];
        for (int i = 0; i < n; i++) {
            if (matriz[v][i] == 1 && !visited[i]) {
                visited[i] = 1;
                parent[i] = v;
                level[i] = level[v] + 1;
                queue[rear++] = i;
            }
        }
    }

    fprintf(out, "Vértice Pai Nível\n");
    for (int i = 0; i < n; i++) {
        fprintf(out, "%d       %d   %d\n", i + 1, parent[i] == -1 ? -1 : parent[i] + 1, level[i]);
    }

    fclose(out);
    free(queue);
    free(visited);
    free(parent);
    free(level);
}

int qtd_componentes(int **matriz, int n) {
    int *visited = calloc(n, sizeof(int));
    if (!visited) {
        fprintf(stderr, "Erro de alocação de memória.\n");
        return 0;
    }

    void dfs_count(int v) {
        visited[v] = 1;
        for (int i = 0; i < n; i++) {
            if (matriz[v][i] == 1 && !visited[i]) {
                dfs_count(i);
            }
        }
    }

    int componentes = 0;
    for (int i = 0; i < n; i++) {
        if (!visited[i]) {
            componentes++;
            dfs_count(i);
        }
    }

    free(visited);
    return componentes;
}
