#include "objLoader.hpp"
#include <fstream>
#include <vector>
#include <map>
using namespace std;

size_t order;

class Vertex
{
public:
	float data[8];
	Vertex(float x, float y, float z, float u, float v, float nx, float ny, float nz) : data{ x,y,z,u,v,nx,ny,nz } {}
	bool operator==(Vertex const& v) const { for (size_t i = 0; i < 8; i++) if (data[i] != v.data[i]) return false; return true; }
	bool operator<(Vertex const& v) const { for (size_t i = 0; i < 8; i++) if (data[i] > v.data[i]) return false; else if (data[i] < v.data[i]) return true; return false; }
};

class ID
{
public:
	size_t value = 0;
};

void process(vector<float>& vertexCoords, vector<float>& vertexUVs, vector<float>& vertexNormals, vector<Vertex>& vertices, vector<size_t>& indexGroups, vector<size_t>& indices)
{
	order = 0;
	map<Vertex, ID> vertexMap;
	for (size_t i = 0; i < indexGroups.size(); i += 9)
	{
		Vertex v1(
			vertexCoords[3 * indexGroups[i]],
			vertexCoords[3 * indexGroups[i] + 1],
			vertexCoords[3 * indexGroups[i] + 2],
			vertexUVs[2 * indexGroups[i + 1]],
			vertexUVs[2 * indexGroups[i + 1] + 1],
			vertexNormals[3 * indexGroups[i + 2]],
			vertexNormals[3 * indexGroups[i + 2] + 1],
			vertexNormals[3 * indexGroups[i + 2] + 2]);
		Vertex v2(
			vertexCoords[3 * indexGroups[i + 3]],
			vertexCoords[3 * indexGroups[i + 3] + 1],
			vertexCoords[3 * indexGroups[i + 3] + 2],
			vertexUVs[2 * indexGroups[i + 4]],
			vertexUVs[2 * indexGroups[i + 4] + 1],
			vertexNormals[3 * indexGroups[i + 5]],
			vertexNormals[3 * indexGroups[i + 5] + 1],
			vertexNormals[3 * indexGroups[i + 5] + 2]);
		Vertex v3(
			vertexCoords[3 * indexGroups[i + 6]],
			vertexCoords[3 * indexGroups[i + 6] + 1],
			vertexCoords[3 * indexGroups[i + 6] + 2],
			vertexUVs[2 * indexGroups[i + 7]],
			vertexUVs[2 * indexGroups[i + 7] + 1],
			vertexNormals[3 * indexGroups[i + 8]],
			vertexNormals[3 * indexGroups[i + 8] + 1],
			vertexNormals[3 * indexGroups[i + 8] + 2]);
		if (vertexMap[v1].value == 0)
		{
			vertexMap[v1].value = ++order;
			vertices.push_back(v1);
		}
		if (vertexMap[v2].value == 0)
		{
			vertexMap[v2].value = ++order;
			vertices.push_back(v2);
		}
		if (vertexMap[v3].value == 0)
		{
			vertexMap[v3].value = ++order;
			vertices.push_back(v3);
		}
		indices.push_back(vertexMap[v1].value - 1);
		indices.push_back(vertexMap[v2].value - 1);
		indices.push_back(vertexMap[v3].value - 1);
	}
	vertexCoords.clear();
	vertexUVs.clear();
	vertexNormals.clear();
	indexGroups.clear();
}

void printToFile(string const& name, vector<Vertex>& vertices, vector<size_t>& indices)
{
	ofstream output("Meshes\\" + name + ".imd", ios_base::out | ios_base::binary);
	size_t size;
	size = vertices.size() * 8;
	output.write((char*)&size, sizeof(size_t));
	for (Vertex v : vertices)
		output.write((char*)v.data, (streamsize)sizeof(float) * 8);
	size = indices.size();
	output.write((char*)&size, sizeof(size_t));
	output.write((char*)&indices[0], (streamsize)sizeof(size_t) * size);
}

void createIMD(string fileName)
{
	vector<Vertex> vertices;
	vector<float> vertexCoords, vertexUVs, vertexNormals;
	vector<size_t> indices, indexGroups;
	string objectName;
	ifstream input("Meshes\\" + fileName + ".obj");
	if (!input.is_open())
		return;
	while (!input.eof())
	{
		char buffer[1024];
		float value;
		size_t index;
		string type;
		input >> type;
		if (type == "o")
		{
			if (vertexCoords.size())
			{
				process(vertexCoords, vertexUVs, vertexNormals, vertices, indexGroups, indices);
				printToFile(objectName, vertices, indices);
			}
			input >> objectName;
		}
		else if (type == "v")
		{
			input >> value;
			vertexCoords.push_back(value);
			input >> value;
			vertexCoords.push_back(value);
			input >> value;
			vertexCoords.push_back(value);
		}
		else if (type == "vt")
		{
			input >> value;
			vertexUVs.push_back(value);
			input >> value;
			vertexUVs.push_back(value);
		}
		else if (type == "vn")
		{
			input >> value;
			vertexNormals.push_back(value);
			input >> value;
			vertexNormals.push_back(value);
			input >> value;
			vertexNormals.push_back(value);
		}
		else if (type == "f")
		{
			input >> index;
			indexGroups.push_back(index - 1);
			input.ignore(1);
			input >> index;
			indexGroups.push_back(index - 1);
			input.ignore(1);
			input >> index;
			indexGroups.push_back(index - 1);

			input >> index;
			indexGroups.push_back(index - 1);
			input.ignore(1);
			input >> index;
			indexGroups.push_back(index - 1);
			input.ignore(1);
			input >> index;
			indexGroups.push_back(index - 1);

			input >> index;
			indexGroups.push_back(index - 1);
			input.ignore(1);
			input >> index;
			indexGroups.push_back(index - 1);
			input.ignore(1);
			input >> index;
			indexGroups.push_back(index - 1);
		}
		else
			input.getline(buffer, 1024);
	}
	if (vertexCoords.size())
	{
		process(vertexCoords, vertexUVs, vertexNormals, vertices, indexGroups, indices);
		printToFile(objectName, vertices, indices);
	}
}