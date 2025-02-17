#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>
#include <limits.h>

typedef struct Node {
    int vertex;
    int peso;
    struct Node* next;
} Node;

typedef struct Graph {
    int n; // Número de vértices
    Node** adj_list;
} Graph;

Graph* create_graph(int n) {
    Graph* graph = malloc(sizeof(Graph));
    graph->n = n;
    graph->adj_list = malloc(n * sizeof(Node*));

    for (int i = 0; i < n; i++) {
        graph->adj_list[i] = NULL;
    }

    return graph;
}

void add_edge(Graph* graph, int v1, int v2, int peso) {
    if (peso < 0) {
        fprintf(stderr, "Erro: Arestas com pesos negativos não são permitidas.\n");
        return;
    }
    
    Node* new_node = malloc(sizeof(Node));
    new_node->vertex = v2;
    new_node->peso = peso;
    new_node->next = graph->adj_list[v1];
    graph->adj_list[v1] = new_node;

    // Como o grafo é não-direcionado
    new_node = malloc(sizeof(Node));
    new_node->vertex = v1;
    new_node->peso = peso;
    new_node->next = graph->adj_list[v2];
    graph->adj_list[v2] = new_node;
}

void free_graph(Graph* graph) {
    for (int i = 0; i < graph->n; i++) {
        Node* temp = graph->adj_list[i];
        while (temp) {
            Node* to_free = temp;
            temp = temp->next;
            free(to_free);
        }
    }
    free(graph->adj_list);
    free(graph);
}

Graph* read_graph(FILE* arq) {
    int n, v1, v2, peso;
    if (fscanf(arq, "%d", &n) != 1) {
        fprintf(stderr, "Erro ao ler o número de vértices.\n");
        return NULL;
    }

    Graph* graph = create_graph(n);

    while (fscanf(arq, "%d %d %d", &v1, &v2, &peso) == 3) {
        if (peso < 0) {
            fprintf(stderr, "Erro: Peso negativo detectado na aresta (%d, %d) com peso %d.\n", v1, v2, peso);
            continue;
        }
        if (v1 > 0 && v1 <= n && v2 > 0 && v2 <= n) {
            add_edge(graph, v1 - 1, v2 - 1, peso);
        } else {
            fprintf(stderr, "Erro: vértices fora dos limites (1 a %d)\n", n);
        }
    }

    return graph;
}

void dijkstra(Graph* graph, int inicio, int fim) {
    int n = graph->n;
    int* dist = malloc(n * sizeof(int));
    int* parent = malloc(n * sizeof(int));
    bool* visited = calloc(n, sizeof(bool));
    FILE* out = fopen("dijkstra_output.txt", "w");

    if (!dist || !parent || !visited || !out) {
        fprintf(stderr, "Erro de alocação de memória ou ao abrir o arquivo.\n");
        free(dist);
        free(parent);
        free(visited);
        fclose(out);
        return;
    }

    // Inicializa as distâncias
    for (int i = 0; i < n; i++) {
        dist[i] = INT_MAX;
        parent[i] = -1;
    }
    dist[inicio - 1] = 0; // Distância do vértice de início é 0

    for (int count = 0; count < n - 1; count++) {
        int min = INT_MAX, u = -1;

        // Escolhe o vértice de menor distância ainda não visitado
        for (int v = 0; v < n; v++) {
            if (!visited[v] && dist[v] < min) {
                min = dist[v];
                u = v;
            }
        }

        if (u == -1) break; // Se não há mais caminhos possíveis, encerra

        visited[u] = true;

        // Atualiza as distâncias dos vizinhos de u
        Node* adj = graph->adj_list[u];
        while (adj) {
            int v = adj->vertex;
            int peso = adj->peso;
            if (!visited[v] && dist[u] != INT_MAX) {
                int novaDist = dist[u] + peso;
                if (novaDist < dist[v]) {
                    dist[v] = novaDist;
                    parent[v] = u;
                }
            }
            adj = adj->next;
        }
    }

    // Verifica se há caminho para 'fim'
    if (dist[fim - 1] == INT_MAX) {
        fprintf(out, "Não há caminho entre %d e %d.\n", inicio, fim);
    } else {
        fprintf(out, "Distância mínima de %d para %d: %d\n", inicio, fim, dist[fim - 1]);

        // Reconstrói o caminho
        fprintf(out, "Caminho: ");
        int caminho[n], tamanho = 0;
        for (int v = fim - 1; v != -1; v = parent[v]) {
            caminho[tamanho++] = v + 1;
        }
        for (int i = tamanho - 1; i >= 0; i--) {
            fprintf(out, "%d%s", caminho[i], i > 0 ? " -> " : "\n");
        }
    }

    fclose(out);
    free(dist);
    free(parent);
    free(visited);
}

// Função para mostrar o grafo como uma lista de adjacência
void show_graph_as_list(Graph* graph) {
    FILE* out = fopen("lista.txt", "w");
    if (!out) {
        fprintf(stderr, "Erro ao abrir o arquivo lista.txt.\n");
        return;
    }

    fprintf(out, "Lista de adjacência:\n");
    for (int i = 0; i < graph->n; i++) {
        fprintf(out, "%d ->", i + 1);
        Node* adj = graph->adj_list[i];
        while (adj) {
            fprintf(out, " %d (%d)", adj->vertex + 1, adj->peso);
            adj = adj->next;
        }
        fprintf(out, "\n");
    }

    fclose(out);
}

int main() {
    FILE* arq = fopen("trab2grafo_4.txt", "r");
    if (!arq) {
        fprintf(stderr, "Erro ao abrir o arquivo.\n");
        return 1;
    }

    Graph* graph = read_graph(arq);
    fclose(arq);

    dijkstra(graph, 1, 10);
    show_graph_as_list(graph);

    // Libera a memória
    free_graph(graph);

    return 0;
}
