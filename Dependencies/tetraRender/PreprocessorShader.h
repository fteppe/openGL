#pragma once
#include <vector>
#include <string>
class PreprocessorShader
{
public:
	PreprocessorShader();
	~PreprocessorShader();

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>
	/// Process the file described by path, for now it will handle includes for GLSL.
	/// </summary>
	///
	/// <remarks>	Fteppe, 26/04/2018. </remarks>
	///
	/// <param name="path">				Full pathname of the file. </param>
	/// <param name="includedFiles">	[in,out] The included files, we keep track of them to avoid cyclical includes and easier debugging. </param>
	///
	/// <returns>	The content of the shader file with the includes. </returns>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	static std::string processFile(std::string path, std::vector < std::string>& includedFiles);

	protected:

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Process the file, but avoids cyclical inclusions, and numbers the included files. </summary>
		///
		/// <remarks>	Fteppe, 26/04/2018. </remarks>
		///
		/// <param name="path">			Full pathname of the file. </param>
		/// <param name="nbInclude">	The nb include. </param>
		/// <param name="included"> 	The files that have already been included. </param>
		///
		/// <returns>	A std::string. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		static std::string processFile(std::string path, unsigned int nbInclude, std::vector<std::string>& included);

		static bool hasVersion;
};

