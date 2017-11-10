/*******************************************************************************
 * include/Tree.h
 * Implementation of opertaions on Trees.
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
#include"error.h"

#ifndef TREE
#define TREE


using namespace std;


#define edg pair<int,int>
#define adjIt list<int>::iterator

class Tree
{
int size_V;						// Size of the Tree
int t_root;						// Root vertex of Tree (-1 if undefined)

vector<list<int> > adjL;		// Adjacency List of children
vector<int> parent;			// Parent List
vector<adjIt> parIt;			// Iterator of x in parents child list

public:

/*
Print Tree rooted at x as a series of par,child pairs of vertices in DFS order.
Param: <x> root of the tree to be printed
*/
void printT(int x) const;

/* 
Test the validity of a vertex for a tree node
Param:	Index of vertex <x>
Return: 1 if valid 
		0 otherwise 
*/
int validV(int x) const;



/* 
Returns the size of the tree
Return: <Size> if valid 
		-1 otherwise 
*/
int size() const;

/* 
Returns the index of root of the tree
Return: <root> if valid 
		-1 otherwise 
*/
int root() const;

/* 
Sets vertex x as root only if parent[x] = -1
Param: Index of perspective root <x>
Return: 1 for success
		0 for failure
*/
int setRoot(int x);
	

/* 
Returns children of the given vertex
Param:  Index of vertex <x>
Return: List<int> of children for success 
		EmptyList for failure
*/
list<int> child(int x) const;


/* 
Returns parent of x 
Param: Index of vertex <x>
Return:	Parent for success, 
		-1 for failure
*/
int par(int x) const;


/*
Constructor for Tree with given size 
Param: Size of the tree <size>
*/
Tree(int size=0);


/*
Adds a new vertex to the Tree. 
Return: Index of vertex.
*/
int addVertex();
	
/* 
Adds Edge to make x = parent(y), where parent(y) = -1 currently
Param: Index of <x> and <y>
Return: 1 for success, 
		0 for failure 
*/
int addEdge(int x,int y);

/* 
Removes Edge where x = parent(y) currently, leaves parent(y) = -1
Param: Index of <x> and <y>
Return: 1 for success, 
		0 for failure
*/
int remEdge(int x,int y);

};

#endif
