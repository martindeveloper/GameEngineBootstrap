#include "ObjModel.h"

using namespace Graphic::Model;

ObjModel::ObjModel(FileSystem::File* sourceFile)
{
	Source = sourceFile;
}

ObjModel::ObjModel(FileSystem::File* sourceFile, Graphic::ColorRGBA diffuseColor) : ObjModel::ObjModel(sourceFile)
{
	DiffuseColor = diffuseColor;
}

bool ObjModel::Load()
{
	if (Source->IsExists())
	{
		Source->Load();

		std::vector<char>* dataVector = Source->GetBinaryContentPointer();

		uint32 bytesRead = 0;
		uint32 bytesMax = dataVector->size();

		char lineBuffer[128];
		uint32 index = 0;
		char charOnIndex = NULL;

		while (bytesRead != bytesMax)
		{
			charOnIndex = dataVector->at(bytesRead);
			lineBuffer[index] = charOnIndex;

			if (charOnIndex == '\n')
			{
				// Parse line
				ParseLine(lineBuffer);

				// We are at the end of line
				index = 0;
				bytesRead++;

				continue;
			}

			index++;
			bytesRead++;

			if (bytesRead == bytesMax)
			{
				// We read all bytes, but we didnt found ending new line char
				ParseLine(lineBuffer);
			}
		}

		// Run indexing
		BuildVerticies();

		// Free temp and incies buffers
		FreeBuffers();

		// Free file
		Source->Free();

		return true;
	}

	return false;
}

void ObjModel::Free()
{
	FreeBuffers();

	Vertices.clear();
}

void ObjModel::ParseLine(const char* line)
{
	// v float float float = vertex
	// vt float float = UV
	// vn float float float = normal
	// f int/int/int int/int/int int/int/int
		
	if (line[0] == 'v' && line[1] == 't') {
#ifdef VERTEX_HAVE_UV
		UVCoordinates uv;
		sscanf_s(line, "vt %f %f\n", &uv.U, &uv.V);

		UVs.push_back(uv);
#endif
	}
	else if (line[0] == 'v' && line[1] == 'n') {
#ifdef VERTEX_HAVE_NORMAL
		Math::Vector3<float> normal;
		sscanf_s(line, "vn %f %f %f\n", &normal.X, &normal.Y, &normal.Y);

		Normals.push_back(normal);
#endif
	}
	else if (line[0] == 'v') {
		Math::Vector3<float> position;
		sscanf_s(line, "v %f %f %f\n", &position.X, &position.Y, &position.Z);

		Positions.push_back(position);
	}
	else if (line[0] == 'f') {
		uint32 vertexIndex[3], uvIndex[3], normalIndex[3];

		int matches = sscanf_s(line, "f %d/%d/%d %d/%d/%d %d/%d/%d\n", &vertexIndex[0], &uvIndex[0], &normalIndex[0], &vertexIndex[1], &uvIndex[1], &normalIndex[1], &vertexIndex[2], &uvIndex[2], &normalIndex[2]);

		const uint16 maxMatches = 9;

		if (matches != maxMatches)
		{
			throw new std::exception("Can not parse indicies lines.");
		}

		PositionIndices.push_back(vertexIndex[0]);
		PositionIndices.push_back(vertexIndex[1]);
		PositionIndices.push_back(vertexIndex[2]);

		UvIndices.push_back(uvIndex[0]);
		UvIndices.push_back(uvIndex[1]);
		UvIndices.push_back(uvIndex[2]);

		NormalIndices.push_back(normalIndex[0]);
		NormalIndices.push_back(normalIndex[1]);
		NormalIndices.push_back(normalIndex[2]);
	}
	else {
		OutputDebugString("\nObjModel: Line can not be parsed.\n");
		OutputDebugString(line);
		OutputDebugString("\n");
	}
}

void ObjModel::BuildVerticies()
{
	for (uint32 i = 0; i < PositionIndices.size(); i++) {

		// Get the indices of its attributes
		uint32 vertexIndex = PositionIndices[i];
		uint32 uvIndex = UvIndices[i];
#ifdef VERTEX_HAVE_NORMAL
		uint32 normalIndex = NormalIndices[i];
#endif

		// Get the attributes thanks to the index
		Math::Vector3<float> position = Positions[vertexIndex - 1];
		UVCoordinates uv = UVs[uvIndex - 1];
#ifdef VERTEX_HAVE_NORMAL
		Math::Vector3<float> normal = Normals[normalIndex - 1];
#endif

		// Put the attributes in buffers
		Vertex newVertex;

		newVertex.Position = position;
#ifdef VERTEX_HAVE_UV
		newVertex.UV = uv;
#endif

		newVertex.Color = DiffuseColor;

#ifdef VERTEX_HAVE_NORMAL
		newVertex.Normal = normal;
#endif

		Vertices.push_back(newVertex);
	}
}

void ObjModel::FreeBuffers()
{
	Positions.clear();
	UVs.clear();
	Normals.clear();

	PositionIndices.clear();
	UvIndices.clear();
	NormalIndices.clear();
}

ObjModel::~ObjModel()
{
	Free();
}
