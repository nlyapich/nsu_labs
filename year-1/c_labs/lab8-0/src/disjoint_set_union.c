#include "disjoint_set_union.h"

#include <stdlib.h>

struct DSU{
	_set* parent;
	_set* rank;
};

DSU* make_sets(_set finite_set){
	DSU* dsu = (DSU*)malloc(sizeof(DSU));
	if (!dsu) return NULL;
	dsu->parent = (_set*)malloc(sizeof(_set) * finite_set);
	if (!dsu->parent){
		free(dsu);
		return NULL;
	}
	dsu->rank = (_set*)calloc(finite_set, sizeof(_set));
	if (!dsu->rank){
		free(dsu->parent);
		free(dsu);
		return NULL;
	}
	for (_set i = 0; i < finite_set; i++){
		dsu->parent[i] = i + 1;
	}
	return dsu;
}

_set find_set(DSU* dsu, _set x){
	if (x == dsu->parent[x - 1])
		return x;
	dsu->parent[x - 1] = find_set(dsu, dsu->parent[x - 1]);
	return dsu->parent[x - 1];
}

void merge_sets(DSU* dsu, _set first_set, _set second_set){
	if (dsu->rank[first_set - 1] > dsu->rank[second_set - 1]){
		dsu->parent[second_set - 1] = first_set;
		return;
	}
	dsu->parent[first_set - 1] = second_set;
	if (dsu->rank[first_set - 1] == dsu->rank[second_set - 1])
		dsu->rank[second_set - 1]++;
}

void del_dsu(DSU* dsu){
	if (!dsu) return;
	free(dsu->parent);
	free(dsu->rank);
	free(dsu);
}
