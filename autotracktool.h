#pragma once
#include <fstream>
#include <stdio.h>
#include <math.h>
#include <time.h> 
#include <cstdlib>
#include <iostream>
#include <vector>
#include <string>

#include <ogrsf_frmts.h>
#include <gdal.h>
#include <gdal_priv.h>
#include <cpl_string.h>

#include <iomanip>

class autoTracktool {
	
public:
	autoTracktool();
public:
	int exportPolygonToShpFile(std::vector<std::string> tableNames, std::string fileName, std::vector<std::vector<double> > polygon2DPointList, std::vector<std::vector<std::string> > tableValues);
	int createShpFileTest();
	int _epsgCode;
	int _outputEpsgCode;
};	
	
	
	
