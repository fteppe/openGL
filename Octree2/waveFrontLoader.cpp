#include "stdafx.h"
#include "waveFrontLoader.h"
#include <string>
#include <fstream>
#include <iostream>
#include <sstream>
#include <regex>


WaveFrontLoader::WaveFrontLoader()
{
	std::string fileName = "obj/cube.obj";
	std::ifstream inFile(fileName);
	std::string strOneLine;
	while (inFile)
	{
		while (getline(inFile, strOneLine, '\n'))
		{
			//for each line we try to know how it start
			//This line will designate a normal
			if (strOneLine.substr(0,2) == "vn")
			{
				std::istringstream s(strOneLine.substr(2));
				glm::vec3 vertex;
				//We put all the info in a new vertex

				s >> vertex.x; s >> vertex.y; s >> vertex.z;
				normals.push_back(vertex);
			}
			//this line designates a vertex
			if (strOneLine.substr(0,2) == "v ")
			{
				std::istringstream s(strOneLine.substr(2));
				glm::vec3 vertex;
				//We put all the info in a new vertex

				s >> vertex.x; s >> vertex.y; s >> vertex.z;
				vertices.push_back(vertex);
			}
			//this line designates a polygon
			if (strOneLine.substr(0, 2) == "f ")
			{
				std::string s(strOneLine.substr(2));
				//std::string s;
				std::regex r("([0-9]+)\\/([0-9]+)?\\/([0-9]+)?");
				std::smatch match;
				std::regex_search(s, match, r);

				for (std::sregex_iterator i = std::sregex_iterator(s.begin(), s.end(), r);
					i != std::sregex_iterator();
					++i)
				{
					std::smatch match = *i;
					std::cout << match[1] << match[2] << match[3];
				}

				std::cout << std::endl;

			}
			//std::cout << strOneLine << std::endl;
		}
	}

	inFile.close();

	for (int i = 0; i < vertices.size(); i++)
	{
		std::cout << vertices[i].x << " " << vertices[i].y << " " << vertices[i].z << std::endl;
	}

}


WaveFrontLoader::~WaveFrontLoader()
{
}

void WaveFrontLoader::addVertexToPolygon(unsigned int vertex, unsigned int normal,
int &vertexFinal, std::map<int, int> &vertexToNormal, std::map<int, std::vector<int>> &vertexSynonyme, std::vector<glm::vec3> &vertices)
{

	bool vertexCloned = true;
	unsigned int correspondingVertex = vertex;
	//if a vertex has no synonyme, we add himself
	if (vertexSynonyme.find(vertex) == vertexSynonyme.end())
	{
		vertexSynonyme[vertex].push_back(vertex);
	}
	//If this vertex has no normal linked to it, no need to clone it. We just need to link a vertex to it.
	if (vertexToNormal.find(vertex) == vertexToNormal.end())
	{
		//then, since it should be done in order if we add a vertex it should be the right index.
		vertexToNormal[vertex] = (normal);
		vertexCloned = false;
	}
	else
	{
		//What we are trying to do here is figure out if a vertex needs to be cloned because 2 different normals are associated to it.
		//So we look if the vertex (or one of it's clones) was already created with this normal associated to it.
		for (int i = 0; i < vertexSynonyme[vertex].size(); i++)
		{
			unsigned int synonyme = vertexSynonyme[vertex][i];
			//if this vertex has a normal linked to it
			if (vertexToNormal[synonyme] == normal)
			{
				//if the vertex or one of it's clones already has this normal associated to it, it means no need to clone it
				vertexCloned = false;
				//in th epolygon we will use this index of vertex. Because it is the one that has the right normal associated to it.
				correspondingVertex = synonyme;
			}
		}
	}

	//if there is no existing vertex that has had this normal associated to it, we clone the vertex, add it as a synonyme, 
	//and associate to the clone the normal
	if (vertexCloned == true)
	{
		//cloning the vertex
		vertices.push_back(vertices[vertex]);
		//Since it's a cloned index at the end of the vertices vector, it's index is the size of the vector -1
		correspondingVertex = vertices.size() - 1;
		//We set this vertex's index as a clone of the orginial vertex
		vertexSynonyme[vertex].push_back(correspondingVertex);
		vertexToNormal[correspondingVertex] = (normal);
	}
	//once we know what vertex needs to be added, we puhs it to the current polygon;
	vertexFinal = correspondingVertex;
}
