/*
 * Copyright (c) 1994-2014 Stephane Poirier
 *
 * stephane.poirier@oifii.org
 *
 * Stephane Poirier
 * 3532 rue Ste-Famille, #3
 * Montreal, QC, H2X 2L1
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#ifndef _SEGMENTSET_H
#define _SEGMENTSET_H


/*
/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
// segset.mc is part of the offworld technologies SEGMENTSET library
// 
// initially build to compiled under microstation in c flat 32 bit memory
// model with basic microstation mdl gui.
//
// this segset.mc structure is used to store all data required to perform
// a segmentation.  the SEGMENTSET structure contains lists of
// SEGMENT, NEIGHBOR, FUSION as well as segmentation's global data.
//
// for integration within oifii, compiled in as cpp code. new c++ classes
// were added to provide c++ interface in the file owsegmentset.h/.cpp.
//
/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
*/


#ifndef _SEGSET_H
#define _SEGSET_H

/*
// segmentset library macros
*/
#ifndef NULL
#define NULL	((void*)0)
#endif
#ifndef TRUE
#define TRUE	1
#endif
#ifndef FALSE
#define FALSE	0
#endif
#define SEGSET_MAXFLOAT 	   10000000000.0
#define SEGSET_MAXINITIALSEGMENT   10000000000
#define SEGSET_MAXFINALSEGMENT	   20000000000


typedef struct OIFIILIB_API tagSEGMENTSET
{
   struct tagSEGMENT* pListSegment;     /* list of all segment, initial + fusioned */
   int* pListNeighbor;	      			/* list of segment's neighbors, it is a idseg list */
   struct tagFUSION* pListFusion;       /* list of fusions */

   double* pfStatistics; 	 /* statistics for all segments */
   int nStatPerSegment; 	 /* number of statistics per segment */
   int nSizeStatInByte; 	 /* total size of pfStatistics in byte */

   int iInitialNumberOfSegment;
   int iFinalNumberOfSegment;	 /* allocated size */
   int iActualNumberOfSegment;
   int iActualNumberOfNeighbor;  /* allocated size */
   int iActualNumberOfFusion;	 /* allocated size */

   int idneiNextAvailNeighbor;	/* used everytime a new segment's neighbor list is created */

   int iTotalNumberOfByteUsed;

} SEGMENTSET;



OIFIILIB_API SEGMENTSET* NewSegmentSet( 	int initialnumberofsegment,
			   				int finalnumberofsegment);

OIFIILIB_API void DeleteSegmentSet( SEGMENTSET* pSegmentSet );

OIFIILIB_API void InitializeSegmentSet( SEGMENTSET* pSegmentSet,
			   			   struct tagPOINTSET* pPointset );

OIFIILIB_API void BuildFusionTree( SEGMENTSET* pSegmentSet );


#endif /* _SEGSET_H */


/*
/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
// segment.mc is part of the the offworld SEGMENTSET c library
//
// initially build to compiled under microstation in c flat 32 bit memory
// model with basic microstation mdl gui.
//
// this structure is used to store information of each segment as well as
// a pointer to its first neighbor segment.  the neighbors are stored in the
// neighbor linked-list, see neighbor.h
//
//
// for integration within oifii, compiled in as cpp code. new c++ classes
// were added to provide c++ interface in the file owsegmentset.h/.cpp.
//
/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
*/


#ifndef _SEGMENT_H
#define _SEGMENT_H


typedef struct OIFIILIB_API tagSEGMENT
{
   int idsegParentSegment;	  /* owner of this segment, initialy -1 meaning itself	*/ 
   int iNumberOfSegment;	  /* number of segments attached to this one, initialy 1 */
   int idsegBestMatchingSegment;  /* best matching neighbor segment */
   int idneiFirstNeighbor;	  /* first neighbor from neighbor list */
   int iNumberOfNeighbor;	  /* number of neighbor (all stored right after idneiFirstNeighbor) */
   double fBestMatchValue;	  /* merge criteria value, -1.0 after segment is merged */

} SEGMENT;



OIFIILIB_API void InitializeSegmentStatistics( struct tagSEGMENTSET* pSegmentSet,
				  				  struct tagPOINTSET* pPointset);


/* this function also initialize:  	-each segment's parent (to itself), idsegParentSegment = -1 */
/* 									-iNumberOfSegment to 1, initialy all individual segments */
OIFIILIB_API void InitializeBestMatchingNeighborSegment( struct tagSEGMENTSET* pSegmentSet );

OIFIILIB_API void SetBestMatchingNeighborSegment( struct tagSEGMENTSET* pSegmentSet,
				     				 int idseg );

OIFIILIB_API double GetSimilarityCriteria( struct tagSEGMENTSET* pSegmentSet,
			     			 int idseg1,
			     			 int idseg2 );

OIFIILIB_API int GetBestMatchingSegmentPair( struct tagSEGMENTSET* pSegmentSet);

OIFIILIB_API int GetOwnerParentSegment( struct tagSEGMENTSET* pSegmentSet,
			   			   int idseg );

OIFIILIB_API double* GetPointerToStatistics( struct tagSEGMENTSET* pSegmentSet,
			       			   int idseg );

OIFIILIB_API struct tagSEGMENT* GetPointerToSegment( struct tagSEGMENTSET* pSegmentSet,
										int idseg );



/*                                                                     
///////////////////////////////////////////////////////////////////////
//following functions added to draw final partition segments boundaries 
///////////////////////////////////////////////////////////////////////
*/       


/* 
// voronoi edge separating ivertexIn from ivertexOut, the edge can be drawn
// connecting center of itriDualTriCCW1 to center of itriDualTriCCW2 
*/
typedef struct OIFIILIB_API tagEDGE 
{
   int ivertexIn;	  /* index of vertex inside the segment of interest */ 
   int ivertexOut;	  /* index of vertex outside the segment of interest */
   int itriDualTriCCW1; /* index of the first triangle containing the two vertex (searching for neigh tri CCW) */
   int itriDualTriCCW2; /* index of the second triangle containing the two vertex (searching for neigh tri CCW) */
   
} EDGE;

int OIFIILIB_API GetOwnerParentSegmentThreshold( struct tagSEGMENTSET* pSegmentSet,
			   			   			int idseg,
			   			   			int iNumberOfSegmentForFinalPartition );

int OIFIILIB_API GetRemainingSegmentListForFinalPartition(	struct tagSEGMENTSET* pSegmentSet,
												int* pidsegList,
			   			   						int iNumberOfSegmentForFinalPartition );

int OIFIILIB_API GetSegmentHistory(	struct tagSEGMENTSET* pSegmentSet,
						int idsegStart,
						int iNumberOfSegmentForFinalPartition,
						int* pidsegHistoryList,
						int* piNumSegForFinalPartitionList,
						int* inumberofcollectedsegment,
						int imaxhistorylistsize); //size of supplied historylist

BOOL OIFIILIB_API GetLastSegmentPairMerged(	struct tagSEGMENTSET* pSegmentSet, 
								int iNumberOfSegmentForFinalPartition, 
								int* idsegment1,
								int* idsegment2);

int OIFIILIB_API GetOneVertexFromSegment(struct tagSEGMENTSET* pSegmentSet, 
							int idseg);

int OIFIILIB_API GetOneSegmentEdge(	struct tagPOINTSET* pPointset, 
						struct tagSEGMENTSET* pSegmentSet, 
						int idsegOwner, 
						int ivertex, 
						int iNumberOfSegmentForFinalPartition,
						struct tagEDGE* pEdge);
 
int OIFIILIB_API IsSegmentEdge(	struct tagPOINTSET* pPointset, 
					struct tagSEGMENTSET* pSegmentSet, 
					int idsegOwner, 
					int ivertexMovingHead, 
					int iNumberOfSegmentForFinalPartition,
					struct tagEDGE* pEdge);

BOOL OIFIILIB_API GetEdgeFrontierLineCoor(struct tagPOINTSET* pPointset, 
							 struct tagEDGE myEdge, 
							 struct tagPOINT2DD* p1Point2DD, 
							 struct tagPOINT2DD* p2Point2DD);

int OIFIILIB_API GetEdgeInsideVertex(struct tagPOINTSET* pPointset, 
						struct tagEDGE myEdge);

int OIFIILIB_API GetEdgeInsideVertex(struct tagPOINTSET* pPointset, 
						struct tagEDGE myEdge);

BOOL OIFIILIB_API CollectAllSegmentEdge(	struct tagPOINTSET* pPointset,
							struct tagSEGMENTSET* pSegmentset,
							int idsegment, //to be drawn
							int idvertex,	 //any one vertex inside the segment to be drawn
							int iNumberOfSegmentForFinalPartition, 
							struct tagEDGE* pEdgeList,
							int* pNumberOfEdgeFound,
							int imaxnumberofedge);

BOOL OIFIILIB_API CollectInterSegmentFrontierEdge(	struct tagPOINTSET* pPointset, 
										struct tagSEGMENTSET* pSegmentSet, 
										int idsegment1,
										int idsegment2,
										int iNumberOfSegmentForFinalPartition, 
										struct tagEDGE* pEdgeList,
										int* p_iNumberOfFrontierEdgeFound,
										int imaxnumberofedge);

BOOL OIFIILIB_API IsInterSegmentFrontierEdge(struct tagPOINTSET* pPointset, 
								struct tagSEGMENTSET* pSegmentSet, 
								int idsegment1,
								int idsegment2,
								int iNumberOfSegmentForFinalPartition, 
								struct tagEDGE* pEdge);
   
int OIFIILIB_API GetNorthernMostNeighbor(struct tagPOINTSET* pPointset, 
							int ivertexMovingHead, 
							int* p_arrayneighbor, 
							int numneighborfound);
   
int OIFIILIB_API MoveOneStepNorth( struct tagPOINTSET* pPointset, 
					  struct tagSEGMENTSET* pSegmentSet, 
					  int* pivertexMovingHead);
 
int OIFIILIB_API GetNextSegmentEdge(	struct tagPOINTSET* pPointset, 
						struct tagSEGMENTSET* pSegmentSet, 
						int idsegOwner, 
						struct tagEDGE* pPreviousEdge, 
						int iNumberOfSegmentForFinalPartition,
						struct tagEDGE* pNextEdge);

#endif /* _SEGMENT_H */




/*
/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
// neighbor.mc  is part of the offworld technologies SEGMENTSET c library
// 
// initially build to compiled under microstation in c flat 32 bit memory
// model with basic microstation mdl gui.
//
// this structure is used to store each segment's neighbors in a linked-list.
// initialy and everytime a new segment is created, we maintain	a linked-list
// of its neighbors.
//
//
// for integration within oifii, compiled in as cpp code. new c++ classes
// were added to provide c++ interface in the file owsegmentset.h/.cpp.
//
/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
*/

/*
// neighbor.h	 defines the structure NEIGHBOR	and its related functions.
//
//
//
//
//
*/

#ifndef _NEIGHBOR_H
#define _NEIGHBOR_H



typedef int NEIGHBOR;	 /* a NEIGHBOR is in fact a idseg, id to pListSegment */

/*
typedef struct tagNEIGHBOR
{
   int idsegSegment;
   int idneiNextNeighbor;
} NEIGHBOR;
*/


/*
// this macro is used when allocating and reallocating pListNeighbor
*/
#define NEIGHBOR_APPROXNUMBERPERSEGMENT     50      /* 20 was OK with the 520 segments file */




OIFIILIB_API void InitializeListNeighbor( struct tagSEGMENTSET* pSegmentSet,
			     struct tagPOINTSET* pPointset);

OIFIILIB_API NEIGHBOR* ReallocListNeighbor( struct tagSEGMENTSET* pSegmentSet,
			       int iextranumberofneighbor );

OIFIILIB_API void MergeNeighborList( struct tagSEGMENTSET* pSegmentSet,
			int idsegNewParent,
			int idsegChild1,
			int idsegChild2 );

OIFIILIB_API NEIGHBOR* GetPointerToNeighbor( struct tagSEGMENTSET* pSegmentSet,
				int idnei );


#endif /* _NEIGHBOR_H */



/*
/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
// classset.mc  is part of the offworld technologies SEGMENTSET c library
// 
// initially build to compiled under microstation in c flat 32 bit memory
// model with basic microstation mdl gui.
//
// the module classset.h/.mc is similar to segset.h/.mc appart from a few 
// differences. their is no need for initializing the neighbor list since
// the classification process is a global one. for large images or data
// points set classification process time is much greater than segmentation
// process time.
//
//
// for integration within oifii, compiled in as cpp code. new c++ classes
// were added to provide c++ interface in the file owsegmentset.h/.cpp.
//
/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
*/



//CLASSSET.H definition file for class set 

#ifndef _CLASSSET_H
#define _CLASSSET_H


OIFIILIB_API struct tagSEGMENTSET* NewClassSet(int initialnumberofsegment, int finalnumberofsegment);
OIFIILIB_API void DeleteClassSet(struct tagSEGMENTSET* pSegmentSet);

OIFIILIB_API void InitializeClassSet(struct tagSEGMENTSET* pSegmentSet, struct tagPOINTSET* pPointset);
OIFIILIB_API void BuildClassFusionTree(struct tagSEGMENTSET* pSegmentSet);


#endif //_CLASSSET_H



/*
/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
// classset.mc  is part of the offworld technologies SEGMENTSET c library
// 
// initially build to compiled under microstation in c flat 32 bit memory
// model with basic microstation mdl gui.
//
// the module classset.h/.mc is similar to segset.h/.mc appart from a few 
// differences. their is no need for initializing the neighbor list since
// the classification process is a global one. for large images or data
// points set classification process time is much greater than segmentation
// process time.
//
//
// for integration within oifii, compiled in as cpp code. new c++ classes
// were added to provide c++ interface in the file owsegmentset.h/.cpp.
//
//
/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
*/

//class.h definition file for class set library

#ifndef _CLASS_H
#define _CLASS_H

OIFIILIB_API void InitializeClassStatistics(struct tagSEGMENTSET* pSegmentSet, struct tagPOINTSET* pPointset);
OIFIILIB_API void InitializeBestMatchingClass(struct tagSEGMENTSET* pSegmentSet);
OIFIILIB_API void SetBestMatchingClass(struct tagSEGMENTSET* pSegmentSet, int idseg);
OIFIILIB_API double GetClassSimilarityCriteria(struct tagSEGMENTSET* pSegmentSet, int idseg1, int idseg2);
OIFIILIB_API int GetBestMatchingClassPair(struct tagSEGMENTSET* pSegmentSet);
OIFIILIB_API int GetOwnerParentClass(struct tagSEGMENTSET* pSegmentSet, int idseg);
OIFIILIB_API int GetOwnerParentClassThreshold(struct tagSEGMENTSET* pSegmentSet, int idseg, int iNumberOfSegmentForFinalPartition);

#endif //_CLASSSET_H


/*
/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
// fusion.mc  is part of the offworld technologies SEGMENTSET c library
// 
// initially build to compiled under microstation in c flat 32 bit memory
// model with basic microstation mdl gui.
//
// fusion.h	 defines the structure FUSION and its related functions.
//
//
// this structure is used to store each fusion, or pair of segment merged,
// sequentialy.  the list of fusion obtained can be stored in memory or on
// disk, it will be used to retreive different stages of the segmentation.
//
//
// interactive application:
//
// the user should be able to visualize various stages of the segmentation.
// specifying a certain number of desired segments, the user should get
// the corresponding partition (view of the remaining segments).
//
// the maximum number of fusion is equal to the initial number of segments.
// the user should be able to specify how many fusions are desired in a
// first pass and then keep performing additional fusions.
//
// provided the initial state and the list of fusion, the user will be able
// to travel in the fusion list, fastforward and rewind, in order to get
// the desired segmentation stage.
//
//
//
//
// for integration within oifii, compiled in as cpp code. new c++ classes
// were added to provide c++ interface in the file owsegmentset.h/.cpp.
//
/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
*/

#ifndef _FUSION_H
#define _FUSION_H


#define FUSION_NUMBEROFCHANNELNAMES		2
#define FUSION_MAXNUMBEROFINFOCHANNEL	2
#define FUSION_INFO_CRITERIA			0
#define FUSION_INFO_SEGMENTSIZE			1
#ifndef COMPILING_FUSION_CPP
	extern char* pszFusionInfoChannelNames[];
#endif


typedef struct OIFIILIB_API tagFUSION
{
   int idsegChildSegment1;	/* child segment 1 */
   int idsegChildSegment2;	/* child segment 2 */  
   double fCriteria; 		/* criteria on merge */
} FUSION;



OIFIILIB_API void MergeSegmentPair(struct tagSEGMENTSET* pSegmentSet,int idsegNewParent,int idsegChild1,int idsegChild2 );

OIFIILIB_API void MergeClassPair(struct tagSEGMENTSET* pSegmentSet, int idsegNewParent, int idsegChild1, int idsegChild2);

OIFIILIB_API void RegisterFusion(struct tagSEGMENTSET* pSegmentSet,int idsegParent,int idseg1,int idseg2 );

OIFIILIB_API FUSION* GetPointerToFusion( struct tagSEGMENTSET* pSegmentSet,int idfus );


OIFIILIB_API BOOL GetFusionSequenceProfile(struct tagSEGMENTSET* pSegmentSet,int iFusionInfo,double* pfProfileStat,int* p_iNumberOfProfileStat,int imaxnumberofstat);

#endif /* _FUSION_H */


#endif //_SEGMENTSET_H