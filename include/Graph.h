/*******************************************************************************
 * include/Graph.h
 * Implementation of opertaions on Graphs.
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
#include<map>
#include<ctime>
#include"Logger.h"
#include"error.h"

#ifndef GRPH
#define GRPH




using namespace std;

#define edg pair<int,int>
#define adjIt list<int>::iterator

#ifdef UNORD
 #ifndef TREE
 namespace std{
 template <typename T, typename U> 
 struct hash<pair<T,U> > 
 { size_t operator() (pair<T, U> x) const throw()
  { return hash<T>()(x.first) ^ hash<U>()(x.second); }};
 }
 #endif
#endif 

class Graph
{
int size_V; 			// Number of vertices 
int size_E;			// Number of edges
int directed;			// Is graph directed?

vector<list<int> > adjL;		// Adjacency List of neighbours
list<int> delVrtx;			// List of deleted vertices

#ifdef UNORD
unordered_map<int,adjIt> delVrtxM; 	// Hash of deleted vertices
unordered_map<edg ,adjIt> edgM; 	// Hash of avaliable edges // OLD (edg.first<edg,sec)
#else
map<int,adjIt> delVrtxM; 	// Hash of deleted vertices
map<edg ,adjIt> edgM; 	// Hash of avaliable edges // OLD (edg.first<edg,sec)
#endif

clock_t begTime;


public:
/*
Print Graph as a series of adjacency lists of vertices.
*/
void printG() const;

/*
Report whether graph is directed
Return: 1 if directed
	0 otherwise
*/
int isDirected() const;

/* 
Check the validity of an edge <x,y>.
Param: Index of the vertex <x> and <y>
Return: 1 if valid edge 
	0 otherwise
*/
int validE(int x,int y) const;

/* Check the validity of a vertex.
Param: Index of the vertex <x>
Return: 1 if valid vertex 
	0 otherwise
*/
int validV(int x) const;


/* 
The true size of vertices
Return: <Size> if valid 
	-1 otherwise 
*/
int size() const;

/*
Max index of vertex {Size might be smaller}
Return <size> if valid
	-1    otherwise
*/
int sizeV() const;

/*
Number of edges
Return <size> if valid
	-1    otherwise
*/
int sizeE() const;

/* 
Constant Reference to Neighbours of vertex x 
Param: Index of vertex <x>
Return: Constant Ref ti List Neighbours for success 
	EmptyList for failure
*/
const list<int>& nghbrR(int x) const;

/* 
Neighbours of vertex x 
Param: Index of vertex <x>
Return: List Neighbours for success 
	EmptyList for failure
*/
list<int> nghbr(int x) const;

/*
Constructor for graph with given size and directed/undirected flag
Param: Size of the tree <size>
*/
Graph(int size=0,int directed=0);


/*
Adds a new vertex to the Tree. 
Return: Index of vertex.
*/
int addVertex();

/*
Adds a new vertex with index x to the Tree. 
Return: 1 if success 
	0 otherwise
*/
int addVertex(int x);

/* 
Remove vertex x
Param: Index of <x>
Return: 1 for success, 
	0 for failure 
*/
int remVertex(int x);


/* 
Adds Edge to the graph
Param: Index of <x> and <y>
Return: 1 for success, 
	0 for failure 
*/
int addEdge(int x,int y);	

/* 
Removes Edge from Graph
Param: Index of <x> and <y>
Return: 1 for success, 
	0 for failure 
*/
int remEdge(int x,int y);
};

#endif
