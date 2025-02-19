#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>
#include <limits.h>

#include "dijkstra.h"
#include "read.h"

void dijkstra(Graph* graph, int inicio, int fim,FILE* out) {
    int n = graph->n;
    int* dist = malloc(n * sizeof(int));
    int* parent = malloc(n * sizeof(int));
    bool* visited = calloc(n, sizeof(bool));
    
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
    fprintf(out,"\n");

    free(dist);
    free(parent);
    free(visited);
}