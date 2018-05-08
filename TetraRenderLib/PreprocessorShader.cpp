#include "PreprocessorShader.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <experimental\filesystem>

bool PreprocessorShader::hasVersion;

PreprocessorShader::PreprocessorShader()
{
}


PreprocessorShader::~PreprocessorShader()
{
}

std::string PreprocessorShader::processFile(std::string path, std::vector < std::string>& includedFiles)
{
	hasVersion = false;
	includedFiles.push_back(path);
	hasVersion = false;
	std::string source = PreprocessorShader::processFile(path, 0, includedFiles);
	return source;
	
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
			std::string fullPath = currentDir + fileIncludeName;
			if (std::find(included.begin(), included.end(), fullPath) == included.end())
			{
				std::string replaceInclude = "//" + fileIncludeName + "\n";
				nbInclude++;
				replaceInclude = replaceInclude + "#line 0 " + std::to_string(nbInclude) + "\n";
				replaceInclude = replaceInclude + " \n";


				included.push_back(fullPath);
				std::string includeFile = PreprocessorShader::processFile(fullPath, nbInclude, included);
				if (includeFile.size() == 0)
				{
					std::cout << __FILE__ << "::" << __LINE__ << " empty file " + fullPath + "\n";
				}
				replaceInclude += includeFile;
				source += replaceInclude;

				source += "#line " + std::to_string(currentLine + 1) + " 0\n";
			}

		}
		else if (line.substr(0, 8) == "#version")
		{
			if (hasVersion == true)
			{
				//We don't add the line if it is more than the first "version" or it breaks the code.
			}
			else if (hasVersion == false)
			{
				hasVersion = true;
				source = source + line + "\n";
			}
			
		}

		else
		{
			source = source + line + "\n";
		}

		currentLine++;
	}
	return source;
}
