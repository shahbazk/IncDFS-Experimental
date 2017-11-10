/*******************************************************************************
 * tests/testTreeGraph.cpp
 * Exhaustive tests on random graphs to verify the correctness of codes for 
 * operations on Trees and Graphs <Tree.cpp,Graph.cpp> 
 * 
 * Part of Project: Incremental-DFS-Experimental: 
 *                  https://github.com/shahbazk/IncDFS-Experimental
 *
 * Copyright (C) 2017 Shahbaz Khan <shahbazk@cse.iitk.ac.in>
 *
 * All rights reserved. Published under the BSD-2 license in the LICENSE file.
 ******************************************************************************/

#include<cstdio>
#include<cstdlib>
#include<ctime>
#include<cmath>
#include<list>

#include"../include/Graph.h"
#include"../include/Tree.h"
#include"../include/VerifyDFS.h"

//#define SHWTREE               // Showing intermediate steps of constructed tree
//#define SHWGRPH               // Showing intermediate steps of constructed graph

using namespace std;

#define N 10			// Number of Vertices
#define M ((int) (N*sqrt(N)))   // Number of Edges

Graph G;
Tree T;


// DFS Params
int visited[N]={0};
int testV,testE;



/*
Static DFS algorithm 
*/
void DFS(int root)
	{
	list<int> adgL = G.nghbr(root);
	list<int> :: iterator it;
	visited[root]=1;
	testV++;

	for(it=adgL.begin();it!=adgL.end();it++)
		{
		testE++;
		if(!visited[*it]) 	
			{
			T.addEdge(root,*it);
			#ifdef SHWTREE
			printf("Tree: (%d,%d)\n",root,*it);
			#endif
			DFS(*it);
			}		
		}
	}





int main()
{
unsigned int seed = time(NULL);
int i,j;
srand(seed);

printf("Testing Tree.cpp and Graph.cpp on %d vertices and %d edges\n",N,M);
printf("=============================================================\n");


T = Tree(N);
G = Graph(N,0);

for(i=1;i<N;i++)
	{
	G.addEdge(0,i);

	#ifdef SHWGRPH
	printf("Graph: (%d,%d)\n",0,i);
	#endif

	for(j=i+1;j<N;j++)
		{
		if(rand()%(N*N)<M) 
			{

			#ifdef SHWGRPH
			printf("Graph: (%d,%d)\n",i,j);
			#endif



			G.addEdge(i,j);
			}	
		}
	}

#ifdef SHWGRPH
printf("Graph %d Vertices %d Edges, %u Seed\n",G.sizeV(),G.sizeE(),seed);
#endif

testV=testE=0;
DFS(0);
T.setRoot(0);
#ifdef SHWTREE
printf("Tree %d Vertices %d Root, TestV %d, TestE %d \n",T.size(),T.root(),testV,testE/2);
#endif
if(testV==G.sizeV() && testE/2 == G.sizeE() && VerifyDFS::Verify_DFS(G,T))
	printf("Tree and Graph Verified (Undirected)\n");
else 	
	printf("Tree and Graph Verification (Undirected) FAILED!!!\n");


/////////////////////////////////////////////////////////////
/////////////////// TESTING DIRECTED GRAPH //////////////////
/////////////////////////////////////////////////////////////


T = Tree(N);
G = Graph(N,1);

for(i=1;i<N;i++)
	{
	visited[i]=0;
	G.addEdge(0,i);

	#ifdef SHWGRPH
	printf("Graph: (%d,%d)\n",0,i);
	#endif

	for(j=1;j<N;j++)
		{
		if(rand()%(N*N)<M) 
			{

			#ifdef SHWGRPH
			printf("Graph: (%d,%d)\n",i,j);
			#endif



			G.addEdge(i,j);
			}	
		}
	}

#ifdef SHWGRPH
printf("Graph %d Vertices %d Edges, %u Seed\n",G.sizeV(),G.sizeE(),seed);
#endif

testV=testE=0;
DFS(0);
T.setRoot(0);
#ifdef SHWTREE
printf("Tree %d Vertices %d Root, TestV %d, TestE %d \n",T.size(),T.root(),testV,testE/2);
#endif
if(testV==G.sizeV() && testE == G.sizeE() && VerifyDFS::Verify_DFS(G,T))
	printf("Tree and Graph Verified (Directed)\n\n");
else 	
	printf("Tree and Graph Verification (Directed) FAILED!!!\n\n");


return 0;
}
