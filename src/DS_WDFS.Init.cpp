/*******************************************************************************
 * src/DS_WDFS.Init.cpp
 * Initialization of the Data Structure used by WDFS, Fault tolerant and 
 * Fully Dynamiic DFS
 *
 * Part of Project: Incremental-DFS-Experimental: 
 *                  https://github.com/shahbazk/IncDFS-Experimental
 *
 * Copyright (C) 2017 Shahbaz Khan <shahbazk@cse.iitk.ac.in>
 *
 * All rights reserved. Published under the BSD-2 license in the LICENSE file.
 ******************************************************************************/

	
//////////////////////////////////////////////////////////////////////
///////////////////// HANDLING HLD RELATED ///////////////////////////
//////////////////////////////////////////////////////////////////////

/*
Update heavyNo, rHeavyNo, lastTree, pathHead of <dHLD> for each vertex of <T> below <root>
Param:  <dHLD> Ref of dataHLD, <T> Const Ref of Tree, <root> Index of Root  
		<hInd> hNo. of root, <hHead> hNo. of head of path 
Return: <sz> Size of T(root) in <T>
		-1 If invalid root
*/
int DS_WDFS::updateHL(dataHLD& dHLD, const Tree& T, int root,int hInd,int hHead)
	{
	if(!(root>=0 && root<T.size()))
		ERR_MSGR("DS_WDFS::UpdateHL invoked with invalid 'root'\n",-1);

	int max=-1,maxC=-1; 	// max = max size of heavy subtree, maxC = Index of heavy child of root

	dHLD.heavyNo[root] = hInd;
	dHLD.rHeavyNo[hInd] = root;
	dHLD.pathHead[hInd] = hHead;
	hInd++;

	list<int> child = T.child(root);
	list<int> :: iterator it;

	for(it= child.begin(); it!= child.end(); it++)
		{	// Find heavy child
		if(dHLD.sizeTree[*it]>max)
			{
			max = dHLD.sizeTree[*it];
			maxC = *it;
			}
		}

	if(maxC != -1)
		{	// Assing hNo. 
		hInd = updateHL(dHLD, T, maxC,hInd,hHead);		// Traverse heavy subtree
		for(it= child.begin(); it!= child.end(); it++)	// Traverse rest
			if(*it!= maxC) hInd = updateHL(dHLD, T, *it, hInd, hInd);
		}

	dHLD.lastTree[ dHLD.heavyNo[root] ] = hInd-1;
	return hInd;
	}


/*
Update sizeTree of <dHLD>  for each subtree of <T> below <root>
Param:  <dHLD> Ref of dataHLD, <T> Const Ref of Tree, <root> Index of Root   
Return: <sz> Size of T(root) in <T>
		-1 If invalid root
*/
int DS_WDFS::updateSize(dataHLD& dHLD,const Tree& T, int root)
	{
	int sz=0;
	if(!(root>=0 && root<T.size()))
		ERR_MSGR("DS_WDFS::UpdateSize invoked with invalid 'root'\n",-1);

	list<int> child = T.child(root);
	list<int> :: iterator it;
	for(it= child.begin(); it!= child.end(); it++)
		{
		sz+= updateSize(dHLD, T, *it);
		}
	dHLD.sizeTree[root]= sz+1;
	return sz+1;
	}



/*
Initiate Heavy Light Decomposition on T 
Param: <T> Given tree T
*/
void DS_WDFS::initHLD(dataHLD& dHLD, const Tree& T)
	{
	dHLD.heavyNo.resize(T.size()+1,-1); 
	dHLD.rHeavyNo.resize(T.size()+1,-1); 
	dHLD.pathHead.resize(T.size()+1,-1); 
	dHLD.sizeTree.resize(T.size()+1,-1); 
	dHLD.lastTree.resize(T.size()+1,-1);

	updateSize(dHLD,T,T.root());
	updateHL(dHLD,T,T.root(),0,0);
	}



//////////////////////////////////////////////////////////////////////
///////////////////// HANDLING SegT RELATED ///////////////////////////
//////////////////////////////////////////////////////////////////////


/*
Initialize the segment tree nodes in parts upto 'cost' steps.
Param: G,dataHLD, ST, <status> CurrState, <&cost> No. of steps to be executed
Return: <state> Place to continue next time.
*/
stateV DS_WDFS::makeSTpart(const Graph& G,const dataHLD& dHLD, ST& st, stateV status,int& cost)
	{
	#ifdef DS_OLD
	if(status.first == 0) 	return status;
	int loc= status.first;		// Loc of Current vertex with SegNode to be updated
	segNodeIt pos1=status.second.first, pos2=status.second.second;
	#else
	if(status == 0) 	return status;
	int loc= status;		// Loc of Current vertex with SegNode to be updated
	#endif
	
	int hLoc = loc - st.offST;  // hIndex of Current vertex with SegNode to be updated

	if(loc >= st.offST)
		{ // Update Leaf Node
		list<int> adj = G.nghbr( dHLD.rHeavyNo[hLoc] );
		list<int> :: iterator it= adj.begin();

		Logger::incrEP(adj.size());	

		while(it!=adj.end())   
			st.segT[loc][ edg( dHLD.heavyNo[*(it++)], hLoc ) ] = 1;

		cost -= adj.size();

		loc--;
		#ifdef DS_OLD
		pSegNodeIt psni;
		if(loc<st.offST)  psni = pSegNodeIt( st.segT[loc*2].begin(), 
						     st.segT[loc*2+1].begin());
		else		   psni = pSegNodeIt( segNodeIt(), segNodeIt());

		return stateV(loc,psni);
		#else
		return loc;
		#endif
		}
	else
		{ // Update Internal Node
		#ifndef DS_OLD
		segNodeIt pos1= st.segT[loc*2].begin(), pos2 = st.segT[loc*2+1].begin();
		#endif

		#ifdef TEST_DS_WDFS
		printf("TMP::%d::%d,(%d,%d) (%d,%d)\n",validState(status,st),
			loc,(*pos1).first.first,(*pos1).first.second,
			(*pos2).first.first,(*pos2).first.second);
		#endif
		while(pos1 != st.segT[loc*2].end() || pos2 != st.segT[loc*2 +1].end())
			{
			pair<edg,int> pe ;
	
			Logger::incrEP();	

			if(pos1 == st.segT[loc*2].end())  // Left child Exhausted
				pe = *(pos2++);
			else	if(pos2 == st.segT[loc*2+1].end())  // Right child Exhausted
				pe = *(pos1++);
			else
				{  // Merge Step
				#ifdef DS_OLD
				if(  *( pos1) > *(pos2) )	
					{assert(pos2!=st.segT[loc*2 +1].end());
					 pe = *(pos2++);}
				else 	pe = *(pos1++);
				#else	
				if(  (*pos1).first > (*pos2).first )	
					{assert(pos2!=st.segT[loc*2 +1].end());
					 pe = *(pos2++);}
				else if( (*pos1).first < (*pos2).first )	
					pe = *(pos1++);
				else // Same first vertex of both edges
					{pe = *(pos1++); pos2++;}
				#endif
			}

			st.segT[loc].insert( st.segT[loc].end(),  pe );
			
			#ifdef DS_OLD
			cost--;
			if(cost<=0) break;
			#endif
			}
		#ifdef DS_OLD	
		return stateV(loc,pSegNodeIt(pos1,pos2));			
		#else
		cost -= st.segT[loc].size();
		return loc-1;			
		#endif
		}		
	}
