#include "graph.h"

#include <stdlib.h>

#include "disjoint_set_union.h"
#include "heapsort.h"

struct Graph{
	Edge* list_edges;
	_vertex vertex_count;
	_edge edge_count;
	_edge recorded_edges;
};

Graph* create_graph(_vertex vertex_count, _edge edge_count){
	if (!vertex_count || !edge_count) return NULL;

	Graph* graph = (Graph*)malloc(sizeof(Graph));
	if (!graph)
		return NULL;

	graph->list_edges = (Edge*)malloc(sizeof(Edge) * edge_count);
	if (!graph->list_edges){
		free(graph);
		return NULL;
	}

	graph->vertex_count = vertex_count;
	graph->edge_count = edge_count;
	graph->recorded_edges = 0;

	return graph;
}

static void init_edge(Edge* edge, _vertex begin_edge, _vertex end_edge, _edge weight){
	if (!edge) return;
	edge->begin = begin_edge;
	edge->end = end_edge;
	edge->weight = weight;
}

void add_edge(Graph* graph, _vertex begin_edge, _vertex end_edge, _edge weight){
	if (!graph) return;
	if (graph->vertex_count < begin_edge || graph->vertex_count < end_edge || graph->recorded_edges == graph->edge_count) return;
	init_edge(graph->list_edges + graph->recorded_edges, begin_edge, end_edge, weight);
	graph->recorded_edges++;
}

void del_graph(Graph* graph){
	if (!graph) return;
	free(graph->list_edges);
	free(graph);
}

static int edge_comparator(const void* a, const void* b){
	Edge* a_edge = (Edge*)a;
	Edge* b_edge = (Edge*)b;
	if (a_edge->weight > b_edge->weight) return 1;
	if (a_edge->weight < b_edge->weight) return -1;
	return 0;
}

Edge* kruskal_alg(const Graph* graph, _edge* added_edges){
	if (!added_edges) return NULL;
	if (*added_edges) *added_edges = 0;

	DSU* dsu = make_sets(graph->vertex_count);
	if (!dsu) return NULL;

	heapsort(graph->list_edges, graph->recorded_edges, sizeof(Edge), edge_comparator);

	Edge* tree_edges = (Edge*)malloc(sizeof(Edge) * (graph->vertex_count - 1));
	if (!tree_edges) return NULL;

	for (_edge i = 0; i < graph->recorded_edges; i++){
		_set begin_set = find_set(dsu, (graph->list_edges + i)->begin);
		_set end_set = find_set(dsu, (graph->list_edges + i)->end);
		if (begin_set != end_set){
			*(tree_edges + *added_edges) = *(graph->list_edges + i);
			*added_edges += 1;
			merge_sets(dsu, begin_set, end_set);
		}
	}

	del_dsu(dsu);

	if (*added_edges != (_edge)(graph->vertex_count - 1)){
		*added_edges = 0;
		free(tree_edges);
		return NULL;
	}

	return tree_edges;
}
