/**
 *
 * Copyright 2017 Mihai Serban <mihai.serban@gmail.com>
 *
 * SPDX short identifier: MIT
 */

#ifndef _SYM_TREE_H
#define _SYM_TREE_H

#include "rbtree.h"

typedef struct symtree symtree_t;
typedef struct symtree_node symtree_node_t;
typedef struct symtree_list symtree_list_t;

struct symtree_list {
    struct symtree_list *next;
    struct symtree_node *symbol;
};

struct symtree_node {
    rb_node_t node;
    char *funname;
    char *filename;
    int outdone;
    int subtreeoutdone;
    int isroot;
    int icolor;
    struct symtree_list *parents;
    struct symtree_list *children;
};

struct symtree {
    struct rb_tree root;
};

void symtree_init(symtree_t *stree);
void symtree_free(symtree_t *stree);
symtree_node_t *symtree_first(symtree_t *stree);
symtree_node_t *symtree_next(symtree_node_t *node);

symtree_node_t *symtree_add(symtree_t *stree, char *funname, char *filename);
symtree_node_t *symtree_find(symtree_t *stree, char *funname, char *filename);
void symtree_node_add_branch(symtree_node_t *caller, symtree_node_t *callee);

#endif //  _SYM_TREE_H
