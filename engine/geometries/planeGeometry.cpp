#include "planeGeometry.h"

namespace JB
{
	PlaneGeometry::PlaneGeometry()
	{
		std::vector<float> positions = 
		{
			25.0f, -0.5f,  25.0f,
		   -25.0f, -0.5f,  25.0f,
		   -25.0f, -0.5f, -25.0f,
			25.0f, -0.5f,  25.0f,
		   -25.0f, -0.5f, -25.0f,
			25.0f, -0.5f, -25.0f
		};

		std::vector<float> normals = 
		{
			0.0f, 1.0f, 0.0f,
			0.0f, 1.0f, 0.0f,
			0.0f, 1.0f, 0.0f,
			0.0f, 1.0f, 0.0f,
			0.0f, 1.0f, 0.0f,
			0.0f, 1.0f, 0.0f
		};

		std::vector<float> texCoords = 
		{
			25.0f,  0.0f,
			0.0f,   0.0f,
			0.0f,  25.0f,
			25.0f,  0.0f,
			0.0f,  25.0f,
			25.0f, 25.0f
		};

		setAttribute(AttributeType::Position, std::make_shared<AttributeF>(positions, 3));
		setAttribute(AttributeType::Normal, std::make_shared<AttributeF>(normals, 3));
		setAttribute(AttributeType::UV, std::make_shared<AttributeF>(texCoords, 2));
	}

	PlaneGeometry::~PlaneGeometry()
	{
	}
}
