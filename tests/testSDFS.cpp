/*******************************************************************************
 * tests/testSDFS.cpp
 * Exhaustive tests on random graphs to verify the correctness of codes for 
 * static DFS algorithm <StaticDFS.cpp,VerifyDFS.cpp> 
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
#include"../include/VerifyDFS.h"
#include"../include/Logger.h"

//#define SHWTREE               // Showing intermediate steps of constructed tree
//#define SHWGRPH               // Showing intermediate steps of constructed graph
//#define LOGGED                // Logging parameters related to operations 

using namespace std;

#define N 10			// Number of Vertices
#define M ((int) (N*sqrt(N)))   // Number of Edges


Graph G;
Tree T;


int main()
{
int m=1;
int t=100;         // Number of test runs

unsigned int seed =  time(NULL);
srand(seed);
printf("Testing StaticDFS.cpp on %d vertices and %d edges %d times::Seed(%d)\n",N,M,t,seed);
printf("========================================================\n");
	
while(--t)
 	{
	unsigned int seed = rand();
	int i,j;
	srand(seed);
	
//	printf("\n\nTesting StaticDFS.cpp on %d vertices and %d edges\n",N,M);
//	printf("========================================================\n");

	if(t%2)	G = Graph(N);		// Testing Undirected Graph
	else 	G = Graph(N,1);		// Testing Directed Graph
	
	for(i=1;i<N;i++)
		{
		G.addEdge(0,i);
	
		#ifdef SHWGRPH
		printf("Graph: (%d,%d)\n",0,i);
		#endif
	
		for(j=(t%2?i+1:1);j<N;j++)
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
	Logger::resetEP();
	T=SDFS::StaticDFS(G,0);
	
	#ifdef SHWGRPH
	printf("Tree %d Vertices %d Root\n",T.size(),T.root());
	#endif
	#ifdef LOGGED
	printf("Edges Probed: %d, Match 2*Edges %d\n",Logger::getEP(),
	Logger::getEP()==2*G.sizeE());	
	#endif

	if(T.size()!=G.sizeV() || VerifyDFS::Verify_DFS(G,T)==0 )
		m=0;
	}

if(m==1)
	printf("Static DFS Verified\n\n");
else	
	printf("Static DFS Verification failed\n\n");


return 0;
}
