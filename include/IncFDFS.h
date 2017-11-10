/*******************************************************************************
 * include/IncFDFS.h
 * Implementation of the FDFS algorithm.
 *
 * Part of Project: Incremental-DFS-Experimental: 
 *                  https://github.com/shahbazk/IncDFS-Experimental
 *
 * Copyright (C) 2017 Shahbaz Khan <shahbazk@cse.iitk.ac.in>
 *
 * All rights reserved. Published under the BSD-2 license in the LICENSE file.
 ******************************************************************************/

#include"Tree.h"
#include"Graph.h"
#include"StaticDFS.h"
#include"LevelAnc.h"
#include"Logger.h"
#include"error.h"
#include<stack>

#define pii pair<int,int>
#define ppii pair<pii,pii> 


class IncFDFS
{
Tree T;			// DFS tree
Graph G;		// Current Graph

int dir;		// Directed (0/1)
LevelAnc LA;		// Only for Directed
int lcauv;

int art_root;		// Index of artificial Root
vector<int> visited;	// Bit Vector for Visited in DFS
vector<int> DFN;	// DFN number of the vertices in T
			// Post order numbering of DFS Tree
vector<int> RDFN;	// Reverse DFN,RDFN[ DFN[x]] = x

edg uv;			// Inserted Edge (u,v)
map<int,int> uDFN;	// Relative DFN to be updated
map<int,int> fDFN;	// Final DFN values

/*
Update RDFN of vertices in the subtree T(<root>)
Param: <root> Root of Subtree to be updated
*/
void updateRDFN(int root);

/*
Unset the visited flag for each vertex 
Param: <root> The root of subtree that needs to be unset
*/
void unsetT(int root);
	
public:
void printDFN(int root);

/*
Constructor:: Vertices in Graph 1,....<size>
Artificial Root: 0
Param: <size> Number of vertices in Graph
	<dir> If Graph is directed (1) or DAG (0)
*/
IncFDFS(int size,int dir=0);

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
Adds an edge set list<u,v> to the graph
Params: Index of end vertices list<u,v>
Return: 1 if success
	0 otherwise 
*/
int addEdgeS(list<edg> edgAdd);

/*
Perform Partial DFS using edge<w,z>
Param: Edge<w,z>, Relative Rank m
*/
void partialDFS(int w,int z,int &m,int &maxDFN);

/*
Update the DFN No. and RDFN No. of updated vertices
Param: <m> shift in DFN No.s 
*/
void updateRank(int m,int maxDFN);

};
