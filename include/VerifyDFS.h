/*******************************************************************************
 * include/VerifyDFS.h
 * Implementation of procedure verifying the correctness of the computed DFS tree
 *
 * Part of Project: Incremental-DFS-Experimental: 
 *                  https://github.com/shahbazk/IncDFS-Experimental
 *
 * Copyright (C) 2017 Shahbaz Khan <shahbazk@cse.iitk.ac.in>
 *
 * All rights reserved. Published under the BSD-2 license in the LICENSE file.
 ******************************************************************************/

#include<cstdio>
#include<list>
#include"Tree.h"
#include"Graph.h"
#include"LevelAnc.h"
#include"error.h"

class VerifyDFS
{

/*
Verifies whether subtree of <T> rooted at <root> 
is a valid DFS subtree of <G>
Param: <G> graph, <T> DFS Tree, <LA> Level Anc Structure, <root>, <DFN> DFN no. of visited vertice,
	<visit> Number of visited vertices
Return:	1 if Correct
	0 if incorrect
*/
static int testT(const Graph& G,const Tree& T, LevelAnc& LA, int root, int DFN[], int &visit);


public:

/*
Verifies whether <T> is a valid DFS tree of <G>
Param: <G> graph, <T> DFS Tree
Return:	1 if Correct
	0 if incorrect
*/
static int Verify_DFS(const Graph& G,const Tree& T);

};
