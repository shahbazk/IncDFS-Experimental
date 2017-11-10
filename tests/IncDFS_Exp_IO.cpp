/*******************************************************************************
 * tests/IncDFS_Exp_IO.cpp
 * Evaluation of algorithms on Random Graphs in two versions: VarN and VarM
 * Initialization code 
 *
 * Part of Project: Incremental-DFS-Experimental: 
 *                  https://github.com/shahbazk/IncDFS-Experimental
 *
 * Copyright (C) 2017 Shahbaz Khan <shahbazk@cse.iitk.ac.in>
 *
 * All rights reserved. Published under the BSD-2 license in the LICENSE file.
 ******************************************************************************/

// No. of edges in graph of sparsity i
// 1- 2n
// 2- nlogn
// 3- n\sqrt{n}
// 4- complete 
int edgSpars(int n,int i)
{
int ans =0;
if(i==1) ans = 2*n;
if(i==2) ans = n*log2(n);
if(i==3) ans = n*sqrt(n);
#if defined(DIRECTED) 
if(i>3) ans = n*(n-1);
if(ans> n*(n-1)) return n*(n-1);
else return ans;
#else
if(i>3) ans = n*(n-1)/2;
if(ans> (n*(n-1)/2)) return (n*(n-1)/2);
else 	return ans;
#endif

}

#ifdef DAG 
int *perm=NULL;
 #endif


int (*edgS)[2]=NULL;
void randomEinit(int n)
{
int i,j,c=0;
if (edgS!=NULL){
   free(edgS);
}
edgS = (int (*)[2])calloc(n*n,sizeof(edgS));

#ifdef DAG
if(perm!= NULL) {free(perm);}
perm = (int *)calloc(n+1,sizeof(perm));
for(i=0;i<=n;i++) perm[i]=i;
i--;
while(i>1)
 	{
 	j = rand()%(i)+1;
 	c = perm[i];
	perm[i] = perm[j];
	perm[j] = c;
 	i--;
	}
#endif
c=0;
for(i=1;i<=n;i++)
	{
	#if defined(DIRECTED)
	for(j=1;j<=n;j++)
	#else
	for(j=i+1;j<=n;j++)
	#endif
		{
		if(i==j) continue;
		#ifdef DAG
		edgS[c][0]=perm[i]; edgS[c][1]=perm[j];
		#else
		edgS[c][0]=i;	edgS[c][1]=j;
		#endif
		c++;
		}
	}
 int e1,e2;
 while(c)
 	{
 	i = rand()%(c);
 	e1 = edgS[c-1][0];
	e2 = edgS[c-1][1];
 	edgS[c-1][0] = edgS[i][0];
 	edgS[c-1][1] = edgS[i][1];
 	edgS[i][0] = e1;	
 	edgS[i][1] = e2;	
 	c--;
 	}
 }

/*
Generate the connected graph G(n,p=m/n*n)
m = 0 for star graph.
*/
void initGraph(Graph &G, int n, int m)
{
int i,j;

#if defined(DIRECTED) || defined(DAG)
G = Graph(n+1,1);
#else
G = Graph(n+1,0);
#endif

for(i=1;i<=n;i++)
	{
	G.addEdge(0,i);

	#ifdef SHWGRPH
	printf("Graph: (%d,%d)\n",0,i);
	#endif

	for(j=i+1;j<=n;j++)
		{
		if(rand()%(n*n)<m) 
			{
			#ifdef SHWGRPH
			printf("Graph: (%d,%d)\n",i,j);
			#endif
			G.addEdge(i,j);
			}	
		}
	}
}

/////////////////////////////////////////
///////////// ARGUMENTS /////////////////
/////////////////////////////////////////
//#if defined(Static_DFS) || defined(DIRECTED) || defined(ADFS)
#if defined(Static_DFS) || defined(ADFS)
	#define GET_OPT(x) 	\
	int OPT=4; \
	if(argc>x) OPT= atoi(argv[x]);
#else
	#define GET_OPT(x) {}
#endif

#ifdef REAL

	#define GET_ARGS() 	\
	int DP=1000; \
	if(argc>1) DP = atoi(argv[1]);\
	GET_OPT(2)

#else
// RANDOM
	#define GET_ARGS() 	\
	int N=100,DP=100,TST=10,SPARS=4;\
	if(argc>1) N= atoi(argv[1]);\
	if(argc>2) SPARS= atoi(argv[2]);\
	if(argc>3) DP= atoi(argv[3]);\
	if(argc>4) TST= atoi(argv[4]);\
	GET_OPT(5)
#endif



//////////////////////////////////////////
///////// INIT n,m,t /////////////////////
//////////////////////////////////////////
#ifdef REAL
	#define INIT_NM() \
	assert(scanf("%d%d",&n_max,&m_max)==2);
#else
	#define INIT_NM() \
	n_max = N, m_max = edgSpars(N,SPARS);\
	int t=TST;
#endif

