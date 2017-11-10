/*******************************************************************************
 * tests/IncDFS_Exp.cpp
 * Evaluation of algorithms on Random Graphs in two versions: VarN and VarM 
 *
 * Part of Project: Incremental-DFS-Experimental: 
 *                  https://github.com/shahbazk/IncDFS-Experimental
 *
 * Copyright (C) 2017 Shahbaz Khan <shahbazk@cse.iitk.ac.in>
 *
 * All rights reserved. Published under the BSD-2 license in the LICENSE file.
 ******************************************************************************/

/*
Compilation Flags:
=================
TYPE:
-----
VARN      -D VARN                // Compute the parameters for fixed sparsity 
                                 //             and varying number of vertices
VARM                             // Compute the parameters for fixed number of 
					  vertices and varying number of edges
Broomstick -D BROOM 		 // If intended to calculate the properties of broom
           -D BRISTLE 		 // Defined only when BROOM defined and for TST=1;
	   -D COMPONENTS 	 // Defined only when BROOM is defined for TST=1;

Algorithm:
---------
SDFS   -D Static_DFS  
    Undirected
    Directed   -D DIRECTED
    DAG        -D DAG
SDFS-Int (Same as above with -D DFS_INTERRUPT)
FDFS  
    Directed  -D DIRECTED 
    DAG       -D DAG
ADFS   -D ADFS
WDFS   

	
USAGE:
======
Input:			file <N> <SPARS> <DP> <TST> <OPT>
------
N: Number of Vertices
SPARS: Sparsity 
    1- 2n, 2- nlogn, 3- n\sqrt{n}. 4- complete 
DP: Data Points, Number of values for which the properties are reported
TST: Number of test runs to be averaged upon
OPT: Optimalitt level (if any) 
     IncADFS (0 ADFS1, 1 ADFS2)
     IncSDFS (0 SDFS, 2 SDFS2, 4 SDFS3)
      

Output: 
-------
VarN::: verC tEdgProb maxEdgP maxExEdgP crossEdg edgSize maxEdgS 
                     totTime LATime EdgMTime maxTime maxExTime	

verC				// Number of Vertices
tEdgProb                        // Total Edges Processed
maxEdgP				// Max edges processed per edge insertion 
maxExEdgP	                // Max expected edges proc. per edge insertion
crossEdg 		        // Number of cross edges encountered
edgSize				// No. of edges in the graph
maxEdgS				// Maximum size of the graph 
totTime 		        // Total Time taken
LATime			        // Time taken by Level Ancestor Structures
EdgMTime			// Time taken for Graph/Tree operations
maxTime				// Max Time required per edge inseriton
maxExTime			// Max expected time taken per edge insertion

VARM::: edgC tEdgProb cEdgProb cEdgProb2 maxEdgP maxExEdgP probCE crossEdg sizeUpd 
              edgSize maxEdgS totTime LATime EdgMTime currTime maxTime maxExTime

edgC                           	// Number of Edges
tEdgProb                        // Total Edges Processed
cEdgProb                        // Expected Edges probed in this insertion
cEdgProb2			// Expected Edges probed in this cross edge insertion 
maxEdgP				// Max edges processed per edge insertion 
maxExEdgP	                // Max expected edges proc. per edge insertion
probCE		              	// Probability of inserting Cross edges
crossEdg 		        // Number of cross edges encountered
sizeUpd              		// Size of the updated part of DFS tree
edgSize				// No. of edges in the graph
maxEdgS				// Maximum size of the graph 
totTime 		        // Total Time taken
LATime			        // Time taken by Level Ancestor Structures
EdgMTime			// Time taken for Graph/Tree operations
currTime		        // Time taken till present
maxTime				// Max Time required per edge inseriton
maxExTime			// Max expected time taken per edge insertion


BROOM:::  <verC/edgC> broomSL broomSL2 broomMB broomWAB broomTB broomMBH

BRISTLES/COMPONENTS::: <verC/edgC> brisComp c

<verC/edgC>			// Number of Vertices (for VARN) or Edges (for VARM)
broomSL				// Length of Broom Stick
broomSL2 			// Length of Extended broom stick
broomMB				// Maximum Size of Bristles 
broomMBH			// Maximum Height of Bristles
broomTB				// Size of Tail Bristles
broomWAB			// Weighted average size of Bristles
brisComp			// Sizes of different bristles (for BRISTLES) 
                                     or components of bristles (for COMPONENTS)
c				// Number of elements with given size
*/


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
#include"../include/IncWDFS.h"
#include"../include/IncSDFS.h"
#include"../include/IncFDFS.h"
#include"../include/IncADFS.h"
#include"IncDFS_Exp_IO.cpp"

//#define SHWTREE
//#define SHWGRPH
//#define SHWTEST

using namespace std;

#define PRINT_VARM(dp)	{	\
printf("%d %lld %lf %lld %lld %lld \
%lf %lld %f %lld %lld %lf %lf %lf %lf %lf %lf\n",\
edgC[dp],tEdgProb[dp],cEdgProb[dp],cEdgProb2[dp],maxEdgP[dp],maxExEdgP[dp],\
probCE[dp],crossEdg[dp],sizeUpd[dp],edgSize[dp],maxEdgS[dp],\
totTime[dp],LATime[dp],EdgMTime[dp],currTime[dp],maxTime[dp],maxExTime[dp]);}

#define PRINT_VARN(dp) {	\
printf("%d %lld %lld %lld %lld \
%lld %lld %lf %lf %lf %lf %lf\n",\
verC[dp],tEdgProb[dp],maxEdgP[dp],maxExEdgP[dp],crossEdg[dp],\
edgSize[dp],maxEdgS[dp],totTime[dp],LATime[dp],EdgMTime[dp],maxTime[dp],maxExTime[dp]);	}




int main(int argc, char* argv[])
{
GET_ARGS()
int n_max,m_max,n,m,dataP,step,dp,i;
INIT_NM()
//INIT_DATA()

#ifndef BROOM
long long tEdgProb[DP];         // Total Edges Processed
long long crossEdg[DP];         // Number of cross edges encountered
clock_t begTime;
double totTime[DP];             // Total Time taken
double LATime[DP];              // Time taken by Level Ancestor Structures
double EdgMTime[DP];            // Time taken for Graph/Tree operations
double currT;			// Curreent Time
double maxT;			// Max 	Time
long long currEP;		// Current Edg Prob
long long maxEP;		// Max Edg Prob
long long maxES;			// Max Edges Stored
double maxTime[DP];		// Max Time required per edge inseriton
double maxExTime[DP];		// Max expected Time required per edge insertion
long long maxEdgP[DP];		// Max edges processed per edge insertion
long long maxExEdgP[DP];	// Max expected edges processed per edge insertion
long long edgSize[DP];		// No. of edges in the graph
long long maxEdgS[DP];
#else
int broomSL[DP];		// Length of Broom Stick
int broomSL2[DP]; 		// Length of Extended broom stick
int broomMB[DP];		// Maximum Size of Bristles 
int broomMBH[DP];		// Maximum Height of Bristles
int broomTB[DP];		// Size of Tail Bristles
int broomWAB[DP];		// Weighted average size of Bristles
vector<int> brisComp[DP];	// Sizes of different (components of) bristles
#endif
 



#ifdef VARN
dataP = DP/ceil(log10(n_max));
int verC[DP];                         // Number of Vertices

	#ifdef BROOM
	int maxBrstls;
	int maxSecComp;
	#endif	
#else
dataP = DP/ceil(log10(m_max));
int edgC[DP];                           // Number of Edges
	#ifndef BROOM 
	double currTime[DP];            // Time taken till present
	double cEdgProb[DP];		// Expected Edges probed in this insertion
	long long cEdgProb2[DP];	// Expected Edges probed in this cross edge insertion 
	long long currCE;               // Cross edges processed till present
	double probCE[DP];              // Probability of inserting Cross edges
	float sizeUpd[DP];              // Size of the updated part of DFS tree
	#endif
#endif


for(i=0;i<DP;i++)
	{
	#ifndef BROOM
	tEdgProb[i]=0; 	crossEdg[i]=0; edgSize[i]= maxEdgS[i] = 0;
	totTime[i]=0; LATime[i]=0; EdgMTime[i]= 0;
	maxEdgP[i] = maxExEdgP[i] = maxTime[i] = maxExTime[i] = 0;
	#else
	broomSL[i]=0;	broomSL2[i]=0;	broomMB[i]=0;
	broomWAB[i]=0;	broomTB[i]=0; broomMBH[i]=0;
	brisComp[i].clear();	
	#endif

	#ifdef VARN
	verC[i]=0;	
	#else
	edgC[i]=0; 
		#ifndef BROOM
		cEdgProb[i]=0; cEdgProb2[i]=0; probCE[i]=0; 
		currTime[i]=0; sizeUpd[i]=0;
		#endif
	#endif
	}


#ifndef REAL
unsigned int seed = time(NULL);
srand(seed);
dp=0;

while(t--)
 	{
	#ifdef SHWTEST
	printf("Test Case No. %d\n",TST-t);
	#endif
	unsigned int seed = rand();
	srand(seed);
#endif
	Graph G;
	
	dp=0;
	#ifdef VARN
	step = 10/dataP;
	if(step<1) step =1;			

	for(n=(step<10?10:step);n<=n_max;n+=step)
		{
		m_max=edgSpars(n,SPARS);
		#ifdef BROOM
		maxBrstls=0;
		maxSecComp=0;
		#endif
		#ifdef SHWTEST
		printf("N = %d, M = %d\n",n,m_max);
		#endif

		if(ceil(log10(n))>ceil(log10(n-step)))
			{
			step = n*10/dataP;
			if(step<1) step =1;			
			}
	
	#else
	n = n_max;
	step = 10/dataP;
	if(step<1) step = 1;
		#ifndef BROOM
		currCE = 0;
		#endif
	#endif




	initGraph(G,n,0);	
	m=0;
	#ifndef REAL	
	randomEinit(n);
	#endif

	#ifndef BROOM
		currEP = 0;
		currT = 0;
		maxES=0;
		maxEP=0;
		maxT=0;
		Logger::resetEP();
		Logger::resetCE();
		Logger::resetLATime();
		Logger::resetEdgMTime();
		begTime = clock();
	#endif


	
		// Initialize DFS structures

		#ifdef Static_DFS
			#if defined(DIRECTED) || defined(DAG)
			IncSDFS incDFS = IncSDFS(G.sizeV()-1,OPT,1);
			#else
			IncSDFS incDFS = IncSDFS(G.sizeV()-1,OPT,0);
			#endif			
		#else
			#if defined(DIRECTED)
			IncFDFS incDFS = IncFDFS(G.sizeV()-1,1);
			#elif defined(DAG)
			IncFDFS incDFS = IncFDFS(G.sizeV()-1);
			#elif defined(ADFS)
			IncADFS incDFS = IncADFS(G.sizeV()-1,OPT);
			#else
			IncWDFS incDFS = IncWDFS(G.sizeV()-1);
			#endif
		#endif
	#ifndef BROOM
	currEP = Logger::getEP();		
	currT  = ((double) clock()-begTime)/CLOCKS_PER_SEC;
	#endif

		int m1 = 0;	
		while(m+m1<m_max)
			{	
			int e1,e2;
			#ifndef REAL
			e1 = edgS[m+m1][0];
			e2 = edgS[m+m1][1];

			#else		
			int tmp;
			assert(scanf("%d%d%d",&e1,&e2,&tmp)==3);
			#endif
			m++;
			// End DFS add edge
			incDFS.addEdge(e1,e2);

				
			#ifndef BROOM
				if(maxT < ((double) clock()-begTime)/CLOCKS_PER_SEC - currT)
					maxT = ((double) clock()-begTime)/CLOCKS_PER_SEC - currT;
				if(maxEP < Logger::getEP()-currEP)
					maxEP = Logger::getEP()-currEP;		
				if(maxES < incDFS.getG().sizeE())
					maxES = incDFS.getG().sizeE();
			#endif
			
			#ifdef VARN
				#ifdef BROOM
				Logger::calcBStick(incDFS.getT(),0);
				if(maxBrstls< Logger::getMB())
					maxBrstls= Logger::getMB();
				vector<int> v = Logger::getComponents();
				sort(v.begin(),v.end());
				reverse(v.begin(),v.end());
			        if(v.size()>1 && maxSecComp < v[1]) 
					maxSecComp = v[1];		
				#endif
			#else
			if(ceil(log10(m))>ceil(log10(m-1)))
				{
				step = m*10/dataP;
				if(step<1) step = 1;
				}
		
			if((m%step) == 0 || m==m_max)
				{

				edgC[dp] = m;
				#ifndef BROOM
				tEdgProb[ dp] += Logger::getEP();
				cEdgProb[ dp] += Logger::getEP()-currEP;
				totTime [ dp] += ((double) clock()-begTime)/CLOCKS_PER_SEC;
				currTime[ dp] += ((double) clock()-begTime)/CLOCKS_PER_SEC - currT;
				LATime  [ dp] += Logger::getLATime();
				EdgMTime  [ dp] += Logger::getEdgMTime();
				edgSize[dp]   += incDFS.getG().sizeE();
				maxEdgS[dp]   += maxES;
				maxExEdgP[dp] += maxEP;
				if(maxEdgP[dp] < maxEP)
					maxEdgP[dp] = maxEP;
				maxExTime[dp] += maxT;
				if(maxTime[dp] < maxT)
					maxTime[dp] = maxT;
				cEdgProb2[dp] += Logger::getEP()-currEP - (Logger::getCE()==currCE);
				crossEdg[ dp] += Logger::getCE();
				probCE[dp]    += Logger::getCE()-currCE;
				sizeUpd[dp]   += Logger::getSizeUpd();
				#ifndef REAL
				if(TST==1)
				#endif
					PRINT_VARM(dp)
				#else
				Logger::calcBStick(incDFS.getT(),0);
				broomSL[ dp] += Logger::getBS();
				broomSL2[ dp] += Logger::getBS2();
				broomMB[ dp] += Logger::getMB();
				broomMBH[dp] += Logger::getMBH();
				broomWAB[ dp] += Logger::getWAB();
				broomTB[ dp] += Logger::getTB();


				#if defined(BRISTLE) || defined(COMPONENTS)
					#ifdef BRISTLE
					vector<int> v = Logger::getBristles();
					#else
					vector<int> v = Logger::getComponents();
					#endif	
				sort(v.begin(),v.end());
				reverse(v.begin(),v.end());
				for(i=0;i<(int) v.size();i++)
					if(i>=(int) brisComp[dp].size())
						brisComp[dp].push_back(v[i]);
					else
						brisComp[dp][i]+= v[i];	
				#endif

				if(TST==1)
					#if !defined(BRISTLE) && !defined(COMPONENTS)
	        			printf("%d %d %d %d %d %d %d\n",edgC[dp],
					broomSL[dp],broomSL2[dp],broomMB[dp],
					broomWAB[dp],broomTB[dp],broomMBH[dp]);
					#else
					{
					int c=0;
					for(i=0;i<(int) brisComp[dp].size();i++)
						{
						if(i>0 && brisComp[dp][i]!=brisComp[dp][i-1])
							{
							printf("%d %d %d\n",edgC[dp],brisComp[dp][i-1],c);	
							c=0;
							}
						c++;
						}
					if(brisComp[dp].size()>0)	
						printf("%d %d %d\n\n",edgC[dp],brisComp[dp][i-1],c);	
					}
					#endif					
				#endif
				dp++;
				}
			
				#ifndef BROOM
				currCE = Logger::getCE();		
				#endif
			#endif
			
			#ifndef BROOM
			currEP = Logger::getEP();		
			currT  = ((double) clock()-begTime)/CLOCKS_PER_SEC;
			#endif
			}

		#ifdef VARN
			#ifdef SHWTEST
			printf("Step %d, N = %d, EP = %lld\n",n/step,n,Logger::getEP());
			#endif

		verC[dp] = n;
			#ifndef BROOM
			tEdgProb[dp] += Logger::getEP();
			crossEdg[dp] += Logger::getCE();
			edgSize[dp]  += maxES;
			if(maxEdgS[dp] < maxES)
				maxEdgS[dp] = maxES;
			maxExEdgP[dp] += maxEP;
			if(maxEdgP[dp] < maxEP)
				maxEdgP[dp] = maxEP;
			maxExTime[dp] += maxT;
			if(maxTime[dp] < maxT)
				maxTime[dp] = maxT;
			totTime [ dp] += ((double) clock()-begTime)/CLOCKS_PER_SEC;
			LATime  [ dp] += Logger::getLATime();
			EdgMTime  [ dp] += Logger::getEdgMTime();
			if(TST==1)
				PRINT_VARN(dp)
			#else
			Logger::calcBStick(incDFS.getT(),0);
			broomSL[dp] +=  Logger::getBS(); 
			broomSL2[ dp] += Logger::getBS2();
			broomMB[ dp] += maxBrstls;
			broomMBH[ dp] += maxSecComp;
			broomWAB[ dp] += Logger::getWAB();
			broomTB[ dp] += Logger::getTB();
 			if(TST==1)
	      			printf("%d %d %d %d %d %d %d\n",verC[dp],broomSL[dp],broomSL2[dp],
				broomMB[dp],broomWAB[dp],broomTB[dp],broomMBH[dp]);	
			#endif
		dp++;
		}
		#endif
	#ifndef REAL
	}

		
if(TST!=1)
	{
	for(i=0;i<dp;i++)
		{
		#ifndef BROOM
		tEdgProb[i] /= TST;
		crossEdg[i] /= TST;
		maxExEdgP[i]  /= TST;
		edgSize[i] /= TST;
		totTime[i] /= TST;
		LATime[i] /= TST;	
		EdgMTime[i] /= TST;
		maxExTime[i] /= TST;
			#ifdef VARN
			PRINT_VARN(i)
			#else
			currTime[i] /= TST;
			cEdgProb[i] /= TST;
			cEdgProb2[i] /= (probCE[i]>1? probCE[i]:1);
			probCE[i] /= TST;
			sizeUpd[i] /= TST;
			PRINT_VARM(i)
			#endif

		#else
		broomSL[i] /= TST;
		broomSL2[i] /= TST;
		broomMB[i] /= TST;
		broomMBH[i] /= TST;
		broomWAB[i] /= TST;
		broomTB[i] /= TST;
		#if !defined(BRISTLE) && !defined(COMPONENTS)
			#ifdef VARN	
       	 		printf("%d ",verC[i]);
			#else
       	 		printf("%d ",edgC[i]);
			#endif
       	 	printf("%d %d %d %d %d %d\n",broomSL[i],broomSL2[i],
				broomMB[i],broomWAB[i],broomTB[i],broomMBH[i]);	
		#else
		int c=0,j;
		for(j=0;j<(int) brisComp[i].size();j++)
			{
			brisComp[i][j] /= TST;
			if(j>0 && brisComp[i][j]!=brisComp[i][j-1])
				{
				#ifdef VARN
				printf("%d ",verC[i]);	
				#else
				printf("%d ",edgC[i]);	
				#endif
				printf("%d %d\n",brisComp[i][j-1],c);	
				c=0;
				}
			c++;
			}
		if(brisComp[i].size()>0)	
				{
				#ifdef VARN
				printf("%d ",verC[i]);	
				#else
				printf("%d ",edgC[i]);	
				#endif
				printf("%d %d\n",brisComp[i][j-1],c);	
				c=0;
				}
		#endif
		#endif	
		}
	}
#endif

return 0;
}
