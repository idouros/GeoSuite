// LandscapeGenerator.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>
#include <iostream>
#include <filesystem>
#include <Landscape.h>
#include <Helpers.h>
#include <Mesh.h>

LandscapeParams DEFAULT_PARAMS;

int main(int argc, char** argv)
{
	if (argc != 2)
	{
		std::cout << " Usage: " << argv[0] << " ConfigFile" << std::endl;
		return -1;
	}

	// Read the configuration file
	LOG_OUT("Reading config file...");
	ConfigParams configParams;
	boost::property_tree::ini_parser::read_ini(argv[1], configParams);

	// Populate the parameter values
	LandscapeParams p;
	p.grid_scale = configParams.get<double>("landscape.grid_scale", DEFAULT_PARAMS.grid_scale);
	p.grid_rows = configParams.get<size_t>("landscape.grid_rows", DEFAULT_PARAMS.grid_rows);
	p.grid_cols = configParams.get<size_t>("landscape.grid_cols", DEFAULT_PARAMS.grid_cols);
	p.min_altitude = configParams.get<double>("landscape.min_altitude", DEFAULT_PARAMS.min_altitude);
	p.max_altitude = configParams.get<double>("landscape.max_altitude", DEFAULT_PARAMS.max_altitude);
	p.variance = configParams.get<double>("landscape.variance", DEFAULT_PARAMS.variance);
	p.variance_fade = configParams.get<double>("landscape.variance_fade", DEFAULT_PARAMS.variance_fade);
	p.last_random_pass = configParams.get<size_t>("landscape.last_random_pass", DEFAULT_PARAMS.last_random_pass);
	p.sea_output = ReadEnumFromConfigParams(configParams, seaOutputMap, "landscape.sea_output", "SHOW_LEVEL");
	p.smoothing_radius = configParams.get<size_t>("landscape.smoothing_radius", DEFAULT_PARAMS.smoothing_radius);

	// Create the landscape
	auto gm = std::make_shared<GeoMesh>(GeoMesh());
	LOG_OUT(std::format("Creating landscape mesh grid with {} rows and {} columns...", p.grid_rows, p.grid_cols));
	EXEC_TIMED(gm = GeoMesh::CreateGeoMesh(p));

	if (p.smoothing_radius > 0)
	{
		LOG_OUT("Smoothing...")
		EXEC_TIMED(gm->GaussianSmoothing(p.smoothing_radius);)
	}

	// Save
	std::filesystem::path configFilePath(argv[1]);
	auto out_file_name = getFileParameter(configFilePath, configParams, "files.output_file");
	LOG_OUT(std::format("Saving landscape to: {}", out_file_name));
	std::ofstream outFile;
	outFile.open(out_file_name);
	EXEC_TIMED(gm->SaveAsObjFile(outFile, p.sea_output));
	outFile.close();
}

