/*******************************************************************************
 * src/LevelAnc.cpp
 * Implementation of Level Ancestor data structures. 
 *
 * Part of Project: Incremental-DFS-Experimental: 
 *                  https://github.com/shahbazk/IncDFS-Experimental
 *
 * Copyright (C) 2017 Shahbaz Khan <shahbazk@cse.iitk.ac.in>
 *
 * All rights reserved. Published under the BSD-2 license in the LICENSE file.
 ******************************************************************************/

#include"../include/LevelAnc.h"


/* 
Updates level, LCA, LA info of vertex with index x
Assumes value of par(x) are updated
Param: <x> index of vertex whose ancestor information is updated
Return:	1 if success
	0 else
*/
int LevelAnc::update(int x)	
	{
	#ifndef FAST
	if(!(*T).validV(x) || ((*T).par(x)==-1 && (*T).root()!=x) )
		ERR_MSGR("LevelAnc::update Invalid Args\n",0);
	#endif

		
	if((*T).par(x)==-1)
		{
		lev[x] = 1;
		ancUp(x,0,-1);
		}
	else
		{
		int i,y = (*T).par(x);
		lev[x] = lev[y]+1;
		ancUp(x,0,y);

		for(i=1;(1<< i) < lev[x] ; i++)
			ancUp(x,i,  ancstr[   ancstr[x][i-1]   ] [i-1]  );

		ancUp(x,i,-1);
		}
	return 1;			
	}


/* 
Update the ancestor entry corresponding to 2^i to z 
given it has atleast i-1 entries
*/
void LevelAnc::ancUp(int x, int i, int z)
	{
	if(ancstr[x].size()== (unsigned int) i ) ancstr[x].push_back(z);
	else ancstr[x][i]=z;
	}

////////////////////////
/// PUBLIC FUNCTIONS ///
////////////////////////


/*
Find the End of broom Stick (which has >1 children)
starting in subtree T(<x>).
Level of (x) gives the height of broom Stick.
Param: <x> root of tree T(x)
Return:<y> The end of broom Stick
*/
int LevelAnc::broomEnd(int x) const
	{
	#ifndef FAST
	if(!(*T).validV(x))
		ERR_MSGR("LevelAnc::broomEnd Invalid Args\n",-1);
	#endif
	
	int y= x;
	while((*T).child(y).size()==1)
		y = (*T).child(y).front();			
	return y;
	}

/* Empty Constructor */
LevelAnc::LevelAnc(){}

/*
Constructor: Makes the data structure of the corresponding size
Param:	<size> Size of the tree
*/
LevelAnc::LevelAnc(const Tree& Tmp)
	{
	T = &Tmp;
	ancstr.resize((*T).size());
	lev.resize((*T).size(),-1);
	updateT((*T).root());
	}


/* 
Updates the size of data if vertex is added.
*/
void LevelAnc::addVertex()
	{
	vector<int> tmp;		
	ancstr.push_back(tmp);
	lev.push_back(-1);
	}
	

/* 
Updates level, LCA, LA info of vertices in subtree 
rooted at vertex with index x
Assumes values of its parents are updated.
Param: <x> index of root of subtree
Return: 1 if success
	-1 Else
*/
int LevelAnc::updateT(int x)	
	{
        //if(x==-100) {printf("DEBUG_updT:: TreeSize %u\n",(*T).size());return 1;}	
	#ifndef FAST
	clock_t begT= clock();

	if(!(*T).validV(x))
		ERR_MSGR("LevelAnc::updateT Invalid Args\n",-1);
	#endif
		
	update(x);

	list<int> :: const_iterator it;
	list<int> adj = (*T).child(x);
	#ifndef FAST
	Logger::incrLATime(((double) clock()-begT)/CLOCKS_PER_SEC );
	#endif

	for(it= adj.begin(); it!= adj.end(); it++)
		updateT(*it);
	return 1;			
	}
	
/*
Return Level of vertex with index x
Param: <x> index of the vertex
Return:	Level of the vertex if valid
	-1 if invalid
*/
int LevelAnc::level(int x )
	{
	#ifndef FAST
	if(!(*T).validV(x))
		ERR_MSGR("LevelAnc::level Invalid Args",-1);
	#endif
	return lev[x];
	}

/*
Level ancestor of vertex at level Level(x)-k
Param: <x> index of the vertex, <k> corresponding level
Return: LA of vertex with index x at level Level(x)-k
	-1 if invalid
*/
int LevelAnc::la(int x,int k)
	{
	#ifndef FAST
	clock_t begT= clock();
	if(!(*T).validV(x))
		ERR_MSGR("LevelAnc::la Invalid Args",-1);
	#endif
       
	int dist;
	for(dist=0;(1<<dist)<=k;dist++);
	dist--;

	while(k)
		{
		int i;
		for(i=dist;(1<<i)>k;i--);
		x = ancstr[x][i];
		k -= (1<<i);
		dist = i-1;
		}
	#ifndef FAST
	Logger::incrLATime(((double) clock()-begT)/CLOCKS_PER_SEC );
	#endif

	return x;
	}


/*
LCA of vertices with index x and y
Param: <x>, <y> index of vertices whose LCA required.
Return: Corresponding LCA
	-1 if invalid
*/
int LevelAnc::lca(int x,int y )
	{
	#ifndef FAST
 	clock_t begT = clock();	
       // printf("DEBUG_LCA:: TreeSize %u\n",(*T).size());	
	if(!(*T).validV(x) || !(*T).validV(y))
		ERR_MSGR("LevelAnc::la Invalid Args",-1);
	#endif
		
	if(lev[x] > lev[y])
		x = la(x,lev[x]-lev[y]);
	else if(lev[x]<lev[y])
		y = la(y,lev[y]-lev[x]);
	
        int dist;
	for(dist=0;(1<<dist)<=lev[x];dist++);
	dist--;

	while(x!=y)
		{
		while((1<<dist)>lev[x]) dist--;
		if((*T).par(x) == (*T).par(y))  {x=y=(*T).par(x);}
		else 
			{
			int i;
			for(i=dist;ancstr[x][i]==ancstr[y][i];i--);

			x=ancstr[x][i];
			y=ancstr[y][i];	
			dist = i-1;						
			}
		}
	#ifndef FAST
	Logger::incrLATime(((double) clock()-begT)/CLOCKS_PER_SEC );
	#endif
	return x;
	}
