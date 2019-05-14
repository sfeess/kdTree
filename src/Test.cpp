#include "KDTree.h"
#include "Mesh.h"
#include "ClosestPointQuery.h"

#include "OpenEXR/ImathVec.h"

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <algorithm>
#include <vector>
#include <ctime>


using namespace std;
using namespace Imath;



std::vector<V3d> getRandomPoints(int numElements, int seed)
{
	srand( seed );
	std::vector<V3d> points(numElements);
	for (int i = 0; i < numElements; ++i) {
		float x = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
		float y = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
		float z = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
		points[i] = V3d(x, y, z);
	}
	return points;
}


// linear search to compare against
int naiveNeighbor( const std::vector<V3d> &points, V3d &searchPoint )
{
	double minDistance = MAXFLOAT;
	unsigned int neighborIndex = -1;

	for ( unsigned int i=0; i<points.size(); ++i )
	{
		double distance = ( points[i] - searchPoint ).length();
		if ( distance < minDistance )
		{
			minDistance = distance;
			neighborIndex = i;
		}
	}
	return neighborIndex;
}


// test the KD Tree
int main()
{
	Mesh<double> m;
	m.setRandomVerts( 200000, 1 );

	clock_t begin = clock();
	ClosestPointQueryV3d findClosestPoint( m );
	double tConstruction = double(clock() - begin) / CLOCKS_PER_SEC;

	cout << "Construction time: " <<  tConstruction << endl;

	std::vector<V3d> queryPoints = getRandomPoints(5, 5);

	for ( auto &queryPoint : queryPoints )
	{
		double resultDistance = FLT_MAX;

		begin = clock();
		int resultIndex = findClosestPoint( queryPoint, resultDistance );
		double tKdSearch = double(clock() - begin) / CLOCKS_PER_SEC;

		begin = clock();
		unsigned int naiveResultIndex = naiveNeighbor( m.vertices, queryPoint );
		double tNaiveSearch = double(clock() - begin) / CLOCKS_PER_SEC;

		cout << "\nquery:         \t" << queryPoint << endl;
		cout << "KDTree result: \t" << m.vertices[resultIndex] << "\t time: " << tKdSearch << endl;
		cout << "naive result:  \t" << m.vertices[naiveResultIndex] << "\t time: " << tNaiveSearch << endl;

		assert(resultIndex == naiveResultIndex);
	}
	return 0;
}

