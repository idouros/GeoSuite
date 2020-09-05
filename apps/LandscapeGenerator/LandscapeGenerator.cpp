// LandscapeGenerator.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <Mesh.h>

int main()
{
	LandscapeParams p;

	// TODO: read the params from config file
	p.grid_scale = 75.0;
	p.grid_rows = 300;
	p.grid_cols = 600;
	p.min_altitude = 0.0;
	p.max_altitude = 2918.0;
	p.variance = 350.0;
	std::string outFileName = "W:\\Data\\landscape_01.obj";
	
	// Create it
	auto gm = GeoMesh::CreateGeoMesh(p);

	// Save
	std::ofstream outFile;
	outFile.open(outFileName);
	gm->SaveAsObjFile(outFile);
	outFile.close();
}

