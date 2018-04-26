#include "PreprocessorShader.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <experimental\filesystem>

PreprocessorShader::PreprocessorShader()
{
}


PreprocessorShader::~PreprocessorShader()
{
}

std::string PreprocessorShader::processFile(std::string path, std::vector < std::string>& includedFiles)
{
	includedFiles.push_back(path);
	return PreprocessorShader::processFile(path, 0, includedFiles);
}

std::string PreprocessorShader::processFile(std::string path, unsigned int nbInclude, std::vector<std::string>& included)
{
	std::ifstream shaderSource(path);
	std::stringstream ss;

	//We just need to know in which diretory this file is.
	std::string currentDir = "";
	size_t pos = path.size();
	while (path[pos] != '/' && pos >= 0)
	{
		pos--;
	}
	pos++;
	currentDir += path.substr(0, pos);
	/*std::string source = std::string((std::istreambuf_iterator<char>(shaderSource)), std::istreambuf_iterator<char>());
	if (source.size() == 0)
	{
	std::cout << __FILE__ << " " << __LINE__ << "empty shader File" << path << std::endl;
	}*/
	std::string source = "";
	//std::stringstream ss(source);
	std::string line;
	unsigned int currentLine = 1;
	while (getline(shaderSource, line))
	{
		if (line.substr(0, 8) == "#include")
		{
			size_t lineSize = line.size();
			std::string fileInclude = line.substr(9, line.npos);
			std::string fileIncludeName = "";
			bool insideQuotes = false;
			//We want to find the name of the file between the quotes.
			for (int i = 0; i < fileInclude.size(); i++)
			{

				if (insideQuotes == true && fileInclude[i] == '\"')
				{
					insideQuotes = false;
				}
				if (insideQuotes == true)
				{
					fileIncludeName.push_back(fileInclude[i]);
				}
				if (insideQuotes == false && fileInclude[i] == '\"')
				{
					insideQuotes = true;
				}

			}

			std::string replaceInclude = "//" + fileIncludeName + "\n";
			nbInclude++;
			replaceInclude = replaceInclude + "#line 0 " + std::to_string(nbInclude)+"\n";
			replaceInclude = replaceInclude + " \n";

			std::string fullPath = currentDir + fileIncludeName;
			included.push_back(fullPath);
			replaceInclude += PreprocessorShader::processFile(fullPath, nbInclude, included);
			source += replaceInclude;

			source += "#line "+std::to_string(currentLine+1)+" 0\n";
		}
		else
		{
			source = source + line + "\n";
		}

		currentLine++;
	}
	return source;
}
