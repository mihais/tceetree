/**
 *
 * Copyright 2017 Mihai Serban <mihai.serban@gmail.com>
 *
 * SPDX short identifier: MIT
 */

#ifndef _RB_TREE_H
#define _RB_TREE_H

#ifndef offsetof
#define offsetof(st, m) ((size_t) & (((st *)0)->m))
#endif

#define container_of(ptr, type, member) ({ \
    const typeof( ((type *)0)->member) *_mptr = (ptr); \
    (type *)((char*)_mptr - offsetof(type, member)); })

#define RB_TREE_RED 0
#define RB_TREE_BLACK 1

typedef struct rb_tree rb_tree_t;
typedef struct rb_node rb_node_t;
typedef int (*rb_compare)(void *data, rb_node_t *node);

struct rb_tree {
    rb_node_t *node;
    rb_compare insert_cmp;
    rb_compare find_cmp;
};

#define RB_TREE_INIT(cmp1, cmp2) \
    (rb_tree_t) { NULL, cmp1, cmp2, }

struct rb_node {
    unsigned char color;
    rb_node_t *parent;
    rb_node_t *left;
    rb_node_t *right;
};

#define rb_color(node) (((node)->color) & 1)
#define rb_is_black(node) (!rb_color(node))
#define rb_is_red(node) (!rb_is_black(node))

int rb_tree_add(rb_tree_t *tree, rb_node_t *node, void *data);
rb_node_t *rb_tree_find(rb_tree_t *tree, void *data);

rb_node_t *rb_first(rb_tree_t *t);
rb_node_t *rb_next(rb_node_t *node);

#endif //  _RB_TREE_H
