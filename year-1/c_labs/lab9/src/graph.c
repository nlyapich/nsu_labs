#include "graph.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#include "priority_queue.h"

struct Graph {
	_edge* adj_matrix;
	_vertex vertex_count;
};

size_t fscanf_vertex(FILE* fin, _vertex* vertex){
	if (!fin) return 0;
	return fscanf(fin, "%hu", vertex);
}
void fprintf_vertex(FILE* fout, _vertex vertex){
	if (!fout) return;
	fprintf(fout, "%hu ", vertex);
}

size_t fscanf_edge(FILE* fin, _edge* edge){
	if (!fin) return 0;
	return fscanf(fin, "%u ", edge);
}
void fprintf_edge(FILE* fout, _edge edge){
	if (!fout) return;
	fprintf(fout, "%u ", edge);
}

static void set_null_args_graph(Graph* graph){
	if (!graph) return;

	graph->adj_matrix = NULL;
}
static void init_graph(Graph* graph, _vertex vertex_count){
	if (!graph) return;

	graph->vertex_count = vertex_count;
}
void del_graph(Graph* graph){
	if (!graph) return;

	if (graph->adj_matrix) free(graph->adj_matrix);
	free(graph);
}

Graph* create_graph(_vertex vertex_count){
	if (!vertex_count) return NULL;

	Graph* graph = (Graph*)malloc(sizeof(Graph));
	if (!graph)	return NULL;

	set_null_args_graph(graph);

	graph->adj_matrix = (_edge*)calloc(vertex_count * vertex_count, sizeof(_edge));
	if (!graph->adj_matrix) goto quit_graph;

	init_graph(graph, vertex_count);

	return graph;

	quit_graph:

	del_graph(graph);
	return NULL;
}

static int check_vertex(const Graph* graph, _vertex vertex){
	return (graph->vertex_count >= vertex);
}

static void set_edge(Graph* graph, _vertex from, _vertex to, _edge weight){
	graph->adj_matrix[graph->vertex_count * (from - 1) + to - 1] = weight;
	graph->adj_matrix[graph->vertex_count * (to - 1) + from - 1] = weight;
}
void add_edge(Graph* graph, _vertex begin_edge, _vertex end_edge, _edge weight){
	if (!graph) return;
	if (!check_vertex(graph, begin_edge) || !check_vertex(graph, end_edge)) return;

	set_edge(graph, begin_edge, end_edge, weight);
}
_edge get_weight_edge(const Graph* graph, _vertex from, _vertex to){
	if (!check_vertex(graph, from) || !check_vertex(graph, to)) return 0;
	
	return graph->adj_matrix[graph->vertex_count * (from - 1) + to - 1];
}

void del_path(Path* path){
	if (!path) return;

	if (path->distance_to_vertices) free(path->distance_to_vertices);
	if (path->parent) free(path->parent);
	if (path->count_ways) free(path->count_ways);
	free(path);
}
static void set_null_args_path(Path* path){
	if (!path) return;

	path->distance_to_vertices = NULL;
	path->parent = NULL;
	path->count_ways = NULL;
}
static void init_parent(_vertex* parent, _vertex vertex_count, _vertex from){
	if (!parent) return;

	for (_vertex i = 0; i < vertex_count; ++i){
		parent[i] = from;
	}
}
static void init_distance_to_vertices(_edge* distance_to_vertices, const Graph* graph, _vertex from){
	if (!distance_to_vertices || !graph) return;

	_vertex vertex_count = graph->vertex_count;

	for (_vertex i = 0; i < vertex_count; ++i){
		if (i == from - 1) continue;
		_edge weight = get_weight_edge(graph, from, i + 1);
		distance_to_vertices[i] = weight ? weight : kInfinity;
	}
}
static void init_count_ways(_vertex* count_ways, _vertex from){
	if (!count_ways) return;

	count_ways[from - 1] = 1;
}
static void init_path(Path* path, const Graph* graph, _vertex from){
	if (!path) return;

	init_parent(path->parent, graph->vertex_count, from);
	init_distance_to_vertices(path->distance_to_vertices, graph, from);
	init_count_ways(path->count_ways, from);
}
static Path* create_path(const Graph* graph, _vertex from){
	Path* path = (Path*)malloc(sizeof(Path));
	if (!path) return NULL;

	set_null_args_path(path);

	_vertex vertex_count = graph->vertex_count;

	path->distance_to_vertices = (_edge*)calloc(vertex_count, sizeof(_edge));
	if (!path->distance_to_vertices) goto quit_path;

	path->parent = (_vertex*)calloc(vertex_count, sizeof(_vertex));
	if (!path->parent) goto quit_path;

	path->count_ways = (_vertex*)calloc(vertex_count, sizeof(_vertex));
	if (!path->count_ways) goto quit_path;

	init_path(path, graph, from);

	return path;

	quit_path:

	del_path(path);
	return NULL;
}

static void init_pqueue(PQueue* pqueue, _edge* distance_to_vertices,_vertex vertex_count, _vertex from){
	if (!pqueue || !distance_to_vertices) return;

	for (_vertex i = 0; i < vertex_count; ++i){
		if (i == from - 1) continue;
		add_pq(pqueue, distance_to_vertices[i], i + 1);
	}
}

static void increase_count_ways(_vertex* count_ways, _vertex from, _vertex to){
	if (!count_ways) return;

	count_ways[from - 1] += count_ways[to - 1];
}
static void relax(Path* path, PQueue* pqueue, _vertex from, _vertex to, _edge weight){
	if (!pqueue || !path || !path->distance_to_vertices || !path->parent) return;

	increase_count_ways(path->count_ways, from, to);

	_edge* distance_to_vertices = path->distance_to_vertices;

	if (distance_to_vertices[to - 1] > distance_to_vertices[from - 1] + weight){
		if (distance_to_vertices[from - 1] >= kBigNum || weight >= kBigNum || distance_to_vertices[from - 1] + weight >= kBigNum)
			distance_to_vertices[to - 1] = kBigNum;
		else
			distance_to_vertices[to - 1] = distance_to_vertices[from - 1] + weight;
		path->parent[to - 1] = from;
		decrease_priority(pqueue, to, distance_to_vertices[to - 1]);
	}
}

Path* dijkstra(const Graph* graph, _vertex from, _vertex to){
	if (!graph) return NULL;
	if (!check_vertex(graph, from) || !check_vertex(graph, to)) return NULL; 

	Path* path = create_path(graph, from);
	if (!path) return NULL;

	PQueue* pqueue = create_pq(graph->vertex_count - 1);
	if (!pqueue){
		del_path(path);
		return NULL;
	}
	init_pqueue(pqueue, path->distance_to_vertices, graph->vertex_count, from);

	while (!empty_pq(pqueue)){
		_vertex cur = extract_min_pq(pqueue);

		for (_vertex j = 0; j < graph->vertex_count; ++j){
			if (cur == j + 1) continue;
			_edge weight = get_weight_edge(graph, cur, j + 1);
			if (weight) relax(path, pqueue, cur, j + 1, weight);
		}
	}

	del_pq(pqueue);

	return path;
}
