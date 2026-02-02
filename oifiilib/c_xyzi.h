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

////////////////////////////////////////////////////////////////////////////
// xyzi.h/.cpp module definition
//
// xyzi.h/.cpp is part of the offworld vectorial file format library
//
// xyzi.h/.cpp module provides i/o to export and import list of points
// to and from the file system (ANSI C - LAN)
////////////////////////////////////////////////////////////////////////////
//
//		 By Stephane Poirier, for CRG Laval University.
//		 Original C++ release (MFC700 CFile dependent)
//		 Feb 2nd, 1993
//
//		 The XYZI file module is a part of the Object-Oriented
//		 Image Library.  It contains all functions required to
//		 load, save, alloc and dealloc vector-based
//		 text file format
//
//		 The XYZI vector-based txt file format has been defined
//		 by Poirier to make easy the communication between
//		 different existing software and functions in the CRG,
//		 Geomatic Research Center at Laval University.	Second
//		 goal was to have a vector file format that enable
//		 back and forth conversions with raster image file format.
//
//
//		 The XYZI module includes:
//
//		     ooildef.h	(object-oriented image library definitions)
//		     xyzi.h	(ooil xyzi file format prototypes)
//		     xyzi.cpp	(ooil xyzi file format methods)
//		     xyzif.h	(ooil xyzi filters and functions prototypes)
//		     xyzif.cpp	(ooil xyzi filters and functions methods)
//		     xyziw.h	(ooil xyzi window prototypes)
//		     xyziw.cpp	(ooil xyzi window methods)
//
//
//		 The OOILDEF.H is used to caracterize the whole OOIL.LIB
//		 configuration, mostly to include or exclude different
//		 parts of the source code.
//
//		 The XYZI.H and XYZI.CPP contain low-level functions, that
//		 is the dynamic and permanent storage function.
//
//		 The XYZIF.H and XYZIF.CPP contain filters and functions
//		 used to XYZI file processing.
//
//		 The XYZIW.H and XYZIW.CPP contain function that enable
//		 XYZI file visualisation (PAINT message), as well as
//		 the message map used to detect user request and actions.
////////////////////////////////////////////////////////////////////////////

#ifndef _XYZI_H
#define _XYZI_H




typedef struct OIFIILIB_API XYZIHeader
{
     long nombre_objet;       // 100 si 101 lignes dans fichier 
     long nombre_id;	      // 1 si un seul id pour object 
     long nombre_coor;	      // 3 si position decrite par x,y,z 
     long nombre_stat;	      // 1 si seulement voronoi area 
     long liste_voisin_flag;  // 1 si liste voisin presente, autrement 0 
     long liste_centre_flag;  // 1 si liste centre presente, autrement 0 
} XYZIHEADER;


#define XYZI_MAXID			10
#define XYZI_MAXCOOR	    10
#define XYZI_MAXSTAT	    10
#define XYZI_MAXNEIGHBORS   100
#define XYZI_MAXCENTERS	    100


typedef struct OIFIILIB_API XYZILine
{
     long  id[XYZI_MAXID];	// limite de 10 identifiants par segment 
     double coor[XYZI_MAXCOOR];	// limite de 10 coordonnees (x,y,z,t,u,v, ...) 
     double stat[XYZI_MAXSTAT];	// limite de 10 statistiques par segment 
     long  numberofneighbors;
     long  neighborsid[XYZI_MAXNEIGHBORS];	// limite de 100 voisins par segment 
     long  centersid[XYZI_MAXCENTERS];	// limite de 100 voisins par segment 
} XYZILINE;


// if not compiling the lib, declare as externals 
#if !defined(XYZI_COMPILING_XYZI_LIB)
    extern XYZIHEADER xyzi_header;
    extern XYZILINE xyzi_line;
#endif



int OIFIILIB_API xyzi_open_and_readheader(LPCSTR pszFilename);
int OIFIILIB_API xyzi_open_and_writeheader(LPCSTR pszFilename);
int OIFIILIB_API xyzi_readline(void);
int OIFIILIB_API xyzi_writeline(void);
int OIFIILIB_API xyzi_close(void);


#endif //_XYZI_H




/*
////////////////////////////////////////////////////////////////////////////
// XYZI.H	 Object-Oriented Image Library, XYZI file format class
//		 definition.
//
//		 By Stephane Poirier, for CRG Laval University.
//		 Original C++ release (MFC700 CFile dependent)
//		 Feb 2nd, 1993
//
//		 The XYZI file module is a part of the Object-Oriented
//		 Image Library.  It contains all functions required to
//		 load, save, alloc and dealloc vector-based
//		 text file format
//
//		 The XYZI vector-based txt file format has been defined
//		 by Poirier to make easy the communication between
//		 different existing software and functions in the CRG,
//		 Geomatic Research Center at Laval University.	Second
//		 goal was to have a vector file format that enable
//		 back and forth conversions with raster image file format.
//
//
//		 The XYZI module includes:
//
//		     ooildef.h	(object-oriented image library definitions)
//		     xyzi.h	(ooil xyzi file format prototypes)
//		     xyzi.cpp	(ooil xyzi file format methods)
//		     xyzif.h	(ooil xyzi filters and functions prototypes)
//		     xyzif.cpp	(ooil xyzi filters and functions methods)
//		     xyziw.h	(ooil xyzi window prototypes)
//		     xyziw.cpp	(ooil xyzi window methods)
//
//
//		 The OOILDEF.H is used to caracterize the whole OOIL.LIB
//		 configuration, mostly to include or exclude different
//		 parts of the source code.
//
//		 The XYZI.H and XYZI.CPP contain low-level functions, that
//		 is the dynamic and permanent storage function.
//
//		 The XYZIF.H and XYZIF.CPP contain filters and functions
//		 used to XYZI file processing.
//
//		 The XYZIW.H and XYZIW.CPP contain function that enable
//		 XYZI file visualisation (PAINT message), as well as
//		 the message map used to detect user request and actions.
////////////////////////////////////////////////////////////////////////////
#ifndef _XYZILIST_H
#define _XYZILIST_H

struct XYZIHeader
{
	long nombre_objet;	     		//100 si 101 lignes dans fichier (1 line for header)
	long nombre_id;	     			//1 si un seul id pour object
	long nombre_coor;	     		//3 si position decrite par x,y,z
	long nombre_stat;	     		//1 si seulement voronoi area
	long liste_voisin_flag;  		//1 si liste voisin presente, autrement 0
	long liste_centretriangle_flag; //1 si liste centre triangle presente, autrement 0
};

struct XYZILineofData
{
	long*	p_id;			//long  id[10];
	float*	p_coor; 		//float coor[10];
	float*  p_stat;			//float stat[50];
	long 	nombre_voisin; 	//long  nombre_voisin;
	long  	p_voisin; 		//voisin[200];
	long 	p_centre; 		//long  centre[200];
};


class CXyzIFile : public CStdioFile
{
  public:
  //Header definition

  //Pointer declaration
  XYZIHeader*      p_header;
  class CXYZIWnd*  m_parent;
  XYZILineofData*  p_lineofdata;

  CFileException exception;

  //Other variable declaration
  char p_FileName[200];
  long i_headerread;
  long i_dataread;
  char linebuffer[300], temp[50];
  char trash1[50], trash2[50], trash3[50], trash4[50], trash5[50], trash6[50];
  char temp1[50], temp2[50], temp3[50], temp4[50], temp5[50], temp6[50];

  //Constructors
 //COWXYZIList(class CXYZIWnd* parent);
 //COWXYZIList(const char* pszFileName, UINT nOpenFlags, class CXYZIWnd* parent);
 COWXYZIList();	//create empty list

  //Destructors
 ~COWXYZIList();


//Required for filtering on xyzi data
//COWXYZIList::RemoveOverlappingObjects();


};

#endif //_XYZILIST_H
*/