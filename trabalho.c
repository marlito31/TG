#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>

int** read_graph(FILE *arq, int n) {
    int v1, v2;

    // Aloca dinamicamente as linhas da matriz de adjacências
    int **matriz = calloc(n, sizeof(int*));

    // Aloca dinamicamente as colunas da matriz de adjacências
    for (int i = 0; i < n; i++) {
        matriz[i] = calloc(n, sizeof(int));
    }

    // Coloca 1 na matriz de acordo com as adjacências do vértice
    while (fscanf(arq, "%d %d", &v1, &v2) != EOF) {
        if (v1 > 0 && v1 <= n && v2 > 0 && v2 <= n) { // Verifica limites
            matriz[v1 - 1][v2 - 1] = 1;
            matriz[v2 - 1][v1 - 1] = 1;
        } else {
            fprintf(stderr, "Erro: vértices fora dos limites (1 a %d)\n", n);
        }
    }

    return matriz;
}

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

void show_graph_as_list(int **matriz, int n){
    FILE *arq = fopen("lista.txt","w");

    for (int i = 0; i < n; ++i) {
        fprintf(arq,"Vertice %d:", i+1);
        for (int j = 0; j < n; ++j) {
            if (matriz[i][j] == 1) {
                fprintf(arq," %d", j+1);
            }
        }
        fprintf(arq,"\n");
    }
    
}

void show_graph_as_matrix(int **matriz,int n){
    FILE *arq = fopen("matriz.txt","w");

    fprintf(arq,"  ");
    for (int j = 0; j < n; j++) {
        fprintf(arq,"%5d ", j+1);
    }
    fprintf(arq,"\n");

    for (int i = 0; i < n; i++) {
        fprintf(arq,"%d ",i+1);
        for (int j = 0; j < n; j++) {
            fprintf(arq,"%5d ", matriz[i][j]);
        }
        fprintf(arq,"\n");
    }
    fclose(arq);
}

typedef struct queue_t {
    int front;
    int back;
    int size;
    int capacity;
    int *queue;
} queue_t;

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
    
    if (inicio < 0 || inicio >= n) {
        fprintf(stderr, "Erro: índice inicial ou destino fora dos limites.\n");
        return;
    }

    inicio -= 1;

    int *visited = calloc (n,sizeof(int));

    queue_t *q;
    queue_initialize(&q);
    queue_push(q,inicio);

    visited[inicio] = 1;

    while (!queue_empty(q))
    {
        int v = queue_front(q);
        queue_pop(q);
        printf("%d ", v+1);

        for (int i = 0; i < n; i++)
        {
            if (matriz[v][i] == 1 && visited[i] == 0)
            {
                queue_push(q,i);
                visited[i] = 1;
            }
            
        }
        
    }
    printf("\n");
    
    queue_delete(&q);

    free(visited);


}

void qtd_componentes(int **matriz, int n){

}


int main(int argc, char* argv[]) {
    
    FILE *arq = fopen("in.txt", "r");
    if (!arq) {
        fprintf(stderr, "Erro ao abrir o arquivo %s.\n", argv[1]);
        return 1;
    }

    int n;
    if (fscanf(arq, "%d", &n) != 1 || n <= 0) {
        fprintf(stderr, "Erro ao ler o número de vértices.\n");
        fclose(arq);
        return 1;
    }

    int **matriz_adjacencia = read_graph(arq, n);
    fclose(arq);

    bfs(matriz_adjacencia,3,n);

    //show_info_graph(matriz_adjacencia, n);

    //show_graph_as_list(matriz_adjacencia,n);

    // Libera a memória alocada para a matriz
    for (int i = 0; i < n; i++) {
        free(matriz_adjacencia[i]);
    }
    free(matriz_adjacencia);

    return 0;
}
