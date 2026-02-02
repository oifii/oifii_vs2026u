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
// fileio.h
//
//
//
// OFFWORLD Research
// 675 Des Rocailles, Quebec, G2J 1A9
// by Stephane Poirier
//
*/

/*
// public function declarations
*/
#ifndef _FILEIO_H
#define _FILEIO_H

struct OIFIILIB_API tagSEGMENTSET* OpenSegmentSet(char* pszFileName);
void OIFIILIB_API SaveSegmentSet(struct tagSEGMENTSET* pSegmentSet, char* pszFileName);


struct OIFIILIB_API tagPOINTSET* OpenPointset(char* pszFileName);
void OIFIILIB_API SavePointset(struct tagPOINTSET* pPointset, char* pszFileName);

#endif //_FILEIO_H