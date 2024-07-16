#ifndef _SRC_DISJOINT_SET_UNION_H_
#define _SRC_DISJOINT_SET_UNION_H_

typedef unsigned int _set;

typedef struct DSU DSU;

DSU* make_sets(_set finite_set);
_set find_set(DSU* dsu, _set x);
void merge_sets(DSU* dsu, _set first_set, _set second_set);
void del_dsu(DSU* dsu);

#endif  // _SRC_DISJOINT_SET_UNION_H_
