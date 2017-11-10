/*******************************************************************************
 * src/Graph.cpp
 * Implementation of opertaions on Graphs.
 *
 * Part of Project: Incremental-DFS-Experimental: 
 *                  https://github.com/shahbazk/IncDFS-Experimental
 *
 * Copyright (C) 2017 Shahbaz Khan <shahbazk@cse.iitk.ac.in>
 *
 * All rights reserved. Published under the BSD-2 license in the LICENSE file.
 ******************************************************************************/

#include"../include/Graph.h"

////////////////////////
/// Public Functions ///
////////////////////////

/*
Print Graph as a series of adjacency lists of vertices.
*/
void Graph::printG() const
{
list<int> :: const_iterator it;
for(int x=0;x<size_V;x++)
	{
	printf("%d=>",x);
	for(it = adjL[x].begin();it!= adjL[x].end();it++)
		{
		printf("%d,",*it);
		}
	printf("\n");
	}
}

/*
Report whether graph is directed
Return: 1 if directed
	0 otherwise
*/
int Graph::isDirected() const
	{
	#ifndef FAST
	if(directed!=0 && directed!=1)
		ERR_MSGR("Graph::isDirected <directed> value corrupted for graph",0);
	#endif
	return directed;
	}

/* 
Check the validity of an edge <x,y>.
Param: Index of the vertex <x> and <y>
Return: 1 if valid edge 
	0 otherwise
*/
int Graph::validE(int x,int y) const
	{
	#ifndef FAST
	if(!validV(x) || !validV(y)) return 0;

	clock_t begT = clock();
	#endif
	int a = ( edgM.find(edg(x,y))!= edgM.end());
	#ifndef FAST
	Logger::incrEdgMTime(( (double) clock()-begT)/CLOCKS_PER_SEC);
	#endif
	return a;
	}

/* 
Check the validity of a vertex.
Param: Index of the vertex <x>
Return: 1 if valid vertex 
	0 otherwise
*/
int Graph::validV(int x) const
	{
	if(x>= 0 && x<size_V && delVrtxM.find(x)==delVrtxM.end())
		return 1;
	else 	return 0;
	}

/* 
The true size of vertices
Return: <Size> if valid 
	-1 otherwise 
*/
int Graph::size() const
	{return size_V - delVrtx.size();}

/*
Max index of vertex {Size might be smaller}
Return <size> if valid
	-1    otherwise
*/
int Graph::sizeV() const
	{
	return size_V;}	

/*
Number of edges
Return <size> if valid
	-1    otherwise
*/
int Graph::sizeE() const
	{return size_E;}

/* 
Constant Reference to Neighbours of vertex x 
Param: Index of vertex <x>
Return: Constant Ref ti List Neighbours for success 
	EmptyList for failure
*/
const list<int>& Graph::nghbrR(int x) const
{
	return adjL[x];
}

/* 
Neighbours of vertex x 
Param: Index of vertex <x>
Return: List Neighbours for success 
	EmptyList for failure
*/
list<int> Graph::nghbr(int x) const
	{
	#ifndef FAST
	if(!validV(x))						
		ERR_MSGR("Graph::nghbr invoked with invalid args",list<int>());
	#endif

	return adjL[x];
	}


/*
Constructor for graph with given size and directed/undirected flag
Param: Size of the tree <size>
*/
Graph::Graph(int size,int dir)
	{
	#ifndef FAST
	if(size<0)
		ERR_MSG("Graph::Constructor invoked with invalid size\n");
	#endif
	size_V = size;
	size_E = 0;
//	for(int i=0;i<size;i++)
//		adjL.push_back(list<int>());
	adjL.resize(size);
	directed = dir;	
	}

/*
Adds a new vertex to the Tree. 
Return: Index of vertex.
*/
int Graph::addVertex()
	{
	if(delVrtx.size() == 0)
		{
		list<int> tmp2;	
		adjL.push_back(tmp2);
		return size_V++;
		}
	else
		{
		int x = delVrtx.back();
		delVrtx.erase(delVrtxM[x]);
		delVrtxM.erase(x);
		return x;			
		}
	}

/*
Adds a new vertex with index x to the Tree. 
Return: 1 if success 
	0 otherwise
*/
int Graph::addVertex(int x)
	{
	#ifndef FAST
	if(validV(x) || x<=0 )
		ERR_MSGR("Graph::addVertex invoked with invalid arguments\n",0);
	#endif

	if(delVrtxM.find(x)==delVrtxM.end())
		{// Not in range 0,size_V
		while(size_V <= x)
			{
			list<int> tmp;
			adjL.push_back(tmp);
			delVrtx.push_front(size_V);
			delVrtxM[size_V]=delVrtx.begin();
			size_V++;			
			}
		}
	delVrtx.erase(delVrtxM[x]);
	delVrtxM.erase(x);
	return 1;	
	}
	
/* 
Remove vertex x
Param: Index of <x>
Return: 1 for success, 
	0 for failure 
*/
int  Graph::remVertex(int x)
	{
	#ifndef FAST
	if(!validV(x))
		ERR_MSGR("Graph::remVertex invoked with invalid arguments\n",0);
	#endif

	list<int> ngbr = adjL[x];
	list<int>:: iterator it;
	for(it=ngbr.begin();it!=ngbr.end();it++)
		remEdge(x,*it);

	if(directed)
		{
		int y;
		for(y=0; y<size_V;y++)
			if(validV(y) && validE(y,x))
				remEdge(y,x);
		}

	delVrtx.push_front(x);	
	delVrtxM[x]=delVrtx.begin();
	return 1;			
	}



/* 
Adds Edge to the graph
Param: Index of <x> and <y>
Return: 1 for success, 
	0 for failure 
*/
int Graph::addEdge(int x,int y)	
	{
	#ifndef FAST
	if(!(validV(x) && validV(y) && !validE(x,y)))
		ERR_MSGR("Graph::addEdge invoked with invalid arguments\n",0);

	begTime = clock();	
	#endif
	adjL[x].push_front(y);
	edgM[edg(x,y)] = adjL[x].begin();
	if(!directed)
		{
		adjL[y].push_front(x);
		edgM[edg(y,x)] = adjL[y].begin();
		}
	#ifndef FAST
	Logger::incrEdgMTime(( (double) clock()-begTime)/CLOCKS_PER_SEC);
	#endif

	size_E++;
	return 1;
	}

/* 
Removes Edge from Graph
Param: Index of <x> and <y>
Return: 1 for success, 
	0 for failure 
*/
int Graph::remEdge(int x,int y)	
	{
	#ifndef FAST
	if(!validE(x,y))
		ERR_MSGR("Graph::remEdge invoked with invalid arguments\n",0);
	begTime = clock();	
	#endif
	adjL[x].erase(edgM[edg(x,y)]);
	edgM.erase(edg(x,y));
	if(!directed)
		{	
		adjL[y].erase(edgM[edg(y,x)]);
		edgM.erase(edg(y,x));	
		}
	#ifndef FAST
	Logger::incrEdgMTime(( (double) clock()-begTime)/CLOCKS_PER_SEC);
	#endif

	size_E--;
	return 1;
	}
