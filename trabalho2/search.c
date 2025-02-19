#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>
#include <limits.h>

#include "search.h"

void queue_initialize(queue_t **q) {
    (*q) = malloc(sizeof(queue_t));
    (*q)->front = 0;
    (*q)->back = 3;
    (*q)->size = 0;
    (*q)->capacity = 4;
    (*q)->queue = malloc(sizeof(int) * 4);
}

void queue_delete(queue_t **q) {
    free((*q)->queue);
    free(*q);
    *q = NULL;
}

void queue_push(queue_t *q, int data) {
    if (q->size == q->capacity) {
        int old_capacity = q->capacity;
        q->capacity *= 2;
        q->queue = realloc(q->queue, sizeof(int) * q->capacity);
        if (q->front > q->back) {
            for (int i = q->front; i < old_capacity; i++) {
                q->queue[i + old_capacity] = q->queue[i];
            }
            q->front = q->front + old_capacity;
        }
    }
    q->back++;
    if (q->back == q->capacity)
        q->back = 0;
    q->queue[q->back] = data;
    q->size++;
}

void queue_pop(queue_t *q) {
    assert(q->size > 0);
    if (q->size == q->capacity / 4 && q->capacity > 4) {
        int new_capacity = q->capacity / 2;
        if (q->front <= q->back) {
            for (int i = q->front, j = 0; i <= q->back; i++, j++) {
                q->queue[j] = q->queue[i];
            }
        } else {
            int front_len = q->capacity - q->front;
            for (int i = q->back; i >= 0; i--) {
                q->queue[i + front_len] = q->queue[i];
            }
            for (int i = q->front, j = 0; i < q->capacity; i++, j++) {
                q->queue[j] = q->queue[i];
            }
        }
        q->front = 0;
        q->back = q->size - 1;
        q->capacity = new_capacity;
        q->queue = realloc(q->queue, q->capacity * sizeof(int));
    }
    q->front++;
    q->size--;
    if (q->front == q->capacity)
        q->front = 0;
}

int queue_front(queue_t *q) {
    assert(q->front < q->capacity);
    return q->queue[q->front];
}

int queue_size(queue_t *q) {
    return q->size;
}

bool queue_empty(queue_t *q) {
    return queue_size(q) == 0;
}


void bfs(int **matriz, int inicio,int n){
    
    if (inicio <= 0 || inicio >= n) {
        fprintf(stderr, "Erro: índice inicial ou destino fora dos limites.\n");
        return;
    }

    inicio -= 1; // Ajusta para índice 0 baseado
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

    // Inicializa arrays
    for (int i = 0; i < n; i++) {
        parent[i] = -1; // -1 indica que não há pai
        level[i] = -1;  // -1 indica que não foi visitado
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

    // Salva resultados no arquivo
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


void dfs(int **matriz, int inicio, int n) {
    if (inicio <= 0 || inicio > n) {
        fprintf(stderr, "Erro: índice inicial fora dos limites.\n");
        return;
    }

    inicio -= 1;
    int *visited = calloc(n, sizeof(int));
    int *parent = calloc(n, sizeof(int));
    int *level = calloc(n, sizeof(int));
    FILE *out = fopen("dfs_output.txt", "w");
    
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

    void dfs_recursive(int v, int current_level) {
        visited[v] = 1;
        level[v] = current_level;
        
        for (int i = 0; i < n; i++) {
            if (matriz[v][i] == 1 && !visited[i]) {
                parent[i] = v;
                dfs_recursive(i, current_level + 1);
            }
        }
    }

    dfs_recursive(inicio, 0);

    fprintf(out, "Vértice Pai Nível\n");
    for (int i = 0; i < n; i++) {
        fprintf(out, "%d       %d   %d\n", i + 1, parent[i] == -1 ? -1 : parent[i] + 1, level[i]);
    }

    fclose(out);
    free(visited);
    free(parent);
    free(level);
}

void componentes_tamanhos(int **matriz, int n, int *maior, int *menor) {
    int *visited = calloc(n, sizeof(int));
    if (!visited) {
        fprintf(stderr, "Erro de alocação de memória.\n");
        return;
    }

    int componente_tamanho;
    *maior = 0;
    *menor = n;

    void dfs_count_size(int v, int *size) {
        visited[v] = 1;
        (*size)++;
        for (int i = 0; i < n; i++) {
            if (matriz[v][i] == 1 && !visited[i]) {
                dfs_count_size(i, size);
            }
        }
    }

    for (int i = 0; i < n; i++) {
        if (!visited[i]) {
            componente_tamanho = 0;
            dfs_count_size(i, &componente_tamanho);

            if (componente_tamanho > *maior) {
                *maior = componente_tamanho;
            }
            if (componente_tamanho < *menor) {
                *menor = componente_tamanho;
            }
        }
    }

    free(visited);
}

int bfs_max_distance(int **matriz, int inicio, int n) {
    inicio -= 1; // Ajusta para índice baseado em 0
    int *visited = calloc(n, sizeof(int));
    int *level = calloc(n, sizeof(int));

    if (!visited || !level) {
        fprintf(stderr, "Erro de alocação de memória.\n");
        free(visited);
        free(level);
        return -1;
    }

    for (int i = 0; i < n; i++) {
        level[i] = -1; // Inicializa níveis como não visitados
    }

    int *queue = malloc(n * sizeof(int));
    if (!queue) {
        fprintf(stderr, "Erro de alocação de memória.\n");
        free(visited);
        free(level);
        return -1;
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
                level[i] = level[v] + 1;
                queue[rear++] = i;
            }
        }
    }

    // Encontra o maior nível (maior distância)
    int max_distance = 0;
    for (int i = 0; i < n; i++) {
        if (level[i] > max_distance) {
            max_distance = level[i];
        }
    }

    free(queue);
    free(visited);
    free(level);
    return max_distance;
}

void calculate_max_distances(int **matriz, int n) {
    int max_global_distance = 0;

    for (int i = 1; i <= n; i++) {
        int max_distance = bfs_max_distance(matriz, i, n);
        printf("Maior distância a partir do vértice %d: %d\n", i, max_distance);
        if (max_distance > max_global_distance) {
            max_global_distance = max_distance;
        }
    }

    printf("Maior distância global: %d\n", max_global_distance);
}


int calculate_diameter(int **matriz, int n) {
    int max_diameter = 0; // Inicializa o diâmetro como 0
    int *dist = calloc(n, sizeof(int)); // Array para armazenar distâncias

    if (!dist) {
        fprintf(stderr, "Erro de alocação de memória.\n");
        return -1;
    }

    // Função para executar a BFS e retornar a maior distância
    int bfs_distance(int start) {
        for (int i = 0; i < n; i++) {
            dist[i] = -1; // -1 indica que o vértice ainda não foi visitado
        }

        dist[start] = 0;
        int *queue = malloc(n * sizeof(int));
        int front = 0, rear = 0;

        if (!queue) {
            fprintf(stderr, "Erro de alocação de memória.\n");
            free(dist);
            return -1;
        }

        queue[rear++] = start;
        int max_dist = 0;

        while (front < rear) {
            int v = queue[front++];
            for (int i = 0; i < n; i++) {
                if (matriz[v][i] == 1 && dist[i] == -1) {
                    dist[i] = dist[v] + 1;
                    max_dist = (dist[i] > max_dist) ? dist[i] : max_dist;
                    queue[rear++] = i;
                }
            }
        }

        free(queue);
        return max_dist;
    }

    // Calcula o maior caminho mínimo para cada vértice
    for (int i = 0; i < n; i++) {
        int local_max = bfs_distance(i);
        if (local_max > max_diameter) {
            max_diameter = local_max;
        }
    }

    free(dist);
    return max_diameter;
}
