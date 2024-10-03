#ifndef _LANDSCAPE
#define _LANDSCAPE

#include <map>

#define CREATE_ENUM(name) name,
#define CREATE_MAP(S, ...) { #S, ENUM_CLASS_NAME::##S },

#define SEA_OUTPUTS(sea_output) \
	sea_output(NONE)\
	sea_output(SHOW_LEVEL)\
	sea_output(CHOP)
#define ENUM_CLASS_NAME SeaOutput 
enum class ENUM_CLASS_NAME { SEA_OUTPUTS(CREATE_ENUM) };
static std::map<std::string, ENUM_CLASS_NAME> seaOutputMap = { SEA_OUTPUTS(CREATE_MAP) };
#undef ENUM_CLASS_NAME


#endif