#define _CRT_NO_SECURE_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#include "graph.h"

#define INPUT_FILE "in.txt"
#define OUTPUT_FILE "out.txt"

void lab8(FILE* fin, FILE* fout){
	_vertex n;
	_edge m;
	if (!fscanf(fin, "%hu", &n) || fscanf(fin, "%u", &m) != 1){
		fprintf(fout, "bad number of lines\n");
		return;
	}
	if (n > 5000){
		fprintf(fout, "bad number of vertices\n");
		return;
	}

	if (m > (_edge)(n*(n-1)/2)){
		fprintf(fout, "bad number of edges\n");
		return;
	}
	if (n != 1 && m == 0){
		fprintf(fout, "no spanning tree\n");
		return;
	}

	Graph* graph = create_graph(n, m);
	if (!graph)	return;

	_vertex start_edge, end_edge;
	_edge len_edge;

	for (_edge i = 0; i < m; i++){
		if (fscanf(fin, "%hu %hu %u", &start_edge, &end_edge, &len_edge) != 3){
			fprintf(fout, "bad number of lines\n");
			del_graph(graph);
			return;
		}
		if (start_edge > n || end_edge > n){
			fprintf(fout, "bad vertex\n");
			del_graph(graph);
			return;
		}
		if (len_edge > INT_MAX){
			fprintf(fout, "bad length\n");
			del_graph(graph);
			return;
		}
		add_edge(graph, start_edge, end_edge, len_edge);
	}
	_edge added_edges = 0;
	Edge* tree_edges = kruskal_alg(graph, &added_edges);

	if (!tree_edges){
		fprintf(fout, "no spanning tree\n");
		del_graph(graph);
		return;
	}

	for (_edge i = 0; i < added_edges; i++){
		fprintf(fout, "%hu %hu\n", (tree_edges + i)->begin, (tree_edges + i)->end);
	}

	free(tree_edges);
	del_graph(graph);
}

int main(void){
	FILE* fin;
	fin = fopen(INPUT_FILE, "r");
	if (!fin) return 0;
	
	FILE* fout;
	fout = fopen(OUTPUT_FILE, "w+");

	lab8(fin, fout);

	fclose(fin);
	fclose(fout);


	return 0;
}
