/*******************************************************************************
 * tests/testIncFDFS.cpp
 * Exhaustive tests on random graphs to verify the correctness of codes for 
 * Incremental FDFS algorithm <IncFDFS.cpp> 
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
#include"../include/IncFDFS.h"
#include"../include/VerifyDFS.h"
#include"../include/Logger.h"

//#define SHWTREE               // Showing intermediate steps of constructed tree
//#define SHWGRPH               // Showing intermediate steps of constructed graph
//#define LOGGED                // Logging parameters related to operations 

using namespace std;

#define N 10			// Number of Vertices
#define M ((int) (N*sqrt(N)))   // Number of Edges
#define TST 1000                // Number of Test runs

Graph G;
Tree T;
int connect[N+1][N+1];
int cc;

// For maintaining information of connectivity for verifying DAG property
void updateC(list<int> inV, int x)
	{
	list<int> :: iterator it;
	for(it= inV.begin();it!=inV.end();)
		{
		if(connect[*it][x])
			it = inV.erase(it);
		else
			{
			connect[*it][x] = 1;
			it++;
			}
		}
	if(inV.empty()) return;
	list<int> ngh = G.nghbr(x);
	for(it= ngh.begin();it!=ngh.end();it++)
		updateC(inV,*it);
	cc++;
	}


void updateConnect(int x, int y)
	{
	list<int> inV;
	for(int i=1;i<=N;i++)
		if(connect[i][x])
			inV.push_back(i);
	inV.push_back(x);

	updateC(inV,y);
	}


int main()
{
int t=TST,flag=0;
#ifdef LOGGED
int ep;
#endif

unsigned int seed = time(NULL);
srand(seed);

printf("Testing IncFDFS.cpp on %d vertices and %d edges %d times,Seed(%u)\n",N,M,t,seed);
printf("========================================================\n");



while(t--)
	{
	unsigned int seed =  rand();
	srand(seed);
//	printf("Testing IncFDFS.cpp on %d vertices and %d edges,Seed(%u)\n",N,M,seed);
//	printf("========================================================\n");
	
	for(int i=0;i<=N;i++)
		for(int j=0;j<=N;j++)
			connect[i][j]=0;
	G = Graph(N+1,1);

	Logger::resetEP();

	IncFDFS incFDFS(N,t%2);
	
	#ifdef LOGGED
	printf("Edges Probed while Init IncDFS %d\n",Logger::getEP());
	ep=0;
	#endif	
	
	cc=0;	


	int x = M;
	while(x--)
		{
		int e1,e2;
		e1 = rand()%(N)+1;
		e2 = rand()%(N)+1;

		if(e1!=e2 && !G.validE(e1,e2) && (t%2 || !connect[e2][e1]))
			{
			G.addEdge(e1,e2);
			#ifdef SHWGRPH
			printf("\nInserting edge (%d,%d)\n----------------------\n",e1,e2);
			#endif
			incFDFS.addEdge(e1,e2);
			#ifdef SHWTREE
			printf("DFS Tree %d size: ",incFDFS.getT().size());
			incFDFS.getT().printT(0);
			Logger::calcBStick(incFDFS.getT(),0);	
			printf("\n Broom Stick Length %d\n",  Logger::getBS()+1);
			
			#endif
			if(VerifyDFS::Verify_DFS(G,incFDFS.getT())==0)
				flag=1;
			#ifdef LOGGED
			printf("Edges Probed in Update %d, total %d\n",Logger::getEP()-ep,
				Logger::getEP());
			ep=Logger::getEP();
			#endif	
			#ifdef SHWTREE
			printf("updating connect\n");
			#endif
			if(t%2 == 0) updateConnect(e1,e2);
			#ifdef SHWTREE
			printf("updated connect %d\n",cc);
			#endif			
			}
		}
	#ifdef SHWTREE
	printf("Total connect %d\n",cc);
	#endif	
	}
if(flag) 
	{printf("IncFDFS Verification failed\n\n"); assert(0);}
else
	printf("IncFDFS Verified\n\n");
return 0;
}
