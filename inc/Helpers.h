#ifndef _HELPERS
#define _HELPERS

#include <filesystem>

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

#endif