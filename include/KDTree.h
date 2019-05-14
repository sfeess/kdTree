#ifndef KDTREE_H
#define KDTREE_H

#include "OpenEXR/ImathVec.h"
#include <vector>

using std::vector;



template<class ElementType>
class Node
{

public:
	Node() : pointIndex(-1), isLeaf(false)
	{
	}

	bool isLeaf;
	unsigned int pointIndex;
};


template<class ElementType>
class KDTree
{

public:

	typedef typename vector<unsigned int>::iterator IndexIter;


	KDTree( const vector<ElementType> &points );

	// dimension of the tree
	const unsigned int k = 3;


	// construction of the tree starting at nodeIndex
	void recursiveBuild( const unsigned int nodeIndex, IndexIter itFirst, IndexIter itLast );

	// recursive nearest neighbor search starting from index i and maintaining
	// closest point and the squaredDistance to it
	void findClosest( const unsigned int nodeIndex, double &closestDistance, unsigned int &closestPointIndex, const ElementType &query ) const;

	// returns the depth for a node based on its index
	unsigned int getDepth(const unsigned int &nodeIndex) const
	{
		return (unsigned int)( log(nodeIndex) / log(2) );
	};

	// returns the index of the left child
	unsigned int childIndexLeft(const unsigned int &nodeIndex) const
	{
		return nodeIndex * 2;
	};

	// returns the index of the right child
	unsigned int childIndexRight(const unsigned int &nodeIndex) const
	{
		return nodeIndex * 2 + 1;
	};

	// returns the squared eucledian distance between a and b
	double squareDistance(const ElementType &a, const ElementType &b) const
	{
		return pow( a[0]-b[0], 2 ) + pow( a[1]-b[1], 2 ) + pow( a[2]-b[2], 2 );
	};


private:

	// the indices to be sorted, pointing to the points vector
	vector<unsigned int> m_indices;

	// storage of the nodes for the tree. we assume a balanced tree and linearize
	// the tree nodes to this vector with childIndexLeft() and childIndexLeft()
	vector<Node<ElementType>> m_nodes;

	// points to store as tree nodes
	const vector<ElementType> &m_points;

	// Compares two indices on a given axis based on their corresponding points
	struct CompareAxis
	{
		const vector<ElementType> &m_points;
		unsigned int m_axis;

		CompareAxis(const vector<ElementType> &p, unsigned int axis ) : m_points(p), m_axis(axis)
		{
		}
		bool operator()(unsigned int i1, unsigned int i2) const
		{
			return m_points[i1][m_axis] < m_points[i2][m_axis];
		}
	};

};


typedef KDTree<Imath::V3f> KDTreeV3f;
typedef KDTree<Imath::V3d> KDTreeV3d;


#endif  //KDTREE_H
