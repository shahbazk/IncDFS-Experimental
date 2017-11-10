/*******************************************************************************
 * include/StaticDFS.h
 * Implementation of the standard algorithm for computing DFS in the static setting.
 *
 * Part of Project: Incremental-DFS-Experimental: 
 *                  https://github.com/shahbazk/IncDFS-Experimental
 *
 * Copyright (C) 2017 Shahbaz Khan <shahbazk@cse.iitk.ac.in>
 *
 * All rights reserved. Published under the BSD-2 license in the LICENSE file.
 ******************************************************************************/

#include<cstdio>
#include<cstdlib>
#include<ctime>
#include<cmath>
#include<list>
#include<vector>
#include"Graph.h"
#include"Tree.h"
#include"Logger.h"

//#define SHWTREE
//#define SHWGRPH

#ifndef STATIC_DFS
#define STATIC_DFS

using namespace std;

class SDFS
{

public:

/*
Static DFS algorithm, performs DFS traversal from the root
Param: <root> Index of root, reference of <T>,<G> and
two vectors for <visited> and <DFN> no. (post order no.) and current <dfn>
<unvisited> count of unvisited vertices
Return: 1 if completed
	0 else
*/
int static DFS(int root, Tree& T, const Graph& G, vector<int>& visited, vector<int>& DFN,
		int& dfn,int& unvisited);


/*
Static Function that returns DFS tree of a graph from the root
Param: <@Grph> Reference of Grph, <root>Index of Root
Return: DFS tree of the graph at root
*/
Tree static StaticDFS(const Graph& Grph, int root);

};

#endif
