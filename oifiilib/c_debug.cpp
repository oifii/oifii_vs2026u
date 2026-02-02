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

/* 
// debug.mc
//
//
//
// OFFWORLD Research
// 675 Des Rocailles, Quebec, G2J 1A9
// by Stephane Poirier
//
*/

#include "stdafx.h"

#include "oifiilib.h"

#include <stdio.h>  /* for function that saves on disk */

#include "c_pointset.h"
#include "c_segmentset.h"

#include "c_debug.h"
/*
// module globals
*/

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


void DumpInfoSegmentSet( SEGMENTSET* pSegmentSet )
{
	DumpInfoSegmentSetToFile( pSegmentSet, "segset.dat" );
	return;
}

void DumpInfoSegmentSetToFile( SEGMENTSET* pSegmentSet, char* pszFilename )
{
    FILE* pFile;  

    if( (pFile = fopen(pszFilename, "w")) == NULL )
    {
		printf("Error, cannot open file %s\n", pszFilename);
		exit(1);
    }
    fprintf(pFile, "nStatPerSegment: %d\n",
		    pSegmentSet->nStatPerSegment);
    fprintf(pFile, "nSizeStatInByte: %d\n",
		    pSegmentSet->nSizeStatInByte);
    fprintf(pFile, "iInitialNumberOfSegment: %d\n",
		    pSegmentSet->iInitialNumberOfSegment);
    fprintf(pFile, "iFinalNumberOfSegment: %d\n",
		    pSegmentSet->iFinalNumberOfSegment);
    fprintf(pFile, "iActualNumberOfSegment: %d\n",
		    pSegmentSet->iActualNumberOfSegment);
    fprintf(pFile, "iActualNumberOfNeighbor: %d\n",
		    pSegmentSet->iActualNumberOfNeighbor);
    fprintf(pFile, "iActualNumberOfFusion: %d\n",
		    pSegmentSet->iActualNumberOfFusion);
    fprintf(pFile, "idneiNextAvailNeighbor: %d\n",
		    pSegmentSet->idneiNextAvailNeighbor);
    fprintf(pFile, "iTotalNumberOfByteUsed: %d\n",
		    pSegmentSet->iTotalNumberOfByteUsed);
    fclose(pFile);
    return;
}


void DumpInfoSegment( SEGMENTSET* pSegmentSet )
{
	DumpInfoSegmentToFile( pSegmentSet, "segment.dat" );
	return;
}

void DumpInfoSegmentToFile( SEGMENTSET* pSegmentSet, char* pszFilename )
{
    int i, idseg, idsegEnd;
    SEGMENT* pSegment;
    int* pNeighbor;
    double* pfSegStatistics;
    
    FILE* pFile;

    if( (pFile = fopen(pszFilename, "w")) == NULL )
    {
		printf("Error, cannot open file %s\n", pszFilename);
		exit(1);
    }

    idsegEnd = pSegmentSet->iActualNumberOfSegment;
    fprintf(pFile, "idseg, idsegParent, iNumSeg, idsegBest, idneiFirst, iNumNei, fBestMatch\n");
    for(idseg=0; idseg<idsegEnd; idseg++)
    {
		pSegment = GetPointerToSegment( pSegmentSet, idseg );
		fprintf(pFile, "%d, %d, %d, %d, %d, %d, %g\n",
			  	idseg,
		      	pSegment->idsegParentSegment,
		       	pSegment->iNumberOfSegment,
		       	pSegment->idsegBestMatchingSegment,
		       	pSegment->idneiFirstNeighbor,
		       	pSegment->iNumberOfNeighbor,
		       	pSegment->fBestMatchValue );
		pfSegStatistics = GetPointerToStatistics( pSegmentSet, idseg );
		fprintf(pFile, "  Stats: ");
		for(i=0; i<(pSegmentSet->nStatPerSegment); i++)
		{
		    fprintf(pFile, "%g ",pfSegStatistics[i]);
		}
		fprintf(pFile, "\n");
		pNeighbor = GetPointerToNeighbor( pSegmentSet, pSegment->idneiFirstNeighbor );
		fprintf(pFile, "  Neighbors: ");
		for(i=0; i<(pSegment->iNumberOfNeighbor); i++)
		{
		    fprintf(pFile, "%d ",pNeighbor[i]);
		}
		fprintf(pFile, "\n");
    }
    fclose(pFile);
    return;
}

void DumpInfoFusion( SEGMENTSET* pSegmentSet )
{
	DumpInfoFusionToFile( pSegmentSet, "fusion.dat" );
	return;
}	

void DumpInfoFusionToFile( SEGMENTSET* pSegmentSet, char* pszFilename )
{
    FUSION* pFusion;
    int idsegParent, idseg1, idseg2, idfusion, idfusionEnd;
    double fCriteria;
    
    FILE* pFile;
    FILE* pFile2;
    char* pCharFound;
	char pszFilenameCopyOf[255];
    if( (pFile = fopen(pszFilename, "w")) == NULL )
    {
		printf("Error, cannot open file %s\n", pszFilename);
		exit(1);
    }     
	strcpy(pszFilenameCopyOf, pszFilename);
    pCharFound = strchr(pszFilenameCopyOf, '.'); 
    if(pCharFound==NULL)    
    {
    	printf("Error, in DumpInfoFusionToFile()\n");
    }
    strcpy(pCharFound, ".gph"); //pCharFound within pszFilenameCopyOf
    if( (pFile2 = fopen(pszFilenameCopyOf, "w")) == NULL )
    {
		printf("Error, cannot open file %s\n", pszFilename);
		exit(1);
    }

    idfusionEnd = pSegmentSet->iFinalNumberOfSegment - pSegmentSet->iInitialNumberOfSegment;
    for(idfusion=0; idfusion<idfusionEnd; idfusion++)
    {
		pFusion = GetPointerToFusion( pSegmentSet, idfusion );
		idsegParent = idfusion + pSegmentSet->iInitialNumberOfSegment;
		idseg1 = pFusion->idsegChildSegment1;
		idseg2 = pFusion->idsegChildSegment2;
		fCriteria = pFusion->fCriteria;
		fprintf(pFile, "%d = %d + %d (%f)\n", idsegParent, idseg1, idseg2, fCriteria);
		fprintf(pFile2, "%d\t%f\n", idfusion, fCriteria);
    }
    fclose(pFile);                        
    fclose(pFile2);
    return;
}


