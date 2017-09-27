#include "stdafx.h"
#include "waveFrontLoader.h"
#include <string>
#include <fstream>
#include <iostream>
#include <sstream>
#include <regex>
#include <vector>
#include <stdlib.h>     /* strtol */


WaveFrontLoader::WaveFrontLoader()
{


}


WaveFrontLoader::~WaveFrontLoader()
{
}

std::vector<Solid> WaveFrontLoader::GetSolidsFromFile(std::string fileName)
{
	std::ifstream inFile(fileName);
	std::string strOneLine;
	std::vector<Solid> returnValue;
	std:: cout << "started loading file" << std::endl;
	while (inFile)
	{
		while (getline(inFile, strOneLine, '\n'))
		{

			//for each line we try to know how it start
			//This line will designate a normal
			if (strOneLine.substr(0, 2) == "vn")
			{
				std::istringstream s(strOneLine.substr(2));
				glm::vec3 vertex;
				//We put all the info in a new vertex

				s >> vertex.x; s >> vertex.y; s >> vertex.z;
				normalsObj.push_back(vertex);
			}
			//this line designates a vertex
			if (strOneLine.substr(0, 2) == "v ")
			{
				std::istringstream s(strOneLine.substr(2));
				glm::vec3 vertex;
				//We put all the info in a new vertex

				s >> vertex.x; s >> vertex.y; s >> vertex.z;
				vertices.push_back(vertex);
				//We also put this vertex info in an array specific to this solid.
				solidVertices.push_back(vertex);
				//We map the two so we can know which idex is which vertex
				fileToSolidVertexIndex[vertices.size() - 1] = solidVertices.size() - 1;
				
			}
			//this line designates a polygon
			if (strOneLine.substr(0, 2) == "f ")
			{
				std::string s(strOneLine.substr(2));
				//std::string s;
				std::regex r("([0-9]+)\\/([0-9]+)?\\/([0-9]+)?");
				std::smatch match;
				std::regex_search(s, match, r);

				std::vector<int> face;
				for (std::sregex_iterator i = std::sregex_iterator(s.begin(), s.end(), r);
					i != std::sregex_iterator();
					++i)
				{
					std::smatch match = *i;
					//std::cout << match[1] << match[2] << match[3];
					std::string vertIndex = match[1].str();
					//vertices in a obj file are indexed from 1, our index starts at one
					int index = strtol(vertIndex.c_str(), NULL, 10) - 1;
					//We take the normal index.
					int indexNormal = strtol(match[3].str().c_str(), NULL, 10) - 1;

					index = addVertexToPolygon(index, indexNormal);
					face.push_back(index);
				}
				polygons.push_back(face);
				//std::cout << std::endl;

			}
			//std::cout << strOneLine << std::endl;
			if (strOneLine.substr(0, 2) == "o " && vertices.size() > 0)
			{
				//we have a new object
				Solid result(makeSolidFromData());
				returnValue.push_back(result);
			}
		}
	}

	inFile.close();
	//We build the vector of all the normals at the same index than the corresponding vertex. Using the map

	//for (int i = 0; i < vertices.size(); i++)
	//{
	//	int vertexIndex = i;
	//	int normalIndex = vertexToNormal[i];
	//	glm::vec3 normal = normalsObj[normalIndex];
	//	normals.push_back(normal);
	//}
	//Solid result(vertices, polygons);
	//result.setNormals(normals);
	Solid result(makeSolidFromData());
	returnValue.push_back(result);
	//TODO: change that
	std::cout << "done loading file" << std::endl;
	return returnValue;
}

//THis function is used to clone vertices when to different normals are associated to it.
int WaveFrontLoader::addVertexToPolygon(unsigned int vertex, unsigned int normal)
{
	bool vertexCloned = true;
	unsigned int solidVertex = fileToSolidVertexIndex[vertex];
	unsigned int correspondingVertex = solidVertex;
	//if a vertex has no synonyme, we add the corresponding solid vertex index as his on synonyme.
	if (vertexSynonyme.find(vertex) == vertexSynonyme.end())
	{
		vertexSynonyme[vertex].push_back(solidVertex);
	}
	//If this vertex has no normal linked to it, no need to clone it. We just need to link the vertex to it.
	if (vertexToNormal.find(solidVertex) == vertexToNormal.end())
	{
		//then, since it should be done in order if we add a vertex it should be the right index.
		vertexToNormal[solidVertex] = (normal);
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
		solidVertices.push_back(solidVertices[solidVertex]);
		//Since it's a cloned index at the end of the vertices vector, it's index is the size of the vector -1
		correspondingVertex = solidVertices.size() - 1;
		//We set this vertex's index as a clone of the orginial vertex
		vertexSynonyme[vertex].push_back(correspondingVertex);
		//Since the normal in input was never associated to this vertex, we associate the normal to this newly cloned vertex
		vertexToNormal[correspondingVertex] = (normal);
	}
	//once we know what vertex needs to be added, we puhs it to the current polygon;
	return correspondingVertex;
}

Solid WaveFrontLoader::makeSolidFromData()
{
	std::cout << "making obj"<<std::endl;
	for (int i = 0; i < solidVertices.size(); i++)
	{
		int vertexIndex = i;
		int normalIndex = vertexToNormal[i];
		glm::vec3 normal = normalsObj[normalIndex];
		normals.push_back(normal);
	}
	Solid result(solidVertices, polygons);
	result.setNormals(normals);

	//We clear the info that are exclusive to a single solid
	solidVertices.clear();
	vertexSynonyme.clear();
	polygons.clear();
	normals.clear();
	vertexToNormal.clear();
	/*normals.clear();
	polygons.clear();
	vertices.clear();
	normalsObj.clear();
	vertexSynonyme.clear();
	vertexToNormal.clear();*/
	return result;
}
