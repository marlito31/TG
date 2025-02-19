#ifndef SEARCH_H
#define SEARCH_H

typedef struct queue_t {
    int front;
    int back;
    int size;
    int capacity;
    int *queue;
} queue_t;

int calculate_diameter(int **matriz, int n);
void calculate_max_distances(int **matriz, int n);
int bfs_max_distance(int **matriz, int inicio, int n);
void componentes_tamanhos(int **matriz, int n, int *maior, int *menor);
void dfs(int **matriz, int inicio, int n);
int qtd_componentes(int **matriz, int n);
void bfs(int **matriz, int inicio,int n);
bool queue_empty(queue_t *q);
int queue_size(queue_t *q);
int queue_front(queue_t *q);
void queue_pop(queue_t *q);
void queue_push(queue_t *q, int data);
void queue_delete(queue_t **q);
void queue_initialize(queue_t **q);

#endif