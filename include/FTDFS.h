/*******************************************************************************
 * include/FTDFS.h
 * Implementation of the fault tolerant DFS algorithm handling all kinds of updates.
 * Addition/Deletion of Vertex and Edges. 
 *
 * Part of Project: Incremental-DFS-Experimental: 
 *                  https://github.com/shahbazk/IncDFS-Experimental
 *
 * Copyright (C) 2017 Shahbaz Khan <shahbazk@cse.iitk.ac.in>
 *
 * All rights reserved. Published under the BSD-2 license in the LICENSE file.
 ******************************************************************************/

#include<cstdio>
#include<vector>
#include<list>
#include<stack>
#include"Tree.h"
#include"Graph.h"
#include"DS_WDFS.h"
#include"error.h"
#include"Logger.h"

#ifndef FaultT_DFS
#define FaultT_DFS

#define pTIter pair<int,TIter>
#define TIter  list<int>::iterator

#define pPIter pair<pth,PIter>
#define PIter list<pth>::iterator 

struct dataFTDFS
	{
	vector<list<int> > redAdj;	// reduced adjacency list
	list<int> partT;		// Set of Trees in partition
	list<pth> partP;		// Set of paths in partition (Anc,Des)

	vector<int> info;		// -1 Vertex Invalid
					// 0 if vertex in Tree, 
					// 1 if in path
					// 2 covered
	vector<pTIter> isRoot;		// First: 0 if not root of a tree in partition else 1
					// Second: Iterator in partT
	vector<pPIter> pathParam;	// (x,y) if vertex belongs to path in partition (Anc,Des)
					// SecondL Iterator in partP
	};



class FTDFS
{

/*
Builds the reduced adjacency list for <path> from paths in partT and trees in listT 
Param: DS_WDFS& DS, dataFTDFS& dFT, List of trees to be considered, path <ancestor,descendant>
*/
void static buildRedAdj(const DS_WDFS& DS, dataFTDFS& dFT, list<int> listT, pth path);

/*
Perform DFS_in_Path in the path having <currV>
Params: <&DS> DS_WDFS for previous DFS Tree, <dFT> Data of FTDFS, entry vertex <currV> 
Return: Ancestor Path list (New Ancestor, ... , New Descendant)
*/
list<int> static DFS_Path(const DS_WDFS& DS, dataFTDFS& dFT, int currV);

/*
Perform DFS_in_Tree in the subtree having <currV>
Params: <&DS> DS_WDFS for previous DFS Tree, <dFT> Data of FTDFS, entry vertex <currV> 
Return: Ancestor Path list (New Ancestor, ... , New Descendant)
*/
list<int> static DFS_Tree(const DS_WDFS& DS, dataFTDFS& dFT, int currV);

public:
const static int INFO_INVALID = -1;
const static int INFO_TREE = 0;
const static int INFO_PATH = 1;
const static int INFO_VISITED = 2;

/*
Static Function that returns DFS tree of a graph from the given root <root>
!!!Required!!!: Red Adjacency List of Root populated for singleton paths corresponding to newVertices.
                And root not part of partT or partP
Params: <&DS> DS_WDFS for previous DFS Tree, <dFT> Data of FTDFS, given root 
Return: DFS tree of the graph
*/
Tree static FT_DFS(const DS_WDFS& DS, dataFTDFS& dFT, int root);


/*
Initialize the structure for dataFTDFS <dFT> for a tree with <size> vertices with root <root>
Adds child subtrees of root to partT and changes the info accordingly
Param: <DS> corresponding DS_WDFS,<dFT>& dataFTDFS,  <size> size of tree, <root> root of T
*/

void static initDataFTDFS(const DS_WDFS& DS, dataFTDFS& dFT, int size, int root);

/*
Initialize the <dFT>.info of the whole subtree T(<root>) to INFO_TREE
Param: <DS> DS of the tree, <dFT>, <root>
*/
void static initInfoTree(const DS_WDFS& DS, dataFTDFS& dFT, int root);

/*
Increase size of dFT corresponding to added vertex if required
Return: 1 if success
	0 if failure
*/
int static addVDFT(dataFTDFS& dFT, int x);


/*
Update the partition for deleting an edge<x,y> or vertex<x> from partT
Param: <dFT> dataDFDFS, edge<x,y> or vertex<x> (y==-1)
*/
void static partitionP(DS_WDFS& DS, dataFTDFS& dFT, int x, int y);

/*
Update the partition for deleting an edge<x,y> or vertex<x> from partT
Param: <dFT> dataDFDFS, edge<x,y> or vertex<x> (y==-1)
*/
void static partitionT(DS_WDFS& DS, dataFTDFS& dFT, int x, int y);

/*
Print the partition information of the current tree.
Param: <dFT> dataFTDFS for current partition
*/
void static printDFT(const dataFTDFS dFT);
};
#endif
