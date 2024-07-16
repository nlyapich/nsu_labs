#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>

#include "graph.h"

#define INPUT_FILE "in.txt"
#define OUTPUT_FILE "out.txt"

void lab7(FILE* fin, FILE* fout){
	_vertex n;
	_edge m;
	if (!fscanf(fin, "%hu", &n) || fscanf(fin, "%u", &m) != 1){
		fprintf(fout, "bad number of lines\n");
// 		printf("bad number of lines\n");
		return;
	}
	if (n > 2000){
		fprintf(fout, "bad number of vertices\n");
// 		printf("bad number of vertices\n");
		return;
	}

	if (m > (_edge)(n*(n-1)/2)){
		fprintf(fout, "bad number of edges\n");
// 		printf("bad number of edges\n");
		return;
	}

	Graph* graph = create_graph(n);
	if (!graph) return;
	_vertex start_edge, end_edge;

	for (_edge i = 0; i < m; i++){
		if (fscanf(fin, "%hu %hu", &start_edge, &end_edge) != 2){
			fprintf(fout, "bad number of lines\n");
// 			printf("bad number of lines\n");
			del_graph(graph);
			return;
		}
		if (start_edge > n || end_edge > n){
			fprintf(fout, "bad vertex\n");
// 			printf("bad vertex\n");
			del_graph(graph);
			return;
		}
		add_edge(graph, start_edge, end_edge);
	}

	_vertex* sort_list = topological_sort(graph);
	if (!sort_list){
		fprintf(fout, "impossible to sort\n");
// 		printf("impossible to sort\n");
		del_graph(graph);
		return;
	}
	
	for (_vertex i = 0; i < n; i++){
		fprintf(fout, "%u ", *(sort_list + i));
// 		printf("%u ", *(sort_list + i));
	}
	fprintf(fout, "\n");
// 	printf("\n");

	del_graph(graph);
	free(sort_list);
}

int main(void){
	FILE* fin;
	fin = fopen(INPUT_FILE, "r");
	if (!fin) return 0;
	
	FILE* fout;
	fout = fopen(OUTPUT_FILE, "w+");

	lab7(fin, fout);

	fclose(fin);
	fclose(fout);

	return 0;
}
