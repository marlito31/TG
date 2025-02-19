#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>
#include <limits.h>

#include "read.h"
#include "dijkstra.h"
#include "show.h"
#include "search.h"


int main() {
    FILE* arq = fopen("trab2grafo_1.txt", "r");
    if (!arq) {
        fprintf(stderr, "Erro ao abrir o arquivo.\n");
        return 1;
    }

    Graph* graph = read_graph(arq);
    fclose(arq);

    int v[] = {10,100,1000,10000};

    FILE* out = fopen("dijkstra_output.txt", "w");

    for (int i = 0; i < 4 ; i++)
    {
        dijkstra(graph, 1, v[i], out);
    }
    
    fclose(out);

    // Libera a memória
    free_graph(graph);

    return 0;
}