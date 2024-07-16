#ifndef _SRC_GRAPH_H_
#define _SRC_GRAPH_H_

typedef unsigned short _vertex;
typedef unsigned int _edge;

typedef struct {
	_vertex begin;
	_vertex end;
} Edge;

typedef struct Graph Graph;

Graph* create_graph(_vertex vertex_count, _edge edge_count);
void add_edge(Graph* graph, _vertex begin_edge, _vertex end_edge, _edge weight);
void del_graph(Graph* graph);
Edge* prim_alg(const Graph* graph, _edge* added_edges);

#endif  // _SRC_GRAPH_H_
