/*******************************************************************************
 * src/IncADFS.cpp
 * Implementation of the ADFS algorithm.
 * Opt = 0 : ADFS1 
 * Opt = 1 : ADFS2
 *
 * Part of Project: Incremental-DFS-Experimental: 
 *                  https://github.com/shahbazk/IncDFS-Experimental
 *
 * Copyright (C) 2017 Shahbaz Khan <shahbazk@cse.iitk.ac.in>
 *
 * All rights reserved. Published under the BSD-2 license in the LICENSE file.
 ******************************************************************************/

/////////////////////////////////////////////////
/// For Opt1 edges in DS removed from Graph G ///
/////////////////////////////////////////////////


#include"../include/IncADFS.h"


////////////////////
/// DS FUNCTIONS ///
////////////////////

/*
 Initialize the DS based on optimality
*/
void IncADFS::initDS()
	{
	if(opt)
		{
		szE1 = 0;
		levE1 = -1;
		inDS.resize(G.size());
		E1.resize(G.sizeV()+1);
		E1v.resize(G.sizeV());
		rE1.resize(G.sizeV());
		}
	}


/*
 Adds the list of edges E to the DS
Param:	<E> list of cross edges
*/
void IncADFS::addDS(list<pii> &E)
	{
	if(opt)
		{
		list<pii> :: iterator it;
		for(it=E.begin();it!=E.end();it++)
			{
			pii e = *it;
			int l1 = LA.level(e.first), l2 = LA.level(e.second);

			if(levE1 == -1 || (levE1 < l1 && levE1< l2))		levE1 = (l1<l2? l1: l2);

			if(!inDS[e.first])
				{
				inDS[e.first]=1;
				E1[l1].push_front(e.first);
				rE1[e.first]=pair<int,list<int>::iterator>(l1,E1[l1].begin()); 
				}

			if(!inDS[e.second])
				{
				inDS[e.second]=1;
				E1[l2].push_front(e.second);
				rE1[e.second]=pair<int,list<int>::iterator>(l2,E1[l2].begin()); 
				}

			E1v[e.first].push_front(e.second);
			E1v[e.second].push_front(e.first);
			szE1 ++;
			G.remEdge(e.first,e.second);	 		
			}
		}
	else
		{
		E0.splice(E0.end(), E);
		}
	}

/*
 Choose the next edge from DS
Return: <x,y> the next edge
*/
pii IncADFS::chooseDS()
	{	// REQUIRES emptyDS called before it
	pii e;
	if(opt)
		{
		e = pii(E1[levE1].front(), E1v[  E1[levE1].front() ].front());
		E1v[  E1[levE1].front() ].pop_front();
		G.addEdge(e.first,e.second);
		szE1--;
		}

	else
		{
		e = E0.front();
		E0.pop_front();
		}
	return e;
	}


/*
Updates the DS for vertices in T(root)
Param: <root> Root of subtree for which level of vertices have changed
*/
void IncADFS::updateTDS(int root)
	{
	if(opt)
		{
		if(inDS[root])
			{
			E1[ rE1[root].first ].erase( rE1[root].second);
			E1[LA.level(root)].push_front(root);
			rE1[root] = pair<int,list<int>::iterator>(LA.level(root), 
						E1[LA.level(root)].begin()); 
			}
		list<int> chld = T.child(root);
		list<int> :: iterator it;
		
		for(it=chld.begin(); it!=chld.end();it++)
			updateTDS(*it);
		}
	}


/*
 Check whether the DS is empty
Return: 1 if empty
	0 otherwise
*/
bool IncADFS::emptyDS()
	{
	if(opt)
		{
		while(1)
			{
			if((levE1==-1) || (szE1==0) || (levE1>G.sizeV())) {levE1=-1; break;}
			if(E1[levE1].empty()) {levE1++;continue;}
			int u = E1[levE1].front();
			if(E1v[u].empty())	{E1[levE1].pop_front();inDS[u]=0;continue;}
			int v = E1v[u].front();
			if(G.validE(u,v)) {E1v[u].pop_front(); continue;}
			break;
			}	
	 	return (levE1==-1);	
		}
	else	
		{
		return E0.empty();
		}
	}


///////////////////////////
//// INTERNAL FUNCTIONS ///
///////////////////////////


/*
Reroot T(v) by connecting (x,y) and deleting (v,par(v))
Param:	<v> Root of tree to be rerooted, new root <y>, 
	parent of new root <x>, <u> is sibling of v having x
Return: List of potential cross edges
*/
list<pii> IncADFS::reroot(int u, int v,int x,int y)
	{
	list<pii> E_r;
	
	#ifdef TEST
	assert(u>=0 && u<backEdge.size());
	assert(v>=0 && v<backEdge.size());
	#endif
	if(opt)
	backEdge[u].insert(backEdge[u].end(), backEdge[v].begin(),backEdge[v].end());
	else
	backEdge[u].splice(backEdge[u].end(), backEdge[v]);
	
	backEdge[u].push_back(pii(v,T.par(v)));

	Logger::incrEP();
	int z=y,p=x,nxt,w = T.par(v);	
	while(z!= w)
		{
		#ifdef TEST
		assert(z>=0 && z<backEdge.size());
		#endif
		if(z!= v) 
			{
			if(opt)
			E_r.insert(E_r.end(),backEdge[z].begin(),backEdge[z].end());
			else
			E_r.splice(E_r.end(),backEdge[z]);
			}
		if(opt)
		backEdge[z].clear();
		Logger::incrEP();
		nxt = T.par(z);
		T.remEdge(nxt,z);
		T.addEdge(p,z);
		p = z; z = nxt;
		}
	return E_r;
	}


/*
Build the DS for storing back edges
REQUIRED WHEN NOT STARTING FROM EMPTY GRAPH
Param: Root of the subtree <root>
*/
void IncADFS::buildBE(int root)
	{
	list<int> ngbr = G.nghbr(root);
	list<int>:: iterator it;
	for(it = ngbr.begin();it!= ngbr.end();it++)
		{
		if(LA.lca(root,*it) == *it && (*it)!= T.par(root))
			{
			backEdge[LA.la(root, LA.level(root)-LA.level(*it)-1)].push_back(pii(root,*it));
			}
		}

	list<int> chld = T.child(root);
	
	for(it = chld.begin();it!= chld.end();it++)
		buildBE(root);
	}


////////////////////////
/// PUBLIC FUNCTIONS ///
////////////////////////

/*
Constructor:: Vertices in Graph 1,....<size>
Artificial Root: 0
Param: <size> Number of vertices in Graph, Optimality <opt>
*/
IncADFS::IncADFS(int size,int optimality) 
	{
	G = Graph(size+1,0);
	
	art_root= 0;
	opt = optimality;
	backEdge.resize(size+1);

	for(int i=0; i<G.sizeV();i++)
		if(i!= art_root)   G.addEdge(art_root,i);
		
	vector<int> visited;
	visited.resize(size+1,0);
	vector<int> DFN;
	DFN.resize(size+1,0);
	T = Tree(size+1);
	art_root= 0;

	int dfn =1,unv=size+1;
	SDFS::DFS(art_root,T,G,visited,DFN,dfn,unv);
	T.setRoot(art_root);
	LA = LevelAnc(T);
	initDS();
	}

/*
Get the constant refernce to the current DFS tree
Return: Tree& <T>
*/
const Tree& IncADFS::getT() const
	{return T;}

/*
Get the constant refernce to the current Graph  
Return: Tree& <G>
*/
const Graph& IncADFS::getG() const
	{return G;}

/*
Adds an edge <u,v> to the graph
Params: Index of end vertices <u,v>
Return: 1 if success
	0 otherwise 
*/
int IncADFS::addEdge(int u, int v)
	{
	int w,x,y;

	G.addEdge(u,v);

	list<pii> E_tmp;
	E_tmp.push_back(pii(u,v));
	addDS(E_tmp);

	while(!emptyDS())	
		{
		Logger::incrEP();
		pii e = chooseDS();

		if(LA.level(e.first)<LA.level(e.second))
			e = pii(e.second,e.first);
		
		w = LA.lca(e.first,e.second);  	
		x = LA.la(e.first, LA.level(e.first)-LA.level(w)-1);
		
		if(w == e.second)	
			{
			#ifdef TEST
			assert(x>=0 && x<backEdge.size());
			#endif
			backEdge[x].push_back(e);	
			continue;	//Backedge
			}

		y = LA.la(e.second, LA.level(e.second)-LA.level(w)-1);
		E_tmp = reroot(x,y,e.first,e.second);

		addDS(E_tmp);

		LA.updateT(e.second);
		updateTDS(e.second);
		}
	
	return 1;
	}


/*
Adds a set of edges list<u,v> to the graph
Params: Index of end vertices list<u,v>
Return: 1 if success
	0 otherwise 
*/
int IncADFS::addEdgeS(list<edg> edgAdd)
	{
	int w,x,y;
	list<edg> :: iterator it;
	list<pii> E_tmp;

	for(it=edgAdd.begin();it!=edgAdd.end();it++)
		{
		G.addEdge((*it).first,(*it).second);
		E_tmp.push_back(*it);
		}
	addDS(E_tmp);
	

	while(!emptyDS())	
		{
		Logger::incrEP();
		pii e = chooseDS();

		if(LA.level(e.first)<LA.level(e.second))
			e = pii(e.second,e.first);
		
		w = LA.lca(e.first,e.second);  	
		x = LA.la(e.first, LA.level(e.first)-LA.level(w)-1);
		
		if(w == e.second)	
			{
			#ifdef TEST
			assert(x>=0 && x<backEdge.size());
			#endif
			backEdge[x].push_back(e);	
			continue;	//Backedge
			}

		y = LA.la(e.second, LA.level(e.second)-LA.level(w)-1);
		E_tmp = reroot(x,y,e.first,e.second);

		addDS(E_tmp);


		LA.updateT(e.second);
		updateTDS(e.second);
		}
	
	return 1;
	}

