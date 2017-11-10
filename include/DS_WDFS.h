/*******************************************************************************
 * include/DS_WDFS.h
 * Data Structure operations used by WDFS, Fault tolerant and Fully Dynamiic DFS
 *
 * Part of Project: Incremental-DFS-Experimental: 
 *                  https://github.com/shahbazk/IncDFS-Experimental
 *
 * Copyright (C) 2017 Shahbaz Khan <shahbazk@cse.iitk.ac.in>
 *
 * All rights reserved. Published under the BSD-2 license in the LICENSE file.
 ******************************************************************************/

#include<vector>
#include<list>
#include<cstdio>
#include<cmath>
#include<map>
#include<algorithm>
#include<cmath>

#ifndef WDFS
#define WDFS

//#define WDFS_DEBUG
//#define DS_OLD  // If want to use old implementation

#include"Tree.h"
#include"Graph.h"
#include"error.h"
#include"Logger.h"

using namespace std;

//       Shorts of types used by segT
#ifdef DS_OLD
#define stateV pair<int,pSegNodeIt>	// State Variable [CurrVertex, Pair of SegNit of child]
#else
#define stateV int
#endif

#define pSegNodeIt pair<segNodeIt,segNodeIt>		
#define segNodeIt segNode::iterator		// Iterator of map at each seg node
#define segNode map<edg,int>			// Map stored at each seg node
#define edg pair<int,int>				// Rep of an edge
#define pth pair<int,int>				// Path in a tree


class DS_WDFS
{
Tree T;

struct dataHLD{					// Data From HLD of T
vector<int> heavyNo;	
// Heavy No. assigned to each vertex   {HN(x)> HN(descendants)} (0,n-1)
// A vertex always represented as x,y..   (0,n-1)
// and Heavy No. of vertex represented by hx,hy
vector<int> rHeavyNo;			// Vertex assigned to each heavy no.
vector<int> lastTree;			// Max Heavy no. in subree T(Vertex)      (hVertex to hVertex)
vector<int> pathHead;			// Head of the heavy path a vertex is in  (hVertex to hVertex)
vector<int> sizeTree;			// Size of subtree rooted at the vertex
} dHLD;


// Data For SegTree
struct ST
{
int size; 		// Size of number of covered vertices in tree
int offST;		// Offset of SegT
vector<segNode> segT;	// Nodes of SegT: Internal Node (sorted set of edge labelled by Heavy No.)
} st;

stateV curState;	// Partial Update Status


/*
Tests the validity of <status>. 
Complexity O(m):: Use wisely
Param: <status> curState, <st> SegTree info
Return: 1 if valid
	0 else
*/
int static validState(stateV status,ST& st);

//////////////////////////////
////// INIT FUNCTIONS ////////
//////////////////////////////


/*
Update heavyNo, rHeavyNo, lastTree, pathHead of <dHLD> for each vertex of <T> below <root>
Param:  <dHLD> Ref of dataHLD, <T> Const Ref of Tree, <root> Index of Root  
		<hInd> hNo. of root, <hHead> hNo. of head of path 
Return: <sz> Size of T(root) in <T>
		-1 If invalid root
*/
int static updateHL(dataHLD& dHLD, const Tree& T, int root,int hInd,int hHead);


/*
Update sizeTree of <dHLD>  for each subtree of <T> below <root>
Param:  <dHLD> Ref of dataHLD, <T> Const Ref of Tree, <root> Index of Root   
Return: <sz> Size of T(root) in <T>
		-1 If invalid root
*/
int static updateSize(dataHLD& dHLD,const Tree& T, int root);


/*
Initiate Heavy Light Decomposition data <dHLD> on <T> 
Param: <dHLD> data of HLD, <T> Given tree T
*/
void static initHLD(dataHLD& dHLD, const Tree& T);


/*
Initialize the segment tree nodes in parts upto 'cost' steps.
Param: G,dataHLD, ST, <status> CurrState, <&cost> No. of steps to be executed
Return: <state> Place to continue next time.
*/
stateV static makeSTpart(const Graph& G, const dataHLD& dHLD, ST& st, stateV status,int& cost);



//////////////////////////////
////// QUERY FUNCTIONS ///////
//////////////////////////////


/* FOR DEBUG ONLY
Print List in Map of a node in SegTREE 
*/
void dispMP(const ST& st, int hx, pth interval)	const;

/*
Query Map of SegNode for lowest/highest edge based on type(0/1) in interval
Param: ST, hNo. of node, interval and query type
Return: Required Edge
*/
edg queryM(const ST& st, int hx, pth interval, int type) const;

/*
Query ST from hleft to hright for lowest/highest edge based on type(0/1) in interval
Param: hNo. of all
Return: Required edge
*/
edg queryST(const ST& st, int hLeft, int hRight, pth interval, int type) const;





public:
/*
Reports whether a vertex <x> is covered by DS, ie added to the segTree
Param: <x> Index of the vertex
Return: 1 if covered
	0 else
*/
int isCovered(int x) const;
	
/*
Returns a pointer of the underlying DFS tree
Return: Pointer of T 
*/
const Tree& getTree() const;

/*
Verifies whether x is an ancestor of y in T
Param: <dHLD> Ref of dataHLD, Index of <x> and <y>
Return: 1 if true
	0 otherwise
*/
int isAnc(int x, int y) const;

/*
Empty Constructor for DS_WDFS.
*/
DS_WDFS();



/*
Constructor of DS_WDFS. Makes DS followed by many updates
Param: <Temp> Tree on which DS to be made
*/
DS_WDFS(const Tree Temp);


/*
Partial update DS based T
Param: Ref of Graph <G>. Amount of Cost allowed to build <cost>
Return: 1 if completely built
		0 otherwise
*/
int buildPart(const Graph& G,int cost);


/*
Query Type(w), path(x,y) where ans closer to x.
	  Type 	0 : T(w), 
			1 : w
Report minimum of all such edges from T(w) to path(x,y) if exists else (-1,-1)
Return: Required edge
*/
edg query(int w, int x, int y, int type) const;

/*
Remove Edge	(x,y) from the DS
Param: Index of <x> and <y>
THAT IS SURELY PRESENT IN DS  (OR INTRODUCE RELEVANT CHECKS)
Return: 1 For success 
	0 otherwise
*/
int delE(int x, int y);

/*
Insert Back Edge (x,y) to the DS
Param: Index of <x> and <y>
ASSUMPTION: Edge not present in DS (OR INTRODUCE RELEVANT CHECKS)
Return: 1 For success 
	0 otherwise
*/
int addBEdge(int x, int y);
};
#endif

