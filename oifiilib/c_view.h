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

////////////////////////////////////////////////////////////////////
// view.cpp   	this module contains pointset vizualization functions.  
//				these functions rely on WINGDI to render graphics
//
//
//	     OFFWORLD Research, Stephane Poirier
//	     675 Des Rocailles,
//	     Quebec, Que., G2J 1A9
//	     Canada
//	     Tel: (418) 627-5042
//	     Fax: (418) 622-6018
////////////////////////////////////////////////////////////////////


#ifndef _VIEW_H
#define _VIEW_H

/* //spi 2014, because c_view.cpp is allready all commented out
// Public functions called to draw using WINGDI
void OIFIILIB_API draw_triangle_from_file( void ); 

void OIFIILIB_API draw_triangle_from_memory(	struct tagPOINTSET* pPointset,
								struct tagSEGMENTSET* pSegmentSet,
								int iStatIndex,
								int iOffsetLevel); 

void OIFIILIB_API draw_triangle_centre_from_memory(struct tagPOINTSET* pPointset);
void OIFIILIB_API draw_triangle_labels_from_memory(struct tagPOINTSET* pPointset);
void OIFIILIB_API draw_point_labels_from_memory(struct tagPOINTSET* pPointset);  
void OIFIILIB_API draw_point_from_memory ( struct tagPOINTSET* pPointset);
void OIFIILIB_API draw_point_from_file ( char* filename );
void OIFIILIB_API draw_voronoi_regions_from_memory(struct tagPOINTSET* pPointset);


 
// -these 2 following function could be in segmentset library, which is on top
// of pointset library.
void OIFIILIB_API draw_segment_labels_partition(	struct tagPOINTSET* pPointset, 
									struct tagSEGMENTSET* pSegmentSet,
									int iNumberOfSegmentForFinalPartition,
									int iOffsetLevel);

void OIFIILIB_API draw_segment_polygons_partition(	struct tagPOINTSET* pPointset, 
										struct tagSEGMENTSET* pSegmentSet,
										int iNumberOfSegmentForFinalPartition,
										int iOffsetLevel);
*/

///////////////////////////////////////////////
// Public functions used to convert coordinates
///////////////////////////////////////////////
//void MasterToUors_point ( Dpoint3d* Pnt );
//void MasterToUors_distance ( double* Double );




#endif /* _VIEW_H */
