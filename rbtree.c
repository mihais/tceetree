/**
 *
 * Copyright 2017 Mihai Serban <mihai.serban@gmail.com>
 *
 * SPDX short identifier: MIT
 */

#include <assert.h>
#include <stdlib.h>

#include "rbtree.h"

//#define NDEBUG

static rb_node_t *__insert(rb_tree_t *root, rb_node_t *node, void *data)
{
    int c;
    rb_node_t **x = &(root->node);
    rb_node_t *parent = NULL;

    while (*x) {
        c = root->insert_cmp(data, *x);
        if (c == 0)
            return NULL; // no duplicates
        parent = *x;
        if (c < 0)
            x = &((*x)->left);
        else
            x = &((*x)->right);
    }

    node->left = node->right = NULL;
    node->parent = parent;
    node->color = RB_TREE_RED;
    *x = node;

    return *x; // node
}

static void __rotate_left(rb_tree_t *tree, rb_node_t *x)
{
    rb_node_t *right = x->right;

    assert(x->right); // rotate on left because we have a right child

    x->right = right->left;
    if (x->right)
        x->right->parent = x;

    right->left = x;

    right->parent = x->parent;
    if (x->parent) {
        if (x == x->parent->left)
            right->parent->left = right;
        else
            right->parent->right = right;
    }

    x->parent = right;

    if (x == tree->node)
        tree->node = right;
}

static void __rotate_right(rb_tree_t *tree, rb_node_t *x)
{
    rb_node_t *left = x->left;

    assert(x->left); // rotate on right because we have a left child

    x->left = left->right;
    if (x->left)
        x->left->parent = x;

    left->right = x;

    left->parent = x->parent;
    if (x->parent) {
        if (x == x->parent->left)
            left->parent->left = left;
        else
            left->parent->right = left;
    }

    x->parent = left;

    if (x == tree->node)
        tree->node = left;
}

int rb_tree_add(rb_tree_t *tree, rb_node_t *node, void *data)
{
    rb_node_t *x = __insert(tree, node, data);
    rb_node_t *parent, *gparent, *uncle;

    if (!x)
        return -1;

    while (x != tree->node && rb_color(x) == RB_TREE_RED) {

        parent = x->parent;
        if (!parent)
            break;
        gparent = parent->parent;
        if (!gparent)
            break;

        // on left
        if (parent == gparent->left) {
            uncle = gparent->right;
            if (uncle && rb_color(uncle) == RB_TREE_RED) {
                // case 1: recolor, recurse on gparent
                gparent->color = RB_TREE_RED;
                parent->color = RB_TREE_BLACK;
                uncle->color = RB_TREE_BLACK;
                x = gparent;
            } else {                      // uncle is black
                if (x == parent->right) { // left rotate on parent, zig-zag --> zig-zig, new x becomes parent
                    // case 2
                    __rotate_left(tree, parent);

                    // prepare for case 3 below
                    x = parent;
                    parent = x->parent;
                    gparent = parent->parent;
                }

                // case 3: black->red->red on left, fix with right rotation on
                // gparent and recolor
                __rotate_right(tree, gparent);

                parent->color = RB_TREE_BLACK;
                x->color = RB_TREE_RED; // already red but does not matter
                gparent->color = RB_TREE_RED;

                x = parent; // this will end because color(parent)=black
            }
        } else { // on right, mirror the left side cases
            uncle = gparent->left;
            if (uncle && rb_color(uncle) == RB_TREE_RED) {
                // case 1: recolor, recurse on gparent
                gparent->color = RB_TREE_RED;
                parent->color = RB_TREE_BLACK;
                uncle->color = RB_TREE_BLACK;
                x = gparent;
            } else {                     // uncle is black
                if (x == parent->left) { // right rotate on parent, zig-zag --> zig-zig, new x becomes parent
                    // case 2
                    __rotate_right(tree, parent);

                    // prepare for case 3 below
                    x = parent;
                    parent = x->parent;
                    gparent = parent->parent;
                }

                // case 3: black->red->red on right, fix with left rotation on
                // gparent and recolor
                __rotate_left(tree, gparent);

                parent->color = RB_TREE_BLACK;
                x->color = RB_TREE_RED; // already red but does not matter
                gparent->color = RB_TREE_RED;

                x = parent; // this will end because color(parent)=black
            }
        }
    }

    // root is black
    tree->node->color = RB_TREE_BLACK;

    return 0;
}

rb_node_t *rb_tree_find(rb_tree_t *tree, void *data)
{
    rb_node_t *x = tree->node;
    int c;

    while (x) {
        c = tree->find_cmp(data, x);
        if (c == 0)
            return x;
        if (c < 1)
            x = x->left;
        else
            x = x->right;
    }

    return NULL;
}

rb_node_t *rb_first(rb_tree_t *t)
{
    rb_node_t *node = t->node;

    if (!node)
        return NULL;

    while (node->left)
        node = node->left;

    return node;
}

rb_node_t *rb_next(rb_node_t *node)
{
    rb_node_t *x;

    // get left-most node from right child
    if (node->right) {
        x = node->right;
        while (x->left)
            x = x->left;
        return x;
    }

    // go up to the first parent that has me on its left leaf
    while ((x = node->parent) && x->right == node)
        node = x;

    return x;
}
