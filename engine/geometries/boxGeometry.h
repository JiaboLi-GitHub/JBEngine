#pragma once 
#include "geometry.h"

namespace JB {

	class BoxGeometry :public Geometry {
	public:
		static constexpr uint32_t U = 0;
		static constexpr uint32_t V = 1;
		static constexpr uint32_t W = 2;

		using PositionArray = std::vector<float>;
		using NormalArray = std::vector<float>;
		using UVArray = std::vector<float>;
		using IndexArray = std::vector<uint32_t>;

		using Ptr = std::shared_ptr<BoxGeometry>;
		static Ptr create(float width, float height, float depth, uint32_t widthSegment = 1, uint32_t heightSegment = 1, uint32_t depthSegment = 1) {
			return std::make_shared<BoxGeometry>(width, height, depth, widthSegment, heightSegment, depthSegment);
		}

		BoxGeometry(float width, float height, float depth, uint32_t widthSegment = 1, uint32_t heightSegment = 1, uint32_t depthSegment = 1);

		~BoxGeometry();

	private:
		void buildPlane(
			uint32_t u, uint32_t v, uint32_t w,
			int16_t udir, int16_t vdir,
			float width, float height, float depth,
			uint32_t gradX, uint32_t gradY,
			uint32_t& numberOfVertices,
			PositionArray& positions,
			NormalArray& normals,
			UVArray& uvs,
			IndexArray& indices
		);

	private:
		float m_width{ 0.0f };
		float m_height{ 0.0f };
		float m_depth{ 0.0f };
		uint32_t m_widthSegments{ 0 };
		uint32_t m_heightSegments{ 0 };
		uint32_t m_depthSegments{ 0 };
	};
}
