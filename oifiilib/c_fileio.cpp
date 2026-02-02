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
// fileio.mc
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

#include "c_fileio.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/*  
// module globals
*/

/*   
// private function declarations
*/
void OpenListSegment(struct tagSEGMENTSET* pSegmentSet, char* pszFileName);
void OpenSegmentStats(struct tagSEGMENTSET* pSegmentSet, char* pszFileName);
void OpenListNeighbor(struct tagSEGMENTSET* pSegmentSet, char* pszFileName);
void OpenListFusion(struct tagSEGMENTSET* pSegmentSet, char* pszFileName);

void SaveListSegment(struct tagSEGMENTSET* pSegmentSet, char* pszFileName);
void SaveSegmentStats(struct tagSEGMENTSET* pSegmentSet, char* pszFileName);
void SaveListNeighbor(struct tagSEGMENTSET* pSegmentSet, char* pszFileName);
void SaveListFusion(struct tagSEGMENTSET* pSegmentSet, char* pszFileName);



/*    
// public function definitions
*/

SEGMENTSET* OpenSegmentSet(char* pszFileName)
{
    SEGMENTSET* pSegmentSet;
    FILE* pFile;
    char* pszFileExtension;
    int initialnumberofsegment, finalnumberofsegment, maxallowedfinalsegment;
    int inumberofbyte, ibyteread, iNumBytePreviouslyStored;
    //int iextranumberofneighbor;
    //NEIGHBOR* pNewListNeighbor;


    inumberofbyte = sizeof(SEGMENTSET);
    pSegmentSet = (SEGMENTSET*) malloc( inumberofbyte );
    if( pSegmentSet==NULL )
    {
		printf("not able to allocate SEGMENTSET structure\n");
		return NULL;
    }

    /*  
    // 1) read the *.set file from disk
    */
    pszFileExtension = strchr(pszFileName,'.');
    if( pszFileExtension != NULL ) strcpy(pszFileExtension, ".set");
      else strcat(pszFileName, ".set");
    if( (pFile = fopen(pszFileName, "r")) == NULL )
    {
		printf("Error, cannot open file %s\n", pszFileName);
		return NULL;
    }
    ibyteread = fread(pSegmentSet, sizeof(SEGMENTSET), 1, pFile);
    fclose(pFile);
    if(ibyteread != 1)
    {     
    	/* //spoirier march 1st 95, mdl's fread() does not return number of read items
		printf("Error, file %s not properly read\n", pszFileName);
		printf("ibyteread = %d\n",ibyteread);
		return NULL;
		*/
    }

    /*  
    // 2) validate parameters
    */
    initialnumberofsegment = pSegmentSet->iInitialNumberOfSegment;
    finalnumberofsegment = pSegmentSet->iFinalNumberOfSegment;
    maxallowedfinalsegment = 2*pSegmentSet->iInitialNumberOfSegment - 1;
    if( finalnumberofsegment == 0 ) finalnumberofsegment = maxallowedfinalsegment;
    if( initialnumberofsegment <=0  ||
	initialnumberofsegment > SEGSET_MAXINITIALSEGMENT  ||
	finalnumberofsegment <= initialnumberofsegment	||
	finalnumberofsegment > maxallowedfinalsegment )
    {
		printf("SEGMENTSET cannot be properly initialized from file %s\n", pszFileName);
		printf("fatal error, initial and final inconsistent.\n");
		return NULL;
    }

    iNumBytePreviouslyStored = pSegmentSet->iTotalNumberOfByteUsed;
    pSegmentSet->iTotalNumberOfByteUsed = inumberofbyte;

    /*        
    // 3.1) allocate list segment
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
    // 3.2) allocate list neighbor
    */
    pSegmentSet->iActualNumberOfNeighbor = pSegmentSet->idneiNextAvailNeighbor;
    inumberofbyte = pSegmentSet->iActualNumberOfNeighbor * sizeof(NEIGHBOR) *2; /* *2 fudge factor */
    pSegmentSet->pListNeighbor = (NEIGHBOR*) malloc( inumberofbyte );
    if( pSegmentSet->pListNeighbor==NULL )
    {
		printf("not able to allocate list of NEIGHBOR\n");
		free(pSegmentSet->pListSegment);
		free(pSegmentSet);
		return NULL;
    }
    pSegmentSet->iTotalNumberOfByteUsed += inumberofbyte;

    /* 
    // 3.3) allocate fusion list
    */
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
    // 3.4) allocate pfStatistics
    */
    inumberofbyte = pSegmentSet->nSizeStatInByte;
    pSegmentSet->pfStatistics = (double*) malloc( inumberofbyte );
    if( pSegmentSet->pfStatistics==NULL )
    {
		printf("not able to allocate list of FUSION\n");
		free(pSegmentSet->pListFusion);
		free(pSegmentSet->pListNeighbor);
		free(pSegmentSet->pListSegment);
		free(pSegmentSet);
		return NULL;
    }

    /*    
    // 4) open list segment
    */
    pszFileExtension = strchr(pszFileName,'.');
    if( pszFileExtension != NULL ) strcpy(pszFileExtension, ".seg");
      else strcat(pszFileName, ".seg");
    OpenListSegment(pSegmentSet, pszFileName);

    /*   
    // 5) open list segment
    */
    pszFileExtension = strchr(pszFileName,'.');
    if( pszFileExtension != NULL ) strcpy(pszFileExtension, ".sta");
      else strcat(pszFileName, ".sta");
    OpenSegmentStats(pSegmentSet, pszFileName);

    /*   
    // 6) open list neighbor
    */
    pszFileExtension = strchr(pszFileName,'.');
    if( pszFileExtension != NULL ) strcpy(pszFileExtension, ".nei");
      else strcat(pszFileName, ".nei");
    OpenListNeighbor(pSegmentSet, pszFileName);

    /* 
    // 7) open list fusion
    */
    pszFileExtension = strchr(pszFileName,'.');
    if( pszFileExtension != NULL ) strcpy(pszFileExtension, ".fus");
      else strcat(pszFileName, ".fus");
    OpenListFusion(pSegmentSet, pszFileName);

    return pSegmentSet;
}


void SaveSegmentSet(SEGMENTSET* pSegmentSet, char* pszFileName)
{
    char* pszFileExtension;
    FILE* pFile;
    int ibytestored;

    /* 
    // 1) save *.set file to disk
    */
    pszFileExtension = strchr(pszFileName,'.');
    if( pszFileExtension != NULL ) strcpy(pszFileExtension, ".set");
      else strcat(pszFileName, ".set");
    if( (pFile = fopen(pszFileName, "w")) == NULL )
    {
		printf("Error, cannot open file %s\n", pszFileName);
		exit(1);
    }
    ibytestored = fwrite(pSegmentSet, sizeof(SEGMENTSET), 1, pFile);
    fclose(pFile);
    if(ibytestored != 1)
    {
		printf("Error, file %s not properly saved\n", pszFileName);
		return;
    }

    /*  
    // 2) save list segment to disk
    */
    pszFileExtension = strchr(pszFileName,'.');
    if( pszFileExtension != NULL ) strcpy(pszFileExtension, ".seg");
      else strcat(pszFileName, ".seg");
    SaveListSegment(pSegmentSet, pszFileName);

    /*    
    // 3) save list segment to disk
    */
    pszFileExtension = strchr(pszFileName,'.');
    if( pszFileExtension != NULL ) strcpy(pszFileExtension, ".sta");
      else strcat(pszFileName, ".sta");
    SaveSegmentStats(pSegmentSet, pszFileName);

    /*   
    // 4) save list neighbor to disk
    */
    pszFileExtension = strchr(pszFileName,'.');
    if( pszFileExtension != NULL ) strcpy(pszFileExtension, ".nei");
      else strcat(pszFileName, ".nei");
    SaveListNeighbor(pSegmentSet, pszFileName);

    /*   
    // 5) save list fusion to disk
    */
    pszFileExtension = strchr(pszFileName,'.');
    if( pszFileExtension != NULL ) strcpy(pszFileExtension, ".fus");
      else strcat(pszFileName, ".fus");
    SaveListFusion(pSegmentSet, pszFileName);

    return;
}




/*
// private functions definition     
*/

void OpenListSegment(SEGMENTSET* pSegmentSet, char* pszFileName)
{
    FILE* pFile;
    int iCount, iRead, iSize;

    if( (pFile = fopen(pszFileName, "r")) == NULL )
    {
		printf("Error, cannot open file %s\n", pszFileName);
		exit(1);
    }
    iSize = sizeof(SEGMENT);
    iCount = pSegmentSet->iFinalNumberOfSegment;
    iRead = fread(pSegmentSet->pListSegment, iSize, iCount, pFile);
    fclose(pFile);
    if(iRead != iCount)
    {   
    	/* //spoirier march 1st 95, mdl's fread() does not return number of read items
		printf("Error, file %s not properly read\n", pszFileName);
		printf("iCount=%i, iRead=%i, iSize=%i\n", iCount, iRead, iSize);
		return;
		*/
    }
    return;
}

void OpenSegmentStats(SEGMENTSET* pSegmentSet, char* pszFileName)
{
    FILE* pFile;
    int iSize, iCount, iRead;

    if( (pFile = fopen(pszFileName, "r")) == NULL )
    {
		printf("Error, cannot open file %s\n", pszFileName);
		exit(1);
    }
    iSize = (pSegmentSet->nStatPerSegment)*sizeof(double);
    iCount = pSegmentSet->iInitialNumberOfSegment;
    iRead = fread(pSegmentSet->pfStatistics, iSize, iCount, pFile);
    fclose(pFile);
    if(iRead != iCount)
    {    
    	/* //spoirier march 1st 95, mdl's fread() does not return number of read items
		printf("Error, file %s not properly read\n", pszFileName);
		printf("iCount=%i, iRead=%i, iSize=%i\n", iCount, iRead, iSize);
		return;
		*/
    }
    return;
}

void OpenListNeighbor(SEGMENTSET* pSegmentSet, char* pszFileName)
{
    FILE* pFile;
    int iCount, iRead, iSize;

    if( (pFile = fopen(pszFileName, "r")) == NULL )
    {
		printf("Error, cannot open file %s\n", pszFileName);
		exit(1);
    }
    iSize = sizeof(NEIGHBOR);
    iCount = pSegmentSet->idneiNextAvailNeighbor;
    iRead = fread(pSegmentSet->pListNeighbor, iSize, iCount, pFile);
    fclose(pFile);
    if(iRead != iCount)
    {   
    	/* //spoirier march 1st 95, mdl's fread() does not return number of read items
		printf("Error, file %s not properly read\n", pszFileName);
		printf("iCount=%i, iRead=%i, iSize=%i\n", iCount, iRead, iSize);
		return;
		*/
    }
    return;
}

void OpenListFusion(SEGMENTSET* pSegmentSet, char* pszFileName)
{
    FILE* pFile;
    int iCount, iRead, iSize;

    if( (pFile = fopen(pszFileName, "r")) == NULL )
    {
		printf("Error, cannot open file %s\n", pszFileName);
		exit(1);
    }
    iSize = sizeof(FUSION);
    iCount = (pSegmentSet->iFinalNumberOfSegment - pSegmentSet->iInitialNumberOfSegment);
    iRead = fread(pSegmentSet->pListFusion, iSize, iCount, pFile);
    fclose(pFile);
    if(iRead != iCount)
    {     
    	/* //spoirier march 1st 95, mdl's fread() does not return number of read items
		printf("Error, file %s not properly read\n", pszFileName);
		printf("iCount=%i, iRead=%i, iSize=%i\n", iCount, iRead, iSize);
		return;
		*/
    }
    return;
}

void SaveListSegment(SEGMENTSET* pSegmentSet, char* pszFileName)
{
    FILE* pFile;
    int iCount, iStored;

    if( (pFile = fopen(pszFileName, "w")) == NULL )
    {
		printf("Error, cannot open file %s\n", pszFileName);
		exit(1);
    }
    iCount = (pSegmentSet->iFinalNumberOfSegment);
    iStored = fwrite(pSegmentSet->pListSegment, sizeof(SEGMENT), iCount, pFile);
    fclose(pFile);
    if(iStored != iCount)
    {
		printf("Error, file %s not properly saved\n", pszFileName);
		return;
    }
    return;
}

void SaveSegmentStats(SEGMENTSET* pSegmentSet, char* pszFileName)
{
    FILE* pFile;
    int iSize, iCount, iStored;

    if( (pFile = fopen(pszFileName, "w")) == NULL )
    {
		printf("Error, cannot open file %s\n", pszFileName);
		exit(1);
    }
    iSize = (pSegmentSet->nStatPerSegment)*sizeof(double);
    iCount = pSegmentSet->iInitialNumberOfSegment;
    iStored = fwrite(pSegmentSet->pfStatistics, iSize, iCount, pFile);
    fclose(pFile);
    if(iStored != iCount)
    {
		printf("Error, file %s not properly saved\n", pszFileName);
		return;
    }
    return;
}

void SaveListNeighbor(SEGMENTSET* pSegmentSet, char* pszFileName)
{
    FILE* pFile;
    int iCount, iStored;

    if( (pFile = fopen(pszFileName, "w")) == NULL )
    {
		printf("Error, cannot open file %s\n", pszFileName);
		exit(1);
    }
    iCount = pSegmentSet->idneiNextAvailNeighbor;
    iStored = fwrite(pSegmentSet->pListNeighbor, sizeof(NEIGHBOR), iCount, pFile);
    fclose(pFile);
    if(iStored != iCount)
    {
		printf("Error, file %s not properly saved\n", pszFileName);
		return;
    }
    return;
}

void SaveListFusion(SEGMENTSET* pSegmentSet, char* pszFileName)
{
    FILE* pFile;
    int iCount, iStored;

    if( (pFile = fopen(pszFileName, "w")) == NULL )
    {
		printf("Error, cannot open file %s\n", pszFileName);
		exit(1);
    }
    iCount = (pSegmentSet->iFinalNumberOfSegment - pSegmentSet->iInitialNumberOfSegment);
    iStored = fwrite(pSegmentSet->pListFusion, sizeof(FUSION), iCount, pFile);
    fclose(pFile);
    if(iStored != iCount)
    {
		printf("Error, file %s not properly saved\n", pszFileName);
		return;
    }
    return;
}




POINTSET* OpenPointset(char* pszFileName)
{
    POINTSET* pPointset;

	/*
	pPointset = NewPointset(long maxnumberofelements);
	if(pPointset==NULL)
	{
		printf("not able to allocate POINTSET\n");
		return NULL;
	}
	*/

	/*
    pPointset = (POINTSET*) malloc(sizeof(POINTSET));
    if(pPointset==NULL)
    {
		printf("not able to allocate POINTSET structure\n");
		return NULL;
    }

    //
    // alloc array vt
    //
    pPointset->vt[0] = (int*) malloc(3*(maxnumberofelements*2)*sizeof(int));
    if(pPointset->vt[0]==NULL)
    {
		printf("not able to allocate POINTSET structure (vt)\n");
		free(pPointset);
		return NULL;
    }
    pPointset->vt[1] = (int*) &(pPointset->vt[0][1*(maxnumberofelements*2)]);
    pPointset->vt[2] = (int*) &(pPointset->vt[0][2*(maxnumberofelements*2)]);
    //
    // alloc array nt
    //
    pPointset->nt[0] = (int*) malloc(3*(maxnumberofelements*2)*sizeof(int));
    if(pPointset->nt[0]==NULL)
    {
		printf("not able to allocate POINTSET structure (nt)\n");
		free(pPointset->vt[0]);
		free(pPointset);
		return NULL;
    }
    pPointset->nt[1] = (int*) &(pPointset->nt[0][1*(maxnumberofelements*2)]);
    pPointset->nt[2] = (int*) &(pPointset->nt[0][2*(maxnumberofelements*2)]);
    //
    // alloc ctx, cty
    //
    pPointset->ctx = (double*) malloc(maxnumberofelements*2*sizeof(double));
    if(pPointset->ctx==NULL)
    {
		printf("not able to allocate POINTSET structure (ctx)\n");
		free(pPointset->nt[0]);
		free(pPointset->vt[0]);
		free(pPointset);
		return NULL;
    }
    pPointset->cty = (double*) malloc(maxnumberofelements*2*sizeof(double));
    if(pPointset->cty==NULL)
    {
		printf("not able to allocate POINTSET structure (cty)\n");
		free(pPointset->ctx);
		free(pPointset->nt[0]);
		free(pPointset->vt[0]);
		free(pPointset);
		return NULL;
    }
    pPointset->px = (double*) malloc(maxnumberofelements*sizeof(double));
    if(pPointset->px==NULL)
    {
		printf("not able to allocate POINTSET structure (px)\n");
		free(pPointset->cty);
		free(pPointset->ctx);
		free(pPointset->nt[0]);
		free(pPointset->vt[0]);
		free(pPointset);
		return NULL;
    }
    pPointset->py = (double*) malloc(maxnumberofelements*sizeof(double));
    if(pPointset->py==NULL)
    {
		printf("not able to allocate POINTSET structure (py)\n");
		free(pPointset->px);
		free(pPointset->cty);
		free(pPointset->ctx);
		free(pPointset->nt[0]);
		free(pPointset->vt[0]);
		free(pPointset);
		return NULL;
    }
    pPointset->dcd = (int*) malloc(maxnumberofelements*sizeof(int));
    if(pPointset->dcd==NULL)
    {
		printf("not able to allocate POINTSET structure (dcd)\n");
		free(pPointset->py);
		free(pPointset->px);
		free(pPointset->cty);
		free(pPointset->ctx);
		free(pPointset->nt[0]);
		free(pPointset->vt[0]);
		free(pPointset);
		return NULL;
    }
	*/

	pPointset = NULL;
	return pPointset;
}


void SavePointset(POINTSET* pPointset, char* pszFileName)
{
	return;
}
