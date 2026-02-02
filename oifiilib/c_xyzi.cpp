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

//////////////////////////////////////////////////////////////////////
// xyzi.mc
//
// xyzi file-format support module
//
// Code limitations:  only one .xyi file can be opened at a time.
////////////////////////////////////////////////////////////////////////



//
// XYZI File Format support
//
// This XYZI file format has been developped to provide a common vectorial
// file format between multi-platform applications.  It is a text format.
//




// Prototypes de fonctions MStation


#include "stdafx.h"

#include <stdio.h>
#include <stdlib.h>

//because of TRUE and FALSE
//#include <wtypes.h>


//this macro is used inside xyzi.h header 
#define XYZI_COMPILING_XYZI_LIB
//#include "c_xyzi.h"
#include "oifiilib.h"
#undef XYZI_COMPILING_XYZI_LIB

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//
// Module global
//
XYZIHEADER xyzi_header;
XYZILINE xyzi_line;

//
// Module local
//
char trash[256], temp[256];
char pszErrormsg[256];
FILE* pFile;




int xyzi_open_and_readheader(LPCSTR pszFilename)
{
    //
    //open the xyzi vector file
    //
    pFile = NULL;
    pFile = fopen(pszFilename, "r");
    if(pFile == NULL)
    {
		sprintf(pszErrormsg, "Cannot open file %s\n", pszFilename);
		//mdlDialog_openAlert(pszErrormsg);
		printf("%s\n", pszErrormsg);
		return FALSE;
    }
    //
    //read the xyzi header, that is the first line
    //
    fscanf(pFile, "%s", trash);
    fscanf(pFile, "%s", temp);
    xyzi_header.nombre_objet = atol(temp);
    fscanf(pFile, "%s", trash);
    fscanf(pFile, "%s", temp);
    xyzi_header.nombre_id = atol(temp);
    fscanf(pFile, "%s", trash);
    fscanf(pFile, "%s", temp);
    xyzi_header.nombre_coor = atol(temp);
    fscanf(pFile, "%s", trash);
    fscanf(pFile, "%s", temp);
    xyzi_header.nombre_stat = atol(temp);
    fscanf(pFile, "%s", trash);
    fscanf(pFile, "%s", temp);
    xyzi_header.liste_voisin_flag = atol(temp);
    fscanf(pFile, "%s", trash);
    fscanf(pFile, "%s", temp);
    xyzi_header.liste_centre_flag = atol(temp);
    //
    // verify header consistency, use
    // liste_centre_flag and liste_voisin_flag to do so.
    //
    if( (xyzi_header.liste_voisin_flag != 0 &&
	 xyzi_header.liste_voisin_flag != 1 )
	|| (xyzi_header.liste_centre_flag != 0 &&
	 xyzi_header.liste_centre_flag != 1 ) )
    {
		sprintf(pszErrormsg, "Inconsistency in header of file %s\n", pszFilename);
		//mdlDialog_openAlert(pszErrormsg);
		printf("%s\n", pszErrormsg);
		return FALSE;
    }
    //
    // validate header variables: number of ids, coor, etc..
    //
    if( xyzi_header.nombre_id < 0 ||
	xyzi_header.nombre_id >= XYZI_MAXID ||
	xyzi_header.nombre_coor < 0 ||
	xyzi_header.nombre_coor >= XYZI_MAXCOOR ||
	xyzi_header.nombre_stat < 0 ||
	xyzi_header.nombre_stat >= XYZI_MAXSTAT
      )
    {
		sprintf(pszErrormsg, "Variable out of range in header of file %s\n", pszFilename);
		//mdlDialog_openAlert(pszErrormsg);
		printf("%s\n", pszErrormsg);
		return FALSE;
    }
    return TRUE;
}

int xyzi_open_and_writeheader(LPCSTR pszFilename)
{
    //
    //open the xyzi vector file
    //
    pFile = NULL;
    pFile = fopen(pszFilename, "w");
    if(pFile == NULL)
    {
		sprintf(pszErrormsg, "Cannot create file %s\n", pszFilename);
		//mdlDialog_openAlert(pszErrormsg);
		printf("%s\n", pszErrormsg);
		return FALSE;
    }
    //
    //write the xyzi header, that is the first line
    //
    fprintf(pFile, "N_objet:%9d  N_id:%6d  N_coor:%6d  N_stat:%6d  N_voisin_flag:%6d  N_centre_flag:%6d\n", xyzi_header.nombre_objet, xyzi_header.nombre_id, xyzi_header.nombre_coor, xyzi_header.nombre_stat, xyzi_header.liste_voisin_flag, xyzi_header.liste_centre_flag);
    return TRUE;
}


//
//to read a line from xyzi vectorfile
//
int xyzi_readline()
{
    long i;
    //
    //now, according to the values in the header, this function
    //reads one line of data from the xyzi file.
    //
    for(i=0; i<xyzi_header.nombre_id; i++)
    {
		fscanf(pFile, "%s", temp);
		xyzi_line.id[i] = atol(temp);
    }
    for(i=0; i<xyzi_header.nombre_coor; i++)
    {
		fscanf(pFile, "%s", temp);
		xyzi_line.coor[i] = atof(temp);
    }
    for(i=0; i<xyzi_header.nombre_stat; i++)
    {
		fscanf(pFile, "%s", temp);
		xyzi_line.stat[i] = atof(temp);
    }
    if(xyzi_header.liste_voisin_flag == 1)
    {
       fscanf(pFile, "%s", temp);
       xyzi_line.numberofneighbors = atol(temp);
       for(i=0; i<xyzi_line.numberofneighbors; i++)
       {
	  fscanf(pFile, "%s", temp);
	  xyzi_line.neighborsid[i] = atol(temp);
       }
    }
    else if(xyzi_header.liste_centre_flag == 1)
    {
       fscanf(pFile, "%s", temp);
       xyzi_line.numberofneighbors = atol(temp);
       for(i=0; i<xyzi_line.numberofneighbors; i++)
       {
	  fscanf(pFile, "%s", temp);
	  xyzi_line.centersid[i] = atol(temp);
       }
    }
    //
    //si liste voisin presente et liste centre presente
    //
    if(xyzi_header.liste_voisin_flag == 1 &&
       xyzi_header.liste_centre_flag == 1)
    {
       for(i=0; i<xyzi_line.numberofneighbors; i++)
       {
	  fscanf(pFile, "%s", temp);
	  xyzi_line.centersid[i] = atol(temp);
       }
    }
    return TRUE;
}



//
//to write a line to xyzi vectorfile
//
int xyzi_writeline()
{
    long i;
    //
    //now, according to the values in the header, this function
    //writes one line of data to the xyzi file.
    //
    for(i=0; i<xyzi_header.nombre_id; i++)
    {
		fprintf(pFile, "%7d ", xyzi_line.id[i]);
    }
    for(i=0; i<xyzi_header.nombre_coor; i++)
    {
		fprintf(pFile, "%12.4f ", xyzi_line.coor[i]);
    }
    for(i=0; i<xyzi_header.nombre_stat; i++)
    {
		fprintf(pFile, "%12.4f ", xyzi_line.stat[i]);
    }
    if(xyzi_header.liste_voisin_flag == 1)
    {
		fprintf(pFile, "%7d ", xyzi_line.numberofneighbors);
		for(i=0; i<xyzi_line.numberofneighbors; i++)
		{
			fprintf(pFile, "%7d ", xyzi_line.neighborsid[i]);
		}
    }
    else if(xyzi_header.liste_centre_flag == 1)
    {
		fprintf(pFile, "%7d ", xyzi_line.numberofneighbors);
		for(i=0; i<xyzi_line.numberofneighbors; i++)
		{
			fprintf(pFile, "%7d ", xyzi_line.centersid[i]);
		}
    }
    //
    //si liste voisin presente et liste centre presente
    //
    if(xyzi_header.liste_voisin_flag == 1 &&
       xyzi_header.liste_centre_flag == 1)
    {
		for(i=0; i<xyzi_line.numberofneighbors; i++)
		{
			fprintf(pFile, "%7d", xyzi_line.centersid[i]);
		}
    }
	fprintf(pFile, "\n");
    return TRUE;
}



//
//close the xyzi vector file, this function must be used because the xyzi
//file handle is local to this module.
//
int xyzi_close()
{
    fclose(pFile);
    return TRUE;
}
