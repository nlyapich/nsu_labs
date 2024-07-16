#define _CRT_NO_SECURE_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#include "graph.h"

#define INPUT_FILE "in.txt"
#define OUTPUT_FILE "out.txt"

void lab9(FILE* fin, FILE* fout){
	_vertex n, from, to;
	_edge m;
	if (!fscanf_vertex(fin, &n) || !fscanf_vertex(fin, &from) || !fscanf_vertex(fin, &to) || !fscanf_edge(fin, &m)){
		fprintf(fout, "bad number of lines\n");
		return;
	}
	if (n > 5000){
		fprintf(fout, "bad number of vertices\n");
		return;
	}
	if (n == 0 || from > n || to > n){
		fprintf(fout, "bad vertex\n");
		return;
	}

	if (m > (_edge)(n*(n-1)/2)){
		fprintf(fout, "bad number of edges\n");
		return;
	}
	if (from == 0 || to == 0){
		fprintf(fout, "no path\n");
		return;
	}

	Graph* graph = create_graph(n);
	if (!graph)	return;

	_vertex start_edge, end_edge;
	_edge weight_edge;

	for (_edge i = 0; i < m; i++){
		if (!fscanf_vertex(fin, &start_edge) || !fscanf_vertex(fin, &end_edge) || !fscanf_edge(fin, &weight_edge)){
			fprintf(fout, "bad number of lines\n");
			del_graph(graph);
			return;
		}
		if (start_edge > n || end_edge > n){
			fprintf(fout, "bad vertex\n");
			del_graph(graph);
			return;
		}
		if (weight_edge > INT_MAX){
			fprintf(fout, "bad length\n");
			del_graph(graph);
			return;
		}
		add_edge(graph, start_edge, end_edge, weight_edge);
	}
	
	Path* path = dijkstra(graph, from, to);
	del_graph(graph);
	if (!path) return;

	for (_vertex i  = 0; i < n; ++i){
		_edge len = path->distance_to_vertices[i];
		
		if (len == kInfinity){
			fprintf(fout, "oo ");
		}
		else if (len == kBigNum){
			fprintf(fout, "INT_MAX+ ");
		}
		else {
			fprintf_edge(fout, len);
		}
	}
	fprintf(fout, "\n");

	if ((!path->count_ways[to - 1] && m != 0 && from != to) || (m == 0 && from != to)){
		fprintf(fout, "no path\n");
	}
	else {
		if (path->distance_to_vertices[to - 1] >= kBigNum && path->count_ways[to - 1] >= 2){
			fprintf(fout, "overflow\n");
		}
		else {
			_vertex cur = to;

			_vertex* parent = path->parent;

			while (parent[cur - 1] != from){
				fprintf_vertex(fout, cur);
				cur = parent[cur - 1];
			}
			fprintf_vertex(fout, cur);
			fprintf_vertex(fout, from);

			fprintf(fout, "\n");
		}
	}
	
	del_path(path);
}

int main(void){
	FILE* fin;
	fin = fopen(INPUT_FILE, "r");
	if (!fin) return 0;
	
	FILE* fout;
	fout = fopen(OUTPUT_FILE, "w+");

	lab9(fin, fout);

	fclose(fin);
	fclose(fout);

	return 0;
}
