//{{NO_DEPENDENCIES}}
// Microsoft Visual C++ generated include file.
// Used by Octree2.rc

// Next default values for new objects
// 
#include <string>
#include "ParameterContainer.h"

namespace tetraRender
{
	class Resource
{
public:
	Resource();
	~Resource();
	void setName(std::string name);
	std::string getName();
protected:
	std::string name;
	ParameterContainer parameters;

};
}

