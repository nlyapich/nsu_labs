#ifndef _SRC_GRAPH_H_
#define _SRC_GRAPH_H_

typedef unsigned short _vertex;
typedef unsigned int _edge;
typedef struct Graph Graph;

Graph* create_graph(_vertex vertex_count);
void add_edge(Graph* graph, _vertex begin_edge, _vertex end_edge);
void del_graph(Graph* graph);
_vertex* topological_sort(const Graph* graph);

#endif // _SRC_GRAPH_H_
