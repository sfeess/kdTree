#ifndef CLOSESTPOINTQUERY_H
#define CLOSESTPOINTQUERY_H

#include "Mesh.h"
#include "KDTree.h"

#include "OpenEXR/ImathVec.h"
#include <vector>


using std::vector;
using namespace Imath;


template<class T>
class ClosestPointQuery
{

public:

    ClosestPointQuery(const Mesh<T>& m) : m_tree(m.vertices)
	{
	}

    // Return the closest point on the mesh within the specified maximum search distance.
    unsigned int operator() (const Vec3<T>& queryPoint, double maxDist) const
	{
		unsigned int resultIndex = -1;
		m_tree.findClosest( 1, maxDist, resultIndex, queryPoint );
		return resultIndex;
	}

private:

	KDTree<Vec3<T>> m_tree;


};

typedef ClosestPointQuery<float> ClosestPointQueryV3f;
typedef ClosestPointQuery<double> ClosestPointQueryV3d;

template class ClosestPointQuery<float>;
template class ClosestPointQuery<double>;



#endif  //CLOSESTPOINTQUERY_H
