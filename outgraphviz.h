/*
 * This source code is released for free distribution under the terms of the MIT License (MIT):
 *
 * Copyright (c) 2014, Fabio Visona'
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#ifndef _OUTGRAPHVIZ_H
#define _OUTGRAPHVIZ_H

#ifndef _ALL_IN_ONE
#include "symtree.h"
#include "ttreeparam.h"
#endif // _ALL_IN_ONE

int outopen_gra(symtree_t *ptree, treeparam_t *pparam);
int outclose_gra(symtree_t *ptree, treeparam_t *pparam);
int outnode_gra(symtree_node_t *pnode, treeparam_t *pparam);
int outbranch_gra(symtree_node_t *caller, symtree_node_t *callee, treeparam_t *pparam, int reverse_direction);

#endif // #ifndef _OUTGRAPHVIZ_H
