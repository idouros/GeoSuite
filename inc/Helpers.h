#ifndef _HELPERS
#define _HELPERS

#include <filesystem>
#include <map>

typedef boost::property_tree::ptree ConfigParams;



std::string getFileParameter(const std::filesystem::path& configFilePath,
	const boost::property_tree::ptree& configParams,
	const std::string& key)
{
	auto fileParameter = configParams.get<std::filesystem::path>(key);
	if (fileParameter.is_relative())
	{
		auto configFileFolder = configFilePath.parent_path();
		return configFileFolder.append(configParams.get<std::string>(key)).string();
	}
	return fileParameter.string();
}

template<typename T>
T ReadEnumFromConfigParams(const ConfigParams& configParams,
	const std::map<std::string, T>& enum_map,
	const std::string& key,
	const std::string& defaultValue)
{
	auto stringFromConfig = configParams.get<std::string>(key, defaultValue);
	auto it = enum_map.find(stringFromConfig);
	if (it != enum_map.end())
	{
		return enum_map.at(stringFromConfig);
	}
	else
	{
		std::cout << "Invalid value '" + stringFromConfig + "' for key '" + key + "' found in config file. Exiting" << std::endl;
		exit(-1);
	}
}

#endif