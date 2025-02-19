#ifndef READ_H
#define READ_H

typedef struct{
    int vertex;
    int peso;
    struct Node* next;
} Node;

typedef struct {
    int n; // Número de vértices
    Node** adj_list;
} Graph;

Graph* read_graph(FILE* arq);
void free_graph(Graph* graph);
void add_edge(Graph* graph, int v1, int v2, int peso);
Graph* create_graph(int n);


#endif