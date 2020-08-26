// LandscapeGenerator.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <Mesh.h>

#include <iostream>

int main()
{

	ofstream outfile;
	outfile.open("W:\\Data\\landscape_01.obj");

	GridMesh gm = { 
		{ 0, 0, 0 ,0 }, 
		{ 0, 10, 0, 0 },
		{ 0, 0, 0 ,0 },
		{ 0, 0, 0 ,0 }
		};

	SaveAsObjFile(gm, 10, outfile);
	outfile.close();

}

