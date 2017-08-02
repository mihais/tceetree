/**
 *
 * Copyright 2017 Mihai Serban <mihai.serban@gmail.com>
 *
 * SPDX short identifier: MIT
 */

#include <string.h>
#include <stdlib.h>

#include "symtree.h"

static int cmp_symtree_note_t(symtree_node_t *l, symtree_node_t *r, int ignore_filename)
{
    int cmp;

    if (!l)
        return 1;
    if (!r)
        return -1;

    cmp = strcmp(l->funname, r->funname);
    if (cmp < 0)
        return -1;
    else if (cmp > 0)
        return 1;

    // functions are the same, check filenames
    if (l->filename == NULL && r->filename == NULL)
        return 0;
    if (l->filename == NULL) {
        // when no filename is provided return the first node that matches the
        // function name only.
        if (ignore_filename)
            return 0;
        else
            return 1;
    }
    if (r->filename == NULL)
        return -1;

    return strcmp(l->filename, r->filename);
}

static int symtree_insert_cmp(void *data, rb_node_t *node)
{
    symtree_node_t *l = (symtree_node_t*)data;
    symtree_node_t *r = container_of(node, symtree_node_t, node);

    return cmp_symtree_note_t(l, r, 0);
}

static int symtree_find_cmp(void *data, rb_node_t *node)
{
    symtree_node_t *l = (symtree_node_t*)data;
    symtree_node_t *r = container_of(node, symtree_node_t, node);

    return cmp_symtree_note_t(l, r, 1);
}

static symtree_node_t *alloc_node(char *funname, char *filename)
{
    symtree_node_t *node = (symtree_node_t*)calloc(1, sizeof(symtree_node_t));
    if (!node)
        return NULL;

    node->funname = strdup(funname);
    if (!node->funname)
        goto err;
    if (filename) {
        node->filename = strdup(filename);
        if (!node->filename)
            goto err;
    }

    return node;

err:
    if (node->funname)
        free(node->funname);
    free(node);

    return NULL;
}

static symtree_list_t *list_add(symtree_list_t *list, symtree_node_t *node)
{
    symtree_list_t *l = (symtree_list_t*)malloc(sizeof(symtree_list_t));
    if (!l)
        return list;

    l->symbol = node;
    l->next = list;

    return l;
}

static void list_free(symtree_list_t *list)
{
    symtree_list_t* l = list;
    while (l) {
        l = l->next;
        if (list->symbol)
            free(list->symbol);
        free(list);
        list = l;
    }
}

void free_node(symtree_node_t *node)
{
    if (node) {
        if (node->funname)
            free(node->funname);
        if (node->filename)
            free(node->filename);
        if (node->parents)
            list_free(node->parents);
        if (node->children)
            list_free(node->children);
    }
}

void symtree_init(struct symtree *stree)
{
    stree->root = RB_TREE_INIT(symtree_insert_cmp, symtree_find_cmp);
}

void symtree_free(struct symtree *stree)
{
    // TODO;
}

symtree_node_t *symtree_add(symtree_t *stree, char *funname, char *filename)
{
    symtree_node_t *symbol = alloc_node(funname, filename);
    if (!symbol)
        return NULL;

    if (-1 == rb_tree_add(&stree->root, &symbol->node, symbol)) {
        // no duplicates
        free_node(symbol);
        return NULL;
    }

    return symbol;
}

symtree_node_t *symtree_find(symtree_t *stree, char *funname, char *filename)
{
    rb_node_t *rbnode;
    symtree_node_t symbol = {
        .funname = funname,
        .filename = filename,
    };

    if (!funname)
        return NULL;

    rbnode = rb_tree_find(&(stree->root), &symbol);

    return container_of(rbnode, symtree_node_t, node);
}

void symtree_node_add_branch(symtree_node_t *caller, symtree_node_t *callee)
{
    caller->children = list_add(caller->children, callee);
    callee->parents = list_add(callee->parents, caller);
}

symtree_node_t *symtree_first(symtree_t *stree)
{
    rb_node_t *rbnode = rb_first(&stree->root);
    if (rbnode)
        return container_of(rbnode, symtree_node_t, node);

    return NULL;
}

symtree_node_t *symtree_next(symtree_node_t *node)
{
    rb_node_t *rbnode = rb_next(&node->node);
    if (rbnode)
        return container_of(rbnode, symtree_node_t, node);

    return NULL;
}
