#include "KDTree.h"

#include "OpenEXR/ImathVec.h"
#include <algorithm>
#include <stdio.h>
#include <numeric>

using namespace std;


template<class ElementType>
KDTree<ElementType>::KDTree( const vector<ElementType> &points ) : m_points(points)
{
	m_nodes.resize( points.size() );

	m_indices = vector<unsigned int>( points.size() );
	for (unsigned int i=0;i<points.size(); i++)
	{
		m_indices[i] = i;
	}

	recursiveBuild( 1, m_indices.begin(), m_indices.end() );
}


template<class ElementType>
void KDTree<ElementType>::recursiveBuild( const unsigned int nodeIndex, IndexIter itFirst, IndexIter itLast )
{
	unsigned int axis = getDepth( nodeIndex ) % k;

	if ( nodeIndex >= m_nodes.size() )
	{
		m_nodes.resize( nodeIndex + 1 );
	}

	Node<ElementType> &n = m_nodes[nodeIndex];

	if ( ( itLast - itFirst ) == 1 )
	{
		n.pointIndex = *itFirst;
		n.isLeaf = true;
	}

	// make a branch if we have more than two elements
	else
	{
		// sort the indices based on the point values
		// get median and move smaller entries to the left and greater to the
		// right (not sorted within the split sets)
		IndexIter itMid = itFirst + ( itLast - itFirst ) / 2;
		nth_element( itFirst, itMid, itLast, CompareAxis( m_points, axis ) );

		n.pointIndex = *itMid;

		if ( (itMid - itFirst) > 0 )
		{
			recursiveBuild( childIndexLeft( nodeIndex ), itFirst, itMid );
		}

		++itMid;

		if ( (itLast - itMid ) > 0 )
		{
			recursiveBuild( childIndexRight( nodeIndex ), itMid, itLast );
		}
	}
}


template<class ElementType>
void KDTree<ElementType>::findClosest( const unsigned int nodeIndex, double &closestDistance, unsigned int &closestPointIndex, const ElementType &query ) const
{
	if ( nodeIndex >= m_nodes.size() || m_nodes[nodeIndex].pointIndex == -1)
	{
		return;
	}

	const Node<ElementType> &n = m_nodes[nodeIndex];

	// if node is a branch compare to point at pointIndex
	if ( n.pointIndex != -1 )
	{
		double d = squareDistance( query, m_points[n.pointIndex] );
		if ( d < closestDistance )
		{
			closestDistance = d;
			closestPointIndex = n.pointIndex;
		}

		if ( !n.isLeaf )
		{
			unsigned int axis = getDepth( nodeIndex ) % k;
			double dist = query[axis] - m_points[n.pointIndex][axis];
			bool leftChildIsCloser = dist <= 0;
			unsigned int childIndexNear = leftChildIsCloser ? childIndexLeft( nodeIndex ) : childIndexRight( nodeIndex );
			unsigned int childIndexFar = leftChildIsCloser ? childIndexRight( nodeIndex ) : childIndexLeft( nodeIndex );

			findClosest( childIndexNear, closestDistance, closestPointIndex, query );

			if ( pow( dist, 2 ) < closestDistance )
			{
				findClosest( childIndexFar, closestDistance, closestPointIndex, query );
			}
		}
	}
}


template class KDTree<Imath::V3f>;
template class KDTree<Imath::V3d>;
