/*******************************************************************************
 * include/LevelAnc.h
 * Implementation of Level Ancestor data structures. 
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
#include<ctime>
#include"Tree.h"
#include"Logger.h"
using namespace std;

#ifndef LEVEL_ANC
#define LEVEL_ANC

class LevelAnc
{
const Tree *T;
vector<int> lev;		// Level   Root 1;
vector<vector<int> > ancstr;	// Ancestor at level 1,2,4,..., levels above			

/* 
Update the ancestor entry corresponding to 2^i to z 
given it has atleast i-1 entries
*/
void ancUp(int x, int i, int z);
		
/* 
Updates level, LCA, LA info of vertex with index x
Assumes value of par(x) are updated
Param: <x> index of vertex whose ancestor information is updated
*/
int update(int x);	

public:
/*
Find the End of broom Stick (which has >1 children)
starting in subtree T(<x>).
Level of (x) gives the height of broom Stick.
Param: <x> root of tree T(x)
Return:<y> The end of broom Stick
*/
int broomEnd(int x) const;

/*
Constructor: Empty Constructor
*/
LevelAnc();
	
/*
Constructor: Makes the data structure of the corresponding size
Param:	<size> Size of the tree
*/
LevelAnc(const Tree& Tmp);
	
/* 
Updates the size of data if vertex is added.
*/
void addVertex();


/* 
Updates level, LCA, LA info of vertices in subtree 
rooted at vertex with index x
Assumes values of its parents are updated.
Param: <x> index of root of subtree
Return:	1 if success
	-1 else
*/
int updateT(int x);	
	
/*
Return Level of vertex with index x
Param: <x> index of the vertex
Return:	Level of the vertex if valid
	-1 if invalid
*/
int level(int x );

/*
Level ancestor of vertex at level Level(x)-k
Param: <x> index of the vertex, <k> corresponding level
Return: LA of vertex with index x at level Level(x)-k
	-1 if invalid
*/
int la(int x,int k);

/*
LCA of vertices with index x and y
Param: <x>, <y> index of vertices whose LCA required.
Return: Corresponding LCA
	-1 if invalid
*/
int lca(int x,int y );
		
};
#endif
