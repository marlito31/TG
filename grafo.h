#ifndef GRAFO_H
#define GRAFO_H

#include <stdio.h>
#include <stdlib.h>

void show_graph_as_matrix(int **matriz, int n);

void show_graph_as_list(int **matriz, int n);

void bfs(int **matriz, int inicio, int n);

int qtd_componentes(int **matriz, int n);

#endif
