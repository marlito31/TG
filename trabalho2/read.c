#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>
#include <limits.h>

#include "read.h"


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