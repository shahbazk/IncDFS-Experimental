/*******************************************************************************
 * src/IncSDFS.cpp
 * Implementation of the SDFS algorithm.
 * Opt = 0 : SDFS 
 * Opt = 2 : SDFS2
 * Opt = 4 : SDFS3 
 *
 * Part of Project: Incremental-DFS-Experimental: 
 *                  https://github.com/shahbazk/IncDFS-Experimental
 *
 * Copyright (C) 2017 Shahbaz Khan <shahbazk@cse.iitk.ac.in>
 *
 * All rights reserved. Published under the BSD-2 license in the LICENSE file.
 ******************************************************************************/

#include"../include/IncSDFS.h"


/*
Deletes the edges of vertices on the stick
*/
void IncSDFS::delEdges(int pbroom,int broom)
	{
	list<int> chld;
	list<int>::iterator it;

	while(pbroom!=broom)
		{
		chld = G.nghbr(pbroom);
		for(it=chld.begin();it!=chld.end();it++)
			if(((*it) != T.par(pbroom)) && ((*it)!= T.child(pbroom).front()))
				G.remEdge(pbroom, *it);
		pbroom = T.child(pbroom).front();			
		}
	}

/*
Reports if the new edge <x,y> is a back edge
Param: Edge <x,y>
Return: 1 if back edge
	0 if cross edge
*/

int IncSDFS::isBackEdge(int x, int y)
	{
	if((!dir && ((LA.lca(x,y)==x) || (LA.lca(x,y)==y))) ||
	   (dir && ((DFN[x]>DFN[y])|| (LA.lca(x,y)==y))))
		return 1;
	else return 0;
	}

/*
Update RDFN of vertices in the subtree T(<root>)
Param: <root> Root of Subtree to be updated
*/
void IncSDFS::updateRDFN(int root)
	{
	list<int> chld= T.child(root);
	list<int>:: iterator it;

	RDFN[ DFN[root] ] = root;
	for(it = chld.begin(); it!= chld.end(); it++)
		updateRDFN(*it);
	}


/*
Return the end point of the new edge <x,y> in the 
smaller subtree hanging from lca
Return: <x> or <y> accordingly
*/
int IncSDFS::lca_small(int x,int y)
	{
 	int l = LA.lca(x,y);
 	list<int> chld;
 	list<int> :: iterator it,it2;
 	stack<int> st1,st2;
 
 	int z = LA.la(x,LA.level(x)-LA.level(l)-1);
 	st1.push(z);	
 	z = LA.la(y,LA.level(y)-LA.level(l)-1);
 	st2.push(z);
 
 	while(!st1.empty() && !st2.empty())
 		{
 		z = st1.top();
 		chld = T.child(z);
 		st1.pop();
 		if(!chld.empty())  
 			{
 			for(it = chld.begin();it!= chld.end();it++)
 				st1.push(*it);
 			}
 		z = st2.top();
 		chld = T.child(z);
 		st2.pop();
 		if(!chld.empty())  
 			{
 			for(it = chld.begin();it!= chld.end();it++)
 				st2.push(*it);
 			}
 		}	
 	if(st1.empty()) return x;
 	else 		return y;
	}

/*
Unset the visited flag for each vertex and delete all 
edges in the subtree T(root) of T.
Param: <root> The root of subtree that needs to be set
Return: min DFN number of the subtree
*/
int IncSDFS::unsetT(int root)
	{
	Logger::incrEP();
	Logger::incrSizeUpd();
	int mDFN = DFN[root],tDFN;

 	visited[root] = 0;
	list<int> chld = T.child(root);
	list<int> :: iterator it;
	for(it=chld.begin();it!=chld.end();it++)
		{
		T.remEdge(root,*it);
		tDFN = unsetT(*it);
		if(mDFN>tDFN) mDFN = tDFN;
		}
	return mDFN;
	}



////////////////////////
/// PUBLIC FUNCTIONS ///
////////////////////////

	
/*
Constructor:: Vertices in Graph 1,....<size> for optimality <optimality>
<directed> specifies whether given graph is directed.
Artificial Root: 0
Param: <size> Number of vertices in Graph, <opt> Optimization level
	<directed> 1 if directed, 0 otherwise
	<spaceOpt> Space Optimization
*/
IncSDFS::IncSDFS(int size,int optimality,int directed, int spaceOpt)
	{
	G = Graph(size+1,directed);
	visited.resize(size+1,0);
	DFN.resize(size+1,0);
	T = Tree(size+1);
	art_root= 0;
	opt = optimality;
	dir = directed;	
	sopt = spaceOpt;

	if(dir && opt>=4)	RDFN.resize(size+2,0);

	for(int i=0; i<G.sizeV();i++)
		if(i!= art_root)   G.addEdge(art_root,i);
		
	int dfn =1,unv=size+1;
	SDFS::DFS(art_root,T,G,visited,DFN,dfn,unv);
	T.setRoot(art_root);
	LA = LevelAnc(T);
	broomE = LA.broomEnd(art_root);
	if(dir && opt>=4)	updateRDFN(art_root);
	}

/*
Get the constant refernce to the current DFS tree
Return: Tree& <T>
*/
const Tree& IncSDFS::getT() const
	{return T;}

/*
Get the constant refernce to the current Graph  
Return: Tree& <G>
*/
const Graph& IncSDFS::getG() const
	{return G;}


/*
Adds an edge <x,y> to the graph
Params: Index of end vertices <x,y>
Return: 1 if success
	0 otherwise 
*/
int IncSDFS::addEdge(int x, int y)
	{
	int root=art_root;	// Root of the tree for DFS
	int rootU = art_root;	// Root of the tree to be unset
	int parU = -1;		// Parent of rootU
	int parR = -1;		// Parent of rootR
	int dfn = -1;

	if((sopt == 0) || (opt == 0) || 
	  ((LA.level(x)>LA.level(broomE)) 
				      && (LA.level(y)>LA.level(broomE))))
		G.addEdge(x,y);
	Logger::incrEP();

	if(opt>0 && isBackEdge(x,y))
		return 1;
	
	Logger::incrCE();

	if(opt == 2)  	{rootU=root= broomE; parU=parR=T.par(broomE);}
	if(opt == 3)	{rootU=root= LA.lca(x,y); parU=parR=T.par(root);}
	if(opt >= 4 && dir){
		if(opt == 4)	return addCrossEdgeDirOpt4(x,y);
		else		return addCrossEdgeDirOpt5(x,y);}
	if(opt >= 4 && !dir){
  		if(opt== 5)	{root = x;	parR = y;}
  		else 	{root = lca_small(x,y); parR = (root==x? y:x);}
  		 parU = LA.lca(x,y);
  		rootU = LA.la(root, LA.level(root)-LA.level(parU)-1);}

	if(parU != -1)		T.remEdge(parU,rootU);

	Logger::resetSizeUpd();
	dfn = unsetT(rootU);		
	int unv = Logger::getSizeUpd();

	if(parR != -1)
		T.addEdge(parR,root);
	SDFS::DFS(root,T,G,visited,DFN,dfn,unv);
	LA.updateT(root);

	int pBroomE = broomE; 
	broomE = LA.broomEnd(broomE);

	if(sopt>1 && opt>0)		// Delete edges on broom Stick
		delEdges(pBroomE,broomE);

	return 1;
	}

/*
Adds a list of edges list<x,y> to the graph
Params: Index of end vertices list <x,y>
Return: 1 if success
	0 otherwise 
*/
int IncSDFS::addEdgeS(list<edg> edgAdd)
	{
	int x,y,flag=0;
	list<edg>:: iterator it;

	for(it=edgAdd.begin();it!= edgAdd.end();it++)
		{
		x = (*it).first; 	y=(*it).second;
		if(opt>2)
			addEdge(x,y);
		else	{
			if((sopt == 0) || (opt == 0) || 
				  ((LA.level(x)>LA.level(broomE)) 
				   && (LA.level(y)>LA.level(broomE))))
				G.addEdge(x,y);
			Logger::incrEP();
			if(!isBackEdge(x,y))	flag=1;
			}
		}
	if(opt>2 || (opt>0 && flag==0))
		return 1;
	
	Logger::incrCE();
	int root=art_root;	// Root of the tree for DFS
	int rootU = art_root;	// Root of the tree to be unset
	int parU = -1;		// Parent of rootU
	int parR = -1;		// Parent of rootR
	int dfn = -1;

	if(opt == 2)  	{rootU=root= broomE; parU=parR=T.par(broomE);}

	if(parU != -1)		T.remEdge(parU,rootU);

	Logger::resetSizeUpd();
	dfn = unsetT(rootU);		
	int unv = Logger::getSizeUpd();

	if(parR != -1)
		T.addEdge(parR,root);
	SDFS::DFS(root,T,G,visited,DFN,dfn,unv);
	LA.updateT(root);

	int pBroomE = broomE; 
	broomE = LA.broomEnd(broomE);

	if(sopt>1 && opt>0)		// Delete edges on broom Stick
		delEdges(pBroomE,broomE);

	return 1;
	}


/*
Adds a cross edge <x,y> to the digraph with OPT 4
Params: Index of end vertices <x,y>
Return: 1 if success
	0 otherwise 
*/
int IncSDFS::addCrossEdgeDirOpt4(int x, int y)
	{
	int root = y;		// Root of the tree for DFS
	int parR = x;		// Parent of rootR
	int parU = LA.lca(x,y);	// Parent of rootU
	int rootU = LA.la(root, LA.level(root)-LA.level(parU)-1);
				// Root of the tree to be unset
	int dfn,dfnC;

	list<ppii> unSetDFS;	// List of subtrees to be unset and DFS performed
				//  (root,parR),(rootU,parU)
	list<ppii> :: iterator it;

	dfn = DFN[x];
	dfnC = DFN[x]+1;	

	int currR = T.par(x);
	while(LA.lca(RDFN[dfnC],rootU) != rootU)
		{
		if(RDFN[dfnC]!= currR)
			{
			while(T.par(RDFN[dfnC]) != currR) dfnC++;
			unSetDFS.push_back(ppii(pii(RDFN[dfnC],currR),
					pii(RDFN[dfnC],currR)));		
			dfnC++;
			}  // Check Special cAse of T(rootU)
		else if(currR!=art_root)
			{
			currR = T.par(currR);
			dfnC++;	
			}
		}

	unSetDFS.push_back(ppii(pii(rootU,parU),pii(rootU,parU)));

	Logger::resetSizeUpd();
        for(it = unSetDFS.begin();it!= unSetDFS.end();it++)
		{
		T.remEdge((*it).second.second,(*it).second.first);
		unsetT((*it).second.first);		
		}	
	int unv = Logger::getSizeUpd();

	unSetDFS.push_front(ppii(pii(root,parR),pii(rootU,parU)));

	currR = x;
        for(it = unSetDFS.begin();it!= unSetDFS.end();)
		{
		if(!visited[(*it).first.first]) 
			{
			T.addEdge((*it).first.second,(*it).first.first);
			SDFS::DFS((*it).first.first,T,G,visited,DFN,dfn,unv);
			updateRDFN((*it).first.first);
			LA.updateT((*it).first.first);
			}
		it++;
		while((currR != parU) &&   (it== unSetDFS.end() || (*it).second.second != currR))
			{
			DFN[currR] = dfn;
			RDFN[dfn++] = currR;
			currR = T.par(currR);
			}	
		}	

	int pBroomE = broomE; 
	broomE = LA.broomEnd(broomE);

	if(sopt>1 && opt>0)		// Delete edges on broom Stick
		delEdges(pBroomE,broomE);

	
	return 1;

	}
/*
Adds a cross edge <x,y> to the digraph with OPT 4
Params: Index of end vertices <x,y>
Return: 1 if success
	0 otherwise 
*/
int IncSDFS::addCrossEdgeDirOpt5(int x, int y)
	{
	int root = y;		// Root of the tree for DFS
	int parR = x;		// Parent of rootR
	int parU = LA.lca(x,y);	// Parent of rootU
	int rootU = LA.la(root, LA.level(root)-LA.level(parU)-1);
				// Root of the tree to be unset
	int dfn,dfnC;

	list<ppii> unSetDFS;	// List of subtrees to be unset and DFS performed
				//  (root,parR),(rootU,parU)
	list<ppii> :: iterator it;

	dfn = DFN[x];
	dfnC = DFN[x]+1;	

	int currR = T.par(x);
	while(LA.lca(RDFN[dfnC],rootU) != rootU)
		{
		if(RDFN[dfnC]!= currR)
			{
			while(T.par(RDFN[dfnC]) != currR) dfnC++;
			unSetDFS.push_back(ppii(pii(RDFN[dfnC],currR),
					pii(RDFN[dfnC],currR)));		
			dfnC++;
			}  // Check Special cAse of T(rootU)
		else if(currR!=art_root)
			{
			currR = T.par(currR);
			dfnC++;	
			}
		}
	int dfnMAX=DFN[root];
	
	while(LA.lca(RDFN[dfnC],root) != root)
		{
		while(T.par(RDFN[dfnC]) != LA.lca(RDFN[dfnC],root)) dfnC++;
		unSetDFS.push_back(ppii(pii(RDFN[dfnC],T.par(RDFN[dfnC])),
					pii(RDFN[dfnC],T.par(RDFN[dfnC]))));		
		maxDFN(RDFN[dfnC],DFN[parU],dfnMAX);
		dfnC++;
		}
	

	while(1)
		{
		int t = dfnMAX;
		maxDFN(RDFN[dfnMAX],DFN[parU],dfnMAX);
		if(t==dfnMAX) break;
		}
	while(1)
		{
		ppii p = unSetDFS.back();
		if(LA.lca(p.first.first,RDFN[dfnMAX]) == RDFN[dfnMAX])
			unSetDFS.pop_back();
		else 
			break;
		
		}
	unSetDFS.push_back(ppii(pii(RDFN[dfnMAX],T.par(RDFN[dfnMAX])),
				pii(RDFN[dfnMAX],T.par(RDFN[dfnMAX]))));		

	Logger::resetSizeUpd();
        for(it = unSetDFS.begin();it!= unSetDFS.end();it++)
		{
		T.remEdge((*it).second.second,(*it).second.first);
		unsetT((*it).second.first);		
		}	
	int unv = Logger::getSizeUpd();

	unSetDFS.push_front(ppii(pii(root,parR),pii(rootU,parU)));
	
	currR = x;
        for(it = unSetDFS.begin();it!= unSetDFS.end();)
		{
		if(LA.lca((*it).first.second,parR)!= (*it).first.second)
			break;		
		if(!visited[(*it).first.first]) 
			{
			T.addEdge((*it).first.second,(*it).first.first);
			SDFS::DFS((*it).first.first,T,G,visited,DFN,dfn,unv);
			updateRDFN((*it).first.first);
			LA.updateT((*it).first.first);
			}
		it++;
		while((currR != parU) &&   (it== unSetDFS.end() || (*it).second.second != currR))
			{
			DFN[currR] = dfn;
			RDFN[dfn++] = currR;
			currR = T.par(currR);
			}	
		}	
        for(;it!= unSetDFS.end();)
		{
		if(!visited[(*it).first.first]) 
			{
			T.addEdge((*it).first.second,(*it).first.first);
			SDFS::DFS((*it).first.first,T,G,visited,DFN,dfn,unv);
			updateRDFN((*it).first.first);
			LA.updateT((*it).first.first);
			}
		it++;
		}	



	int pBroomE = broomE; 
	broomE = LA.broomEnd(broomE);

	if(sopt>1 && opt>0)		// Delete edges on broom Stick
		delEdges(pBroomE,broomE);

	return 1;

	}

void IncSDFS::printDFN(int root)
{
list<int> ch  = G.nghbr(root);
list<int>:: iterator it;
for(it= ch.begin();it!=ch.end();it++)
	assert((DFN[*it] < DFN[root]) || (LA.lca(root,*it)==*it));

ch  = T.child(root);
for(it = ch.begin();it!=ch.end();it++)
	{
	assert(DFN[*it]< DFN[root]);
	assert(visited[*it]==1);
	printf("%d(%d)->%d(%d) ",root,DFN[root],*it,DFN[*it]);
	printDFN(*it);
	}

}

void IncSDFS::maxDFN(int root, int dfnL, int& dfnMAX)
	{
	if(visited[root]==2) return;
	visited[root] = 2;
	list<int> ngh = G.nghbr(root);
	list<int>:: iterator it;
	for(it= ngh.begin();it!=ngh.end();it++)
		if(DFN[*it]<dfnL && DFN[*it]> dfnMAX)
			dfnMAX = DFN[*it];
	ngh = T.child(root);
	for(it= ngh.begin();it!=ngh.end();it++)
		maxDFN(*it,dfnL,dfnMAX);
	}
