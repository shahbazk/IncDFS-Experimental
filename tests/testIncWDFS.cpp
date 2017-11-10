/*******************************************************************************
 * tests/testIncWDFS.cpp
 * Exhaustive tests on random graphs to verify the correctness of codes for 
 * Incremental WDFS algorithm <IncWDFS.cpp> 
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
#include"../include/FTDFS.h"
#include"../include/IncWDFS.h"
#include"../include/VerifyDFS.h"
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
int t=TST,flag=0;
#ifdef LOGGED
int ep;
#endif

unsigned int seed = time(NULL);
srand(seed);

printf("Testing IncWDFS.cpp on %d vertices and %d edges %d times,Seed(%u)\n",N,M,t,seed);
printf("========================================================\n");

while(t--)
	{
	unsigned int seed = rand();//time(NULL);
	srand(seed);
	
//	printf("Testing IncWDFS.cpp on %d vertices and %d edges,Seed(%u)\n",N,M,seed);
//	printf("========================================================\n");
	
	
	G = Graph(N+1);

	Logger::resetEP();

	IncWDFS incDFS = IncWDFS(N,0);
	
	#ifdef LOGGED
	printf("Edges Probed while Init IncWDFS %d\n",Logger::getEP());
	ep=0;
	#endif	
	
	


	int x = M;
	while(x--)
		{
		int e1,e2;
		if(x%21==0)
			{
			list<edg> edgS;
			while((x+10)%21==0)
				{
				e1 = rand()%(N)+1;
				e2 = rand()%(N)+1;
				if(e1!=e2 && !G.validE(e1,e2))
					{
					G.addEdge(e1,e2);
					edgS.push_back(edg(e1,e2));
					#ifdef SHWGRPH
					printf("\nInserting edge (%d,%d)\n----------------------\n",e1,e2);
					#endif
					x--;
					}
				}
			incDFS.addEdgeS(edgS);
			#ifdef SHWTREE
			printf("DFS Tree %d size: ",incDFS.getT().size());
			incDFS.getT().printT(0);
			printf("\n Broom Stick Lenght %d\n",  
				Tree::calcBStick(incDFS.getT(),0).second+1);
			#endif
			if(VerifyDFS::Verify_DFS(G,incDFS.getT())==0)
				flag=1;
			#ifdef LOGGED
			printf("Edges Probed in Update %d, total %d\n",
				Logger::getEP()-ep, Logger::getEP());
			ep=Logger::getEP();
			#endif	
			}
		else
			{
			e1 = rand()%(N)+1;
			e2 = rand()%(N)+1;
			if(e1!=e2 && !G.validE(e1,e2))
				{
				G.addEdge(e1,e2);
				#ifdef SHWGRPH
				printf("\nInserting edge (%d,%d)\n----------------------\n",e1,e2);
				#endif
				incDFS.addEdge(e1,e2);
				#ifdef SHWTREE
				printf("DFS Tree %d size: ",incDFS.getT().size());
				incDFS.getT().printT(0);
				printf("\n Broom Stick Lenght %d\n",  Tree::calcBStick(incDFS.getT(),0).second+1);
				
				#endif
				if(VerifyDFS::Verify_DFS(G,incDFS.getT())==0)
					flag=1;
				#ifdef LOGGED
				printf("Edges Probed in Update %d, total %d\n",Logger::getEP()-ep,
					Logger::getEP());
				ep=Logger::getEP();
				#endif	
				}
			}
		}
	}
if(flag) 
	{printf("IncWDFS Verification failed\n\n"); assert(0);}
else
	printf("IncWDFS Verified\n\n");
return 0;
}
