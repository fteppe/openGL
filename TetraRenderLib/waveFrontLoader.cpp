#include "stdafx.h"
#include "waveFrontLoader.h"
#include <string>
#include <fstream>
#include <iostream>
#include <sstream>
#include <regex>
#include <vector>
#include <stdlib.h>     /* strtol */

using namespace tetraRender;

WaveFrontLoader::WaveFrontLoader()
{
	nbVerticesFile = 0;

}


WaveFrontLoader::~WaveFrontLoader()
{

}


void WaveFrontLoader::loadVertexObjectVectorFromFile(std::string fileName, std::vector<Mesh*> &vertexObjects)
{
	std::ifstream inFile(fileName);
	std::string strOneLine;
	std::cout << "started loading file" << std::endl;
	file.first = fileName;
	
	while (inFile)
	{
		std::string prevLine = "";
		while (getline(inFile, strOneLine, '\n'))
		{
			//building new obj
			if (strOneLine.substr(0, 2) == "o ")
			{
				//If we reach the build of a new object then we need to make the previous one.
				if (polygons.size() > 0)
				{
					vertexObjects.push_back(makeVBOFromData());
				}
				std::cout << strOneLine << std::endl;
				std::istringstream s(strOneLine.substr(2));
				s >> file.second;
			}
			else
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
				//This indicates the coordinates of a UV attribute
				if (strOneLine.substr(0, 2) == "vt")
				{
					std::istringstream s(strOneLine.substr(2));
					glm::vec3 vertex;
					//We put all the info in a new vertex

					s >> vertex.x; s >> vertex.y; s >> vertex.z;
					UVobj.push_back(vertex);
				}
				//this line designates a polygon
				if (strOneLine.substr(0, 2) == "f ")
				{

					std::string s(strOneLine.substr(2));
					//We use a regex to get the indexes of the vertex and it's attributes
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
						index = fileToSolidVertexIndex[index];
						//We take the normal index.
						int normalIndex = strtol(match[3].str().c_str(), NULL, 10) - 1;
						//the UV index
						int UVindex = strtol(match[2].str().c_str(), NULL, 10) - 1;
						//For now, we consider the UV to be on an unclonned vertex
						int VertToUVindex = index;
						if (UVindex != -1)
						{
							//We check if we need to clone the vertex, or if there is a vertex synonym without UV coordinate.
							VertToUVindex = vertexAndAttributeLink(index, UVindex, vertexToUV);
						}
						int vertToNormalIndex = vertexAndAttributeLink(index, normalIndex, vertexToNormal);

						//A problem arises when you don't need to clone UV but clone the normal. You need to give the cloned vertex it's UV attributes
						//When we clone we set one of the two attributes, leaving one empty. Each vertex must have both the UV AND the normal attribute. If it doesn't the rendering will have bugs. 
						if (VertToUVindex > vertToNormalIndex)
						{
							//If the two vertices index are different, it means that one was cloned, but not the other. In that case the cloned vertex needs to get the informations of the original.					
							if (vertexToNormal.find(VertToUVindex) == vertexToNormal.end())
							{
								vertexToNormal[VertToUVindex] = vertexToNormal[vertToNormalIndex];
							}
						}
						else if (VertToUVindex < vertToNormalIndex)
						{
							//if it doesn't already have a value.
							if (vertexToUV.find(vertToNormalIndex) == vertexToUV.end())
							{
								vertexToUV[vertToNormalIndex] = vertexToUV[VertToUVindex];
							}
						}
						int finalIndex = vertToNormalIndex;
						//We try to figure out which of our cloned vertex is the right one. It needs to give the right value for the UV vector and the normal vector
						if (vertexToUV[VertToUVindex] == UVindex && vertexToNormal[VertToUVindex] == normalIndex)
						{
							finalIndex = VertToUVindex;
						}
						else if (vertexToUV[vertToNormalIndex] == UVindex && vertexToNormal[vertToNormalIndex] == normalIndex)
						{
							finalIndex = vertToNormalIndex;
						}


						face.push_back(finalIndex);
					}
					polygons.push_back(face);
				}
				// this doesn't work to indicate the move to another obj. The end of faces would be a better indicator.
				//if the current line isn't a face when the previous was one, then we change file.
				if ((strOneLine.substr(0, 2) != "f " && strOneLine.substr(0, 2) != "s ") && prevLine.substr(0, 2) == "f ")
				{
					//we have a new object
					//Solid result();
					vertexObjects.push_back(makeVBOFromData());
				}
			}
			

			prevLine = strOneLine;
		}
	}

	inFile.close();
	//if there are some polygons left to make.
	if (polygons.size() > 0)
	{
		vertexObjects.push_back(makeVBOFromData());
	}

	//TODO: change that
	std::cout << "done loading file" << std::endl;
}

Mesh * WaveFrontLoader::loadSpecificVBO(std::string fileName, std::string objectName)
{
	std::vector<Mesh*> obj;
	loadVertexObjectVectorFromFile(fileName, obj);

	for (auto elem : obj)
	{
		if (elem->getFilePath().second == objectName)
		{
			return elem;
		}
	}
	std::cout << __FILE__ << "::" << __LINE__ << " " << fileName << " not found \n";
}



int WaveFrontLoader::vertexAndAttributeLink(unsigned int vertex, unsigned int attribute, std::map<int, int>& vertexToAttribute)
{
	bool vertexCloned = true;
	unsigned int solidVertex = vertex;
	unsigned int correspondingVertex = solidVertex;
	//if a vertex has no synonyme, we add the corresponding solid vertex index as his on synonyme.
	if (vertexSynonyme.find(vertex) == vertexSynonyme.end())
	{
		vertexSynonyme[vertex].push_back(solidVertex);
	}
	//If this vertex has no normal linked to it, no need to clone it. We just need to link the vertex to it.


	//What we are trying to do here is figure out if a vertex needs to be cloned because 2 different normals are associated to it.
	//So we look if the vertex (or one of it's clones/synonyme) was already created with this normal associated to it.
	for (int i = 0; i < vertexSynonyme[vertex].size(); i++)
	{
		unsigned int synonyme = vertexSynonyme[vertex][i];

		//if one of the synonymes has no attribute linked to it, then we give it this attribute
		if (vertexToAttribute.find(synonyme) == vertexToAttribute.end())
		{
			//then, since it should be done in order if we add a vertex it should be the right index.
			correspondingVertex = synonyme;
			vertexCloned = false;
		}

		//if this vertex has the attribute linked to it.
		if (vertexToAttribute[synonyme] == attribute)
		{
			//if the vertex or one of it's clones already has this attribute associated to it, it means no need to clone it
			vertexCloned = false;
			//in th epolygon we will use this index of vertex. Because it is the one that has the right normal associated to it.
			correspondingVertex = synonyme;
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
		
	}
	//once we know what vertex needs to be added, we puhs it to the current polygon;
	vertexToAttribute[correspondingVertex] = (attribute);
	return correspondingVertex;
}



Mesh * WaveFrontLoader::makeVBOFromData()
{
	std::cout << "making obj" << std::endl;
	std::vector<glm::vec3> UVs;
	//the normals indexed as in the solid
	std::vector<glm::vec3> normals;
	for (int i = 0; i < solidVertices.size(); i++)
	{
		int vertexIndex = i;

		int normalIndex = vertexToNormal[vertexIndex];
		glm::vec3 normal = normalsObj[normalIndex];
		normals.push_back(normal);
		//we make sure there are UVs to add to our solid
		if (UVobj.size() > 0)
		{
			int UVindex = vertexToUV[vertexIndex];
			glm::vec3 UV = UVobj[UVindex];
			UVs.push_back(UV);
		}

	}
	//Solid result(solidVertices, polygons);
	Mesh* vbo = new Mesh(solidVertices, polygons);
	vbo->setNormals(normals);
	vbo->setUVs(UVs);
	vbo->setFilePath(file);

	//We clear the info that are exclusive to a single solid
	solidVertices.clear();
	vertexSynonyme.clear();
	polygons.clear();
	vertexToNormal.clear();
	vertexToUV.clear();
	return vbo;
}
