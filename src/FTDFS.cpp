/*******************************************************************************
 * src/FTDFS.cpp
 * Implementation of the fault tolerant DFS algorithm handling all kinds of updates.
 * Addition/Deletion of Vertex and Edges. 
 *
 * Part of Project: Incremental-DFS-Experimental: 
 *                  https://github.com/shahbazk/IncDFS-Experimental
 *
 * Copyright (C) 2017 Shahbaz Khan <shahbazk@cse.iitk.ac.in>
 *
 * All rights reserved. Published under the BSD-2 license in the LICENSE file.
 ******************************************************************************/

#include"../include/FTDFS.h"
///To debug add this to compiler options /#define TEST_FTDFS


/*
Builds the reduced adjacency list for <path> from paths in partT and trees in listT 
Param: DS_WDFS& DS, dataFTDFS& dFT, List of trees to be considered, final path <anc,des>
*/
void FTDFS::buildRedAdj(const DS_WDFS& DS, dataFTDFS& dFT, list<int> listT, pth path)
{
edg e;
const Tree& T_ds = DS.getTree();




if(path.first == path.second && (!T_ds.validV(path.first) || !DS.isCovered(path.first)))
	return;		 // Avoid Singleton paths corresponding to new vertices

TIter itT = listT.begin();
while(itT!= listT.end())
	{	// Handle trees to <path>
	e =  DS.query((*itT),path.second,path.first,0);
	
	if(e.first != -1) 
		dFT.redAdj[e.second].push_back(e.first);
	itT++;
	}

if(DS.isAnc(path.second,path.first))
	path = pth(path.second,path.first);
int currV = path.second;
while(1)
	{
	PIter itx = dFT.partP.begin();
	while(itx!= dFT.partP.end())
		{	// Handle paths to <path>
		if((*itx).first != (*itx).second || 
		   (T_ds.validV((*itx).first) && DS.isCovered((*itx).first)))
			{	// Avoid Singleton paths corresponding to new vertices
			e = DS.query(currV,(*itx).first, (*itx).second, 1);	
			if(e.first == currV) 
				dFT.redAdj[currV].push_back(e.second);		
			}
		itx++;
		}		
	if(currV!= path.first)
		currV = T_ds.par(currV);
	else break;
	}
}


/*
Perform DFS_in_Path in the path having <currV>
Params: <&DS> DS_WDFS for previous DFS Tree, <dFT> Data of FTDFS, entry vertex <currV> 
Return: Ancestor Path list (New Ancestor, ... , New Descendant)
*/
list<int> FTDFS::DFS_Path(const DS_WDFS& DS, dataFTDFS& dFT, int currV)
{		
#ifdef TEST_FTDFS	
printf("FTDFS::DFS_Path with Vertex %d\n",currV);
#endif

int childV, headV;
pth oPath = dFT.pathParam[currV].first;
pth tPath, rPath;  		// Traversed/Remainder Path
list<int> path;
const Tree& T_ds = DS.getTree();

dFT.partP.erase(dFT.pathParam[currV].second);

int lenA,lenD; 			// Length of Anc/Des to currV on oPath 
if(oPath.first == currV)	
	{lenA = 0; headV = currV;}
else	
	for(lenA=1,headV=currV; T_ds.par(headV)!=oPath.first; lenA++,headV=T_ds.par(headV));

if(oPath.second == currV)	
	{lenD = 0; childV = currV;}
else	
	for(lenD=1,childV=oPath.second; T_ds.par(childV)!=currV; lenD++,childV=T_ds.par(childV));

Logger::incrEP(lenA+lenD);


if(lenA >= lenD)
	{
	tPath = pth(currV,oPath.first);
	rPath = (lenD? pth(childV,oPath.second): pth(-1,-1));

	for(childV = currV; childV!= oPath.first; childV=T_ds.par(childV))
		path.push_back(childV);
	path.push_back(childV);
	}	
else	
	{
	tPath = pth(currV,oPath.second);
	rPath = (lenA?pth(oPath.first,T_ds.par(currV)):pth(-1,-1));

	for(childV = oPath.second; childV!= currV; childV=T_ds.par(childV))
		path.push_front(childV);
	path.push_front(childV);
	}

if(rPath.first != -1)
	{// Path Not exhausted
	dFT.partP.push_front( rPath );
	for(childV = rPath.second; childV!= rPath.first; childV=T_ds.par(childV))
		dFT.pathParam[childV] = pPIter(rPath, dFT.partP.begin());
	dFT.pathParam[childV] = pPIter(rPath, dFT.partP.begin());
	}

buildRedAdj(DS, dFT, dFT.partT, tPath);

return path;
}



/*
Perform DFS_in_Tree in the subtree having <currV>
Params: <&DS> DS_WDFS for previous DFS Tree, <dFT> Data of FTDFS, entry vertex <currV> 
Return: Ancestor Path list (New Ancestor, ... , New Descendant)
*/
list<int> FTDFS::DFS_Tree(const DS_WDFS& DS, dataFTDFS& dFT, int currV)
{			
#ifdef TEST_FTDFS
printf("FTDFS::DFS_Tree with Vertex %d\n",currV);
#endif

int childV = -1;			// Child on the path
const Tree& T_ds = DS.getTree();

int parV=currV;
while(dFT.isRoot[parV].first!= 1)	// Root of tree in partT   
	parV= T_ds.par(parV);		
pth oPath = pth(currV,parV);

dFT.partT.erase(dFT.isRoot[parV].second);
dFT.isRoot[parV] = pTIter(0, TIter());

list<int> path;
list<int> listT;

while(childV != parV)
	{
	path.push_back(currV);
	list<int> chld = T_ds.child(currV);
	list<int>::iterator it,it2;

	Logger::incrEP(chld.size());
	

	for(it = chld.begin(); it!=chld.end(); it++)
		{	// Subtrees hanging from the path
		if((*it)!= childV)
			{
			dFT.partT.push_back((*it)); 
			it2 = dFT.partT.end(); it2--; 
			dFT.isRoot[(*it)] = pTIter(1, it2);					
			listT.push_back(*it);
			}
		}

	childV = currV;
	currV = T_ds.par(currV);
	}

buildRedAdj(DS, dFT, listT, oPath);

return path;
}


//////////////////////////////
/////// PUBLIC FUNCTIONS /////
//////////////////////////////


/*
Static Function that returns DFS tree of a graph from the given root <root>
!!!Required!!!: Red Adjacency List of Root populated for singleton paths corresponding to newVertices.
                And root not part of partT or partP
Params: <&DS> DS_WDFS for previous DFS Tree, <dFT> Data of FTDFS, given root 
Return: DFS tree of the graph
*/
Tree FTDFS::FT_DFS(const DS_WDFS& DS, dataFTDFS& dFT, int root)
{
	int currV, nghbrV; 
	stack<int> S;
	Tree T = Tree(dFT.redAdj.size());
	T.setRoot(root);
	
	S.push(root);
	dFT.info[root] = FTDFS::INFO_VISITED;
	buildRedAdj(DS, dFT, dFT.partT, pth(root,root));
	
	while(!S.empty())
		{
		currV = S.top();
		#ifdef TEST_FTDFS	
		printf("FTDFS::FT_DFS Current Vertex %d\n",currV);
		printf("FTDFS::FT_DFS Red List: ");
		list<int> :: iterator it;
		for(it = dFT.redAdj[currV].begin(); it!= dFT.redAdj[currV].end();it++)
			printf(" %d,",*it);
		printf("\n");
		printDFT(dFT);
		#endif
		if(dFT.redAdj[currV].empty())  
			{S.pop();  continue;}
		nghbrV = dFT.redAdj[currV].front();		
		dFT.redAdj[currV].pop_front();

		Logger::incrEP();


		if(dFT.info[nghbrV] == FTDFS::INFO_VISITED) continue;

		list<int> path;
	
		if(dFT.info[nghbrV] == FTDFS::INFO_TREE)		// In Tree
			path = DFS_Tree(DS,dFT,nghbrV);
		else if(dFT.info[nghbrV] == FTDFS::INFO_PATH) 
			path = DFS_Path(DS,dFT,nghbrV);
		else
			ERR_MSG("FTDFS::FT_DFS INVALID INFO\n");

		Logger::incrEP(path.size());
		

		nghbrV = currV;
		while(!path.empty())
			{
			T.addEdge(nghbrV,path.front());
			#ifdef TEST_FTDFS	
			printf("FTDFS::FT_DFS: Added Edge (%d,%d)\n",nghbrV,path.front());
			#endif
			nghbrV = path.front(); 
			path.pop_front();
			dFT.info[nghbrV] = FTDFS::INFO_VISITED;
			S.push(nghbrV);  
			}
		}

	return T;
}


/*
Initialize the structure for dataFTDFS <dFT> for a tree with <size> vertices with root <root>
Adds child subtrees of root to partT and changes the info accordingly
Param: <DS> corresponding DS_WDFS,<dFT>& dataFTDFS,  <size> size of tree, <root> root of T
*/
void FTDFS::initDataFTDFS(const DS_WDFS& DS, dataFTDFS& dFT, int size, int root)
{
dFT.redAdj.resize(size,list<int> ());
dFT.partT.clear();
dFT.partP.clear();
dFT.info.resize(size, (int) INFO_INVALID);
dFT.isRoot.resize(size,pTIter(0,TIter()));
dFT.pathParam.resize(size,pPIter(pth(-1,-1),PIter()) );

dFT.partT = DS.getTree().child(root);
TIter it = dFT.partT.begin();

while(it!= dFT.partT.end())
	{
	dFT.isRoot[*it] = pTIter(1, it);
	initInfoTree(DS,dFT,*it);
	it++;
	}
}

/*
Initialize the <dFT>.info of the whole subtree T(<root>) to INFO_TREE
Param: <DS> DS of the tree, <dFT>, <root>
*/
void FTDFS::initInfoTree(const DS_WDFS& DS, dataFTDFS& dFT, int root)
{
dFT.info[root] = INFO_TREE;
list<int> children = DS.getTree().child(root);
list<int> :: iterator it;

Logger::incrEP();

for(it=children.begin(); it!= children.end();it++)
	initInfoTree(DS,dFT,*it);
}

/*
Increase size of dFT corresponding to added vertex if required
Return: 1 if success
	0 if failure
*/
int FTDFS::addVDFT(dataFTDFS& dFT, int x)
{
if(x> (int) dFT.redAdj.size() || x<0)
	return 0;
if(x== (int) dFT.redAdj.size())
	{
	dFT.redAdj.push_back(list<int>());
	dFT.info.push_back((int) INFO_INVALID);
	dFT.isRoot.push_back(pTIter(0,TIter()));
	dFT.pathParam.push_back(pPIter(pth(-1,-1),PIter()) );
	}
return 1;
}



/*
Update the partition for deleting an edge<x,y> or vertex<x> from partT
Param: <dFT> dataDFDFS, edge<x,y> or vertex<x> (y==-1)
*/
void FTDFS::partitionP(DS_WDFS& DS, dataFTDFS& dFT, int x, int y)
{
const Tree& T_ds = DS.getTree();
if(y!=-1 && T_ds.par(x)==y)
	{int t; t=x;x=y;y=t;}
pth p = dFT.pathParam[x].first;
dFT.partP.erase(dFT.pathParam[x].second);

if(p.second !=x)
	{
	int y1 = p.second;
	while(T_ds.par(y1)!=x) y1=T_ds.par(y1);
	dFT.partP.push_front(pth(y1,p.second));
	y1 = p.second;
	while(y1!=x) 
		{

		Logger::incrEP();

		dFT.pathParam[y1] = pPIter(dFT.partP.front(),dFT.partP.begin());
		y1 = T_ds.par(y1);
		}
	}

if(p.first != x || y!=-1)
	{
	int x1 = (y==-1?T_ds.par(x):x );
	dFT.partP.push_front(pth(p.first,x1));
	while(1)
		{

		Logger::incrEP();

		dFT.pathParam[x1] = pPIter(dFT.partP.front(),dFT.partP.begin());
		if(x1 == p.first) break;
		x1 = T_ds.par(x1);
		}
	}
}



/*
Update the partition for deleting an edge<x,y> or vertex<x> from partT
Param: <dFT> dataDFDFS, edge<x,y> or vertex<x> (y==-1)
*/
void FTDFS::partitionT(DS_WDFS& DS, dataFTDFS& dFT, int x, int y)
{
int childV = -1;			// Child on the path
const Tree& T_ds = DS.getTree();
if(y!=-1 && T_ds.par(x)==y)
	{int t; t=x;x=y;y=t;}

int currV = x;
int parV=currV;
while(dFT.isRoot[parV].first!= 1)	// Root of tree in partT   
	parV= T_ds.par(parV);		

if(parV!= x || y!=-1 )	// Path Added
	{
	dFT.partP.push_front(pth(parV, (y==-1?T_ds.par(x):x)));
	}
dFT.partT.erase(dFT.isRoot[parV].second);
dFT.isRoot[parV] = pTIter(0, TIter());

list<int> listT;

while(childV != parV)
	{
	list<int> chld = T_ds.child(currV);
	list<int>::iterator it,it2;

	Logger::incrEP(chld.size());
	
	for(it = chld.begin(); it!=chld.end(); it++)
		{	// Subtrees hanging from the path
		if((*it)!= childV)
			{
			dFT.partT.push_front((*it)); 
			dFT.isRoot[(*it)] = pTIter(1, dFT.partT.begin());		
			listT.push_back(*it);
			}
		}

	if(currV!=x || y!=-1)
		{
		dFT.info[currV] = FTDFS::INFO_PATH;
		dFT.pathParam[currV] = pPIter(dFT.partP.front(),dFT.partP.begin());
		}

	childV = currV;
	currV = T_ds.par(currV);
	}
}


/*
Print the partition information of the current tree.
Param: <dFT> dataFTDFS for current partition
*/
void FTDFS::printDFT(const dataFTDFS dFT)
{
printf("Partition Information: \n  Trees: ");
list<int> :: const_iterator itT;
for(itT=dFT.partT.begin(); itT!=dFT.partT.end(); itT++)
	printf("(%d) ",*itT);
printf("\n  Paths: ");
list<pth> :: const_iterator itP;
for(itP=dFT.partP.begin(); itP!=dFT.partP.end(); itP++)
	printf("(%d,%d) ",(*itP).first,(*itP).second);
printf("\n");
}

