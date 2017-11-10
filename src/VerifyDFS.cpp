/*******************************************************************************
 * src/VerifyDFS.cpp
 * Implementation of procedure verifying the correctness of the computed DFS tree
 *
 * Part of Project: Incremental-DFS-Experimental: 
 *                  https://github.com/shahbazk/IncDFS-Experimental
 *
 * Copyright (C) 2017 Shahbaz Khan <shahbazk@cse.iitk.ac.in>
 *
 * All rights reserved. Published under the BSD-2 license in the LICENSE file.
 ******************************************************************************/


#include"../include/VerifyDFS.h"

/*
Verifies whether subtree of <T> rooted at <root> 
is a valid DFS subtree of <G>
Param: <G> graph, <T> DFS Tree, <LA> Level Anc Structure, <root>, <DFN> DFN no. of visited vertice,
	<visit> Number of visited vertices
Return:	1 if Correct
	0 if incorrect
*/
int VerifyDFS::testT(const Graph& G,const Tree& T, LevelAnc& LA, int root, int DFN[],int &visit)
{
DFN[root] = visit++;
list<int> nghbr = G.nghbr(root);
list<int> :: iterator it;
for(it = nghbr.begin();it!=nghbr.end();it++)
	if(!T.validV(*it) || 
		(!G.isDirected() && !(LA.lca(root,*it) == root || LA.lca(root,*it) == *it)) ||
		(G.isDirected() && !(DFN[*it]<DFN[root] || LA.lca(root,*it)==root)) )
		{
		#ifdef TEST_VERIFY_DFS
		printf("Test Verify DFS:: D%d LCA%d DFN%d Visit%d\n",G.isDirected(),LA.lca(root,*it),
									DFN[*it],visit);
		
		printf("VerifyDFS::testT Invalid/Cross Edge (%d,%d)\n",root,*it);
		#endif
		return 0;
		} 

nghbr = T.child(root);
for(it = nghbr.begin();it!=nghbr.end();it++)
	if(!testT(G,T,LA,*it,DFN,visit)) return 0;
return 1;
}


/*
Verifies whether <T> is a valid DFS tree of <G>
Param: <G> graph, <T> DFS Tree
Return:	1 if Correct
	0 if incorrect
*/
int VerifyDFS::Verify_DFS(const Graph& G,const Tree& T)
{
LevelAnc LA = LevelAnc(T);
int DFN[G.sizeV()],visit=0;
for(int i=0;i<G.sizeV();i++) 
	{DFN[i]=0;
	}

return testT(G,T,LA,T.root(),DFN,visit);
}


