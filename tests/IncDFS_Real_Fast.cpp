/*******************************************************************************
 * tests/IncDFS_Real_Fast.cpp
 * Evaluation of algorithms on Real Graphs 
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
Input:			file <BATCH> <OPT>
------
BATCH    0- Treat edge insertion singly, 1- Consider Batch Insertions
OPT: Optimalitt level (if any) 
     IncADFS (0 ADFS1, 1 ADFS2)
     IncSDFS (0 SDFS, 2 SDFS2, 4 SDFS3)
      

Output:               tEdgProb totTime 
-------	
tEdgProb                        // Total Edges Processed
totTime 		        // Total Time taken
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

using namespace std;




int main(int argc, char* argv[])
{
int batch=0;
if(argc>1) batch= atoi(argv[1]);

#if defined(Static_DFS) || defined(ADFS)
	int OPT=4; 
	if(argc>2) OPT= atoi(argv[2]);
#endif

int m_max,n,m=0;
assert(scanf("%d%d",&n,&m_max)==2);

Logger::resetEP();
clock_t begTime = clock();

#ifdef Static_DFS
	#if defined(DIRECTED) || defined(DAG)
	IncSDFS incDFS = IncSDFS(n,OPT,1);
	#else
	IncSDFS incDFS = IncSDFS(n,OPT,0);
	#endif			
#else
	#if defined(DIRECTED)
	IncFDFS incDFS = IncFDFS(n,1);
	#elif defined(DAG)
	IncFDFS incDFS = IncFDFS(n);
	#elif defined(ADFS)
	IncADFS incDFS = IncADFS(n,OPT);
	#else
	IncWDFS incDFS = IncWDFS(n);
	#endif
#endif

int e1,e2,t,t1=-1;	
list<edg> edgAdd;

while(m<m_max)
	{	
	assert(scanf("%d%d%d",&e1,&e2,&t)==3);
	m++;
	if(!batch)	incDFS.addEdge(e1,e2);
	else {
	     if(t1!= t && edgAdd.size())
		{
		incDFS.addEdgeS(edgAdd);	
	     	edgAdd.clear();
		}
	     t1 = t;
	     edgAdd.push_back(edg(e1,e2));
	     }	
	}
printf("%lld %lf\n",Logger::getEP(),((double) clock()-begTime)/CLOCKS_PER_SEC);

return 0;
}
