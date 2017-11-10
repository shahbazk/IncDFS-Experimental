/*******************************************************************************
 * src/Tree.cpp
 * Implementation of opertaions on Trees.
 *
 * Part of Project: Incremental-DFS-Experimental: 
 *                  https://github.com/shahbazk/IncDFS-Experimental
 *
 * Copyright (C) 2017 Shahbaz Khan <shahbazk@cse.iitk.ac.in>
 *
 * All rights reserved. Published under the BSD-2 license in the LICENSE file.
 ******************************************************************************/

#include"../include/Tree.h"


////////////////////////
// PUBLIC FUNCTIONS ////
////////////////////////

/*
Print Tree rooted at x as a series of par,child pairs of vertices in DFS order.
Param: <x> root of the tree to be printed
*/
void Tree::printT(int x) const
{
if(!validV(x))
	ERR_MSG("Tree::printT invoked with invalid root");
list<int> :: const_iterator it;
for(it = adjL[x].begin();it!= adjL[x].end();it++)
	{
	printf(" (%d)->(%d) ",x,*it);
	printT(*it);
	}
}


/* 
Test the validity of a vertex for a tree node
Param:	Index of vertex <x>
Return: 1 if valid 
		0 otherwise 
*/
int Tree::validV(int x) const
	{
	if(x>= 0 && x<size_V)		
			return 1;
	else		return 0;
	}



/* 
Returns the size of the tree
Return: <Size> if valid 
	-1 otherwise 
*/
int Tree::size() const
	{return size_V;}

/* 
Returns the index of root of the tree
Return: <root> if valid 
	-1 otherwise 
*/
int Tree::root() const
	{return t_root;}

/* 
Sets vertex x as root only if parent[x] = -1
Param: Index of perspective root <x>
Return: 1 for success
	0 for failure
*/
int Tree::setRoot(int x)
	{
	#ifndef FAST
	if(!(validV(x)  && parent[x]==-1))
		ERR_MSGR("Tree::setRoot invoked with invalid 'x'\n",0);
	#endif

	t_root = x;
	return 1;
	}
	

/* 
Returns children of the given vertex
Param:  Index of vertex <x>
Return: List<int> of children for success 
		EmptyList for failure
*/
list<int> Tree::child(int x) const
	{
	#ifndef FAST
	if(!validV(x))		
		ERR_MSGR("Tree::child invoked with invalid 'x'\n", list<int>());
	#endif

	return adjL[x];
	}


/* 
Returns parent of x 
Param: Index of vertex <x>
Return:	Parent for success, 
		-1 for failure
*/
int Tree::par(int x) const
	{
	#ifndef FAST
	if(!validV(x))		
		ERR_MSGR("Tree::par invoked with invalid 'x'\n",-1);
	#endif

	return parent[x];
	}

/*
Constructor for Tree with given size 
Param: Size of the tree <size>
*/
Tree::Tree(int size)
	{
	#ifndef FAST
	if(size<0)
		ERR_MSG("Tree::Constructor invoked with invalid 'size'\n");
	#endif

	this->size_V = size;
	this->t_root = -1;
	this->adjL.resize(size);
	this->parent.resize(size,-1);
	this->parIt.resize(size);
	}


/*
Adds a new vertex to the Tree. 
Return: Index of vertex.
*/
int Tree::addVertex()
	{
	list<int> tmp2;
	
	adjL.push_back(tmp2);
	parent.push_back(-1);
	parIt.push_back(tmp2.begin());

	return size_V++;
	}


/* 
Adds Edge to make x = parent(y), where parent(y) = -1 currently
Param: Index of <x> and <y>
Return: 1 for success, 
		0 for failure 
*/
int Tree::addEdge(int x,int y)	
	{
	#ifndef FAST
	if(!(validV(x) && validV(y) && parent[y]==-1))
		ERR_MSGR("Tree::addEdge invoked with invalid arguments\n",0);
	#endif
	
	adjL[x].push_front(y);
	
	parIt[y] = adjL[x].begin();
	parent[y]=x;
	return 1;
	}

/* 
Removes Edge where x = parent(y) currently, leaves parent(y) = -1
Param: Index of <x> and <y>
Return: 1 for success, 
		0 for failure
*/
int Tree::remEdge(int x,int y)	
	{
	#ifndef FAST
	if(!(validV(x) && validV(y) && parent[y]==x))
		ERR_MSGR("Tree::remEdge invoked with invalid arguments\n",0);
	#endif

	adjL[x].erase(parIt[y]);
	parent[y]=-1;
	return 1;
	}


