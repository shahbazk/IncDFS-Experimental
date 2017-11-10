/*******************************************************************************
 * src/IncFDFS.cpp
 * Implementation of the FDFS algorithm.
 *
 * Part of Project: Incremental-DFS-Experimental: 
 *                  https://github.com/shahbazk/IncDFS-Experimental
 *
 * Copyright (C) 2017 Shahbaz Khan <shahbazk@cse.iitk.ac.in>
 *
 * All rights reserved. Published under the BSD-2 license in the LICENSE file.
 ******************************************************************************/

#include"../include/IncFDFS.h"

/*
Update RDFN of vertices in the subtree T(<root>)
Param: <root> Root of Subtree to be updated
*/
void IncFDFS::updateRDFN(int root)
	{
	list<int> chld= T.child(root);
	list<int>:: iterator it;
	RDFN[ DFN[root] ] = root;
	for(it = chld.begin(); it!= chld.end(); it++)
		updateRDFN(*it);
	}

/*
Unset the visited flag for each vertex 
Param: <root> The root of subtree that needs to be unset
*/
void IncFDFS::unsetT(int root)
	{
	Logger::incrEP();
	Logger::incrSizeUpd();

 	visited[root] = 0;
	list<int> chld = T.child(root);
	list<int> :: iterator it;
	for(it=chld.begin();it!=chld.end();it++)
		{
		 unsetT(*it);
		}
	}



////////////////////////
/// PUBLIC FUNCTIONS ///
////////////////////////

/*
Constructor:: Vertices in Graph 1,....<size>
Artificial Root: 0
Param: <size> Number of vertices in Graph
	<dir> If Graph is directed (1) or DAG (0)
*/
IncFDFS::IncFDFS(int size,int directed) 
	{
	dir = directed;
	G = Graph(size+1,1);
	visited.resize(size+1,0);
	DFN.resize(size+1,0);
	T = Tree(size+1);
	art_root= 0;

	RDFN.resize(size+2,0);

	for(int i=0; i<G.sizeV();i++)
		if(i!= art_root)   G.addEdge(art_root,i);
		
	int dfn =1,unv=size+1;
	SDFS::DFS(art_root,T,G,visited,DFN,dfn,unv);
	T.setRoot(art_root);
	if(dir)	LA = LevelAnc(T);
	updateRDFN(art_root);
	unsetT(art_root);
	}

/*
Get the constant refernce to the current DFS tree
Return: Tree& <T>
*/
const Tree& IncFDFS::getT() const
	{return T;}

/*
Get the constant refernce to the current Graph  
Return: Tree& <G>
*/
const Graph& IncFDFS::getG() const
	{return G;}

/*
Adds an edge <u,v> to the graph
Params: Index of end vertices <u,v>
Return: 1 if success
	0 otherwise 
*/
int IncFDFS::addEdge(int u, int v)
	{
	G.addEdge(u,v);
	Logger::incrEP();
	Logger::resetSizeUpd();
	
	if(!dir && DFN[u]>DFN[v]) return 1;
	if(dir && ((DFN[u]>DFN[v])|| (LA.lca(u,v)==v)))
		return 1;
	int m = 0, maxDFN = 0;
	uv = edg(u,v);
	if(dir)
		{
		lcauv = LA.lca(u,v);
		int x = u;
		while(DFN[x]<DFN[v])
			{
			visited[x] = -1;
			x = T.par(x);
			}
		visited[x] = -1;
		}
	partialDFS(u,v,m,maxDFN);

	updateRank(m,maxDFN);

	map<int,int> :: iterator it;
	for(it= fDFN.begin();it!= fDFN.end();it++)
		{
		DFN[ it->first ] = it->second;
		RDFN[it->second] = it->first;
		} 
	
	uDFN.clear();
	fDFN.clear();
	visited[lcauv] = 0;
	if(dir)
	LA.updateT(v);	
	return 1;
	}

/*
Adds an edge set list<u,v> to the graph
Params: Index of end vertices list<u,v>
Return: 1 if success
	0 otherwise 
*/
int IncFDFS::addEdgeS(list<edg> edgAdd)
	{
	list<edg> :: iterator it;
	for(it=edgAdd.begin();it!=edgAdd.end();it++)
		if(!addEdge((*it).first,(*it).second)) break;

	if(it==edgAdd.end()) 	return 1;
	else			return 0;
	}

void IncFDFS::printDFN(int root)
{
list<int> ch  = T.child(root);
list<int> ::iterator it;
for(it = ch.begin();it!=ch.end();it++)
	{
	assert(DFN[*it]< DFN[root]);
	assert(visited[*it]==0);
	printf("%d(%d)->%d(%d) ",root,DFN[root],*it,DFN[*it]);
	printDFN(*it);
	}

}



/*
Perform Partial DFS using edge<w,z>
Param: Edge<w,z>, Relative Rank m
*/
void IncFDFS::partialDFS(int w,int z,int &m,int &maxDFN)
	{
	Logger::incrEP();
	
	if(!dir && (visited[z] || DFN[z]<DFN[uv.first])) return;
	if(dir && (visited[z] || (DFN[z]<DFN[uv.first]) || 
		(DFN[z]> DFN[lcauv]))) return;

	if(maxDFN < DFN[z]) maxDFN = DFN[z];
	
	visited[z]=1;
	T.remEdge(T.par(z),z);
	T.addEdge(w,z);
	list<int> ngh = G.nghbr(z);
	list<int> :: iterator it;

	for(it=ngh.begin();it!=ngh.end();it++)
		{
		partialDFS(z,*it,m,maxDFN);
		}	

	uDFN[z] = ++m;
	}


/*
Update the DFN No. and RDFN No. of updated vertices
Param: <m> shift in DFN No.s 
*/
void IncFDFS::updateRank(int m,int maxDFN)
	{
	int shift = m,dfnU = DFN[uv.first];
	for(int i = DFN[uv.first]; i<= maxDFN; i++)	
		{
		Logger::incrSizeUpd();
		
		int x = RDFN[i];
		if(visited[x]==1)
			{
			fDFN[x] = dfnU + uDFN[x] -1;
			}
		else
			{
			fDFN[x] = dfnU + shift;
			shift++;
			}
		visited[x] = 0;
		}
	}

