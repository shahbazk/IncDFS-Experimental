/*******************************************************************************
 * tests/testFTDFS.cpp
 * Exhaustive tests on random graphs to verify the correctness of codes for 
 * Fault Tolerant DFS used by Incremental WDFS and Fully Dynamic DFS <FTDFS.cpp> 
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
#include"../include/VerifyDFS.h"
#include"../include/Logger.h"

//#define SHWTREE               // Showing intermediate steps of constructed tree
//#define SHWGRPH               // Showing intermediate steps of constructed graph
//#define LOGGED                // Logging parameters related to operations 

using namespace std;

#define N 10			// Number of Vertices
#define M (2*N)			// Number of Edges
#define TST 100                 // Number of Test runs

Graph G;
Tree T;


/*
Removes the (sanitized) edge e from list edgL. 
Param: <int> edge, <edgL> edge list
Return:	1 if success (if edg.second!= -1)
	0 if failure
*/
int delEdge(int x,list<int>& edgL)
	{
	list<int>:: iterator it;
	it = edgL.begin();
	while(it!= edgL.end())
		{
		if(x==(*it)) {edgL.erase(it); return 1;}
		it++;		
		}	
	return 0;
	}



int main()
{
int t=TST,flag=0;
unsigned int seed = time(NULL);
srand(seed);

printf("Testing FTDFS.cpp on %d vertices and %d edges %d times::Seed(%u)\n",N,M,t,seed);
printf("========================================================\n");

while(t--)
	{
	unsigned int seed = rand();//time(NULL);
	int i,j;
	srand(seed);
	
//	printf("Testing FTDFS.cpp on %d vertices and %d edges, %u seed\n",N,M,seed);
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
	printf("Graph %d Vertices %d Edges, %u seed\n",G.sizeV(),G.sizeE(),seed);
	#endif
	T=SDFS::StaticDFS(G,0);
	#ifdef SHWTREE
	printf("Tree %d Vertices %d Root\n",T.size(),T.root());
	T.printT(0);
	printf("\n");
	#endif

	Logger::resetEP();

	DS_WDFS DS = DS_WDFS(T);
	int m =0;
	while(!DS.buildPart(G,sqrt(G.sizeV()*G.sizeE())))  
		++m;
	#ifdef SHWTREE
	printf("DS built in %d steps\n\n",m);
	#endif
	#ifdef LOGGED
	printf("Edges Probed while building DS %d\n",Logger::getEP());
	#endif	
	
	dataFTDFS dFT;
	FTDFS::initDataFTDFS(DS,dFT,T.size(),T.root());
	map<int,int> newVertices;	

	int x = M;
	while(x--)
		{
		#ifdef SHWGRPH
		printf("\nFinal Partition\n");
		FTDFS::printDFT(dFT);
		printf("Info of vertices: ");
		for(unsigned int i=0;i<dFT.info.size();i++)
			{
			if(dFT.info[i]==FTDFS::INFO_PATH) 
			    printf("{%d}[%d,(%d,%d)(%d,%d)],\t",i,dFT.info[i],
			      dFT.pathParam[i].first.first, dFT.pathParam[i].first.second,
			      (*dFT.pathParam[i].second).first,(*dFT.pathParam[i].second).second);
			else if(dFT.info[i]==FTDFS::INFO_TREE)
				{
				if(!dFT.isRoot[i].first)	
				printf("{%d}[%d,%d]),\t",i,dFT.info[i], dFT.isRoot[i].first);
			     	else  	
				printf("{%d}[%d,%d,(%d)],\t",i,dFT.info[i],
					dFT.isRoot[i].first,(*dFT.isRoot[i].second));
				}
			}
			
		printf("\n");
		#endif

		int e1,e2,t;
		t = rand()%4 +1;
		e1 = rand()%(N-1)+1;
		e2 = rand()%(N-1)+1;
		switch(t)
			{
			case 1 : 
			if(G.validV(e1) && G.validV(e2) && !G.validE(e1,e2) && e1!=e2)
				{	// Add edge
				#ifdef SHWGRPH
				printf("Adding Edge (%d,%d)\n",e1,e2);
				#endif
				G.addEdge(e1,e2);
				dFT.redAdj[e1].push_back(e2);
				dFT.redAdj[e2].push_back(e1);
				} 
			break;
					
			case 2 : 
			if(G.validE(e1,e2))
				{	// Rem Edge
				#ifdef SHWGRPH
				printf("Removing Edge (%d,%d)\n",e1,e2);
				#endif
				G.remEdge(e1,e2);
				if(delEdge(e1,dFT.redAdj[e2]) && delEdge(e2,dFT.redAdj[e1]));
				else
					{
					DS.delE(e1,e2);
					if(DS.getTree().par(e1) == e2 || 
					   DS.getTree().par(e2) == e1)
						{  // Tree edge in DS[cDS]
						if(dFT.info[e1]==FTDFS::INFO_PATH &&  
						   dFT.info[e2]==FTDFS::INFO_PATH && 
		   				  dFT.pathParam[e1].first == 
						  dFT.pathParam[e2].first) 
							FTDFS::partitionP(DS,dFT,e1,e2);// In path
						else if(dFT.info[e1]==FTDFS::INFO_TREE && 
							dFT.info[e2]==FTDFS::INFO_TREE) 
							FTDFS::partitionT(DS,dFT,e1,e2);// In tree
						}
					}
				} 
			break;
	
			case 3 : 	// Add Vertex
			{
			list<int> l;
			int t=e1;
			e1 = G.addVertex();
			FTDFS::addVDFT(dFT,e1);
			newVertices[e1]=1;

			#ifdef SHWGRPH
			printf("Vertex %d added\n",e1);
			printf("Neighbours: ");
			#endif

			while(t--)
				{ e2 = rand()%(N-1)+1;
				if(G.validV(e2) && !G.validE(e1,e2) && e1!=e2)
					{

					l.push_back(e2);
					G.addEdge(e1,e2);
					dFT.redAdj[e1].push_back(e2);
					dFT.redAdj[e2].push_back(e1);
					#ifdef SHWGRPH
					printf("%d, ",e2);
					#endif
					}
				}
			#ifdef SHWGRPH
			printf("\n");
			#endif
			dFT.partP.push_front(pth(e1,e1));
			dFT.pathParam[e1] = pPIter(pth(e1,e1),dFT.partP.begin() );
			dFT.isRoot[e1] = pTIter(0,TIter());
			dFT.info[e1] = FTDFS::INFO_PATH;
			dFT.redAdj[0].push_back(e1);
			
			break;
			}
	
			case 4 : 
			if(G.validV(e1))
				{	// Rem Vertex
				#ifdef SHWGRPH
				printf("Removing vertex (%d)\n",e1);
				#endif

				list<int> ngbr = G.nghbr(e1);
				list<int> :: iterator itn;
				for(itn=ngbr.begin();itn!=ngbr.end();itn++)
					{
					G.remEdge(e1,*itn);
					list<int>:: iterator itx = find(dFT.redAdj[e1].begin(),
							       dFT.redAdj[e1].end(),*itn);
					if(itx == dFT.redAdj[e1].end())
						DS.delE(e1,*itn);
					else
						{
						dFT.redAdj[e1].erase(itx);
						dFT.redAdj[*itn].erase(find(
							dFT.redAdj[*itn].begin(),
							dFT.redAdj[*itn].end(),e1));
						}
					}

				G.remVertex(e1);
					if(dFT.info[e1]==FTDFS::INFO_PATH)
						FTDFS::partitionP(DS,dFT,e1,-1);
					else if(dFT.info[e1]==FTDFS::INFO_TREE)  	
						FTDFS::partitionT(DS,dFT,e1,-1);
					else
						ERR_MSG("TestFTDFS::Invalid INFO\n");

				itn = find(dFT.redAdj[0].begin(),dFT.redAdj[0].end(),e1);
				if(itn != dFT.redAdj[0].end())	
					dFT.redAdj[0].erase(itn);
	
				dFT.pathParam[e1] = pPIter(pth(-1,-1),PIter());
				dFT.isRoot[e1] = pTIter(0,TIter());
				dFT.info[e1] = FTDFS::INFO_INVALID;
				} 
			break;
			}
		}
	
	Tree T1 = FTDFS::FT_DFS(DS,dFT,T.root());
	#ifdef SHWTREE
	printf("Tree After FT_DFS\n Tree: ");
	T1.printT(0);
	printf("\n");
	#endif
	#ifdef LOGGED
	printf("Total Edges Probed after building DFS %d\n\n",Logger::getEP());
	#endif	
	if(VerifyDFS::Verify_DFS(G,T1)==0)
		flag=1;
	
	}

if(flag) 
	{printf("FTDFS Verification failed\n\n"); assert(0);}
else
	printf("FTDFS Verified\n\n");
return 0;
}
