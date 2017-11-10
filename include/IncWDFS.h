/*******************************************************************************
 * include/IncWDFS.h
 * Implementation of the WDFS algorithm.
 *
 * Part of Project: Incremental-DFS-Experimental: 
 *                  https://github.com/shahbazk/IncDFS-Experimental
 *
 * Copyright (C) 2017 Shahbaz Khan <shahbazk@cse.iitk.ac.in>
 *
 * All rights reserved. Published under the BSD-2 license in the LICENSE file.
 ******************************************************************************/

/*
Incremental DFS handling insertion of edges

PSEUDO ROOT:
The vertex 0 is used as the pseudo root. Will always be the root of the tree.
The invoker will not delete this vertex ever,
Or add/delete any edges,vertices to this.
And each inserted vertex will automatically have an edge to 0; 

VERTEX INSERTION:
Newly added vertex will be assigned a new number and returned. 
The invoker must use this number for future edge additions/deletion 
to this vertex.

Optimality Level:
0- Build every thing from scratch after each update
1- Avoid building if added edge a back edge
*/


#include<cstdio>
#include<vector>
#include<list>
#include<stack>

#include"Tree.h"
#include"Graph.h"
#include"DS_WDFS.h"
#include"StaticDFS.h"
#include"FTDFS.h"
#include"LevelAnc.h"
#include"error.h"
#include"Logger.h"

//#define AVOID_BACK_EDGE		// Does not recompute if inserted edge a back edge

class IncWDFS
{	

Tree T;					// Current DFS TREE
Graph G;				// Graph for which DS being generated,
LevelAnc LA;				// Level Ancestor DS for T
DS_WDFS DS[2];				// DS being used and constructed
int cDS;				// DS[cDS] being used, DS[!cDS] Partially constructed

int opt;				// Optimality Level of the algorithm
int art_root; 				// Artificial Root
int cost_upd;				// Cost for partial DS update

list<edg> edgAdd[2];			// List of edges added, 
					// edgAdd[cDS]:since DFS Tree of DS[cDS]
					// edgAdd[!cDS]: since DFS Tree of DS[!cDS]

/*
Updates the DS[!cDS] using build part, with required conversions if completed.
*/
void updateDS();



/*
Updates the DFS tree <T> based on edgAdd lists
*/
void updateDFS();

public:

// DEBUG:: Tests the LA structure used
void test(int x);

/*
Constructor:: Vertices of the graph 1,...,<size>
Artificial Root: 0, Optimality Level <optimality>
Time: O(m \log n)
Param: <size> Number of vertices in the graphs
*/
IncWDFS(int size,int optimality=0);



/*
Get the constant refernce to the current DFS tree
Return: Tree& <T>
*/
const Tree& getT() const; 

/*
Get the constant refernce to the current Graph for VERTEX INFO ONLY 
Return: Tree& <Gv>
*/
const Graph& getG() const;

/*
Adds an edge <x,y> to the graph
Params: Index of end vertices <x,y>
Return: 1 if success
	0 otherwise 
*/
int addEdge(int x, int y);

/*
Adds an edge set list<x,y> to the graph
Params: Index of end vertices list<x,y>
Return: 1 if success
	0 otherwise 
*/
int addEdgeS(list<edg> edgAdded);
};

