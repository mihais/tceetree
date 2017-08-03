[![Build Status](https://travis-ci.org/mihais/tceetree.svg?branch=master)](https://travis-ci.org/mihais/tceetree)
<a href="https://scan.coverity.com/projects/mihais-tceetree">
  <img alt="Coverity Scan Build Status"
       src="https://scan.coverity.com/projects/13370/badge.svg"/>
</a>

This is a fork of the original project hosted on SourceForge:
* https://sourceforge.net/projects/tceetree/

The purpose of the project is generating a function call tree for a software
application written in C. This utility takes as input an uncompressed CScope
(http://cscope.sourceforge.net/) output file.
With a few options, an output DOT language file can be generated.
The DOT file may be used as input for Graphviz (http://www.graphviz.org/)
to make an image of the function call tree.

To compile with MinGW (windows) or gcc (linux, cygwin) just do:

```
cc -o tceetree tceetree.c gettree.c  outgraphviz.c  outtree.c  rbtree.c  slib.c  symtree.c
```

Only the C standard library is needed, so you can adapt this to any other
compiler of your choice.

Follow these steps to quickly get a graphical representation of the function
call tree from any C application:

* Install CScope and Graphviz
    * In Ubuntu issue `sudo apt install graphviz cscope`
* Go to your sources root directory and do:
    * under Windows: `dir /B /S *.c > cscope.files`
    * under Linux: `find . -name '*.c' > cscope.files`
That will recurse subdirectories and list all C files in `cscope.files`.
* Execute `cscope -b -c`. This builds the cross reference only, without
compression from the files listed in `cscope.files`.
* Run `tceetree` with cscope.out as input (default) to get tceetree.out (DOT
language representation of function call tree).
* Execute `dot -Tpng -O tceetree.out` to get a graphical representation of the
tree in `tceetree.out.png`.
* Open the PNG file with your image viewer.
    * In Ubuntu use `shotwell tceetree.out.png`

There are a lot of options you can specify when invoking dot to customize
the graph, for example:

* you can change output format e.g. to pdf:
    * `dot -Tpdf -O tceetree.out`
* you may modify layout from left to right instead of top to bottom:
    * `dot -Grankdir=LR -Tpng -O tceetree.out`
* you may modify graph, nodes and edges font:
    * `dot -Gfontname=Helvetica -Nfontname=Helvetica -Efontname=Helvetica -Tpng -O tceetree.out`

See dot.1.pdf and dotguide.pdf within Graphviz documentation for many other options.
You may wish to give a try to one of the other tools provided by Graphviz
instead of dot (see dot.1.pdf for a list).

This is the synopsis of tceetree:

```
tceetree [-c <depth>] [-C <depth>] [-d <file>] [-f] [-F] [-h] [-i <file>]
		 [-o <file>] [-p <function>] [-r <root>] [-s <style>] [-v] [-V] [-x <function>]

Option	Description
-c <depth>	Depth of tree for called functions: default is max. Depth is
			measured starting from root(s) function(s).
-C <depth>	Depth of tree for calling functions: default is 0. Depth is
			measured starting from root(s) function(s) in backward direction.
-d <file>	Output a shortened cscope output file: default is no output.
			The shortened file includes only function information and can be
			used as input (-i) for following calls to tceetree to increase speed
			on big projects.
-f	Print the file name where the call is near to branch.
-F	Group functions into one cluster for each source file.
-h	Print help.
-i <file>	Input cscope output file: default is cscope.out.
-o <file>	Output file for graphviz: default is tceetree.out.
-p <function>	Highlight call path till function. Path starts from root(s)
				till the specified function (only one), in backward or forward
				direction.
-r <root>	Root function of tree: default is main. This option may occur more
			than once for multiple roots (max 5).
-s <style>	Style for highlight call path:
			0 = red color (default)
			1 = blue color
			2 = green color
			3 = bold
			4 = dashed
			5 = dotted
-v	Print version.
-V	Verbose output (mainly for debugging purposes).
-x <function>	Function to be excluded from tree. This option may occur more
				than once for multiple functions (max 20).
				-x LIBRARY is a special case for excluding all library functions,
				i.e. not found defined in any file. All the functions, called
				(calling) directly or indirectly from the excluded one(s) only,
				will be excluded too.
```

tceetree can be called with no option at all: default options will be used.

Examples:
* `tceetree -F -x LIBRARY`
* `tceetree -C max -r gettree -p rb_tree_find`
