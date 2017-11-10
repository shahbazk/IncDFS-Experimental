/*******************************************************************************
 * tests/testDS_WDFS.cpp
 * Exhaustive tests on random graphs to verify the correctness of codes for 
 * datastructure for Incremental WDFS algorithm <DS_WDFS.cpp> 
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
#include"../include/StaticDFS.h"
#include"../include/DS_WDFS.h"
#include"../include/Logger.h"

//#define SHWTREE               // Showing intermediate steps of constructed tree
//#define SHWGRPH               // Showing intermediate steps of constructed graph
//#define LOGGED                // Logging parameters related to operations 

using namespace std;

#define N 10			// Number of Vertices
#define M ((int) (N*sqrt(N)))   // Number of Edges
#define TST 100                 // Number of Test runs

Graph G;
Tree T;



int main()
{
int t=TST;
unsigned int seed = time(NULL);
srand(seed);

printf("Testing DS_WDFS.cpp on %d vertices and %d edges %d times:Seed(%u)\n",N,M,t,seed);
printf("========================================================\n");

while(t--)
	{
	unsigned int seed = rand();//time(NULL);
	int i,j;
	srand(seed);
	
//	printf("\n\nTesting DS_WDFS.cpp on %d vertices and %d edges: Seed(%u)\n",N,M,seed);
//	printf("========================================================\n");
	
	G = Graph(N);
	
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
	T=SDFS::StaticDFS(G,0);
	#ifdef SHWTREE
	printf("Tree %d Vertices %d Root\n",T.size(),T.root());
	#endif

	Logger::resetEP();	

	DS_WDFS DS = DS_WDFS(T);
	int m =0;
	while(!DS.buildPart(G,sqrt(G.sizeE()*G.sizeV())))  
		++m;
	#ifdef SHWTREE
	printf("DS built in %d steps\n\n",m);
	#endif
	#ifdef LOGGED
	printf("Edges Probed while building DS %d\n",Logger::getEP());
	#endif	
	
	int x = 20;
	while(x--)
		{
		pair<int,int> ans;
		if(rand()%2)
		ans=DS.query(rand()%N,rand()%N,0,rand()%2);
		else
		ans=DS.query(rand()%N,0,rand()%N,rand()%2);
	
		#ifdef SHWTREE
		printf("%d %d\n",ans.first,ans.second);
		#endif
		}

	#ifdef LOGGED
	printf("Total edges Probed for building DS and Query %d\n\n",Logger::getEP());
	#endif	
	
	}
printf("DS_WDFS Verified\n\n");

return 0;
}
