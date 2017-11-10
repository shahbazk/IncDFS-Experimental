/*******************************************************************************
 * src/DS_WDFS.cpp
 * Data Structure operations used by WDFS, Fault tolerant and Fully Dynamiic DFS
 *
 * Part of Project: Incremental-DFS-Experimental: 
 *                  https://github.com/shahbazk/IncDFS-Experimental
 *
 * Copyright (C) 2017 Shahbaz Khan <shahbazk@cse.iitk.ac.in>
 *
 * All rights reserved. Published under the BSD-2 license in the LICENSE file.
 ******************************************************************************/

#include"../include/DS_WDFS.h"
#include"DS_WDFS.Init.cpp"


//#define TEST_DS_WDFS

/*
Tests the validity of <status>. 
Complexity O(m):: Use wisely
Param: <status> curState, <st> SegTree info
Return: 1 if valid
	0 else
*/
int DS_WDFS::validState(stateV status,ST& st)
{
#ifdef DS_OLD
if(status.first >= st.offST)
	return 1;
segNodeIt st1,st2, l1,l2,u1,u2;
int loc = status.first;
st1 = status.second.first;
l1 = st.segT[loc*2].begin();
u1 = st.segT[loc*2].end();
st2 = status.second.second; 
l2 = st.segT[loc*2+1].begin();
u2 = st.segT[loc*2+1].end();
while(l1!= u1) {if(l1==st1) break; l1++;}
while(l2!= u2) {if(l2==st2) break; l2++;}

if((l1==u1 && st1!=u1) || (l2==u2 && st2!=u2))
	return 0;
#else
if(status >= st.offST)
	return 1;
#endif
return 1;
}


/* FOR DEBUG ONLY
Print List in Map of a node in SegTREE 
*/
void DS_WDFS::dispMP(const ST& st, int hx, pth interval)	const
	{
	segNode::const_iterator it;
	printf("Map of %d in [%d,%d]=>",hx-st.offST,interval.first,interval.second);
	for(it= st.segT[hx].begin(); it!= st.segT[hx].end();it++)
		{
		if((*it).first.first>= interval.first && (*it).first.first <=interval.second)
			printf("(%d,%d) ",(*it).first.first, (*it).first.second);
		}
	printf("\n");
	}


/*
Query Map of SegNode for lowest/highest edge based on type(0/1) in interval
Param: ST, hNo. of node, interval and query type
Return: Required Edge
*/
edg DS_WDFS::queryM(const ST& st, int hx, pth interval, int type)	const
	{
	segNode::const_iterator it;
	 	

	#ifdef TEST_DS_WDFS
	dispMP(st,hx,interval);
	printf("QueryM %d, %d %d\n",hx-st.offST,interval.first,interval.second);
	#endif
	
	Logger::incrEP(log2(st.segT[hx].size()+1)+1);	
	
	if(!type)
		{
		it = (st.segT[hx].lower_bound(edg(interval.first,0)));
		if(it!= st.segT[hx].end() && (*it).first.first <= interval.second) 	
					return (*it).first;
		else			return edg(T.size()+2,T.size()+2);			
		}
	else
		{
		it = (st.segT[hx].upper_bound(edg(interval.second,T.size()+2)));
		if(it!= st.segT[hx].begin() && (*(--it)).first.first>=interval.first) 
					return (*it).first;
		else 			return edg(-1,-1); 
		}	
	}




/*
Query ST from hleft to hright for lowest/highest edge based on type(0/1) in interval
Param: hNo. of all
Return: Required edge
*/
edg DS_WDFS::queryST(const ST& st, int hLeft, int hRight, pth interval, int type) const
	{
	edg ans,tmp;
	hLeft += st.offST; hRight += st.offST;
	
	ans = queryM(st, hLeft, interval, type);
	if(hLeft == hRight) return ans;

	tmp = queryM(st, hRight, interval, type);
	if(type?  tmp>ans : ans>tmp  )
		ans = tmp;
	
	while(hLeft/2 != hRight/2)
		{
		if(hLeft == (hLeft/2 )*2)        // Left is left Child
			{
			tmp = queryM(st, hLeft+1, interval, type);
			if(type?  tmp>ans : ans>tmp  )
				ans = tmp;
			}

		if(hRight != (hRight/2 )*2)        // Right is Right Child
			{
			tmp = queryM(st, hRight-1, interval, type);
			if(type?  tmp>ans : ans>tmp  )
				ans = tmp;
			}
		hLeft/=2;hRight/=2;
		}
	
	return ans;
	}












////////////////////////
/// Public Functions ///
////////////////////////
/*
Reports whether a vertex <x> is covered by DS, ie added to the segTree
Param: <x> Index of the vertex
Return: 1 if covered
	0 else
	-1 error
*/
int DS_WDFS::isCovered(int x) const
	{
	if(!T.validV(x)) 
		ERR_MSGR("DS_WDFS::isCovered invoked on invalid vertex",-1);
	if(dHLD.heavyNo[x] != -1) return 1;
	else			  return 0;	
	}	

/*
Returns a pointer of the underlying DFS tree
Return: Pointer of T 
*/

const Tree& DS_WDFS::getTree() const
	{ return T;}

/*
Verifies whether x is an ancestor of y in T
Param: <dHLD> Ref of dataHLD, Index of <x> and <y>
Return: 1 if true
	0 otherwise
*/
int DS_WDFS::isAnc(int x, int y) const
	{
	if(dHLD.heavyNo[x] <= dHLD.heavyNo[y]  && dHLD.lastTree[ dHLD.heavyNo[x] ] >= dHLD.heavyNo[y]) 	
				return 1;
	else 		return 0;
	}

/*
Empty Constructor for DS_WDFS.
*/
DS_WDFS::DS_WDFS(){}


/*
Constructor of DS_WDFS. Makes DS followed by many updates
Param: <Temp> Tree on which DS to be made
*/
DS_WDFS::DS_WDFS(Tree Temp)
	{
	T = Temp;

	initHLD(dHLD,T);

	// Assuming not all vertices in tree are covered vertices.
	// Hence forth size is dHLD.lastTree[ dHLD.heavyNo[T.root()] ]+1  
//	#ifdef WDFS_DEBUG	
//	if(dHLD.lastTree[ dHLD.heavyNo[T.root()] ] == T.size()-1)	
//		printf("HLD Verified\n\n");
//	else			
//		printf("HLD ERROR!!!\n\n");	
//	#endif
	st.size = dHLD.lastTree[ dHLD.heavyNo[T.root()] ]+1; 

	for(st.offST=1; st.offST< st.size;st.offST*=2);
	st.segT.resize(st.offST*2);
	#ifdef DS_OLD
	curState = stateV(st.offST+st.size-1,pSegNodeIt( segNodeIt(),segNodeIt()));
	#else 
	curState = st.offST+st.size-1;
	#endif
	}



/*
Partial update DS based T
Param: Ref of Graph <G>. Amount of Cost allowed to build <cost>
Return: 1 if completely built
		0 otherwise
*/
int DS_WDFS::buildPart(const Graph& G,int cost)
	{
	while(cost> 0)
		{
		curState = makeSTpart(G, dHLD, st, curState,cost);
		
		#ifdef DS_OLD
		if(curState.first == 0) 	return 1;
		#else
		if(curState == 0) 	return 1;
		#endif
		}
	return 0;		
	}







/*
Query Type(w), path(x,y) where ans closer to x.
	  Type 	0 : T(w), 
		1 : w
Report minimum of all such edges from T(w) to path(x,y) if exists else (-1,-1)
Return: Required edge
*/
edg DS_WDFS::query(int w, int x, int y, int type)	const
	{
	if(!isAnc(x,y) && !isAnc(y,x))	
		ERR_MSGR("DS_WDFS::query invoked with non anc-desc path\n",edg());

	int tmp1,high=1;         		// Highest in tree i.e. lowest HNo.
	int hx,hy;
	list<pth> paths;
	hx = dHLD.heavyNo[x];
	hy = dHLD.heavyNo[y];

	if(hx < hy)  {tmp1 = hx; hx =hy; hy = tmp1; high=0;}	
	// x is now a descendant of y
	
	while(hx > hy)
		{
		if( dHLD.pathHead[hx] <= hy)		break;
		
		paths.push_front( pth( dHLD.pathHead[hx],hx));
		hx = dHLD.heavyNo[ T.par( dHLD.rHeavyNo[ dHLD.pathHead[hx] ] )];
		}	
	paths.push_front(pth(hy,hx));
	
	list<pth> :: iterator it;
	edg ans,tmp;
	ans = (high? edg(-1,-1): edg(T.size()+2, T.size()+2));

	for(it = paths.begin();it!= paths.end();it++)
		{
		if(!type)
		tmp = queryST(st, dHLD.heavyNo[w], dHLD.lastTree[ dHLD.heavyNo[w]], (*it), high);	
		else		
		tmp = queryST(st, dHLD.heavyNo[w], dHLD.heavyNo[w], (*it), high);	
		
		#ifdef TEST_DS_WDFS
		printf("ANS(%d,%d) Type %d => Q(%d,Pth(%d,%d),high %d) = (%d,%d)\n",
			ans.first,ans.second,type, dHLD.heavyNo[w], (*it).first,(*it).second,high,tmp.first,tmp.second);
		#endif

		if(high? tmp>ans : tmp<ans  )
			ans = tmp;
		}

	#ifdef TEST_DS_WDFS
	printf("ANS(%d,%d) \n",ans.first,ans.second);
	#endif

	if(ans.second>= dHLD.heavyNo[w] && ans.second<= dHLD.lastTree[ dHLD.heavyNo[w]]) 
		{tmp1=ans.first;ans.first=ans.second;ans.second=tmp1;}

	if(ans.first >= T.size() || ans.first<0) return edg(-1,-1);
	else 	return edg(dHLD.rHeavyNo[ans.first],dHLD.rHeavyNo[ans.second]);	
	}




/*
Remove Edge	(x,y) from the DS
Param: Index of <x> and <y>
THAT IS SURELY PRESENT IN DS  (OR INTRODUCE RELEVANT CHECKS)
Return: 1 For success 
	0 otherwise	{INCOMPLETE}
*/
int DS_WDFS::delE(int x, int y)
	{
	if(!T.validV(x) || !T.validV(y)) 	return 0;
	
	int hx = dHLD.heavyNo[x];
	int hy = dHLD.heavyNo[y];
	int l = hx + st.offST;
 	int r = hy + st.offST;
	#ifdef TEST_DS_WDFS
	printf("DS_WDFS::delE Deleted Edge (%d,%d) Valid %d\n",x,y,validState(curState,st)); 
	#endif	
	segNodeIt it;

	while(l != 0)
		{
		it = st.segT[l].find(edg(hy,hx));
 		if(it!= st.segT[l].end())
 			{
 			#ifdef DS_OLD	
			if(it== curState.second.first)
 				curState.second.first = st.segT[l].erase(it);
 			else if(it== curState.second.second)
 				curState.second.second = st.segT[l].erase(it);
 			else 
			#endif
 				st.segT[l].erase(it);
 			}
 
		it = st.segT[r].find(edg(hx,hy));
 		if(it!= st.segT[r].end())
 			{
			#ifdef DS_OLD
 			if(it== curState.second.first)
 				curState.second.first = st.segT[r].erase(it);
 			else if(it== curState.second.second)
 				curState.second.second = st.segT[r].erase(it);
 			else 
			#endif
 				st.segT[r].erase(it);
 			}
		Logger::incrEP();	
		Logger::incrEP();	
		l/=2;r/=2;
		}
	#ifdef TEST_DS_WDFS
	printf("DS_WDFS::delE Deleted Edge (%d,%d) Valid %d\n",x,y,validState(curState,st)); 
	#endif	
	return 1;
	}


/*
Insert Back Edge (x,y) to the DS
Param: Index of <x> and <y>
ASSUMPTION: Edge not present in DS (OR INTRODUCE RELEVANT CHECKS)
NOT HANDLED FOR DS_OLD
Return: 1 For success 
	0 otherwise
*/
int DS_WDFS::addBEdge(int x, int y)
{
	if(!T.validV(x) || !T.validV(y) || (!isAnc(x,y)&& !isAnc(y,x))) 	
		return 0;
	
	int hx = dHLD.heavyNo[x];
	int hy = dHLD.heavyNo[y];
	int l = hx + st.offST;
 	int r = hy + st.offST;
	#ifdef TEST_DS_WDFS
	printf("DS_WDFS::addBEdge Inserted Back Edge (%d,%d) Valid %d\n",x,y,validState(curState,st)); 
	#endif	
	
	int status;
	#ifdef DS_OLD
		status = curState.first;
	#else
		status = curState;
	#endif

	while(l != 0 && l<status)
		{
		Logger::incrEP();	
		if(st.segT[l].find(edg(hy,hx)) != st.segT[l].end())
			return 0; //Already Exist
		
		st.segT[l][edg(hy,hx)] = 1;

		Logger::incrEP();	
		if(r>=status)
			return 1;
		st.segT[r][edg(hx,hy)] = 1;
		
		l/=2;r/=2;
		}
	#ifdef TEST_DS_WDFS
	printf("DS_WDFS::delE Inserted Back Edge (%d,%d) Valid %d\n",x,y,validState(curState,st)); 
	#endif	
	return 1;
}


