/*******************************************************************************
 * include/.h
 * Implementation of the ADFS algorithm.
 * Opt = 0 : ADFS1 
 * Opt = 1 : ADFS2
 *
 * Part of Project: Incremental-DFS-Experimental: 
 *                  https://github.com/shahbazk/IncDFS-Experimental
 *
 * Copyright (C) 2017 Shahbaz Khan <shahbazk@cse.iitk.ac.in>
 *
 * All rights reserved. Published under the BSD-2 license in the LICENSE file.
 ******************************************************************************/

/*
Incremental Amortized DFS (ICALP14) versions: (Optimality Level)
0- ADFS1 (Process Cross edges Arbitrarily)
1- ADFS2 (Process highest cross edge first)
*/

#include"Tree.h"
#include"Graph.h"
#include"StaticDFS.h"
#include"LevelAnc.h"
#include"Logger.h"
#include"error.h"
#include<stack>

#define pii pair<int,int>
#define ppii pair<pii,pii> 
#define lpit list<pii>::iterator

class IncADFS
{
Tree T;			// DFS tree
Graph G;		// Current Graph

int opt;		// Optimality Level
int art_root;		// Index of artificial Root
LevelAnc LA; 		// LCA/LA Data structure

vector<list<pii> > backEdge;


// OPT 0
list<pii> E0;		

// Opt 1
int szE1;		// Size of the DS, 0 if empty
int levE1;		// Level Currently being processed, -1 if empty
vector<bool> inDS;	// Indicates if a vertex is present in DS
vector<list<int> > E1;	// List of vertices at each level
vector<list<int> > E1v; // List of edges on each vertex
vector<pair< int , list<int>::iterator> > rE1;  
			// Reverse E1 from vertices to corresponding level and list entry


////////////////////
/// DS FUNCTIONS ///
////////////////////

/*
 Initialize the DS based on optimality
*/
void initDS();


/*
 Adds the list of edges E to the DS
Param:	<E> list of cross edges
*/
void addDS(list<pii> &E);

/*
 Choose the next edge from DS
Return: <x,y> the next edge
*/
pii chooseDS();

/*
Updates the DS for vertices in T(root)
Param: <root> Root of subtree for which level of vertices have changed
*/
void updateTDS(int root);

/*
 Check whether the DS is empty
Return: 1 if empty
	0 otherwise
*/
bool emptyDS();



///////////////////////////
//// INTERNAL FUNCTIONS ///
///////////////////////////

/*
Reroot T(v) by connecting (x,y) and deleting (v,par(v))
Param:	<v> Root of tree to be rerooted, new root <y>, 
	parent of new root <x>, <u> is sibling of v having x
Return: List of potential cross edges
*/
list<pii> reroot(int u, int v,int x,int y);

/*
Build the DS for storing back edges
REQUIRED WHEN NOT STARTING FROM EMPTY GRAPH
Param: Root of the subtree <root>
*/
void buildBE(int root);



public:
/*
Constructor:: Vertices in Graph 1,....<size>
Artificial Root: 0
Param: <size> Number of vertices in Graph, Optimality<opt>
*/
IncADFS(int size,int opt=0);

/*
Constructor:: Vertices in Graph 1,....<size>
Artificial Root: 0
Param: <size> Number of vertices in Graph
*/
//IncADFS(int size);

/*
Get the constant refernce to the current DFS tree
Return: Tree& <T>
*/
const Tree& getT() const; 

/*
Get the constant refernce to the current Graph  
Return: Tree& <G>
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
Adds a set of edges list<u,v> to the graph
Params: Index of end vertices list<u,v>
Return: 1 if success
	0 otherwise 
*/
int addEdgeS(list<edg> edgAdd);
	

};
