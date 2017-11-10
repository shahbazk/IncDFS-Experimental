/*******************************************************************************
 * src/StaticDFS.cpp
 * Implementation of the standard algorithm for computing DFS in the static setting.
 *
 * Part of Project: Incremental-DFS-Experimental: 
 *                  https://github.com/shahbazk/IncDFS-Experimental
 *
 * Copyright (C) 2017 Shahbaz Khan <shahbazk@cse.iitk.ac.in>
 *
 * All rights reserved. Published under the BSD-2 license in the LICENSE file.
 ******************************************************************************/

#include"../include/StaticDFS.h"

//#define DFS_INTERRUPT         // For implementing SDFS-Int

//#define SHWTREE
//#define SHWGRPH

////////////////////////
/// Public Functions ///
////////////////////////

/*
Static DFS algorithm, performs DFS traversal from the root
Param: <root> Index of root, reference of <T>,<G> and
two vectors for <visited> and <DFN> no. (post order no.) and current <dfn>
<unvisited> count of unvisited vertices
Return: 1 if completed
	0 else
*/
int SDFS::DFS(int root, Tree& T, const Graph& G, vector<int>& visited, vector<int>& DFN,
		int& dfn,int& unvisited)
{
	const list<int> &adgL = G.nghbrR(root);
	list<int> :: const_iterator it;
	visited[root]=1;
	Logger::incrEP();	
	#ifdef DFS_INTERRUPT
	unvisited--;
	if(!unvisited) {DFN[root]=dfn++; return 1;}
	#endif	


	for(it=adgL.begin();it!=adgL.end();it++)
		{
		Logger::incrEP();	
		if(!visited[*it]) 	
			{
			T.addEdge(root,*it);
			#ifdef SHWTREE
			printf("Tree: (%d,%d)\n",root,*it);
			#endif
			#ifdef DFS_INTERRUPT
			if(DFS(*it,T,G,visited,DFN,dfn,unvisited))
				{DFN[root]=dfn++; return 1;}
			#else
			DFS(*it,T,G,visited,DFN,dfn,unvisited);
			#endif
			}		
		}
	DFN[root] = dfn++;	
	return 0;
	}

/*
Static Function that returns DFS tree of a graph from the root
Param: <@Grph> Reference of Grph, <root>Index of Root
Return: DFS tree of the graph at root
*/
Tree SDFS::StaticDFS(const Graph& Grph, int root)
	{
	vector<int> visited;		// Bit Vector for Visited in DFS
	vector<int> DFN;
	int dfn=1;
	int unvisited = Grph.size();
	visited.resize(Grph.size(),0);
	DFN.resize(Grph.size(),0);
	Tree T = Tree(Grph.size());
	DFS(root,T,Grph,visited,DFN,dfn,unvisited);
	T.setRoot(root);
	return T;	
	}

