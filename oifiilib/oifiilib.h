/*
 * Copyright (c) 1994-2022 Stephane Poirier
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

////////////////////////////////////////////////////////////////
//nakedsoftware.org, spi@oifii.org or stephane.poirier@oifii.org
//
//2014april06, oifiilib.h/.cpp creation for oifiilib.lib/.dll
//
//
//nakedsoftware.org, spi@oifii.org or stephane.poirier@oifii.org
////////////////////////////////////////////////////////////////

//2022jan04, spi, begin
#ifndef _OIFIILIB_H
#define _OIFIILIB_H
//2022jan04, spi, end


#ifdef OIFIILIB_EXPORTS
#define OIFIILIB_API __declspec(dllexport) 
#else
#define OIFIILIB_API __declspec(dllimport) 
#endif

#include <stdio.h>      // printf, scanf, NULL
#include <stdlib.h>     // malloc, free, rand 

#include <gl\gl.h>
#include <gl\glu.h>

#include "resource.h"

#include "c_debug.h"
#include "c_fileio.h"
#include "c_pointset.h" //offworld c pointset library
#include "c_segmentset.h" //offworld c segmentset library
#include "c_xyzi.h"

#include "OWpixelset.h" //offworld c++ pixelset library
#include "OWpointset.h" //offworld c++ pointset library (interface sitting on pointset.h)
#include "OWsegmentset.h" //offworld c++ segmentset library (interface sitting on segmentset.h)
#include "OWclassset.h" //offworld c++ classset library (interface sitting on segmentset.h)
#include "OWedgeset.h" //offworld edgeset library (for line, polyline, polygone  and shape)

#include "gl_owgraph.h"

#include "owgl_histo.h"
#include "owgl_histovw.h"
#include "owgl_profile.h"
#include "owgl_profilevw.h"
#include "owgl_scatter.h"
#include "owgl_scattervw.h"

#include "logdoc.h"
#include "logmdi.h"
#include "logview.h"

#include "owdoc.h"
#include "owview.h"
#include "owmultidoc.h"
#include "owhtmlview.h"

#include "ow2doc.h"
#include "ow2mdi.h"
#include "ow2view.h"
#include "ow3view.h"
#include "ow2agdoc.h"
#include "ow2agmdi.h"
#include "ow2agview.h"

#include "oifii_gotodlg.h"
#include "oifii_mainfrm.h"
#include "oifii_app.h"
#include "oifii_proppage.h"
#include "oifii_propsettings.h"
#include "oifii_propsheet.h"


//2022jan04, spi, begin
#endif //_OIFIILIB_H
//2022jan04, spi, end
