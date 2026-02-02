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

#include "stdafx.h"
#include "oifiilib.h"

#include <math.h>

/* //spi 2014
#include "logdoc.h"
#include "oifii_app.h"
*/

#include "c_pointset.h"
//since in this file fusion.mc get compiled, and since it is now segmentset.h 
//encapsulate fusion.h
#define COMPILING_FUSION_CPP	1
#include "c_segmentset.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


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
// segset.mc implements the segment set, embedding segment list, neighbor 
// list and merge or fusion list.
//
// for integration within oifii, compiled in as cpp code. new c++ classes
// were added to provide c++ interface in the file owsegmentset.h/.cpp.
//
/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
*/

/*
#include "stdafx.h"
#include <math.h>

#include "logdoc.h"
#include "oifii_app.h"

#include "pointset.h"

#include "segmentset.h"


#include "segment.h"
#include "neighbor.h"
#include "fusion.h"
#include "segset.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
*/

/*
// module globals
*/
int inumberofbyte;


/*///////////////////////
// SegmentSet Constructor
///////////////////////*/
SEGMENTSET* NewSegmentSet( int initialnumberofsegment,
			   int finalnumberofsegment)
{
    SEGMENTSET* pSegmentSet;
    int maxallowedfinalsegment;

    inumberofbyte = sizeof(SEGMENTSET);
    pSegmentSet = (SEGMENTSET*) malloc( inumberofbyte );
    if( pSegmentSet==NULL )
    {
		printf("not able to allocate SEGMENTSET structure\n");
		return NULL;
    }
    pSegmentSet->iTotalNumberOfByteUsed = inumberofbyte;

    /*
    // validate parameters
    */
    maxallowedfinalsegment = 2*initialnumberofsegment - 1;
    if( finalnumberofsegment == 0 ) finalnumberofsegment = maxallowedfinalsegment;
    if( initialnumberofsegment <=0  ||
		initialnumberofsegment > SEGSET_MAXINITIALSEGMENT  ||
		finalnumberofsegment <= initialnumberofsegment	||
		finalnumberofsegment > maxallowedfinalsegment )
    {
		printf("not able to allocate SEGMENTSET structure\n");
		printf("fatal error, initial and final inconsistent.\n");
		return NULL;
    }

    /*
    // these variables must be initialized at this stage
    */
    pSegmentSet->iInitialNumberOfSegment = initialnumberofsegment;
    pSegmentSet->iActualNumberOfSegment = initialnumberofsegment;
    pSegmentSet->iFinalNumberOfSegment = finalnumberofsegment;

    /*
    // allocate space for all segments, initial + fusioned.
    */
    inumberofbyte = pSegmentSet->iFinalNumberOfSegment * sizeof(SEGMENT);
    pSegmentSet->pListSegment = (SEGMENT*) malloc( inumberofbyte );
    if( pSegmentSet->pListSegment==NULL )
    {
		printf("not able to allocate list of SEGMENT\n");
		free(pSegmentSet);
		return NULL;
    }
    pSegmentSet->iTotalNumberOfByteUsed += inumberofbyte;

    /*
    // allocate arbitrary number of neighbor, approx. 10*2*initialnumberofsegment.
    // use realloc when list is full.
    */
    pSegmentSet->iActualNumberOfNeighbor = 10*2*pSegmentSet->iInitialNumberOfSegment;
    inumberofbyte = pSegmentSet->iActualNumberOfNeighbor * sizeof(NEIGHBOR);
    pSegmentSet->pListNeighbor = (NEIGHBOR*) malloc( inumberofbyte );
    if( pSegmentSet->pListNeighbor==NULL )
    {
		printf("not able to allocate list of NEIGHBOR\n");
		free(pSegmentSet->pListSegment);
		free(pSegmentSet);
		return NULL;
    }
    pSegmentSet->idneiNextAvailNeighbor = 0;
    pSegmentSet->iTotalNumberOfByteUsed += inumberofbyte;

    /*
    // allocate fusion list
    */
    pSegmentSet->iActualNumberOfFusion = pSegmentSet->iFinalNumberOfSegment -
					 pSegmentSet->iInitialNumberOfSegment;
    inumberofbyte = pSegmentSet->iActualNumberOfFusion * sizeof(FUSION);
    pSegmentSet->pListFusion = (FUSION*) malloc( inumberofbyte );
    if( pSegmentSet->pListFusion==NULL )
    {
		printf("not able to allocate list of FUSION\n");
		free(pSegmentSet->pListNeighbor);
		free(pSegmentSet->pListSegment);
		free(pSegmentSet);
		return NULL;
    }
    pSegmentSet->iTotalNumberOfByteUsed += inumberofbyte;

    /*
    // we do not allocate pfStatistics here, it will be performed when
    // initializing segmentset from pointset.
    */
    pSegmentSet->pfStatistics = NULL;
    pSegmentSet->nStatPerSegment = 0;
    pSegmentSet->nSizeStatInByte = 0;

    return pSegmentSet;
}


/*//////////////////////////////////////////////
// Used by both NewSegmentSet and OpenSegmentSet
//////////////////////////////////////////////*/
int AllocateSegmentSet(SEGMENTSET* pSegmentSet)
{
    /*
    // allocate space for all segments, initial + fusioned.
    */
    inumberofbyte = pSegmentSet->iFinalNumberOfSegment * sizeof(SEGMENT);
    pSegmentSet->pListSegment = (SEGMENT*) malloc( inumberofbyte );
    if( pSegmentSet->pListSegment==NULL )
    {
		printf("not able to allocate list of SEGMENT\n");
		free(pSegmentSet);
		return FALSE;
    }
    pSegmentSet->iTotalNumberOfByteUsed += inumberofbyte;

    /*
    // allocate arbitrary number of neighbor, approx. 10*2*initialnumberofsegment.
    // use realloc when list is full.
    */
    pSegmentSet->iActualNumberOfNeighbor = 10*2*pSegmentSet->iInitialNumberOfSegment;
    inumberofbyte = pSegmentSet->iActualNumberOfNeighbor * sizeof(NEIGHBOR);
    pSegmentSet->pListNeighbor = (NEIGHBOR*) malloc( inumberofbyte );
    if( pSegmentSet->pListNeighbor==NULL )
    {
		printf("not able to allocate list of NEIGHBOR\n");
		free(pSegmentSet->pListSegment);
		free(pSegmentSet);
		return FALSE;
    }
    pSegmentSet->idneiNextAvailNeighbor = 0;
    pSegmentSet->iTotalNumberOfByteUsed += inumberofbyte;

    /*
    // allocate fusion list
    */
    pSegmentSet->iActualNumberOfFusion = pSegmentSet->iFinalNumberOfSegment -
					 pSegmentSet->iInitialNumberOfSegment;
    inumberofbyte = pSegmentSet->iActualNumberOfFusion * sizeof(FUSION);
    pSegmentSet->pListFusion = (FUSION*) malloc( inumberofbyte );
    if( pSegmentSet->pListFusion==NULL )
    {
		printf("not able to allocate list of FUSION\n");
		free(pSegmentSet->pListNeighbor);
		free(pSegmentSet->pListSegment);
		free(pSegmentSet);
		return FALSE;
    }
    pSegmentSet->iTotalNumberOfByteUsed += inumberofbyte;

    /*
    // we do not allocate pfStatistics here, it will be performed when
    // initializing segmentset from pointset.
    */
    pSegmentSet->pfStatistics = NULL;
    pSegmentSet->nStatPerSegment = 0;
    pSegmentSet->nSizeStatInByte = 0;

    return TRUE;
}


/*//////////////////////
// SegmentSet Destructor
//////////////////////*/
void DeleteSegmentSet( SEGMENTSET* pSegmentSet )
{
    free(pSegmentSet->pListFusion);
    free(pSegmentSet->pListNeighbor);
    free(pSegmentSet->pListSegment);
    if(pSegmentSet->pfStatistics) free(pSegmentSet->pfStatistics);
    free(pSegmentSet);
    return;
}


/*/////////////////////////////////////////////////////////////
// Initialize SegmentSet from a previously constructed pointset
/////////////////////////////////////////////////////////////*/
void InitializeSegmentSet( SEGMENTSET* pSegmentSet, POINTSET* pPointset )
{
    /* 1) Initialize each segment's statistic info */
    InitializeSegmentStatistics( pSegmentSet, pPointset );

    /* 2) Initialize each segment's neighbor list */
    InitializeListNeighbor( pSegmentSet, pPointset );

    /* 3) Initialize each segment's best matching neighbor, parent and criteria */
    InitializeBestMatchingNeighborSegment( pSegmentSet );

    return;
}


/*//////////////////
// Build Fusion Tree
//////////////////*/
void BuildFusionTree( SEGMENTSET* pSegmentSet )
{
    int iStart, iEnd, idsegNewParent, idseg1, idseg2;
    SEGMENT* pSegment1;

    pSegmentSet->iActualNumberOfSegment = pSegmentSet->iInitialNumberOfSegment;
    iStart = pSegmentSet->iActualNumberOfSegment;
    iEnd = pSegmentSet->iFinalNumberOfSegment;
    for(idsegNewParent=iStart; idsegNewParent<iEnd; idsegNewParent++)
    {
		/* //spi 2014
		BOOL bEsc = ((COIIIApp*)AfxGetApp())->PeekKeyState(VK_ESCAPE);
		if(bEsc) 
		{
			CString myString;
			myString.Format("\r\nBuildFusionTree() aborted by user after merging %d segments\r\n", idsegNewParent-iStart);
			((COIIIApp*)AfxGetApp())->GetLogDocument()->AddText(myString);
			return;
		}
		*/

		idseg1 = GetBestMatchingSegmentPair( pSegmentSet );
		pSegment1 = GetPointerToSegment( pSegmentSet, idseg1 );
		idseg2 = pSegment1->idsegBestMatchingSegment;
		MergeSegmentPair( pSegmentSet,
						  idsegNewParent,
						  idseg1,
						  idseg2 );
		pSegmentSet->iActualNumberOfSegment++;
    }
    return;
}



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
// segment.mc implements the SEGMENT list related functions.
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


/*
#include "stdafx.h"
#include <math.h>

#include "pointset.h"
*/

/*
#include "segment.h"
#include "fusion.h"
#include "neighbor.h"
#include "segset.h"
*/

/*
#include "logdoc.h"
#include "oifii_app.h"
*/

/*
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
*/


/*
// module globals
*/
/* //spi 2014, removed because of redefinition
typedef struct tagPOINT2DD 
{ // pt  

    double x;
    double y;
} POINT2DD;
*/



/*
////////////////////////
// Initialize Segment List
//////////////////////////
*/
void InitializeSegmentStatistics( 	SEGMENTSET* pSegmentSet,
				  					POINTSET* pPointset)
{
    /*
    // Initialize Statistics info
    */
    if( pSegmentSet->iInitialNumberOfSegment != pPointset->npts )
    {
		ASSERT(FALSE);
		/* error */
		printf("Fatal Error, initial number of allocated segment inconsistent\n");
		printf("             with pointset's number of element.\n");
		exit(0);
    }

    pSegmentSet->pfStatistics = pPointset->pfStatistics;
    pSegmentSet->nStatPerSegment = pPointset->nStatPerPoint;
    pSegmentSet->nSizeStatInByte = pPointset->nSizeStatInByte;
    if( ((pSegmentSet->nStatPerSegment)
	//*2 added by poirier on april 10th 1996
	     *(pSegmentSet->iInitialNumberOfSegment) * 2 
	     * (int)sizeof(double) ) > pSegmentSet->nSizeStatInByte )	//!= changed for >= 
    {
		ASSERT(FALSE);
		/* error */
		//printf("Fatal Error pfStatistics inconsistent\n");
		printf("Fatal Error not enough allocated space for pfStatistics\n");
		exit(0);
    }
    pSegmentSet->iTotalNumberOfByteUsed += pSegmentSet->nSizeStatInByte;
    pPointset->pfStatistics = NULL; /* null the pointer because it does not belong to pPointset anymore */
    pPointset->nStatPerPoint = 0;
    pPointset->nSizeStatInByte = 0;

    return;
}



void InitializeBestMatchingNeighborSegment( SEGMENTSET* pSegmentSet )
{
    /*
    // this function also initialize each segment's parent (to itself)
    */
    SEGMENT* pSegment;
    int idseg;

    for(idseg=0; idseg<(pSegmentSet->iInitialNumberOfSegment); idseg++)
    {
		pSegment = GetPointerToSegment(pSegmentSet, idseg);
		pSegment->idsegParentSegment = -1; /* each segment owns itself */
		pSegment->iNumberOfSegment = 1; /* each segment is indivudual */
		SetBestMatchingNeighborSegment(pSegmentSet, idseg);
    }
    return;
}


void SetBestMatchingNeighborSegment( SEGMENTSET* pSegmentSet,
				     int idseg )
{
    double fCriteria, fBestCriteriaSoFar;
    int idnei, idneiStart, idneiEnd;
    int idsegNeighbor;
    SEGMENT* pSegment;

    fBestCriteriaSoFar = SEGSET_MAXFLOAT;
    pSegment = GetPointerToSegment(pSegmentSet, idseg);
    idneiStart = pSegment->idneiFirstNeighbor;
    idneiEnd = idneiStart + pSegment->iNumberOfNeighbor;

    if(idneiEnd == idneiStart)
    {
		/*
		// 0) if there is no neighbor in the list, i.e. for the last segment
		*/
		pSegment->idsegBestMatchingSegment = -1;
		pSegment->fBestMatchValue = -1.0;
		return;
    }
    for( idnei=idneiStart; idnei<idneiEnd; idnei++)
    {
		/*
		// 1) get criteria between idseg and idseg(idnei)
		*/
		idsegNeighbor = pSegmentSet->pListNeighbor[idnei];
		fCriteria = GetSimilarityCriteria( 	pSegmentSet,
						   					idseg,
						   					idsegNeighbor );
		/*
		// 2) if(best so far) store proper values
		*/
		if(fCriteria < fBestCriteriaSoFar)
		{
		    pSegment->idsegBestMatchingSegment = idsegNeighbor;
		    pSegment->fBestMatchValue = fCriteria;
		    fBestCriteriaSoFar = fCriteria;
		}
    }
    //
    // used when debugging
    //
    if( fBestCriteriaSoFar == SEGSET_MAXFLOAT )
    {
		ASSERT(FALSE);
		printf("fatal error, in SetBestMatchingNeighborSegment\n");
		printf("             no best matching neighbor set\n");
		exit(0);
    }
    return;
}


double GetSimilarityCriteria( SEGMENTSET* pSegmentSet,
			     			 int idseg1,
			     			 int idseg2 )
{
    // 
    // -could be using Ward criteria or any other, just be carefull
    //  so that the fCriteria returned is minimizing, that is approaching
    //  0.0 for best match. if it is not minimizing, you will have to
    //  modify the code elsewhere.
    //
    // -also, be carefull on the signification of your statistics, different
    //  input data file will have different statistics ordering (stat 0, 1, 2, ...),
    //  therefore the code written in here should be adapted with input file format.
    //
    // -the most obvious criteria to take is to compute the euclidian distance between
    //  the two statistics vectors, it is a minimizing criteria.
    //
    int i, n1, n2;
    double fCriteria, fDiff, fSum;
	//double f1, f2, fStandardDev, fDiffMax;
    double* pfSeg1Statistics;
    double* pfSeg2Statistics;
	SEGMENT* pSegment;

    pfSeg1Statistics = GetPointerToStatistics( pSegmentSet, idseg1 );
    pfSeg2Statistics = GetPointerToStatistics( pSegmentSet, idseg2 );

	pSegment = GetPointerToSegment(pSegmentSet, idseg1);
	n1 = pSegment->iNumberOfSegment;
	pSegment = GetPointerToSegment(pSegmentSet, idseg2);
	n2 = pSegment->iNumberOfSegment;
    /* 
    //example 1:
    //
    //similar to euclidian distance    
    fSum = 0.0;
    for(i=0; i<(pSegmentSet->nStatPerSegment); i++)
    {
		fDiff = pfSeg1Statistics[i] - pfSeg2Statistics[i];
		if( fDiff >= 0.0 ) fSum = fSum + fDiff;
		  else fSum = fSum - fDiff;
    }
    fCriteria = fSum / (float)(pSegmentSet->nStatPerSegment);
    */
    
    /*
    //example 2:
    //
	//-use euclidian distance measure
    //-use neighboring voronoi area (1/(density)) 
    */
    fSum = 0.0;
    i = 0;	//0 for POINTSET_OFFSETSTAT_VOROAREA
			//1 for POINTSET_OFFSETSTAT_VORODENSITY
	if(pfSeg1Statistics[POINTSET_OFFSETSTAT_VOROAREA]<0
		&& pfSeg2Statistics[POINTSET_OFFSETSTAT_VOROAREA]<0)
		return 0.0; //in order to merge the bounding segments all together initially
	/*
	if(pfSeg1Statistics[POINTSET_OFFSETSTAT_VOROAREA]<0
		|| pfSeg2Statistics[POINTSET_OFFSETSTAT_VOROAREA]<0)
		return 9999.99; //in order to avoid merging any of the bounding segments
	*/

	// last used in april 25th 97
	 //with only one statistic
	i = POINTSET_OFFSETSTAT_NEIGHAVERAGEVOROAREA; //neighboring voroarea
	fDiff = pfSeg1Statistics[i] - pfSeg2Statistics[i]; 
	if( fDiff >= 0.0 ) fSum = fSum + fDiff;
	  else fSum = fSum - fDiff;   
    fCriteria = fSum;
   	

	/* //last used in dec 96
	//with more than one statistic
	i = POINTSET_OFFSETSTAT_NEIGHAVERAGEVOROAREA; //neighboring voroarea
	fDiff = pfSeg1Statistics[i] - pfSeg2Statistics[i]; 
	fSum = fSum + fDiff*fDiff;
	*/

	/*
	i = POINTSET_OFFSETSTAT_NEIGHAVERAGETREEAREA; //neighboring treearea
	fDiff = pfSeg1Statistics[i] - pfSeg2Statistics[i]; 
	fSum = fSum + fDiff*fDiff;
	i = POINTSET_OFFSETSTAT_NEIGHAVERAGEINTENSITY; //neighboring treeintensity
	fDiff = pfSeg1Statistics[i] - pfSeg2Statistics[i]; 
	fSum = fSum + fDiff*fDiff;
	*/
    //fCriteria = sqrt(fSum);
    //fCriteria = fSum * n1 * n2 / (n1 + n2);


	//checking the most discriminating texture feature (statistic)
	
	/* poirier, march 27 1997
	i = POINTSET_EXTRA_NUMBEROFSTAT + POINTSET_XYZI_INTENSITY; //tree intensity
	fDiff = pfSeg1Statistics[i] - pfSeg2Statistics[i]; 
	i = POINTSET_OFFSETSTAT_NEIGHVARIANCEINTENSITY; //neighboring tree intensity variance
	f1 = pfSeg1Statistics[i];
	f2 = pfSeg2Statistics[i];
	if(f1>f2) fStandardDev = sqrt(f1);
	  else fStandardDev = sqrt(f2);	
	fDiff = fDiff / fStandardDev;
	fDiffMax = fDiff;

	i = POINTSET_EXTRA_NUMBEROFSTAT + POINTSET_XYZI_TREEAREA; //tree area
	fDiff = pfSeg1Statistics[i] - pfSeg2Statistics[i]; 
	i = POINTSET_OFFSETSTAT_NEIGHVARIANCETREEAREA; //neighboring tree area variance
	f1 = pfSeg1Statistics[i];
	f2 = pfSeg2Statistics[i];
	if(f1>f2) fStandardDev = sqrt(f1);
	  else fStandardDev = sqrt(f2);	
	fDiff = fDiff / fStandardDev;
	if(fDiff>fDiffMax) fDiffMax = fDiff;

	i = POINTSET_OFFSETSTAT_VOROAREA; //voronoi area, tree density
	fDiff = pfSeg1Statistics[i] - pfSeg2Statistics[i]; 
	i = POINTSET_OFFSETSTAT_NEIGHVARIANCEVOROAREA; //neighboring voronoi area variance (tree density variance)
	f1 = pfSeg1Statistics[i];
	f2 = pfSeg2Statistics[i];
	if(f1>f2) fStandardDev = sqrt(f1);
	  else fStandardDev = sqrt(f2);	
	fDiff = fDiff / fStandardDev;
	if(fDiff>fDiffMax) fDiffMax = fDiff;

	fCriteria = fDiffMax;
	*/

    /*
    // used when debugging
    printf("fCriteria = %f\n", fCriteria);
    */
    return fCriteria;
}


int GetBestMatchingSegmentPair( SEGMENTSET* pSegmentSet)
{
    //
    // -assuming best matching neighbor have been previously identified
    //	for each of the segments present in the list.
    // -this function will then return the segment pair that minimizes
    //	the similarity criteria (global segment list search).
    //
    // Note: since performing a global search for every merge is
    //	     time consuming, we could think of randomizing a little
    //	     the way we pick the next pair to be matched.  could also
    //	     be interesting to use an initial ordered list of best
    //	     matching segment pair.
    //
    int idseg, idsegBestMatchingPair;
    SEGMENT* pSegment;
    double fBestMatchValueSoFar, fValue;

    fBestMatchValueSoFar = SEGSET_MAXFLOAT;
    idsegBestMatchingPair = -1;
    for(idseg=0; idseg<pSegmentSet->iActualNumberOfSegment; idseg++)
    {
		pSegment = GetPointerToSegment( pSegmentSet, idseg );
		// if owner segment
		if( pSegment->idsegParentSegment == -1 )
		{
		    fValue = pSegment->fBestMatchValue;
		    if( fValue < fBestMatchValueSoFar )
		    {
				fBestMatchValueSoFar = fValue;
				idsegBestMatchingPair = idseg;
		    }
		}
    }
    if( fValue == SEGSET_MAXFLOAT )
    {
		ASSERT(FALSE);
		printf("fatal error, in GetBestMatchingSegmentPair()\n");
		printf("             no best matching segment pair found\n");
		exit(0);
    }
    return idsegBestMatchingPair;
}


int GetOwnerParentSegment( SEGMENTSET* pSegmentSet,
			   			   int idseg )
{
    //
    // -assuming all new segment's idsegParentSegment initialize with -1
    // -this function should be called for every merge (fusion) in order
    //	to make sure the fusion list is made of segment's owner.
    //                                                         
    SEGMENT* pSegment;
    int idsegOwner;
    
    /* commented out, see below
    // -here, we want to get the parent segment, assuming segmentation performed until the end, 
    //  iNumberOfSegmentForFinalPartition is 1 in this case.
    int iNumberOfSegmentForFinalPartition; 
    iNumberOfSegmentForFinalPartition = (pSegmentSet->iInitialNumberOfSegment)*2 - pSegmentSet->iFinalNumberOfSegment;
	idsegOwner = GetOwnerParentSegmentThreshold( pSegmentSet,
			   			   						 idseg
			   			   						 iNumberOfSegmentForFinalPartition );
	*/
	
    /* 
    // -in order to optimize for speed, because this function is called often when fusionning,
    //  we do not want to call GetOwnerParentSegmentThreshold()
    */
    do
    {
		idsegOwner = idseg;
		/* for debugging only 
		if( idseg < 0 || idseg >= pSegmentSet->iFinalNumberOfSegment )
		{
		    printf("fatal error in GetOwnerParentSegment(), idseg=%i\n", idseg);
		    exit(1);
		}  
		*/
		pSegment = GetPointerToSegment(pSegmentSet, idseg);
		idseg = pSegment->idsegParentSegment;
		/* for debugging only
		if(idseg == idsegOwner)
		{
		    printf("fatal error in GetOwnerParentSegment(), idsegOwner=idseg=%i", idseg);
		    exit(1);
		} 
		*/
    } while( idseg != -1 );
			   			   						 
    return idsegOwner;
}                                    	
    
    

int GetOwnerParentSegmentThreshold( SEGMENTSET* pSegmentSet,
			   			   			int idseg,
			   			   			int iNumberOfSegmentForFinalPartition )
{
    /*
    // -assuming all new segment's idsegParentSegment initialize with -1
    // -this function should be called for every merge (fusion) in order
    //	to make sure the fusion list is made of segment's owner. 
    //
    // -if iNumberOfSegmentForFinalPartition bigger than 1, it means that
    //  these last fusions will not qualify for parent segment.
	//
    // -this mechanism enable to produce partition with various number of
    //  segments.
    */
    SEGMENT* pSegment;
    int idsegOwner;
    int idsegMaxThreshold;
    
    idsegMaxThreshold = (pSegmentSet->iInitialNumberOfSegment)*2 - iNumberOfSegmentForFinalPartition;
    /* for debugging only */ 
    if(idsegMaxThreshold > pSegmentSet->iFinalNumberOfSegment)
    {
		printf("fatal error in GetOwnerParentSegmentThreshold(), idsegMaxThreshold>iFinalNumberOfSegment=%i", idsegMaxThreshold);
		ASSERT(FALSE);
    	//exit(0);
		return -1; //development-time error
    }
	int iCount = 0;
    do
    {
		iCount++;
		idsegOwner = idseg;
		/* for debugging only */
		if( idseg < 0 || idseg >= pSegmentSet->iFinalNumberOfSegment )
		{
		    printf("fatal error in GetOwnerParentSegment(), idseg=%i\n", idseg);
			ASSERT(FALSE);
		    //exit(1);
			return -1; //development-time error
		}
		pSegment = GetPointerToSegment( pSegmentSet, idseg );
		idseg = pSegment->idsegParentSegment;
		/*
		// used when debugging
		*/
		if(idseg == idsegOwner)
		{
		    printf("fatal error in GetOwnerParentSegment(), idsegOwner=idseg=%i", idseg);
			ASSERT(FALSE);
		    //exit(1);
			return -1; //development-time error

		}
		//possibility of endless loop in here if no owner segment found, 
		//this problem occur when generating partition with low number of segment
		if(iCount>pSegmentSet->iInitialNumberOfSegment)
		{
			//this condition might not be robust, but if the search for owner does not
			//converge!!! development-time error
			ASSERT(FALSE);
			return idsegOwner; //the returned owner should not be valid 
		}
    } while( idseg != -1 && idseg < idsegMaxThreshold);

    return idsegOwner;
}


/* 
// user of this function must allocate its own idsegHistoryList (array of int) to 
// collect the id of segment contained in the specified final partition. the user must
// therefore free this idsegHistoryList after done.
//
// some of the idseg returned by this function will correspond to an initial segment
// while some others to segments created at fusion-time (during segmentation). this
// means not all returned idseg will directly correspond to a ivertex, therefore one
// cannot query their neighbors before obtaining a ivertex for these idseg (see function
// GetOneVertexFromSegment()). 
*/
int GetSegmentHistory(	SEGMENTSET* pSegmentSet,
						int idsegStart,
						int iNumberOfSegmentForFinalPartition,
						int* pidsegHistoryList,
						int* piNumSegForFinalPartitionList,
						int* inumberofcollectedsegment,
						int imaxhistorylistsize)
{   
	int iInitNumSeg, idsegFinal;  
	int i, idsegChild1, idsegChild2, idfusion, idfusionStart, idfusionEnd; 
    FUSION* pFusion;
	
	if(pidsegHistoryList==NULL || piNumSegForFinalPartitionList==NULL)
	{
		ASSERT(FALSE);
		return FALSE;
	} 
	
	//browse into the fusion list, starting at ...
	iInitNumSeg = pSegmentSet->iInitialNumberOfSegment;	
	idsegFinal = 2*(iInitNumSeg)-1 -iNumberOfSegmentForFinalPartition; 
	idfusionStart = 0; 
	idfusionEnd = idsegFinal+1 - iInitNumSeg;
	//reset segment's counter
    i=0; 
	//add the input segment 
	/* do not add it for now, because it will usualy be draw separately
	pidsegHistoryList[i] = idsegStart;
	i++;
	*/
	int idsegLast = idsegStart;
    //search in the fusion list, for each of the segment present in the final partition
    for(idfusion=idfusionStart; idfusion<idfusionEnd+1; idfusion++)
    {   
		pFusion = GetPointerToFusion( pSegmentSet, idfusion );
		idsegChild1 = pFusion->idsegChildSegment1;
		idsegChild2 = pFusion->idsegChildSegment2;
        if(idsegChild1==idsegLast || idsegChild2==idsegLast)
        {
			idsegLast = idfusion + iInitNumSeg;
    		pidsegHistoryList[i] = idsegLast;
			piNumSegForFinalPartitionList[i] = iInitNumSeg - idfusion;
    		i++;
			if(i==imaxhistorylistsize-1)
			{
				//not enough room in supply list 
				ASSERT(FALSE);
				//stop collecting history, but return safely
				inumberofcollectedsegment[0] = i;
				return FALSE;
			}
    	}
    } 
	inumberofcollectedsegment[0] = i;
	return TRUE;
}



/* 
// user of this function must allocate its own idsegList (array of int) to collect 
// the id of segment contained in the specified final partition.  the user must
// therefore free this idsegList after done.
//
// some of the idseg returned by this function will correspond to an initial segment
// while some others to segments created at fusion-time (during segmentation). this
// means not all returned idseg will directly correspond to a ivertex, therefore one
// cannot query their neighbors before obtaining a ivertex for these idseg (see function
// GetOneVertexFromSegment()). 
*/
int GetRemainingSegmentListForFinalPartition(	SEGMENTSET* pSegmentSet,
												int* pidsegList,
			   			   						int iNumberOfSegmentForFinalPartition )
{   
	int iInitNumSeg, idsegFinal, idsegStart, idsegEnd;  
	int i, idsegChild1, idsegChild2, idfusion, idfusionStart, idfusionEnd; 
    FUSION* pFusion;
	
	if(pidsegList==NULL)
	{
		ASSERT(FALSE);
		printf("error pidsegList==NULL in GetRemainingSegmentListForFinalPartition()"); 
		exit(0);
	} 
	
	/*	browse into the fusion list, starting at ... */
	iInitNumSeg = pSegmentSet->iInitialNumberOfSegment;	
	idsegFinal = 2*(iInitNumSeg)-1 -iNumberOfSegmentForFinalPartition; 
	//poirier, April 97
	//idsegStart = idsegFinal + 1; 
	idsegStart = idsegFinal + 1; 
	idsegEnd = 2*(iInitNumSeg) - 1 - 1;  
	idfusionStart = idsegStart - iInitNumSeg;
	idfusionEnd = idsegEnd - iInitNumSeg;
    /* 	search in the fusion list, for each of the segment present in the final partition */
    i=0;
	/* //poirier dec 98, modified for <=
    for(idfusion=idfusionStart; idfusion<idfusionEnd+1; idfusion++)
	*/
    /* //poirier sept 2001, modified 
	//for(idfusion=idfusionStart; idfusion<=idfusionEnd+1; idfusion++)
	*/
	for(idfusion=idfusionStart; idfusion<(idfusionEnd+1); idfusion++)
    {   
		pFusion = GetPointerToFusion( pSegmentSet, idfusion );
		idsegChild1 = pFusion->idsegChildSegment1;
		idsegChild2 = pFusion->idsegChildSegment2;
        if(idsegChild1<idsegStart)
        {
    		pidsegList[i] = idsegChild1;
    		i++;			
    	}
    	if(idsegChild2<idsegStart)
    	{
    		pidsegList[i] = idsegChild2;
    		i++;			
    	}
    } 
	if( i!=iNumberOfSegmentForFinalPartition )
	{
		/* //spi 2014
 		CLogDocument* pLogDocument = ((COIIIApp*)AfxGetApp())->GetLogDocument();
		CString myString;
		myString.Format("warning, numsegfound %i != numsegfinalpartition %i (segment.mc)\r\n", i, iNumberOfSegmentForFinalPartition);
		pLogDocument->AddText(myString);
		*/
		printf("we are in trouble! %i!=%i (segment.mc)", i, iNumberOfSegmentForFinalPartition);
		ASSERT(FALSE);
		//exit(0);
		return FALSE;
	}	  
	return TRUE;
}


//return the two segments that were merge to produce partition with 
//iNumberOfSegmentForFinalPartition number of segment
//this function is used to update partition localy when displaying next partition                 
BOOL GetLastSegmentPairMerged(	SEGMENTSET* pSegmentSet, 
								int iNumberOfSegmentForFinalPartition, 
								int* idsegment1,
								int* idsegment2)
{   
	if(iNumberOfSegmentForFinalPartition<1 || 
		iNumberOfSegmentForFinalPartition>pSegmentSet->iInitialNumberOfSegment )
	{
		ASSERT(FALSE);
		idsegment1[0] = -1;
		idsegment2[0] = -1;
		return FALSE;
	}
	/*	go into the fusion list, starting at ... */
	int iInitNumSeg = pSegmentSet->iInitialNumberOfSegment;	
	int idsegFinal = 2*(iInitNumSeg)-1 -iNumberOfSegmentForFinalPartition; 
	int idsegStart = idsegFinal; 
	int idsegEnd = 2*(iInitNumSeg) - 1 - 1;  
	int idfusionStart = idsegStart - iInitNumSeg;
	int idfusionEnd = idsegEnd - iInitNumSeg;

    FUSION* pFusion;
    pFusion = GetPointerToFusion( pSegmentSet, idfusionStart );
    idsegment1[0] = pFusion->idsegChildSegment1;
    idsegment2[0] = pFusion->idsegChildSegment2;
	return TRUE;
}

/* 
// this function is looking in the fusion tree, if idseg is an initial
// segment then it directly correspond to ivertex. otherwise, this function
// looks back in the fusion tree until it hits a idseg part of the initial
// segments list.
//
// i.e. this function can be called after GetRemainingSegmentListForFinalPartition()
//      to get an ivertex contained in each of the segment list.
*/                
int GetOneVertexFromSegment(SEGMENTSET* pSegmentSet, 
							int idseg)
{   
	int ivertex, idsegChild1, idsegChild2, idfusion; 
    FUSION* pFusion;
	
	/* while we have a segment created at fusion-time */
	while(idseg>=pSegmentSet->iInitialNumberOfSegment)
	{
    	idfusion = idseg - (pSegmentSet->iInitialNumberOfSegment);
    	pFusion = GetPointerToFusion( pSegmentSet, idfusion );
    	idsegChild1 = pFusion->idsegChildSegment1;
    	idsegChild2 = pFusion->idsegChildSegment2;
		if(idsegChild1<pSegmentSet->iInitialNumberOfSegment) idseg = idsegChild1;
		  else idseg = idsegChild2;
		/* 
		// this loop should not be infinite because at idseg = iInitialNumberOfSegment 
		// (first fusion) the two childs must be initial segments (that's worse case). 
		*/   
	}
	/* idseg now garanteed to correspond to a vertex */
	ivertex = idseg;
	return ivertex;
}

/*
// -this function is used to get one edge of the specified segment, idsegOwner.
// -ivertex is used as a seed for starting the search of an edge, in the initial
//  version, this function will call MoveOneStepNorth() to perform the search in
//  the north bound direction from the seed location.
//
// -the EDGE returned can be used as starting point when attempting to draw one 
//  segment's contour. 
*/
int GetOneSegmentEdge(	POINTSET* pPointset, 
						SEGMENTSET* pSegmentSet, 
						int idsegOwner, 
						int ivertex, 
						int iNumberOfSegmentForFinalPartition,
						EDGE* pEdge)
{   
	int ivertexMovingHead;
	/*
	FILE* pFile;
	*/

	pEdge->ivertexIn = -1; /* if still -1 at the end, error */
	pEdge->ivertexOut = -1; /* if still -1 at the end, error */
	pEdge->itriDualTriCCW1 = -1;
	pEdge->itriDualTriCCW2 = -1;
	
	ivertexMovingHead = ivertex;
	int iCount = 0;
	/*
	#ifdef _DEBUG
		pFile = fopen("movinghead.dat", "w");
		fprintf(pFile, "%i\t%i\t%f\t%f\n", iCount, ivertexMovingHead, pPointset->px[ivertexMovingHead], pPointset->py[ivertexMovingHead]);
	#endif //_DEBUG
	*/
	while(IsSegmentEdge(pPointset, 
						pSegmentSet, 
						idsegOwner, 
						ivertexMovingHead,
						iNumberOfSegmentForFinalPartition, 
						pEdge) == FALSE ) 
	{	
		iCount++;
		/* //since by default IsSegmentEdge() is now suggesting the next vertex to pick (northward)
		if(!MoveOneStepNorth( pPointset, pSegmentSet, &ivertexMovingHead))
		{
			printf("development-time error in GetOneSegmentEdge()");
			exit(0);
		}
		*/
		if(pEdge->ivertexIn==-1 && pEdge->ivertexOut!=-1)
		{  
			/* case where ivertexOut contains the suggested northernmost vertex */
			ivertexMovingHead = pEdge->ivertexOut;
		}
		/*
		#ifdef _DEBUG
			fprintf(pFile, "%i\t%i\t%f\t%f\n", iCount, ivertexMovingHead, pPointset->px[ivertexMovingHead], pPointset->py[ivertexMovingHead]);
		#endif //_DEBUG
		*/

		//if edge never found, possibility of getting stock in here
		//this situation seems to append when generating partition with small number of final segment				
		if(iCount>pSegmentSet->iInitialNumberOfSegment)
		{
			//development-time error
			ASSERT(FALSE);
			//return FALSE; //serious situation, TO BE FIXED
			pEdge->ivertexIn=-1;
			pEdge->ivertexOut=-1;
			break;
		}
	}
	/*
	#ifdef _DEBUG
		fclose(pFile);
	#endif //_DEBUG
	*/

	/*
	//it is our job in here to garantee that myEdge is valid, going one step
	//north should eventualy bring us to a segment edge, worst case being 
	//ivertexMovingHead hit the bounding segment (for this function to work,
	//there must be more than one segment in final partition).
	*/ 
	if(pEdge->ivertexIn!=-1 && pEdge->ivertexOut==-1)
	{   
		/* case where hitting the boundary segment, let it go for now, will be trapped at higher level*/                                                  
		
		return FALSE;
		/* development-time error, the boundary segment should not be encircled (initialy)
		printf("error, hit the boundary in IsSegmentEdge()\n");
		exit(0); 
		*/
	}
	else if(pEdge->ivertexIn==-1 && pEdge->ivertexOut==-1)
	{
		//when count exceeds, that is no edge was found
		ASSERT(FALSE); //development-time error
		return FALSE;
	}
    return TRUE;
}

 
/*
// this function determines if ivertexMovingHead belongs to another or to the same segment,
// if it belongs to another segment we have an edge, it sets pEdge properly and returns TRUE.
// if the edge touches the data set boundary, some fields of pEdge will be set to -1, see below.
//
// it returns FALSE when no edge found, pEdge then contains info about where to go next.  
//
// comments:  a similar function could be used for region growing clustering, starting from
//            a given initial segment (vertex), we could find an edge in a given direction and then
//            keep finding the next ccw edge trying to close the polygon.  we would base the decision
//            for determining if edge or not on some local statistical measure.  i.e. based on 
//            photo-interpreter input (polyline boundary drawn between two distinct textures)
*/ 
int IsSegmentEdge(	POINTSET* pPointset, 
					SEGMENTSET* pSegmentSet, 
					int idsegOwner, 
					int ivertexMovingHead,
					int iNumberOfSegmentForFinalPartition, 
					EDGE* pEdge)
{ 
    int i, idseg;
    
    int numtrifound, numneighborfound, itriseed;
    int p_arraytri[200];
    int p_arrayneighbor[200];

    itriseed = 0;
	if( FindAllValidTriSurroundingVertex( pPointset,
					       				  ivertexMovingHead,
					       				  &itriseed,
								          &numtrifound,
								          p_arraytri,
								          &numneighborfound,
								          p_arrayneighbor) == TRUE)
	{
		/* ivertexMovingHead is fully surrounded by neighbors */
		
		/* to access safely i+1 tri */ 
		p_arraytri[numtrifound] = p_arraytri[0]; 
		
		/* check if all surrounding neighbors in the same segment */ 
	    for(i=0; i<numneighborfound; i++)
	    {                               
	    	idseg = GetOwnerParentSegmentThreshold(pSegmentSet, p_arrayneighbor[i], iNumberOfSegmentForFinalPartition);
	    	if(idseg != idsegOwner)
	    	{   
	    		/* edge found */
				pEdge->ivertexIn = ivertexMovingHead; 
				pEdge->ivertexOut = p_arrayneighbor[i]; 
				pEdge->itriDualTriCCW1 = p_arraytri[i];
				pEdge->itriDualTriCCW2 = p_arraytri[i+1];
		    	/* if returns TRUE, then myEdge is garanteed to be valid */
				return TRUE;	    		
	    	}
	    	
	    }
	    /* edge not found, all surrounding neighbors have same idsegOwner */
		pEdge->ivertexIn = -1; 
		pEdge->ivertexOut = -1; 
		pEdge->itriDualTriCCW1 = -1;
		pEdge->itriDualTriCCW2 = -1;  
		/* suggest where to go next, north most direction */
		pEdge->ivertexOut = GetNorthernMostNeighbor(pPointset, 
													ivertexMovingHead, 
													p_arrayneighbor, 
													numneighborfound);
	    return FALSE; /* not an edge, but ivertexOut contains a suggestion for where to go next */
	}
	else	
	{   
		/* ivertexMovingHead is on the boundary */ 
		pEdge->ivertexIn = ivertexMovingHead; 
		pEdge->ivertexOut = -1; 
		pEdge->itriDualTriCCW1 = -1;
		pEdge->itriDualTriCCW2 = -1;
		return TRUE; /* edge forced by the boundary */
	}
}

//To get the frontier line coordinates for the caller specified EDGE
//the caller must allocate a POINT2DD structure to receive coordinates in world coordinates
BOOL GetEdgeFrontierLineCoor(POINTSET* pPointset, 
							 EDGE myEdge, 
							 POINT2DD* p1Point2DD, 
							 POINT2DD* p2Point2DD)
{
	if(pPointset==NULL || p1Point2DD==NULL || p2Point2DD==NULL)
	{
		ASSERT(FALSE);
		return FALSE;
	}
	//point1
	p1Point2DD->x = pPointset->ctx[myEdge.itriDualTriCCW1];
	p1Point2DD->y = pPointset->cty[myEdge.itriDualTriCCW1];
	//point2
	p2Point2DD->x = pPointset->ctx[myEdge.itriDualTriCCW2];
	p2Point2DD->y = pPointset->cty[myEdge.itriDualTriCCW2];
	return TRUE;
}


int GetEdgeInsideVertex(POINTSET* pPointset, 
						EDGE myEdge)
{
	int idvertexIN = -1;
	if(pPointset==NULL)
	{
		ASSERT(FALSE);
		return -1;
	}
	idvertexIN = myEdge.ivertexIn;
	return idvertexIN;
}

int GetEdgeOutsideVertex(POINTSET* pPointset, 
						 EDGE myEdge)
{
	int idvertexOUT = -1;
	if(pPointset==NULL)
	{
		ASSERT(FALSE);
		return -1;
	}
	idvertexOUT = myEdge.ivertexOut;
	return idvertexOUT;
}


//provided idsegment and idvertex, any one vertex in the segment,
//this function finds one segment edge and collect the whole segment edges
BOOL CollectAllSegmentEdge(	POINTSET* pPointset,
							SEGMENTSET* pSegmentset,
							int idsegment, //to be drawn
							int idvertex,	 //any one vertex inside the segment to be drawn
							int iNumberOfSegmentForFinalPartition, 
							EDGE* pEdgeList,
							int* pNumberOfEdgeFound,
							int imaxnumberofedge)
{
    EDGE myEdge, StartEdge, myNextEdge;
	int j;
	if(pPointset==NULL || pSegmentset==NULL || 
		idsegment<0 || idvertex<0 || iNumberOfSegmentForFinalPartition<-1 || iNumberOfSegmentForFinalPartition<-1)
	{
		ASSERT(FALSE);
		pNumberOfEdgeFound[0];
		return FALSE;
	}
	/* //spi 2014
	CLogDocument* pLogDocument = ((COIIIApp*)AfxGetApp())->GetLogDocument();
	*/

	//3.1) find one segment edge, an edge is a pair of vertices, each vertices part of a diff. segment 
	GetOneSegmentEdge(	pPointset, 
						pSegmentset, 
						idsegment, 
						idvertex, 
						iNumberOfSegmentForFinalPartition,
						&myEdge);  
	if(myEdge.ivertexIn == -1 && myEdge.ivertexOut == -1)
	{   
		//development-time error 
		/* //spi 2014
		CString myText;
		myText.Format("     error, edge was not found for idseg %i in ow2view.cpp\r\n", idsegment);
		*/
 		//pLogDocument->AddText(myText);
		ASSERT(FALSE);
		pNumberOfEdgeFound[0] = 0;
		return FALSE;
	}
	else if(myEdge.ivertexIn != -1 && myEdge.ivertexOut == -1)
	{    
		//case for bounding segment, should only append once 
		
		//why do we get in here so often when partition with initial number of segment (maxnumseg)?
		//try to avoid this specific case
		//
		//I think, I got the answer, all the outbound points have voronoi regions that cannot be drawn,
		//and the first segments to be merge are all these points which are tagged by some float -9999.999 
		//value in order to cluster them all together early in the segmentation stage.
		if(iNumberOfSegmentForFinalPartition != pSegmentset->iInitialNumberOfSegment)
		{
			//so, never draw the first segment (which should be the bounding segment)
			//for all other number of desired segment, trace 
			#ifdef _DEBUG
    			/* //spi 2014
				pLogDocument->AddText("     got bounding segment, skip drawing contour for this one\r\n");
				*/
			#endif //_DEBUG
		}
	}
	else 
	{
		//3.2) start collecting edges
		StartEdge = myEdge;  //copy structure 
		pEdgeList[0] = StartEdge; //copy structure
		pNumberOfEdgeFound[0] = 1;
		GetNextSegmentEdge( pPointset, 
							pSegmentset, 
							idsegment, 
							&myEdge, 
							iNumberOfSegmentForFinalPartition,
							&myNextEdge);
		j=1;
		int iCount = 0;    		
		while(myNextEdge.itriDualTriCCW2!=StartEdge.itriDualTriCCW2)
		{
			iCount++;
			pEdgeList[j] = myNextEdge;
			j++; //set j to next index, it is also equals to the actual number of edges collected
			if(j<imaxnumberofedge) //maximum number of allocated edge
			{
				j++;                               
				myEdge = myNextEdge; //copy structure 
				GetNextSegmentEdge( pPointset, 
									pSegmentset, 
									idsegment, 
									&myEdge, 
									iNumberOfSegmentForFinalPartition,
									&myNextEdge);
			}
			else
			{
				//here, the caller should keep collecting the rest of edges
				//it is not a development-time error, but we use ASSERT to make sure the caller will colect the remaining edges
				ASSERT(FALSE);
				pNumberOfEdgeFound[0] = j;
				return FALSE;
			}
			//to avoid getting stock in here, in cases where return Edge not valid
			if(iCount>pSegmentset->iInitialNumberOfSegment)
			{
				//development-time error, we get in trouble when generating final partition with 5 segments
				ASSERT(FALSE);
				pNumberOfEdgeFound[0] = j;
				return FALSE;		
			}							
		} //end of while 
	}//end of start collecting edges	    	
	pNumberOfEdgeFound[0] = j;
	return TRUE;
}

BOOL CollectInterSegmentFrontierEdge(	POINTSET* pPointset, 
										SEGMENTSET* pSegmentSet, 
										int idsegment1,
										int idsegment2,
										int iNumberOfSegmentForFinalPartition, 
										EDGE* pEdgeList,
										int* p_iNumberOfFrontierEdgeFound,
										int imaxnumberofedge)

{
	if(pPointset==NULL || pSegmentSet==NULL || pEdgeList==NULL || p_iNumberOfFrontierEdgeFound==NULL)
	{
		ASSERT(FALSE);
		return FALSE;
	}
	//1) collect the all edges for segment1 or 2, make sure the edgelist will contain a shared frontier
	//1.1) try first with segment1
	int idvertex1 = GetOneVertexFromSegment(pSegmentSet,
											idsegment1);
	ASSERT(idvertex1>-1);
	int iNumberOfEdgeFound;
	BOOL bResult = CollectAllSegmentEdge(	pPointset,
											pSegmentSet,
											idsegment1, 
											idvertex1,	 //any one vertex inside the segment1 
											iNumberOfSegmentForFinalPartition, 
											pEdgeList,
											&iNumberOfEdgeFound,
											imaxnumberofedge);
	if(!bResult)
	{
		ASSERT(FALSE);
		p_iNumberOfFrontierEdgeFound[0]	= 0;
		return FALSE;
	}
	BOOL bStartsWithFrontier = FALSE;
	int indexFrontierEnd = -1;
	int indexFrontierStart = -1;
	if(IsInterSegmentFrontierEdge(	pPointset, 
									pSegmentSet, 
									idsegment1,
									idsegment2,
									iNumberOfSegmentForFinalPartition, 
									&pEdgeList[0]))
	{
		bStartsWithFrontier = TRUE;
	}
	else
	{
		bStartsWithFrontier = FALSE;
		//make sure there is at least one edge that is a shared frontier
		int i=1;
		while(!IsInterSegmentFrontierEdge(	pPointset, 
											pSegmentSet, 
											idsegment1,
											idsegment2,
											iNumberOfSegmentForFinalPartition, 
											&pEdgeList[i]))
		{
			i++;
			if(i==iNumberOfEdgeFound)
			{
				//did not find any edge that is a shared frontier, this can occur when one segment is an island in the other
				//try with segment2
				//collect all edges for the segment2
				int idvertex2 = GetOneVertexFromSegment(pSegmentSet,
														idsegment2);
				ASSERT(idvertex2>-1);
				bResult = CollectAllSegmentEdge(pPointset,
												pSegmentSet,
												idsegment2, 
												idvertex2,	 //any one vertex inside the segment1 
												iNumberOfSegmentForFinalPartition, 
												pEdgeList,
												&iNumberOfEdgeFound,
												imaxnumberofedge);
				if(!bResult)
				{
					ASSERT(FALSE);
					p_iNumberOfFrontierEdgeFound[0]	= 0;
					return FALSE;
				}
				if(IsInterSegmentFrontierEdge(	pPointset, 
												pSegmentSet, 
												idsegment1,
												idsegment2,
												iNumberOfSegmentForFinalPartition, 
												&pEdgeList[0]))
				{
					bStartsWithFrontier = TRUE;
				}
				else
				{
					bStartsWithFrontier = FALSE;
					i=1;
					while(!IsInterSegmentFrontierEdge(	pPointset, 
														pSegmentSet, 
														idsegment1,
														idsegment2,
														iNumberOfSegmentForFinalPartition, 
														&pEdgeList[i]))
					{
						i++;
						if(i==iNumberOfEdgeFound)
						{
							//did not find any edge that is a shared frontier for both segment,
							//this case should not occur (idsement1 and 2 do not share a common frontier in the specified partition)
							ASSERT(FALSE);
							p_iNumberOfFrontierEdgeFound[0] = 0;
							return FALSE;
						}
					}
				}
			}//end if no shared frontier found
		}
	}

	//down here, we are guaranteed to have collected a segment edge that contains a frontier between the two segments in the provided partition
	//2) now, we will identify the starting and ending indexex of that shared frontier
	int i=1;
	int j=0;
	int k=0;

	//while edge is a frontier and edgelist starts with frontier (and frontier ccw start not identified)  
	while(IsInterSegmentFrontierEdge(	pPointset, 
										pSegmentSet, 
										idsegment1,
										idsegment2,
										iNumberOfSegmentForFinalPartition, 
										&pEdgeList[i]) && bStartsWithFrontier==TRUE)
	{
		ASSERT(indexFrontierStart==-1);
		//find frontier ccw end
		indexFrontierEnd = i;
		if(i<iNumberOfEdgeFound-1)
		{
			i++;
		}
		else
		{
			//in this case, the shared frontier is the whole segment contour
			ASSERT(i==iNumberOfEdgeFound-1);
			indexFrontierStart = 0;
			p_iNumberOfFrontierEdgeFound[0] = iNumberOfEdgeFound;
			return TRUE;
		}
	}
	//while edge is not a frontier   
	while(!IsInterSegmentFrontierEdge(	pPointset, 
										pSegmentSet, 
										idsegment1,
										idsegment2,
										iNumberOfSegmentForFinalPartition, 
										&pEdgeList[i]))
	{
		if(i<iNumberOfEdgeFound-1)
		{
			i++;
		}
		else
		{
			ASSERT(i==iNumberOfEdgeFound-1);
			if(indexFrontierEnd!=-1)
			{
				//in this case, the shared frontier starts precisely at begining of the edge list, so no copy required
				indexFrontierStart = 0;
				p_iNumberOfFrontierEdgeFound[0] = indexFrontierEnd - indexFrontierStart +1;
				return TRUE;
			}
			else
			{
				//in this case, no frontier start and no frontier end identified
				ASSERT(FALSE);
				p_iNumberOfFrontierEdgeFound[0] = 0;
				return FALSE;
			}
		}
	}
	ASSERT(IsInterSegmentFrontierEdge(	pPointset, 
										pSegmentSet, 
										idsegment1,
										idsegment2,
										iNumberOfSegmentForFinalPartition, 
										&pEdgeList[i]));
	ASSERT(indexFrontierStart==-1);
	//at this index, we must have a indexFrontierStart
	indexFrontierStart = i;
	//keep going until edge is not part of the shared frontier anymore
	while(IsInterSegmentFrontierEdge(	pPointset, 
										pSegmentSet, 
										idsegment1,
										idsegment2,
										iNumberOfSegmentForFinalPartition, 
										&pEdgeList[i]))
	{
		if(i<iNumberOfEdgeFound-1)
		{
			i++;
		}
		else
		{
			ASSERT(i==iNumberOfEdgeFound-1);
			if(indexFrontierEnd==-1)
			{
				//in this case, the shared frontier ends precisely at the end of the edge list, copy is required
				indexFrontierEnd = iNumberOfEdgeFound -1;
				p_iNumberOfFrontierEdgeFound[0] = indexFrontierEnd - indexFrontierStart +1;
				for(k=0; k<p_iNumberOfFrontierEdgeFound[0]; k++)
				{
					//copy EDGE struct one by one
					pEdgeList[k] = pEdgeList[k+indexFrontierStart];
				}
				return TRUE;
			}
			else
			{
				//in this case, the frontier starting and ending are disconnected, reconnect
				ASSERT(indexFrontierStart!=-1);
				ASSERT(indexFrontierEnd!=-1);
				ASSERT(indexFrontierEnd<indexFrontierStart);
				int iNumEdgeInLastPart = (indexFrontierEnd - 0 + 1);
				int iNumEdgeInFirstPart = (i - indexFrontierStart +1);
				p_iNumberOfFrontierEdgeFound[0] = iNumEdgeInFirstPart + iNumEdgeInLastPart;
				//now, copy EDGE struct one by one, this for the two parts
				//we must allocate a temporary edgelist
				EDGE* pTempEdgeList = (EDGE*) new EDGE[p_iNumberOfFrontierEdgeFound[0]];
				if(pTempEdgeList==NULL)
				{
					ASSERT(FALSE); //not enough memory for allocation
					p_iNumberOfFrontierEdgeFound[0] = 0;
					return FALSE;
				}
				//copy EDGE struct for first part
				for(k=0; k<iNumEdgeInFirstPart; k++)
				{
					pTempEdgeList[k] = pEdgeList[k+indexFrontierStart];
				}
				//copy EDGE struct for last part
				for(k=0; k<iNumEdgeInLastPart; k++)
				{
					pTempEdgeList[k+iNumEdgeInFirstPart] = pEdgeList[k];
				}
				//copy EDGE struct one by one from tempedgelist to final edgelist
				for(k=0; k<p_iNumberOfFrontierEdgeFound[0]; k++)
				{
					pEdgeList[k] = pTempEdgeList[k];
				}
				delete[] pTempEdgeList;
				return TRUE;
			}

		}
	}
	//if made it down here, it means and frontier end has been found
	ASSERT(indexFrontierStart!=-1);
	indexFrontierEnd = i-1;
	ASSERT(indexFrontierStart<indexFrontierEnd);
	p_iNumberOfFrontierEdgeFound[0] = indexFrontierEnd - indexFrontierStart +1;
	//copy EDGE struct one by one from final edgelist to final edgelist (different offset)
	for(k=0; k<p_iNumberOfFrontierEdgeFound[0]; k++)
	{
		pEdgeList[k] = pEdgeList[k+indexFrontierStart];
	}

	//warning, the current implementation of this function does not guarantee that all
	//shared frontier part will be identified, this implementation returns the first shared
	//frontier found.
	return TRUE;
}

//this function is used when erasing of frontier between two segment is required
//it is usefull for displaying fusion evolution
BOOL IsInterSegmentFrontierEdge(POINTSET* pPointset, 
								SEGMENTSET* pSegmentSet, 
								int idsegment1,
								int idsegment2,
								int iNumberOfSegmentForFinalPartition, 
								EDGE* pEdge)
{ 
	if(idsegment1==idsegment2 || pEdge==NULL || pPointset==NULL || pSegmentSet==NULL)
	{
		ASSERT(FALSE);
		return FALSE;
	}
	#ifdef _DEBUG
		int idseg1Owner = GetOwnerParentSegmentThreshold(pSegmentSet, idsegment1, iNumberOfSegmentForFinalPartition);
		int idseg2Owner = GetOwnerParentSegmentThreshold(pSegmentSet, idsegment2, iNumberOfSegmentForFinalPartition);
		ASSERT(idseg1Owner==idsegment1);
		ASSERT(idseg2Owner==idsegment2);
	#endif

	int idsegOwnerVertexIn = GetOwnerParentSegmentThreshold(pSegmentSet, pEdge->ivertexIn, iNumberOfSegmentForFinalPartition);
	int idsegOwnerVertexOut = GetOwnerParentSegmentThreshold(pSegmentSet, pEdge->ivertexOut, iNumberOfSegmentForFinalPartition);


	if(idsegOwnerVertexIn==idsegment1 && idsegOwnerVertexOut==idsegment2
		|| idsegOwnerVertexIn==idsegment2 && idsegOwnerVertexOut==idsegment1)
	{
		return TRUE;
	}
	return FALSE;
}



int GetNorthernMostNeighbor(POINTSET* pPointset, 
							int ivertexMovingHead, 
							int* p_arrayneighbor, 
							int numneighborfound)
{   
	int i, iNorthernMostNeighbor;
	double fdX, fMovingHeadX, fSmallestdX;
	/* assumption: ivertexMovingHead is fully surrounded by neighbors in array */
	
	iNorthernMostNeighbor = -1;
	/* find first qualifying neighbor (with dy greater than zero) */
	for(i=0; i<numneighborfound; i++)
	{   
		if( (pPointset->py[p_arrayneighbor[i]]-pPointset->py[ivertexMovingHead]) > 0.0) 
		{
			iNorthernMostNeighbor = p_arrayneighbor[i];		 
			fMovingHeadX = pPointset->px[ivertexMovingHead];
			fSmallestdX = fabs(pPointset->px[iNorthernMostNeighbor]-fMovingHeadX);
			break; 
		}	
	}    
	if( iNorthernMostNeighbor==-1 )
	{   
		/* development-time error */
		printf("error, did not find any qualifying neighbor, in GetNorthernMostNeighbor()\n");
		ASSERT(FALSE);
		exit(0);
	}
	
	/* optimize, keep looking for smaller dx but with dy greater than zero */  
	//poirier sept 18th 96, modified
	//while( i<(numneighborfound-1) && (pPointset->py[p_arrayneighbor[i+1]]>0.0) )
	while( i<(numneighborfound-1) 
		&& ((pPointset->py[p_arrayneighbor[i+1]]-pPointset->py[ivertexMovingHead])>0.0) )
	{   
		i++; 
		fdX = fabs(pPointset->px[p_arrayneighbor[i]]-fMovingHeadX);
		if( fdX < fSmallestdX)
		{   
			fSmallestdX = fdX;
			iNorthernMostNeighbor = p_arrayneighbor[i];	
		}
		else
		{
			/* as soon as situation does not improove, stop optimization */
			break; /* definitely more efficient, are there cases where we should keep going? */
		}			
	}
	return iNorthernMostNeighbor;
}
                    
                    
int MoveOneStepNorth( POINTSET* pPointset, 
					  SEGMENTSET* pSegmentSet, 
					  int* pivertexMovingHead)
{   
	int ivertexNext;
	
    int numtrifound, numneighborfound, itriseed;
    int p_arraytri[200];
    int p_arrayneighbor[200];

    itriseed = 0;
	if( FindAllValidTriSurroundingVertex( pPointset,
					       				  *pivertexMovingHead,
					       				  &itriseed,
								          &numtrifound,
								          p_arraytri,
								          &numneighborfound,
								          p_arrayneighbor) == TRUE)
	{
		/* ivertexMovingHead is fully surrounded by neighbors */
		ivertexNext = GetNorthernMostNeighbor(	pPointset, 
												*pivertexMovingHead, 
												p_arrayneighbor, 
												numneighborfound);
	}   
	else
	{
		/* if(error) return FALSE; */   
		/* development-time error, should not hit the border */
		printf("error, ivertexMovingHead is not surrounded by neighbors anymore, in MoveOneStepNorth()\n");
		ASSERT(FALSE);
		exit(0);
	}
	*pivertexMovingHead	= ivertexNext;	
	return TRUE; /* moved successfully */
}


//static int ijk[6] = {0,1,2,0,1,2};
/* 
// the edge returned is the next edge in CCW provided previousEdge
// as well as the id of the segment of interest. 
*/ 
int GetNextSegmentEdge( POINTSET* pPointset, 
						SEGMENTSET* pSegmentSet, 
						int idsegOwner, 
						EDGE* pPreviousEdge, 
						int iNumberOfSegmentForFinalPartition,
						EDGE* pNextEdge)
{
	int i, ivertexNext, idseg;
		
	/* 1) using the 2 edge vertices and the second dual triangle, we can find next vertex
	      to be included or excluded */                      
	
	/* 1.1) find to which index correspond ivertexIn in itriDualTriCCW2 */   
	i=0;
	while(i<3 && pPreviousEdge->ivertexIn!=pPointset->vt[i][pPreviousEdge->itriDualTriCCW2]) i++;	
	/* 1.2) find ivertexNext */
	ivertexNext = pPointset->vt[ijk[i+2]][pPreviousEdge->itriDualTriCCW2];
	
    /* 2) is that ivertexNext in the same segment as ivertexIn? */
	idseg = GetOwnerParentSegmentThreshold(pSegmentSet, ivertexNext, iNumberOfSegmentForFinalPartition);
	/* 2.1) set NextEdge according to ownership of ivertexNext */
	if(idseg == idsegOwner)
	{    
		/* set next edge */
		pNextEdge->ivertexIn = ivertexNext; 
		pNextEdge->ivertexOut = pPreviousEdge->ivertexOut; 
	}
	else
	{
		/* set next edge */
		pNextEdge->ivertexIn = pPreviousEdge->ivertexIn; 
		pNextEdge->ivertexOut = ivertexNext; 
	}  
	/* 2.2) having the 2 vertices and first dual triangle, we can find the 2nd dual tri*/
	pNextEdge->itriDualTriCCW1 = pPreviousEdge->itriDualTriCCW2;	
	pNextEdge->itriDualTriCCW2 = GetAdjTri(	pPointset, 
											pNextEdge->ivertexIn, 
											pNextEdge->ivertexOut, 
											pNextEdge->itriDualTriCCW1);	                      
	return TRUE;
}

        
double* GetPointerToStatistics( SEGMENTSET* pSegmentSet,
			       				int idseg )
{
    double* pfStat;
    int iOffset;

    iOffset = (idseg*(pSegmentSet->nStatPerSegment));
    /*
    // for debugging only
    */
    if( idseg < 0 || idseg >= pSegmentSet->iFinalNumberOfSegment )
    {
		ASSERT(FALSE);
		printf("fatal error in GetPointerToStatistics(), idseg=%i\n", idseg);
		exit(0);
    }
    pfStat = (double*) &(pSegmentSet->pfStatistics[iOffset]);
    return pfStat;
}


SEGMENT* GetPointerToSegment( 	SEGMENTSET* pSegmentSet,
			      				int idseg )
{
    SEGMENT* pSegment;
    /*
    // for debugging only
    */
    if( idseg < 0 || idseg >= pSegmentSet->iFinalNumberOfSegment )
    {
		ASSERT(FALSE);
		printf("fatal error in GetPointerToStatistics(), idseg=%i\n", idseg);
		exit(0);
    }
    pSegment = (SEGMENT*) &(pSegmentSet->pListSegment[idseg]);
    return pSegment;
}



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
// neighbor.mc implements the NEIGHBOR list related functionality
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
#include "stdafx.h"

#include "pointset.h"

#include "fusion.h"
#include "segment.h"
#include "neighbor.h"

#include "segset.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
*/


/*
// module globals
*/


/*/////////////////////////
// Initialize Neighbor List
/////////////////////////*/
void InitializeListNeighbor( SEGMENTSET* pSegmentSet,
			     			 POINTSET* pPointset)
{
    int itriseed;
    //int idnei;
    int numtrifound, p_arraytri[200];
    int numneighborfound, p_arrayneighbor[200];
    NEIGHBOR* pBase;
    NEIGHBOR* pNewListNeighbor;
    SEGMENT* pSegment;
    int sizeinbyte, idseg, iextranumberofneighbor;

    itriseed = 0;
    for(idseg=0; idseg<(pSegmentSet->iInitialNumberOfSegment); idseg++)
    {
		FindAllValidTriSurroundingVertex( pPointset,
						  				  idseg,
										  &itriseed,
										  &numtrifound,
										  p_arraytri,
										  &numneighborfound,
										  p_arrayneighbor );
		pSegment = GetPointerToSegment(pSegmentSet, idseg);
		pSegment->idneiFirstNeighbor = pSegmentSet->idneiNextAvailNeighbor;
		pSegment->iNumberOfNeighbor = numneighborfound;
		if( (pSegmentSet->idneiNextAvailNeighbor + numneighborfound)
		      > (pSegmentSet->iActualNumberOfNeighbor) )
		{
		    iextranumberofneighbor = (pSegmentSet->iFinalNumberOfSegment - idseg )
					      *NEIGHBOR_APPROXNUMBERPERSEGMENT + numneighborfound;
		    pNewListNeighbor = (NEIGHBOR*) ReallocListNeighbor(pSegmentSet, iextranumberofneighbor );
		    if( pNewListNeighbor == NULL ) return;
		    pSegmentSet->pListNeighbor = pNewListNeighbor;
		}
		/*
		// memcpy p_arrayneighbor into pListNeighbor
		*/
		pBase = GetPointerToNeighbor( pSegmentSet, (int)(pSegmentSet->idneiNextAvailNeighbor) );
		sizeinbyte = numneighborfound*sizeof(NEIGHBOR);
		memcpy(pBase, p_arrayneighbor, sizeinbyte);
		pSegmentSet->idneiNextAvailNeighbor += numneighborfound;
    }

    return;
}


NEIGHBOR* ReallocListNeighbor(  SEGMENTSET* pSegmentSet,
			       				int iextranumberofneighbor )
{
    int iextrasizeinbyte, inewsizeinbyte;
    NEIGHBOR* pNewBase;

    iextrasizeinbyte = iextranumberofneighbor*sizeof(NEIGHBOR);
    inewsizeinbyte = (pSegmentSet->iActualNumberOfNeighbor)*sizeof(NEIGHBOR) + iextrasizeinbyte;
    pNewBase = (NEIGHBOR*) realloc(pSegmentSet->pListNeighbor, inewsizeinbyte );
    if(pNewBase==NULL)
    {
	printf("Fatal Error, cannot reallocate pListNeighbor\n");
	return NULL;
    }
    pSegmentSet->iActualNumberOfNeighbor += iextranumberofneighbor;
    pSegmentSet->iTotalNumberOfByteUsed += iextrasizeinbyte;
    printf("reallocation O.K., new size of neighbor list is %i\n", pSegmentSet->iActualNumberOfNeighbor);
    return pNewBase;
}


void MergeNeighborList( SEGMENTSET* pSegmentSet,
						int idsegNewParent,
						int idsegChild1,
						int idsegChild2 )
{
    /*
    // -assuming idsegChild1 and idsegChild2 are owner of themself. this
    //	should have previously been checked by the function that merges
    //	these two childs together.
    // -the new list of neighbor is obtained by gathering sequentialy
    //	the owners of child1's neighbor list and then the ones of child2.
    // -while gathering we make sure not to add twice the same segment in
    //	the new neighbor list.	we also exclude the 2 childs segment id.
    */

    int i, iStart, iEnd, idseg, ivalid, imaxnumnei, iextranumberofneighbor;
    int j, jEnd;
	//int jStart;
    NEIGHBOR* pNewParentNeighborList;
    NEIGHBOR* pChild1NeighborList;
    NEIGHBOR* pChild2NeighborList;
    NEIGHBOR* pNewListNeighbor;
    SEGMENT* pNewParentSegment;
    SEGMENT* pChild1Segment;
    SEGMENT* pChild2Segment;

    /*
    // 1) adding new neighbor list directly into pListNeighbor, therefore
    //	  checking first if realloc required.
    */
    pChild1Segment = GetPointerToSegment( pSegmentSet, idsegChild1 );
    imaxnumnei = pChild1Segment->iNumberOfNeighbor;
    pChild2Segment = GetPointerToSegment( pSegmentSet, idsegChild2 );
    imaxnumnei += pChild2Segment->iNumberOfNeighbor;
    if( (pSegmentSet->iActualNumberOfNeighbor - pSegmentSet->idneiNextAvailNeighbor) < imaxnumnei )
    {
		iextranumberofneighbor = ( 	pSegmentSet->iFinalNumberOfSegment -
					   				pSegmentSet->iActualNumberOfSegment)
					  			 * NEIGHBOR_APPROXNUMBERPERSEGMENT + imaxnumnei;
		pNewListNeighbor = ReallocListNeighbor( pSegmentSet,
							iextranumberofneighbor );
		if( pNewListNeighbor == NULL )
		{
		    printf("fatal error on attempt to realloc neighbor list\n");
		    exit(0);
		}
		pSegmentSet->pListNeighbor = pNewListNeighbor;
    }

    /*
    // 2) get neighbors from idsegChild1
    //	  2.1)
    //	  2.2)
    */
    pNewParentSegment = GetPointerToSegment( pSegmentSet, idsegNewParent );
    pNewParentSegment->idneiFirstNeighbor = pSegmentSet->idneiNextAvailNeighbor;
    pNewParentSegment->iNumberOfNeighbor = 0;

    pNewParentNeighborList = GetPointerToNeighbor( pSegmentSet, pNewParentSegment->idneiFirstNeighbor );
    pChild1Segment = GetPointerToSegment( pSegmentSet, idsegChild1 );
    pChild1NeighborList = GetPointerToNeighbor( pSegmentSet, (int)(pChild1Segment->idneiFirstNeighbor) );
    iStart = 0;
    iEnd = pChild1Segment->iNumberOfNeighbor;
    for(i=0; i<iEnd; i++)
    {
	idseg = GetOwnerParentSegment( pSegmentSet, pChild1NeighborList[i] );
	ivalid = TRUE;
	if( (idseg == idsegChild1) || (idseg == idsegChild2) )
	{
	    ivalid = FALSE;
	}
	else
	{
	    jEnd = pNewParentSegment->iNumberOfNeighbor;
	    j=0;
	    while( (j<jEnd) && (idseg != pNewParentNeighborList[j]) ) j++;
	    if( idseg == pNewParentNeighborList[j] ) ivalid = FALSE;
	}
	if( ivalid ) /* if valid neighbor, add to the parent neighbor list */
	{
	    pNewParentNeighborList[pNewParentSegment->iNumberOfNeighbor] = idseg;
	    pNewParentSegment->iNumberOfNeighbor ++;
	}
    }

    /*
    // now, do the same for idsegChild2
    //
    */
    pChild2Segment = GetPointerToSegment( pSegmentSet, idsegChild2 );
    pChild2NeighborList = GetPointerToNeighbor( pSegmentSet, (int)(pChild2Segment->idneiFirstNeighbor) );
    iStart = 0;
    iEnd = pChild2Segment->iNumberOfNeighbor;
    for(i=0; i<iEnd; i++)
    {
	idseg = GetOwnerParentSegment( pSegmentSet, pChild2NeighborList[i] );
	ivalid = TRUE;
	if( (idseg == idsegChild1) || (idseg == idsegChild2) )
	{
	    ivalid = FALSE;
	}
	else
	{
	    jEnd = pNewParentSegment->iNumberOfNeighbor;
	    j=0;
	    while( (j<jEnd) && (idseg != pNewParentNeighborList[j]) ) j++;
	    if( idseg == pNewParentNeighborList[j] ) ivalid = FALSE;
	}
	if( ivalid ) /* if valid neighbor, add to the parent neighbor list */
	{
	    pNewParentNeighborList[pNewParentSegment->iNumberOfNeighbor] = idseg;
	    pNewParentSegment->iNumberOfNeighbor ++;
	}
    }
    pSegmentSet->idneiNextAvailNeighbor += pNewParentSegment->iNumberOfNeighbor;
    return;
}



NEIGHBOR* GetPointerToNeighbor( SEGMENTSET* pSegmentSet,
				int idnei )
{
    NEIGHBOR* pNeighbor;
    if( idnei > pSegmentSet->iActualNumberOfNeighbor )
    {
	printf("error in GetPointerToNeighbor(), idnei=%i\n", idnei);
	exit(0);
    }
    pNeighbor = (NEIGHBOR*) &( pSegmentSet->pListNeighbor[idnei] );
    return pNeighbor;
}



/*
/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
// classset.mc is part of the offworld technologies SEGMENTSET library
// 
// initially build to compiled under microstation in c flat 32 bit memory
// model with basic microstation mdl gui.
//
// segset.mc implements the segment set, embedding segment list, neighbor 
// list and merge or fusion list.
//
// for integration within oifii, compiled in as cpp code. new c++ classes
// were added to provide c++ interface in the file owsegmentset.h/.cpp.
//
/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
*/

/*
#include "stdafx.h"

#include "pointset.h"

#include "segment.h"
#include "fusion.h"

#include "segset.h"

#include "class.h"
#include "classset.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
*/

// segset module globals
extern int inumberofbyte;


/*///////////////////////
// ClassSet Constructor
///////////////////////*/
SEGMENTSET* NewClassSet(int initialnumberofsegment, int finalnumberofsegment)
{
    SEGMENTSET* pSegmentSet;
	pSegmentSet = NewSegmentSet(initialnumberofsegment, finalnumberofsegment);
	return pSegmentSet;
}


/*//////////////////////
// ClassSet Destructor
//////////////////////*/
void DeleteClassSet(SEGMENTSET* pSegmentSet)
{
    pSegmentSet->pfStatistics = NULL; //set it null, so it does not get deleted twice
	DeleteSegmentSet(pSegmentSet);
    return;
}


/*/////////////////////////////////////////////////////////////
// Initialize ClassSet from a previously constructed pointset
/////////////////////////////////////////////////////////////*/
void InitializeClassSet(SEGMENTSET* pSegmentSet, POINTSET* pPointset)
{
    // 1) Initialize each segment's statistic info
    InitializeClassStatistics(pSegmentSet, pPointset);

    /* 2) Initialize each segment's neighbor list 
    InitializeListNeighbor( pSegmentSet, pPointset );
	*/

    // 3) Initialize each segment's best matching neighbor, parent and criteria
    InitializeBestMatchingClass(pSegmentSet);

    return;
}


////////////////////
// Build Fusion Tree
////////////////////
void BuildClassFusionTree(SEGMENTSET* pClassSet)
{
    int iStart, iEnd, idsegNewParent, idseg1, idseg2;
    SEGMENT* pSegment1;

    pClassSet->iActualNumberOfSegment = pClassSet->iInitialNumberOfSegment;
    iStart = pClassSet->iActualNumberOfSegment; //id of first fusioned segment
    iEnd = pClassSet->iFinalNumberOfSegment; //id of last fusioned segment
    for(idsegNewParent=iStart; idsegNewParent<iEnd; idsegNewParent++)
    {
		idseg1 = GetBestMatchingSegmentPair(pClassSet);
		pSegment1 = GetPointerToSegment( pClassSet, idseg1);
		idseg2 = pSegment1->idsegBestMatchingSegment;
		MergeClassPair(pClassSet, idsegNewParent, idseg1, idseg2);
		pClassSet->iActualNumberOfSegment++;
    }
    return;
}



/*
/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
// class.mc is part of the offworld technologies SEGMENTSET library
// 
// initially build to compiled under microstation in c flat 32 bit memory
// model with basic microstation mdl gui.
//
// class.h/.mc module implements the class list structure and functionality
//
// for integration within oifii, compiled in as cpp code. new c++ classes
// were added to provide c++ interface in the file owsegmentset.h/.cpp.
//
/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
*/

/*
#include "stdafx.h"
#include <math.h>

#include "pointset.h"

#include "segment.h"
#include "fusion.h"

#include "segset.h"
#include "classset.h"

#include "logdoc.h"
#include "oifii_app.h"

#include "class.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
*/



////////////////////////
// Initialize class list
//////////////////////////
void InitializeClassStatistics(SEGMENTSET* pSegmentSet, POINTSET* pPointset)
{
    // Initialize Statistics info
    if( pSegmentSet->iInitialNumberOfSegment != pPointset->npts )
    {
		ASSERT(FALSE);
		//error 
		printf("Fatal Error, initial number of allocated segment inconsistent\n");
		printf("             with pointset's number of element.\n");
		exit(0);
    }

    pSegmentSet->pfStatistics = pPointset->pfStatistics;
    pSegmentSet->nStatPerSegment = pPointset->nStatPerPoint;
    pSegmentSet->nSizeStatInByte = pPointset->nSizeStatInByte;
    if( ((pSegmentSet->nStatPerSegment)
	//*2 added by poirier on april 10th 1996
	     *(pSegmentSet->iInitialNumberOfSegment) * 2 
	     * (int)sizeof(double) ) > pSegmentSet->nSizeStatInByte )	//!= changed for >= 
    {
		ASSERT(FALSE);
		//error
		//printf("Fatal Error pfStatistics inconsistent\n");
		printf("Fatal Error not enough allocated space for pfStatistics\n");
		exit(0);
    }
    pSegmentSet->iTotalNumberOfByteUsed += pSegmentSet->nSizeStatInByte;
    /*
	pPointset->pfStatistics = NULL; //null the pointer because it does not belong to pPointset anymore 
    pPointset->nStatPerPoint = 0;
    pPointset->nSizeStatInByte = 0;
	*/

    return;
}



void InitializeBestMatchingClass(SEGMENTSET* pSegmentSet)
{
    //this function also initialize each segment's parent (to itself)
    SEGMENT* pSegment;
    int idseg;
    for(idseg=0; idseg<(pSegmentSet->iInitialNumberOfSegment); idseg++)
    {
		pSegment = GetPointerToSegment(pSegmentSet, idseg);
		pSegment->idsegParentSegment = -1; //each segment owns itself 
		pSegment->iNumberOfSegment = 1; //each segment is indivudual 
		//initialize idfirstneighbor and numneighbor to -1 and 0
		pSegment->idneiFirstNeighbor = -1;
		pSegment->iNumberOfNeighbor = 0;
	}
    for(idseg=0; idseg<(pSegmentSet->iInitialNumberOfSegment); idseg++)
    {
		//poirier dec 98, classification version requires ownership to be priorly defined
		SetBestMatchingClass(pSegmentSet, idseg);
    }
    return;
}


//calling this function for each initial segment can be quite time consuming.
//a solution for optimizing the speed, is to merge all identical segment pairs first 
//and then, for each remaining segments, call this function.
void SetBestMatchingClass(SEGMENTSET* pClassSet, int idseg)
{
    double fCriteria, fBestCriteriaSoFar;
    int idsegOther, idsegParentOther, idsegStart, idsegEnd;
    SEGMENT* pSegment;

    fBestCriteriaSoFar = SEGSET_MAXFLOAT;
    pSegment = GetPointerToSegment(pClassSet, idseg);
	ASSERT(pSegment->idsegParentSegment==-1);  //poirier dec 98, ensure idseg owns itself
    idsegStart = 0;
    idsegEnd = pClassSet->iActualNumberOfSegment-1;

    for(idsegOther=idsegStart; idsegOther<idsegEnd; idsegOther++)
    {
		idsegParentOther = GetOwnerParentClass(pClassSet, idsegOther);
		if(idsegParentOther==idseg) 
		{
			continue; //goto next loop iteration
		}
		if(idsegOther==idseg) 
		{
			continue; //goto next loop iteration
		}

		// 1) get criteria between idseg and idsegOther
		fCriteria = GetClassSimilarityCriteria(pClassSet, idseg, idsegParentOther);
		// 2.1) if(best so far) store proper values
		if(fCriteria < fBestCriteriaSoFar)
		{
		    pSegment->idsegBestMatchingSegment = idsegParentOther;
		    pSegment->fBestMatchValue = fCriteria;
		    fBestCriteriaSoFar = fCriteria;
		}
		// 2.2) if zero, stop searching
		if(fCriteria==0.0f) 
		{
			//this accelerates considerably the search for best matching segment/class pair
			break; //exit loop iteration
		}
    }
    
    // used when debugging
    if(fBestCriteriaSoFar==SEGSET_MAXFLOAT)
    {
		printf("not best matching class for idseg %d in SetBestMatchingClass\n", idseg);
		pSegment->idsegBestMatchingSegment = -1;
		pSegment->fBestMatchValue = fBestCriteriaSoFar; 
    }
    return;
}


double GetClassSimilarityCriteria(SEGMENTSET* pClassSet, int idseg1, int idseg2)
{
    // 
    // -could be using Ward criteria or any other, just be carefull
    //  so that the fCriteria returned is minimizing, that is approaching
    //  0.0 for best match. if it is not minimizing, you will have to
    //  modify the code elsewhere.
    //
    // -also, be carefull on the signification of your statistics, different
    //  input data file will have different statistics ordering (stat 0, 1, 2, ...),
    //  therefore the code written in here should be adapted with input file format.
    //
    // -the most obvious criteria to take is to compute the euclidian distance between
    //  the two statistics vectors, it is a minimizing criteria.
    //
    int i, n1, n2;
    double fCriteria, fDiff, fSum;
    double* pfSeg1Statistics;
    double* pfSeg2Statistics;
	SEGMENT* pSegment;

    pfSeg1Statistics = GetPointerToStatistics(pClassSet, idseg1);
    pfSeg2Statistics = GetPointerToStatistics(pClassSet, idseg2);

	pSegment = GetPointerToSegment(pClassSet, idseg1);
	n1 = pSegment->iNumberOfSegment;
	pSegment = GetPointerToSegment(pClassSet, idseg2);
	n2 = pSegment->iNumberOfSegment;
    
	//-use euclidian distance measure between intensities
    fSum = 0.0;
	i = POINTSET_OFFSETSTAT_INTENSITY; 
	fDiff = pfSeg1Statistics[i] - pfSeg2Statistics[i]; 
	if( fDiff >= 0.0 ) fSum = fSum + fDiff;
	  else fSum = fSum - fDiff;   
    fCriteria = fSum;
    return fCriteria;
}


int GetBestMatchingClassPair(SEGMENTSET* pClassSet)
{
    //
    // -assuming best matching neighbor have been previously identified
    //	for each of the segments present in the list.
    // -this function will then return the segment pair that minimizes
    //	the similarity criteria (global segment list search).
    //
    // Note: since performing a global search for every merge is
    //	     time consuming, we could think of randomizing a little
    //	     the way we pick the next pair to be matched.  could also
    //	     be interesting to use an initial ordered list of best
    //	     matching segment pair.
    //
    int idseg, idsegBestMatchingPair;
    SEGMENT* pSegment;
    double fBestMatchValueSoFar, fValue;

    fBestMatchValueSoFar = SEGSET_MAXFLOAT;
    idsegBestMatchingPair = -1;
    for(idseg=0; idseg<pClassSet->iActualNumberOfSegment; idseg++)
    {
		pSegment = GetPointerToSegment(pClassSet, idseg);
		// if idseg owns itself (if it is a parent)
		if( pSegment->idsegParentSegment == -1 )
		{
			//poirier dec 98, for classification, begin
			//if best matching id does not own itself anymore, find new best matching
			int idsegParent = GetOwnerParentClass(pClassSet, pSegment->idsegBestMatchingSegment);
			if(idsegParent!=pSegment->idsegBestMatchingSegment)
			{
				SetBestMatchingClass(pClassSet, idseg);
			}
			//poirier dec 98, for classification, end
		    fValue = pSegment->fBestMatchValue;
		    if( fValue < fBestMatchValueSoFar )
		    {
				fBestMatchValueSoFar = fValue;
				idsegBestMatchingPair = idseg;
		    }
		}
    }
    if( fValue == SEGSET_MAXFLOAT )
    {
		ASSERT(FALSE);
		printf("fatal error, in GetBestMatchingSegmentPair()\n");
		printf("             no best matching segment pair found\n");
		exit(0);
    }
    return idsegBestMatchingPair;
}

int GetOwnerParentClass(SEGMENTSET* pClassSet, int idseg)
{
	return GetOwnerParentSegment(pClassSet, idseg);
}                                    	

    
int GetOwnerParentClassThreshold(SEGMENTSET* pClassSet, int idseg, int iNumberOfSegmentForFinalClassification)
{
    return GetOwnerParentSegmentThreshold(pClassSet, idseg, iNumberOfSegmentForFinalClassification);
}




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
// neighbor.mc implements the FUSION list related functionality
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
#include "stdafx.h"

#include "pointset.h"
#include "segment.h"
#include "neighbor.h"
#include "segset.h"
*/

/*
#include "class.h"
#include "classset.h"
*/

/*
#define COMPILING_FUSION_CPP	1
#include "fusion.h"
*/

/*
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
*/

/*
// module globals
*/

char* pszFusionInfoChannelNames[FUSION_NUMBEROFCHANNELNAMES] = 
{
	"Criteria",
	"SegmentSize"
};


void MergeSegmentPair( 	SEGMENTSET* pSegmentSet,
		       			int idsegNewParent,
		       			int idsegChild1,
		       			int idsegChild2 )
{
     int i, iStart, iEnd, n1, n2;
     SEGMENT* pNewParentSegment;
     SEGMENT* pSegmentChild1;
     SEGMENT* pSegmentChild2;
     double* pfChild1Statistics;
     double* pfChild2Statistics;
     double* pfNewParentStatistics;
     //NEIGHBOR* pNeighbor;

     /*
     // 1) make sure idsegChild1 and idsegChild2 are owner
     */
     idsegChild1 = GetOwnerParentSegment( pSegmentSet, idsegChild1 );
     idsegChild2 = GetOwnerParentSegment( pSegmentSet, idsegChild2 );

     /*
     // 2) compute new segment statistics, weighted average of the 2 childs
     */
     pfChild1Statistics = GetPointerToStatistics( pSegmentSet, idsegChild1 );
     pfChild2Statistics = GetPointerToStatistics( pSegmentSet, idsegChild2 );
     pfNewParentStatistics = GetPointerToStatistics( pSegmentSet, idsegNewParent );
     
     pSegmentChild1 = GetPointerToSegment(pSegmentSet, idsegChild1);
     pSegmentChild2 = GetPointerToSegment(pSegmentSet, idsegChild2);
     pNewParentSegment = GetPointerToSegment(pSegmentSet, idsegNewParent);
     
     iStart = 0;
     iEnd = (pSegmentSet->nStatPerSegment);
     n1 = pSegmentChild1->iNumberOfSegment; 
     n2 = pSegmentChild2->iNumberOfSegment;
     for(i=iStart; i<iEnd; i++)
     {
	 	pfNewParentStatistics[i] = ( pfChild1Statistics[i]*n1 + pfChild2Statistics[i]*n2 ) / (n1+n2);
     }
     pNewParentSegment->iNumberOfSegment = n1+n2;
     
     /*
     // 3) get new neighbor list, merge the 2 child's neighbor lists
     */
     MergeNeighborList( pSegmentSet,
						idsegNewParent,
						idsegChild1,
						idsegChild2 );
     /*
     // 4) update best matching segment and criteria
     */
     SetBestMatchingNeighborSegment( pSegmentSet, idsegNewParent );

     /*
     // 5) set ownership for the 2 childs and the new parent
     //
     //    note: ownership should be set after merging the two child's
     // 	 neighbor list, otherwise the new parent segment id
     // 	 would be inserted in the new parent neighbor list.
     */
     pSegmentChild1->idsegParentSegment = idsegNewParent;
     pSegmentChild2->idsegParentSegment = idsegNewParent;
     pNewParentSegment->idsegParentSegment = -1;

     /*
     // 6) save into fusion list
     */
     RegisterFusion(pSegmentSet,
		     		idsegNewParent,
		     		idsegChild1,
		     		idsegChild2 );

     /*
     // used when debugging only
     //
     printf("%d, idneiNext%d\n", idsegNewParent, pSegmentSet->idneiNextAvailNeighbor);
     */

     /*
     // used when debugging only
     //
     printf("%d = %d + %d\n", idsegNewParent, idsegChild1, idsegChild2);
     printf(" Neighbors: ");
     pNeighbor = GetPointerToNeighbor(pSegmentSet, pNewParentSegment->idneiFirstNeighbor);
     for(i=0; i<pNewParentSegment->iNumberOfNeighbor; i++)
     {
		 printf("%d, ", pNeighbor[i]);
     }
     printf("\n");
     */

     return;
}

void MergeClassPair(SEGMENTSET* pSegmentSet, int idsegNewParent, int idsegChild1, int idsegChild2)
{
     int i, iStart, iEnd, n1, n2;
     SEGMENT* pNewParentSegment;
     SEGMENT* pSegmentChild1;
     SEGMENT* pSegmentChild2;
     double* pfChild1Statistics;
     double* pfChild2Statistics;
     double* pfNewParentStatistics;
     //NEIGHBOR* pNeighbor;

     // 1) make sure idsegChild1 and idsegChild2 are owner
     idsegChild1 = GetOwnerParentSegment( pSegmentSet, idsegChild1 );
     idsegChild2 = GetOwnerParentSegment( pSegmentSet, idsegChild2 );

     // 2) compute new segment statistics, weighted average of the 2 childs
     pfChild1Statistics = GetPointerToStatistics( pSegmentSet, idsegChild1 );
     pfChild2Statistics = GetPointerToStatistics( pSegmentSet, idsegChild2 );
     pfNewParentStatistics = GetPointerToStatistics( pSegmentSet, idsegNewParent );
     
     pSegmentChild1 = GetPointerToSegment(pSegmentSet, idsegChild1);
     pSegmentChild2 = GetPointerToSegment(pSegmentSet, idsegChild2);
     pNewParentSegment = GetPointerToSegment(pSegmentSet, idsegNewParent);
     
     iStart = 0;
     iEnd = (pSegmentSet->nStatPerSegment);
     n1 = pSegmentChild1->iNumberOfSegment; 
     n2 = pSegmentChild2->iNumberOfSegment;
     for(i=iStart; i<iEnd; i++)
     {
	 	pfNewParentStatistics[i] = ( pfChild1Statistics[i]*n1 + pfChild2Statistics[i]*n2 ) / (n1+n2);
     }
     pNewParentSegment->iNumberOfSegment = n1+n2;
     
	 //3) set idfirstneighbor and numneighbor to -1 and 0
	 pNewParentSegment->idneiFirstNeighbor = -1;
	 pNewParentSegment->iNumberOfNeighbor = 0;

     //4) set ownership for the 2 childs and the new parent
     //
     //    note: ownership should be set after merging the two child's
     // 		 neighbor list, otherwise the new parent segment id
     // 		 would be inserted in the new parent neighbor list.
     pSegmentChild1->idsegParentSegment = idsegNewParent;
     pSegmentChild2->idsegParentSegment = idsegNewParent;
     pNewParentSegment->idsegParentSegment = -1;

     //5) update best matching segment and criteria
     SetBestMatchingClass(pSegmentSet, idsegNewParent);

     //6) save into fusion list
     RegisterFusion(pSegmentSet, idsegNewParent, idsegChild1, idsegChild2);
     return;
}

void RegisterFusion(SEGMENTSET* pSegmentSet,
		     		int idsegParent,
		     		int idseg1,
		     		int idseg2 )
{
    FUSION* pFusion;
    int idfusion;
    SEGMENT* pSegment;
    
    idfusion = idsegParent - (pSegmentSet->iInitialNumberOfSegment);
    pFusion = GetPointerToFusion( pSegmentSet, idfusion );
    pFusion->idsegChildSegment1 = idseg1;
    pFusion->idsegChildSegment2 = idseg2;
    
    pSegment = GetPointerToSegment( pSegmentSet, idseg1 );
    pFusion->fCriteria = pSegment->fBestMatchValue;
    return;
}



FUSION* GetPointerToFusion( SEGMENTSET* pSegmentSet,
			    			int idfus )
{
    FUSION* pFusion;
    pFusion = (FUSION*) &( pSegmentSet->pListFusion[idfus] );
    return pFusion;
}


BOOL GetFusionSequenceProfile(SEGMENTSET* pSegmentSet,
							  int iFusionInfo,
							  double* pfProfileStat,
							  int* p_iNumberOfProfileStat,
							  int imaxnumberofstat)
{
	int imaxnumberoffusion = pSegmentSet->iInitialNumberOfSegment-1;
	if(pSegmentSet==NULL || pfProfileStat==NULL || p_iNumberOfProfileStat==NULL
		|| iFusionInfo<0 || iFusionInfo>FUSION_MAXNUMBEROFINFOCHANNEL
		|| imaxnumberofstat<imaxnumberoffusion)
	{
		ASSERT(FALSE);
		p_iNumberOfProfileStat[0] = 0;
		return FALSE;
	}
    FUSION* pFusion;
    int idfusion, idsegment;
    SEGMENT* pSegment;

	//do not include last fusion, because it is the border segment
    imaxnumberoffusion = imaxnumberoffusion -1;

	for(idfusion=0; idfusion<imaxnumberoffusion; idfusion++)
	{
		pFusion = GetPointerToFusion( pSegmentSet, idfusion );
		if(iFusionInfo==FUSION_INFO_CRITERIA)
		{
			pfProfileStat[idfusion] = pFusion->fCriteria;
		}
		else if(iFusionInfo==FUSION_INFO_SEGMENTSIZE)
		{
			idsegment = idfusion + (pSegmentSet->iInitialNumberOfSegment);
			pSegment = GetPointerToSegment( pSegmentSet, idsegment );
			pfProfileStat[idfusion] = pSegment->iNumberOfSegment;
		}
		else
		{
			ASSERT(FALSE);
			p_iNumberOfProfileStat[0] = 0;
			return FALSE;
		}
	}
	p_iNumberOfProfileStat[0] = idfusion;
	return TRUE;
}