#include "graph.h"

#include <stdlib.h>

#include "bool_matrix.h"

typedef enum Color{
	kWhite,
	kGrey,
	kBlack,
} Color;

struct Graph{
	Bool_Matrix* adj;
	_vertex vertex_count;
};

struct Graph* create_graph(_vertex vertex_count){
	struct Graph* graph = (struct Graph*)malloc(sizeof(struct Graph));
	if (!graph) return NULL;

	graph->adj = create_matrix(vertex_count, vertex_count);
	if (!graph->adj){
		free(graph);
		return NULL;
	}

	graph->vertex_count = vertex_count;

	return graph;
}

void add_edge(struct Graph* graph, _vertex begin_edge, _vertex end_edge){
	if (!graph) return;
	if (begin_edge > graph->vertex_count || end_edge > graph->vertex_count) return;
	set_value(graph->adj, begin_edge - 1, end_edge - 1, 1);
}

void del_graph(struct Graph* graph){
	delete_matrix(graph->adj);
	free(graph);
}

static void reverse(_vertex* sort_list, _vertex count_list){
	if (!sort_list) return;
	for (_vertex i = 0; i < (_vertex)(count_list/2); i++){
		_vertex tmp = *(sort_list + i);
		*(sort_list + i) = *(sort_list + count_list - i - 1);
		*(sort_list + count_list - i - 1) = tmp;
	}
}

static void search(_vertex vertex, const struct Graph* graph, Color* color, _vertex* sort_list, _vertex* count_list, unsigned char* have_cycle){
	if (!*have_cycle) return;
	*(color + vertex - 1) = kGrey;

	_vertex cur_vertex = 1;
	_vertex vertex_count = graph->vertex_count;

	while(cur_vertex <= vertex_count){
		if (get_value(graph->adj, vertex - 1, cur_vertex - 1) == 1){
			if (*(color + cur_vertex - 1) == kWhite){
				search(cur_vertex, graph, color, sort_list, count_list, have_cycle);
			}
			if (*(color + cur_vertex - 1) == kGrey){ //if cycle graph
				*have_cycle = 0;
				return;
			}
		}
		cur_vertex++;
	}
	*(color + vertex - 1) = kBlack;
	*(sort_list + *count_list) = vertex;
	*count_list += 1;
}

_vertex* topological_sort(const struct Graph* graph){
	_vertex vertex_count = graph->vertex_count;
	_vertex* sort_list = (_vertex*)malloc(sizeof(_vertex) * vertex_count);
	if (!sort_list) return NULL;
	_vertex count_list = 0;


	Color* color = (Color*)malloc(sizeof(Color) * vertex_count);
	if (!color){
		free(sort_list);
		return NULL;
	}
	for (_vertex i = 0; i < vertex_count; i++){
		*(color + i) = kWhite;
	}

	unsigned char have_cycle = 1;
	for (_vertex vertex = 1; vertex <= vertex_count; vertex++){
		if (*(color + vertex - 1) == kWhite) search(vertex, graph, color, sort_list, &count_list, &have_cycle);
		if (!have_cycle) break;
	}

	free(color);
	
	if (!have_cycle){
		free(sort_list);
		return NULL;
	}
	
	reverse(sort_list, count_list);
	return sort_list;
}
