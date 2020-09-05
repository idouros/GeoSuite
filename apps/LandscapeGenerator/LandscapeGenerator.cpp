// LandscapeGenerator.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <Mesh.h>

int main()
{
	LandscapeParams p;

	// TODO: read the params from config file
	p.grid_scale = 100.0;
	p.grid_rows = 500;
	p.grid_cols = 500;
	p.min_altitude = 0.0;
	p.max_altitude = 5000.0;
	p.variance = 2750.0;
	p.variance_fade = 0.3;
	p.last_random_pass = 2;
	p.out_file_name = "W:\\Data\\landscape_01.obj";
	
	// Create it
	auto gm = GeoMesh::CreateGeoMesh(p);

	// Save
	std::ofstream outFile;
	outFile.open(p.out_file_name);
	gm->SaveAsObjFile(outFile);
	outFile.close();
}

