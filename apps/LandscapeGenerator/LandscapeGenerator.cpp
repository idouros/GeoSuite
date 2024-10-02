// LandscapeGenerator.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>

#include <Mesh.h>

int main()
{
	LandscapeParams p;

	// TODO: read the params from config file
	p.grid_scale = 100.0;
	p.grid_rows = 500;
	p.grid_cols = 500;
	p.min_altitude = -1000.0;
	p.max_altitude = 5000.0;
	p.variance = 6000.0;		// Typically max_altitude minus min_altitude
	p.variance_fade = 0.53;		// Typically just over 0.5, to conceal the grid ridges
	p.last_random_pass = 1;
	p.out_file_name = "\\Output\\LandscapeGenerator\\landscape_01.obj";
	p.sea_output = SeaOutput::SHOW_LEVEL;
	
	// Create it
	auto gm = GeoMesh::CreateGeoMesh(p);

	// Save
	std::ofstream outFile;
	outFile.open(p.out_file_name);
	gm->SaveAsObjFile(outFile, p.sea_output);
	outFile.close();
}

