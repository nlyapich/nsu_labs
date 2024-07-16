#include "graph.h"

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#include "priority_queue.h"

struct Graph{
	_edge* adj_matrix;
	_vertex vertex_count;
	_edge edge_count;
	_edge recorded_edges;
};

Graph* create_graph(_vertex vertex_count, _edge edge_count){
	if (!vertex_count || !edge_count) return NULL;

	Graph* graph = (Graph*)malloc(sizeof(Graph));
	if (!graph)
		return NULL;

	graph->adj_matrix = (_edge*)calloc(vertex_count * vertex_count, sizeof(_edge));
	if (!graph->adj_matrix){
		free(graph);
		return NULL;
	}

	graph->vertex_count = vertex_count;
	graph->edge_count = edge_count;
	graph->recorded_edges = 0;

	return graph;
}

void add_edge(Graph* graph, _vertex begin_edge, _vertex end_edge, _edge weight){
	if (!graph) return;
	if (graph->vertex_count < begin_edge || graph->vertex_count < end_edge || graph->recorded_edges == graph->edge_count) return;
	*(graph->adj_matrix + graph->vertex_count * (begin_edge - 1) + end_edge - 1) = weight;
	*(graph->adj_matrix + graph->vertex_count * (end_edge - 1) + begin_edge - 1) = weight;
	graph->recorded_edges++;
}

void del_graph(Graph* graph){
	if (!graph) return;
	free(graph->adj_matrix);
	free(graph);
}

Edge* prim_alg(const Graph* graph, _edge* added_edges){
	if (!graph) return NULL;
	if (!added_edges) return NULL;
	if (*added_edges) *added_edges = 0;

	Edge* tree_edges = (Edge*)malloc(sizeof(Edge) * (graph->vertex_count - 1));
	if (!tree_edges) return NULL;	

	_edge* priority = (_edge*)malloc(sizeof(_edge) * graph->vertex_count);
	if (!priority){
		free(tree_edges);
		return NULL;
	}

	for (_vertex i = 0; i < graph->vertex_count; i++){
		priority[i] = (unsigned int)INT_MAX + 1;
	}

	_vertex* parent = (_vertex*)malloc(sizeof(_vertex) * graph->vertex_count);
	if (!parent){
		free(priority);
		free(tree_edges);
		return NULL;
	}
	for (_vertex i = 0; i < graph->vertex_count; i++){
		parent[i] = i + 1;
	}

	PQueue* pqueue = create_pq();
	if (!pqueue){
		free(parent);
		free(priority);
		free(tree_edges);
		return NULL;
	}
	for (_vertex i = 0; i < graph->vertex_count; i++){
		add_pq(pqueue, priority[i], i + 1);
	}

	_vertex random_v = 1; //number of random vertex
	priority[random_v - 1] = 0;
	decrease_priority(pqueue, random_v, priority[random_v]);
	parent[random_v - 1] = random_v;

	while (!empty_pq(pqueue)){
		_vertex cur = extract_min_pq(pqueue);
		priority[cur - 1] = 0;

		if (cur != parent[cur - 1]){
			(tree_edges + *added_edges)->begin = parent[cur - 1];
			(tree_edges + *added_edges)->end = cur;
			*added_edges += 1;
		}

		for (_vertex i = 0; i < graph->vertex_count; i++){
			_edge cur_weight = *(graph->adj_matrix + graph->vertex_count * (cur - 1) + i);
			if (!cur_weight) continue;

			if (priority[i] && cur_weight < priority[i]){
				parent[i] = cur;
				priority[i] = cur_weight;
				decrease_priority(pqueue, i + 1, priority[i]);
			}
		}
	}

	free(priority);
	free(parent);
	del_pq(pqueue);

	if (*added_edges != (_edge)(graph->vertex_count - 1)){
		*added_edges = 0;
		free(tree_edges);
		return NULL;
	}

	return tree_edges;
}
