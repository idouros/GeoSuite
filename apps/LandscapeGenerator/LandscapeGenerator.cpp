// LandscapeGenerator.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <Mesh.h>

int main()
{
	LandscapeParams p;

	// TODO: read the params from config file
	p.scale = 10;
	std::string outFileName = "W:\\Data\\landscape_01.obj";
	
	// Create it
	auto gm = GeoMesh::CreateGeoMesh(p);

	// Save
	std::ofstream outFile;
	outFile.open(outFileName);
	gm->SaveAsObjFile(outFile);
	outFile.close();

}

