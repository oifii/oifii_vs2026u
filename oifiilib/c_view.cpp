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

/*
#include "c_pointset.h"  // should be included before "view.h" 
#include "c_segmentset.h"

#include "c_view.h"


void draw_triangle_from_memory(	POINTSET* pPointset, 
								SEGMENTSET* pSegmentSet,
								int iStatIndex,
								int iOffsetLevel)
{
    Dpoint3d    Pnt[10];
    MSElement   Shape;
    int i,k, ivertex;
    int Color, iBaseLevel, iLevel;
    float* pfStat;
    int nStatPerPoint;
    
    Color = 2; //2 is green in SVGA 16 colors mode 
    iBaseLevel = 2;
    iLevel = iBaseLevel + iOffsetLevel;
    
    pfStat=NULL;  
    nStatPerPoint = 0;
    if(pPointset->pfStatistics != NULL) 
    {   
    	printf("using poinset's statistics\n");
    	pfStat = pPointset->pfStatistics; 
    	nStatPerPoint = pPointset->nStatPerPoint;
    }
    else
    {
		if(pSegmentSet!=NULL)
		{
			if(pSegmentSet->pfStatistics != NULL)
			{ 
				printf("using segmentset's statistics\n");  
				pfStat = pSegmentSet->pfStatistics;
    			nStatPerPoint = pSegmentSet->nStatPerSegment;
			}
			else
			{
				printf("not using any statistics (pSegmentSet->pfStatistics is NULL)\n");
				pfStat = NULL;
			}
		}
		else
		{
			printf("not using any statistics (pSegmentSet is NULL)\n");	
			pfStat = NULL;	
		}	
    }	
    
    
    for (i=0; i<=pPointset->ntri; i++)
    {
		if( !InvalidTri(pPointset,i) )
		{
		    //
		    //Pour chacun des triangles valides:
		    //
		    for (k=0; k<3; k++)
		    { 
			    ivertex = pPointset->vt[k][i];
				Pnt[k].x = pPointset->px[ivertex];
				Pnt[k].y = pPointset->py[ivertex];
				////////////////////////////////////////////////////////////
				// SPoirier Jan 8th 1994, store tree average intensity in z  
				if(pfStat != NULL)
				{
					// assuming the very first statistic is the intensity   
					// we add POINTSET_EXTRA_NUMBEROFSTAT to skip the voronoiarea and other basic stats 
				    Pnt[k].z = pfStat[ivertex*(nStatPerPoint)+iStatIndex]; 
					// printf("ivertex=%i, z=%f\n", ivertex, Pnt[k].z); 
				}
				else
				{
					Pnt[k].z = 0.0;
				}					
				// SPoirier Jan 8th 1994, store tree average intensity in z  
				////////////////////////////////////////////////////////////
				MasterToUors_point(&Pnt[k]);
		    }
	
		    // To close the polygon, the last point is equal to the new point 
		    Pnt[3].x = Pnt[0].x;
		    Pnt[3].y = Pnt[0].y;
		    Pnt[3].z = Pnt[0].z;
	
		    mdlShape_create ( &Shape, NULL, Pnt, 4, 0);
		    mdlElement_setSymbology ( &Shape, &Color, NULL, NULL );
		    mdlElement_setProperties (&Shape, &iLevel, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
		    mdlElement_add ( &Shape );
		    mdlElement_display ( &Shape, NORMALDRAW );
		}
    }
}


void draw_triangle_centre_from_memory(POINTSET* pPointset)
{
    Dpoint3d    Pnt[10];
    MSElement	Ellipse;
    int i;
    int Color, Level;

    float sizex, sizey;

    Color = 3; //3 is red in SVGA 16 colors mode 
    Level = 3;

    sizex = 0.2;
    sizey = 0.2;
    MasterToUors_distance(&sizex);
    MasterToUors_distance(&sizey);

    for (i=0; i<=pPointset->ntri; i++)
    {
		Pnt[0].x = pPointset->ctx[i];
		Pnt[0].y = pPointset->cty[i];
		Pnt[0].z = 0.0;
		MasterToUors_point(&Pnt[0]);


		mdlEllipse_create ( &Ellipse,NULL,Pnt,sizex,sizey,NULL, 0);
		mdlElement_setSymbology ( &Ellipse, &Color, NULL, NULL );
		mdlElement_setProperties (&Ellipse, &Level, NULL, NULL, NULL,
					  NULL, NULL, NULL, NULL);
		mdlElement_add ( &Ellipse );
		mdlElement_display ( &Ellipse, NORMALDRAW );
    }
}


void draw_triangle_labels_from_memory(POINTSET* pPointset)
{
    Dpoint3d    Pnt[10];
    MSElement	TextElement;
    int i;
    int Color, Level;
    char text[100];
    float sizex, sizey;
    TextSizeParam* txSize;


    Color = 2; //2 is green in SVGA 16 colors mode 
    Level = 4;

    //
    //sizex = 0.2;
    //sizey = 0.3;
    //MasterToUors_distance(&sizex);
    //MasterToUors_distance(&sizey);
    //txSize->mode = TXT_BY_TILE_SIZE;
    //txSize->size.width = sizex;
    //txSize->size.height = sizey;
    //

    for (i=0; i<=pPointset->ntri; i++)
    {
		Pnt[0].x = pPointset->ctx[i];
		Pnt[0].y = pPointset->cty[i];
		Pnt[0].z = 0.0;
		MasterToUors_point(&Pnt[0]);

		sprintf(text, "%i", i);
		mdlText_create ( &TextElement,NULL,text,Pnt,NULL,NULL,NULL,NULL);
		mdlElement_setSymbology ( &TextElement, &Color, NULL, NULL );
		mdlElement_setProperties (&TextElement, &Level, NULL, NULL, NULL,
					  NULL, NULL, NULL, NULL);
		mdlElement_add ( &TextElement );
		mdlElement_display ( &TextElement, NORMALDRAW );
    }
}



void draw_point_labels_from_memory(POINTSET* pPointset)
{
    Dpoint3d    Pnt[10];
    MSElement	TextElement;
    int i;
    int Color, Level;
    char text[100];
    float sizex, sizey;
    TextSizeParam* txSize;


    Color = 0; //0 is white in SVGA 16 colors mode 
    Level = 5;

    //
    //sizex = 0.2;
    //sizey = 0.3;
    //MasterToUors_distance(&sizex);
    //MasterToUors_distance(&sizey);
    //txSize->mode = 1;
    //txSize->size.width = sizex;
    //txSize->size.height = sizey;
    //

    for (i=0; i<pPointset->npts; i++)
    {
		Pnt[0].x = pPointset->px[i];
		Pnt[0].y = pPointset->py[i];
		Pnt[0].z = 0.0;
		MasterToUors_point(&Pnt[0]);

		sprintf(text, "%i", i);
		mdlText_create ( &TextElement,NULL,text,Pnt,NULL,NULL,NULL,NULL);
		mdlElement_setSymbology ( &TextElement, &Color, NULL, NULL );
		mdlElement_setProperties (&TextElement, &Level, NULL, NULL, NULL,
					  NULL, NULL, NULL, NULL);
		mdlElement_add ( &TextElement );
		mdlElement_display ( &TextElement, NORMALDRAW );
    }
}




void draw_point_from_memory ( POINTSET* pPointset )
{

/////////////////////////////////
// This actual version will draw an
// ellipse around each input data
// point.
//
// In our specific application, the
// point is actualy a tree position
// and the ellipse x and y axis are
// given by the tree spatial x-y
// dimension found by the TreeFinder
// Application.
/////////////////////////////////

    int      	i, iStatBaseOffset, nStatPerPoint; 
    //float    	fIntensity, fTreeDimX, fTreeDimY; 
    double 		dfIntensity, dfTreeDimX, dfTreeDimY;
    float*		pfStatistics;
    Dpoint3d    Pnt;
    MSElement	Ellipse;
    int 		Color, Level;
    RotMatrix*	rMatrix;            
                
    Color = 0; //0 is white in SVGA 16 colors mode 
    			// in default ms palette, 0, 16, 32, 48, 64, 80, 96, 112, ... are the gray levels 
    Level = 1;
    
    if(pPointset->pfStatistics == NULL)
    {
    	printf("cannot view points, statistics are now stored into pSegmentset\n");
    	// this should be fixed, development-time error, get the statistics from pSegmentSet  
    	return;
    }   
    pfStatistics = pPointset->pfStatistics;
    nStatPerPoint = pPointset->nStatPerPoint; 
    
    
    //mdlRMatrix_getIdentity(rMatrix);
    
    
    for (i=0; i<pPointset->npts; i++)
    {   
    	iStatBaseOffset = i*nStatPerPoint+POINTSET_EXTRA_NUMBEROFSTAT;
		Pnt.x = pPointset->px[i];
		Pnt.y = pPointset->py[i];
		Pnt.z = 0.0;
		MasterToUors_point(&Pnt);
		dfIntensity = pfStatistics[iStatBaseOffset+POINTSET_XYZI_INTENSITY]; 
		dfIntensity = dfIntensity/16.0;
		Color = abs(dfIntensity)*16;
		dfTreeDimX = pfStatistics[iStatBaseOffset+POINTSET_XYZI_TREEDIMX];
		dfTreeDimY = pfStatistics[iStatBaseOffset+POINTSET_XYZI_TREEDIMY]; 
		 
		//printf("%i, %f, %f\n", Color, fTreeDimX, fTreeDimY);
		
		MasterToUors_distance(&dfTreeDimX);
		MasterToUors_distance(&dfTreeDimY);
	
		mdlEllipse_create ( &Ellipse,NULL,&Pnt,dfTreeDimX,dfTreeDimY,NULL, 1);
		mdlElement_setSymbology ( &Ellipse, &Color, NULL, NULL );
		mdlElement_add ( &Ellipse );
		mdlElement_display ( &Ellipse, NORMALDRAW );		
  	} 
}


void draw_point_from_file ( char* filename )
{
	///////////////////////////////////
	// This actual version will draw an
	// ellipse around each input data
	// point.
	//
	// In our specific application, the
	// point is actualy a tree position
	// and the ellipse x and y axis are
	// given by the tree spatial x-y
	// dimension found by the TreeFinder
	// Application.
	///////////////////////////////////
    FILE     *Ptr;
    float    temp[10];
    int      id;

    Dpoint3d    Pnt[10];
    MSElement	TextElement;
    int 	Color, Level,i;
    char	message[255];
    char	text[100];

    Color = 0; //0 is white in SVGA 16 colors mode 
    Level = 5;


    //////////////////////////////////
    //Open .xyi point information file
    //////////////////////////////////
    if ( ( Ptr = fopen ( filename, "r" ) ) == NULL )
    {
		sprintf(message, "Error Opening %s", filename);
		mdlDialog_openAlert(message);
		return;
    }
    // 
    // reading: index, xcoor, ycoor, intensity, dimx*dimy, dimx, dimy 
    //
    while( fscanf(Ptr, "%f%f%f", &temp[0], &temp[1], &temp[2]) == 3 )
    {
		Pnt[0].x = temp[0];
		Pnt[0].y = temp[1];
		Pnt[0].z = 0.0;
		MasterToUors_point(&Pnt[0]);
	
		i = (long) temp[2];
		sprintf(text, "%i", i);
	
		mdlText_create ( &TextElement,NULL,text,Pnt,NULL,NULL,NULL,NULL);
		mdlElement_setSymbology ( &TextElement, &Color, NULL, NULL );
		mdlElement_setProperties (&TextElement, &Level, NULL, NULL, NULL,
					  NULL, NULL, NULL, NULL);
		mdlElement_add ( &TextElement );
		mdlElement_display ( &TextElement, NORMALDRAW );
    }
    fclose ( Ptr );
}



void draw_voronoi_regions_from_memory(POINTSET* pPointset)
{
    int ivertex,j;

    int numtrifound, numneighborfound, itriseed;
    int p_arraytri[200];
    int p_arrayneighbor[200];

    Dpoint3d	Pnt[100];
    MSElement	Shape;
    int         Color, Level;
    char        message[255];

    float    temp[10];
    int      id;

    FILE* Ptr;
    if ( ( Ptr = fopen ( "xpoint.dat", "w" ) ) == NULL )
    {
		sprintf(message, "Error Creating %s", "xpoint.dat");
		mdlDialog_openAlert(message);
		return;
    }


    Color = 3; //3 is red in SVGA 16 colors mode 
    Level = 10;


    // the triangle seed must be an existing triangle index 
    itriseed = 0;

    for (ivertex=0; ivertex<(pPointset->npts); ivertex++)
    {
	 //
	 //Pour chacun des points qui n'ont aucun triangle adjacents
	 //touchant directement la frontiere convexe ...
	 //
	 if( FindAllValidTriSurroundingVertex( pPointset,
					       ivertex,
					       &itriseed,
					       &numtrifound,
					       p_arraytri,
					       &numneighborfound,
					       p_arrayneighbor) == TRUE)
	 {
	     //if all surrounding triangles are valid,		 
	     // build voronoi polygon using each adjtri's center 
	     for(j=0; j<numtrifound; j++)
	     {
		 Pnt[j].x = pPointset->ctx[p_arraytri[j]];
		 Pnt[j].y = pPointset->cty[p_arraytri[j]];
		 Pnt[j].z = 0.0;
		 MasterToUors_point(&Pnt[j]);
	     }
	     // To close the polygon, the last point is equal to the first point 
	     Pnt[numtrifound].x = Pnt[0].x;
	     Pnt[numtrifound].y = Pnt[0].y;
	     Pnt[numtrifound].z = Pnt[0].z;


	     // with temp[2] being the intensity, was previously read from file 
	     // Color = (int) temp[2]/16; // in the file, color ranges from	0 to 255 
	     // Color = (15-Color)*16; // we should get only the grey sub-palette 


	     mdlShape_create ( &Shape, NULL, Pnt, numtrifound+1, 1);
	     mdlElement_setSymbology ( &Shape, &Color, NULL, NULL );
	     mdlElement_setProperties (&Shape, &Level, NULL, NULL, NULL,
				  NULL, NULL, NULL, NULL);
	     mdlElement_add ( &Shape );
	     mdlElement_display ( &Shape, NORMALDRAW );
	 }
	 else
	 {
	     fprintf(Ptr, "vertex=%d, numneighbor=%d,  ",ivertex,numneighborfound);
	     for(j=0; j<numneighborfound; j++)
	     {
		 	fprintf(Ptr, " %d", p_arrayneighbor[j]);
	     }
	     fprintf(Ptr, "\n");
	 }
    }
    fclose ( Ptr );
    return;
}




///////////////////////////////////////////////////////////
// Cette fonction converti un point en coordonn‚es UORs.
// Par: Jean-Francois Duguay, Novembre 1991, CRG, Univ. Laval
///////////////////////////////////////////////////////////

//void MasterToUors_point ( Dpoint3d* Pnt )
//{
//    extern Tcb  *tcb;
//    double      WU;
//    Dpoint3d    GO;
//
//    GO = tcb->globorg;
//    WU = tcb->subpermast * tcb->uorpersub;
//
//    Pnt->x = ( Pnt->x * WU ) + GO.x;
//    Pnt->y = ( Pnt->y * WU ) + GO.y;
//    Pnt->z = ( Pnt->z * WU ) + GO.z;
//}



///////////////////////////////////////////////////////////
// Cette fonction converti une distance en coordonnees UORs.
// Par: Stephane Poirier, Novembre 1993
///////////////////////////////////////////////////////////

//void MasterToUors_distance ( double* Double )
//{
//    extern Tcb  *tcb;
//    double      WU;
//    WU = tcb->subpermast * tcb->uorpersub;
//    Double[0] = Double[0] * WU;
//}





//////////////////////////////////////////////////////////////
//Les fonctions suivantes permettent d'afficher des partitions
//Par:  Stephane Poirier, Mars 1995 
//
//they should be eventualy moved into the segment set library
//////////////////////////////////////////////////////////////  



///////////////////////////////////////////  
//draw_segment_labels_partition()
//
//display a partition, tagging each initial 
//segments with its owner segment number.
//////////////////////////////////////////
void draw_segment_labels_partition(	POINTSET* pPointset, 
									SEGMENTSET* pSegmentSet,
									int iNumberOfSegmentForFinalPartition,
									int iOffsetLevel)
{
    Dpoint3d    Pnt[10];
    MSElement	TextElement;
    int Color, iBaseLevel, iLevel;
    char text[100];
    float sizex, sizey;
    TextSizeParam txSize;
    int idseg, idowner, imaxnumseg; 

    Color = 0; //0 is white in SVGA 16 colors mode 
    
    sizex = 1; // 0.2 
    sizey = 1.5; // 0.3 
    MasterToUors_distance(&sizex);
    MasterToUors_distance(&sizey);
    txSize.mode = TXT_BY_TILE_SIZE; // TXT_BY_TILE_SIZE | TXT_NO_TRANSFORM 
    txSize.size.width = sizex;
    txSize.size.height = sizey;    
    
    // validate iNumberOfSegmentForFinalPartition 
    if(iNumberOfSegmentForFinalPartition<1 || iNumberOfSegmentForFinalPartition>pPointset->npts)
    {          
    	printf("invalid iNumberOfSegmentForFinalPartition (%i) in view.mc, draw_segment_labels_partition()\n", iNumberOfSegmentForFinalPartition);
    	return;
    } 
    
    iBaseLevel = 50;    
    iLevel = iBaseLevel+iOffsetLevel;
    if(iLevel<iBaseLevel || iLevel>63)
    {          
    	printf("invalid iOffsetLevel (%i) in view.mc, draw_segment_labels_partition()\n", iOffsetLevel);
    	return;
    }
    
    imaxnumseg = 2*(pPointset->npts)-1 -iNumberOfSegmentForFinalPartition;
    for (idseg=0; idseg<pPointset->npts; idseg++)
    {
		Pnt[0].x = pPointset->px[idseg];
		Pnt[0].y = pPointset->py[idseg];
		Pnt[0].z = 0.0;
		MasterToUors_point(&Pnt[0]);
	    
	    idowner = GetOwnerParentSegmentThreshold( pSegmentSet, idseg, iNumberOfSegmentForFinalPartition );
	    idowner = imaxnumseg-idowner;	    
		sprintf(text, "%i", idowner);
		mdlText_create ( &TextElement,NULL,text,Pnt,&txSize,NULL,NULL,NULL);
		mdlElement_setSymbology ( &TextElement, &Color, NULL, NULL );
		mdlElement_setProperties (&TextElement, &iLevel, NULL, NULL, NULL,
					  NULL, NULL, NULL, NULL);
		mdlElement_add ( &TextElement );
		mdlElement_display ( &TextElement, NORMALDRAW );
    }	
	return;
}




//////////////////////////////////////////////  
//draw_segment_polygons_partition()
//
//display a partition, creating a polygon
//for each present segment (since microstation
//polygon are limited to 101 vertices, we 
//create many polylines instead).
//////////////////////////////////////////////
void draw_segment_polygons_partition(	POINTSET* pPointset, 
										SEGMENTSET* pSegmentSet,
										int iNumberOfSegmentForFinalPartition,
										int iOffsetLevel)
{                             
	MSElement LineElement;
    Dpoint3d pPointList[105];
    int Color, iBaseLevel, iLevel;
    
    Dpoint3d 	Pnt[10];
    MSElement	TextElement;
    char text[100];
    float sizex, sizey;
    TextSizeParam txSize;

    int i, j, idseg, idowner, idsegFinal, idsegStart, idsegEnd;    
    int ivertex;
    
    int* pidsegList;
    int* pivertexList;
    EDGE myEdge, StartEdge, myNextEdge;



    Color = 0; //0 is white in SVGA 16 colors mode 
    
    sizex = 1; // 0.2 
    sizey = 1.5; // 0.3 
    MasterToUors_distance(&sizex);
    MasterToUors_distance(&sizey);
    txSize.mode = TXT_BY_TILE_SIZE; // TXT_BY_TILE_SIZE | TXT_NO_TRANSFORM 
    txSize.size.width = sizex;
    txSize.size.height = sizey;    
    
    //0.1) validate iNumberOfSegmentForFinalPartition 
    if(iNumberOfSegmentForFinalPartition<1 || iNumberOfSegmentForFinalPartition>pPointset->npts)
    {          
    	printf("invalid iNumberOfSegmentForFinalPartition (%i) in view.mc, draw_segment_labels_partition()\n", iNumberOfSegmentForFinalPartition);
    	return;
    } 
    
    //0.2) validate iOffsetLevel 
    iBaseLevel = 30;    
    iLevel = iBaseLevel+iOffsetLevel;
    if(iLevel<iBaseLevel || iLevel>63)
    {          
    	printf("invalid iOffsetLevel (%i) in view.mc, draw_segment_polygons_partition()\n", iOffsetLevel);
    	return;
    }
    
     
    //1) collect the id of the segment present in the final partition  
    pidsegList = malloc(iNumberOfSegmentForFinalPartition*sizeof(int));
    pivertexList = malloc(iNumberOfSegmentForFinalPartition*sizeof(int));
    if(pidsegList==NULL || pivertexList==NULL)
    {
    	printf("problem allocating pidsegList or pivertexList in view.mc\n");
    	exit(0);
    } 
	GetRemainingSegmentListForFinalPartition( pSegmentSet, pidsegList, iNumberOfSegmentForFinalPartition );

    //2) get one vertex corresponding to each final segment 
    for(i=0; i<iNumberOfSegmentForFinalPartition; i++)
    {
		ivertex = GetOneVertexFromSegment(pSegmentSet, pidsegList[i]);
		pivertexList[i] = ivertex;
    }
    
    //3) for each final segment ... find one segment edge and start drawing polygone 
    for(i=0; i<iNumberOfSegmentForFinalPartition; i++)
    {   
    	//3.1) find one segment edge, an edge is a pair of vertices, each vertices part of a diff. segment 
    	GetOneSegmentEdge(	pPointset, 
							pSegmentSet, 
							pidsegList[i], 
							pivertexList[i], 
							iNumberOfSegmentForFinalPartition,
							&myEdge);  
    	if(myEdge.ivertexIn == -1 && myEdge.ivertexOut == -1)
    	{   
    		//development-time error 
    		printf("error, edge was not found for idseg %i in view.mc\n", pidsegList[i]); 
    		exit(0);
    	}
    	else if(myEdge.ivertexIn != -1 && myEdge.ivertexOut == -1)
    	{    
    		//case for bounding segment, should only append once 
    		printf("got bounding segment, skip drawing contour for this one\n");
    	}
    	else 
    	{
    		//3.2) start drawing in microstation, at every 100 edge points draw a polyline (ms limitation) 
    		StartEdge = myEdge;  //copy structure 
    		pPointList[0].x = pPointset->ctx[StartEdge.itriDualTriCCW1];
    		pPointList[0].y = pPointset->cty[StartEdge.itriDualTriCCW1];
			pPointList[0].z = 0.0;
			MasterToUors_point(&pPointList[0]);    		
    		pPointList[1].x = pPointset->ctx[StartEdge.itriDualTriCCW2];
    		pPointList[1].y = pPointset->cty[StartEdge.itriDualTriCCW2];
			pPointList[1].z = 0.0;
			MasterToUors_point(&pPointList[1]);    		
			GetNextSegmentEdge( pPointset, 
								pSegmentSet, 
								pidsegList[i], 
								&myEdge, 
								iNumberOfSegmentForFinalPartition,
								&myNextEdge);
    		j=2;    		
    		while(myNextEdge.itriDualTriCCW2!=StartEdge.itriDualTriCCW2)
    		{
	    		pPointList[j].x = pPointset->ctx[myNextEdge.itriDualTriCCW2];
	    		pPointList[j].y = pPointset->cty[myNextEdge.itriDualTriCCW2];
				pPointList[j].z = 0.0;
				MasterToUors_point(&pPointList[j]);    		
	    		if(j<100)
	    		{
		    		j++;                               
		    		myEdge = myNextEdge; //copy structure 
					GetNextSegmentEdge( pPointset, 
										pSegmentSet, 
										pidsegList[i], 
										&myEdge, 
										iNumberOfSegmentForFinalPartition,
										&myNextEdge);
				}
				else
				{
					// we got 100 points but segment is not fully surrounded yet 
					
					//3.2.1) draw the polyline in microstation 
					mdlLineString_create ( &LineElement, NULL, pPointList, j);
					mdlElement_setSymbology ( &LineElement, &Color, NULL, NULL );
					mdlElement_setProperties (&LineElement, &iLevel, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
					mdlElement_add ( &LineElement );
					mdlElement_display ( &LineElement, NORMALDRAW );
					
					
					//3.2.2) reset j index to zero and keep going 
					j=0;
				}							
    		}  
			//3.3) draw the polyline or the rest of the polyline in microstation 
			mdlLineString_create ( &LineElement, NULL, pPointList, j);
			mdlElement_setSymbology ( &LineElement, &Color, NULL, NULL );
			mdlElement_setProperties (&LineElement, &iLevel, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
			mdlElement_add ( &LineElement );
			mdlElement_display ( &LineElement, NORMALDRAW );
    		
    		// segment should be closed, let's draw a label for the segment 
    		
			//Pnt[0].x = pPointset->px[pidsegList[i]];
			//Pnt[0].y = pPointset->py[pidsegList[i]];
			//Pnt[0].z = 0.0;
			//MasterToUors_point(&Pnt[0]);
			//sprintf(text, "%i", i);
			//mdlText_create ( &TextElement, NULL, text, Pnt, &txSize, NULL, NULL, NULL);
			//mdlElement_setSymbology ( &TextElement, &Color, NULL, NULL );
			//mdlElement_setProperties (&TextElement, &iLevel, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
			//mdlElement_add ( &TextElement );
			//mdlElement_display ( &TextElement, NORMALDRAW ); 
			
    	}	
    	
	}
    // freeing the previously allocated list 
    if(pidsegList) free(pidsegList); 
    if(pivertexList) free(pivertexList);
    return;
}    
    
*/
