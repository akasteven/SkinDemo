#include "AABB.h"


AABB::~AABB()
{
}


void AABB::BuildFromVertices(Vertex::VertexBase * vertex, int numVer)
{
	Clear();
	if (numVer <= 0)
		return;

	for (int i = 0; i < numVer; i++)
	{
		Maxs = MathHelper::VectorMax(Maxs, vertex[i].Pos);
		Mins = MathHelper::VectorMin(Maxs, vertex[i].Pos);
	}
}

void AABB::ComputeCenterExt()
{
	Center = XMFLOAT3( ( Maxs.x + Mins.x ) * 0.5f, ( Maxs.y + Mins.y ) * 0.5f, ( Maxs.z + Mins.z ) * 0.5f) ;
	Extents = XMFLOAT3( Maxs.x - Mins.x, Maxs.y - Mins.y, Maxs.z - Mins.z );
}