// LandscapeGenerator.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <Mesh.h>

int main()
{
	LandscapeParams p;

	// TODO: read the params from config file
	p.grid_scale = 10.0;
	p.grid_rows = 500;
	p.grid_cols = 1000;
	p.min_altitude = 1.0;
	p.max_altitude = 2000.0;
	std::string outFileName = "W:\\Data\\landscape_01.obj";
	
	// Create it
	auto gm = GeoMesh::CreateGeoMesh(p);

	// Save
	std::ofstream outFile;
	outFile.open(outFileName);
	gm->SaveAsObjFile(outFile);
	outFile.close();

}

