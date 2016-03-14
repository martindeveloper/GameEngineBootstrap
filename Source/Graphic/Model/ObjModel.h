#pragma once

#include "../../Common.h"
#include "../../Math/Vector3.h"
#include "../UVCoordinates.h"
#include "../Vertex.h"
#include "../../FileSystem/File.h"

namespace Graphic
{
	namespace Model
	{
		class ObjModel
		{
		public:
			std::vector<Vertex> Vertices;

			ObjModel(FileSystem::File* sourceFile);
			ObjModel(FileSystem::File* sourceFile, Graphic::ColorRGBA diffuseColor);

			virtual ~ObjModel();

			bool Load();
			void Free();
			void FreeBuffers();

		private:
			Graphic::ColorRGBA DiffuseColor = Graphic::ColorRGBA::White();
			FileSystem::File* Source;

			std::vector<uint32> PositionIndices;
			std::vector<uint32> UvIndices;
			std::vector<uint32> NormalIndices;

			std::vector<Math::Vector3<float>> Positions;
			std::vector<Graphic::UVCoordinates> UVs;
			std::vector<Math::Vector3<float>> Normals;

			void ParseLine(const char* line);
			void BuildVerticies();
		};
	};
};