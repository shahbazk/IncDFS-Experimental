/*******************************************************************************
 * src/IncWDFS.cpp
 * Implementation of the WDFS algorithm.
 *
 * Part of Project: Incremental-DFS-Experimental: 
 *                  https://github.com/shahbazk/IncDFS-Experimental
 *
 * Copyright (C) 2017 Shahbaz Khan <shahbazk@cse.iitk.ac.in>
 *
 * All rights reserved. Published under the BSD-2 license in the LICENSE file.
 ******************************************************************************/

#include"../include/IncWDFS.h"

void IncWDFS::test(int x)
{
printf("Status IncWDFS %d ::",x);
LA.updateT(-100);
}

/*
Updates the DS[!cDS] using build part, with required conversions if completed.
*/
void IncWDFS::updateDS()
	{
	if(DS[!cDS].buildPart(G,cost_upd))
		{	// Updatation of DS[!cDS] completed
		list<edg> :: iterator it = edgAdd[!cDS].begin();
		for(it=edgAdd[!cDS].begin();it!=edgAdd[!cDS].end();it++)
			{G.addEdge((*it).first,(*it).second);}
	
		Logger::incrEP(edgAdd[!cDS].size());

		cDS = !cDS;
		DS[!cDS] = DS_WDFS(T);
		DS[!cDS].buildPart(G,cost_upd);
		edgAdd[!cDS].clear();

		if(opt>0) //AVOID_BACK_EDGE
		for(it=edgAdd[cDS].begin();it!=edgAdd[cDS].end();)
			{
			if(DS[cDS].isAnc((*it).first,(*it).second) || 
			   DS[cDS].isAnc((*it).second,(*it).first))
				{
				DS[cDS].addBEdge((*it).first,(*it).second);
				it = edgAdd[cDS].erase(it);
				}
			else break;
			}
		}
	}

/*
Updates the DFS tree <T> based on edgAdd lists
*/
void IncWDFS::updateDFS()
	{
	dataFTDFS dFT;
	FTDFS::initDataFTDFS(DS[cDS],dFT,G.size(),art_root);

	list<edg> :: iterator it;
	it = edgAdd[cDS].begin();
	
	Logger::incrEP(edgAdd[cDS].size());

	while(it!= edgAdd[!cDS].end())
		{
		if(it == edgAdd[cDS].end())
			{it = edgAdd[!cDS].begin(); continue;} 
		dFT.redAdj[(*it).first ].push_back((*it).second);
		dFT.redAdj[(*it).second].push_back((*it).first );
		it++;
		}

	T = FTDFS::FT_DFS(DS[cDS],dFT,art_root);

	}


////////////////////////
/// PUBLIC FUNCTIONS ///
////////////////////////

/*
Constructor:: Vertices of the graph 1,...,<size>
Artificial Root: 0, Optimality Level <optimality>
Time: O(m \log n)
Param: <size> Number of vertices in the graphs
*/
IncWDFS::IncWDFS(int size,int optimality)
	{
	G = Graph(size+1);
	art_root = 0;
	cost_upd = size;
	opt = optimality;

	for(int i=0; i<G.sizeV();i++)
		if(i!= art_root)   G.addEdge(art_root,i);
	
	T = SDFS::StaticDFS(G,art_root);
	
	LA = LevelAnc(T);

	cDS = 0;                   // Current Data Structure

	DS[cDS] = DS_WDFS(T);
	while(!DS[cDS].buildPart(G,cost_upd));

	DS[!cDS] = DS_WDFS(T);
	DS[!cDS].buildPart(G,cost_upd);
	}


/*
Get the constant refernce to the current DFS tree
Return: Tree& <T>
*/
const Tree& IncWDFS::getT() const 
	{
	return T;	
	}

/*
Get the constant refernce to the current Graph for VERTEX INFO ONLY 
Return: Tree& <G>
*/
const Graph& IncWDFS::getG() const
	{	return G;	}


/*
Adds an edge <x,y> to the graph
Params: Index of end vertices <x,y>
Return: 1 if success
	0 otherwise 
*/
int IncWDFS::addEdge(int x, int y)
	{
	if(!G.validV(x) || !G.validV(y) || x==art_root || y==art_root) 
		ERR_MSGR("IncWDFS::addEdge Invalid Update args\n",0);
		
	if(x>y) {int t; t=x;x=y;y=t;}
	edg e = edg(x,y);

	if(opt>0)	
	{
	// Back edge in orginal and current tree.
	if(edgAdd[cDS].empty()  && edgAdd[!cDS].empty() && 
	  (LA.lca(e.first,e.second)==e.first || LA.lca(e.first,e.second)==e.second))
		{
		G.addEdge(e.first,e.second);
		DS[cDS].addBEdge(e.first,e.second);
		DS[!cDS].addBEdge(e.first,e.second);	
	        return 1;	
		}
	} 

	Logger::incrEP(edgAdd[cDS].size()+edgAdd[!cDS].size());

	if(find(edgAdd[cDS].begin(), edgAdd[cDS].end(), e) == edgAdd[cDS].end() &&
	   find(edgAdd[!cDS].begin(),edgAdd[!cDS].end(), e) == edgAdd[!cDS].end() &&
	   !G.validE(x,y)) 	
			edgAdd[!cDS].push_back(e);
	else
		ERR_MSGR("IncWDFS::addEdge Edge already present\n",0);

	if(opt>0)
		{
		if(LA.lca(e.first,e.second)==e.first || LA.lca(e.first,e.second)==e.second)
			{
			if(edgAdd[cDS].size()+ edgAdd[!cDS].size() > (unsigned int) cost_upd)
				updateDS();
			return 1;
			}
		}

	Logger::incrCE();

	updateDFS();
 	updateDS();	
	LA.updateT(T.root());
	return 1;
	}


/*
Adds an edge set list<x,y> to the graph
Params: Index of end vertices list<x,y>
Return: 1 if success
	0 otherwise 
*/
int IncWDFS::addEdgeS(list<edg> edgAdded)
	{
	int x,y,flag=0;
	list<edg> :: iterator it;

	for(it=edgAdded.begin();it!= edgAdded.end();it++)
		{
		x = (*it).first; 	y=(*it).second;
		if(!G.validV(x) || !G.validV(y) || x==art_root || y==art_root) 
			ERR_MSGR("IncWDFS::addEdge Invalid Update args\n",0);
			
		if(x>y) {int t; t=x;x=y;y=t;}
		edg e = edg(x,y);

		if(opt>0)	
		{
			// Back edge in orginal and current tree.
			if(edgAdd[cDS].empty()  && edgAdd[!cDS].empty() && 
		  	(LA.lca(e.first,e.second)==e.first || 
			 LA.lca(e.first,e.second)==e.second))
			{
				G.addEdge(e.first,e.second);
				DS[cDS].addBEdge(e.first,e.second);
				DS[!cDS].addBEdge(e.first,e.second);	
			        return 1;	
			}
		} 

		Logger::incrEP(edgAdd[cDS].size()+edgAdd[!cDS].size());

		if(find(edgAdd[cDS].begin(),edgAdd[cDS].end(),e) 
			== edgAdd[cDS].end() &&
		   find(edgAdd[!cDS].begin(),edgAdd[!cDS].end(), e) 
			== edgAdd[!cDS].end() && !G.validE(x,y)) 	
			edgAdd[!cDS].push_back(e);
		else
			ERR_MSGR("IncWDFS::addEdge Edge already present\n",0);

		if(opt>0)
			{
			if(LA.lca(e.first,e.second)==e.first || 
			   LA.lca(e.first,e.second)==e.second)
				{
				if(edgAdd[cDS].size()+ edgAdd[!cDS].size() 
					> (unsigned int) cost_upd)
					updateDS();
				}
			else flag = 1;
			}

		}
	if(opt>0 && flag == 0)
		return 1;

	Logger::incrCE();
	updateDFS();
 	updateDS();	
	LA.updateT(T.root());
	return 1;
	}
