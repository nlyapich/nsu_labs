#ifndef _SRC_GRAPH_H_
#define _SRC_GRAPH_H_

#include <stdio.h>
#include <limits.h>

typedef unsigned short _vertex;
typedef unsigned int _edge;

static const unsigned int kInfinity = ~((unsigned int)0);
static const unsigned int kBigNum = (unsigned int)INT_MAX + 1;

typedef struct {
	_edge* distance_to_vertices;
	_vertex* parent;
	_vertex* count_ways;
} Path;

typedef struct Graph Graph;

size_t fscanf_vertex(FILE* fin, _vertex* vertex);
void fprintf_vertex(FILE* fout, _vertex vertex);
size_t fscanf_edge(FILE* fin, _edge* edge);
void fprintf_edge(FILE* fout, _edge edge);

Graph* create_graph(_vertex vertex_count);
void add_edge(Graph* graph, _vertex begin_edge, _vertex end_edge, _edge weight);
_edge get_weight_edge(const Graph* graph, _vertex from, _vertex to);
void del_graph(Graph* graph);
Path* dijkstra(const Graph* graph, _vertex from, _vertex to);
void del_path(Path* path);

#endif  // _SRC_GRAPH_H_
