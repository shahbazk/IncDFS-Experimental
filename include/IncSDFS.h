/*******************************************************************************
 * include/IncSDFS.h
 * Implementation of the SDFS algorithm.
 * Opt = 0 : SDFS 
 * Opt = 2 : SDFS2
 * Opt = 4 : SDFS3 
 *
 * Part of Project: Incremental-DFS-Experimental: 
 *                  https://github.com/shahbazk/IncDFS-Experimental
 *
 * Copyright (C) 2017 Shahbaz Khan <shahbazk@cse.iitk.ac.in>
 *
 * All rights reserved. Published under the BSD-2 license in the LICENSE file.
 ******************************************************************************/

/*
Incremental Static DFS versions: (Optimality Level)
0- (SDFS)  Basic (Repeat every thing after every change)
1-         ABE   (Avoid repeating everything if back edge inserted)
2- (SDFS2) ABE_Broom (Rebuild everything in bristles)
3-         ABE_LCA (Rebuild everything in LCA)
4- (SDFS3) ABE_LCA_S (Rebuild Smaller subtree in LCA)
5-         ABE_LCA_A (Build either of subtree of LCA)

Space Opt:
0 - Basic (Store all)
1- (Ignore storing new edges with endpoint in Stick)
2- (Delete non-tree edges of Stick)

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


class IncSDFS
{
Tree T;			// DFS tree
Graph G;		// Current Graph
LevelAnc LA;		// LA on current DFS tree

int opt;		// Optimality Level
int sopt;		// Space Opt 
int dir;		// Directed (0/1)
int art_root;		// Index of artificial Root
vector<int> visited;	// Bit Vector for Visited in DFS

vector<int> DFN;	// DFN number of the vertices in T
			// Post order numbering of DFS Tree
			// Not valid for Undirected Opt 4
vector<int> RDFN;	// Reverse DFN,RDFN[ DFN[x]] = x
			// Not valid or initialized for Undirected

int broomE;		// Last Vertex of stick of broom


/*
Deletes the edges of vertices on the stick
*/
void delEdges(int pbroom,int broom);


/*
Reports if the new edge <x,y> is a back edge
Param: Edge <x,y>
Return: 1 if back edge
	0 if cross edge
*/
int isBackEdge(int x, int y);
	
/*
Update RDFN of vertices in the subtree T(<root>)
Param: <root> Root of Subtree to be updated
*/
void updateRDFN(int root);

/*
Return the end point of the new edge <x,y> in the 
smaller subtree hanging from lca
Return: <x> or <y> accordingly
*/
int lca_small(int x,int y);
	
/*
Unset the visited flag for each vertex and delete all 
edges in the subtree T(root) of T.
Param: <root> The root of subtree that needs to be set
Return: min DFN number of the subtree
*/
int unsetT(int root);
	

public:

/*
Constructor:: Vertices in Graph 1,....<size> for optimality <optimality>
<directed> specifies whether given graph is directed.
Artificial Root: 0
Param: <size> Number of vertices in Graph, <opt> Optimization level
	<directed> 1 if directed, 0 otherwise 
	<spaceOpt> space optimization
*/
IncSDFS(int size,int optimality=0,int directed=0, int spaceOpt=2);

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
Adds a list of edges list<x,y> to the graph
Params: Index of end vertices list <x,y>
Return: 1 if success
	0 otherwise 
*/
int addEdgeS(list<edg> edgAdd);
	
/*
Adds a cross edge <x,y> to the digraph with OPT 4
Params: Index of end vertices <x,y>
Return: 1 if success
	0 otherwise 
*/
int addCrossEdgeDirOpt4(int x, int y);
	/*
Adds a cross edge <x,y> to the digraph with OPT 4
Params: Index of end vertices <x,y>
Return: 1 if success
	0 otherwise 
*/
int addCrossEdgeDirOpt5(int x, int y);
void maxDFN(int root, int dfnL, int& dfnMAX);
void printDFN(int root);
	
};

