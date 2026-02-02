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

#ifndef _PROFILE_H
#define _PROFILE_H


      
class OIFIILIB_API CProfileGraph : public CObject
{  
	DECLARE_DYNCREATE(CProfileGraph)
// Attributes
public:
	class COWGraphDoc* m_pProfileGraphDoc;    
private:
    class CProfileGraphView* m_pProfileGraphView; 

	BOOL m_pBOOL[10];
	int m_nBOOL;

// Implementation
public:
	CProfileGraph(); //should not be called                                                                 
	
   	CProfileGraph(	CRect rectProfileGraphView, 
				class COW2Doc* pOW2Doc, 
				CWnd* pParentWnd,
				int iPointType,
				int iPointCollectionType,
				int idROI,
				int idVertex,
				int iNumNeighbor,
				int idSegment,
				int iStatChannel);   

	~CProfileGraph();

	void CreateContext(	CRect rectProfileGraphView, 
						class COW2Doc* pOW2Doc, 
						CWnd* pParentWnd);

	BOOL Update(class COW2Doc* pDoc, 
				int iPointType,
				int iPointCollectionType,
				int idROI,
				int idVertex, 
				int iNumNeighbor, 
				int idSegment, 
				int iStatChannel); 

	BOOL MapIntoProfile(class COW2Doc* pDoc, 
						int iPointType,
						int iPointCollectionType,
						int idROI,
						int idVertex, 
						int iNumNeighbor, 
						int idSegment,
						int iStatChannel);
protected:  
};


#endif // _PROFILE_H
