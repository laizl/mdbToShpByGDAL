#include <string>
#include <iostream>
#include "autotracktool.h"
#include "NtAccessManager.h"

std::string g_version = "0.0.4";

void displayHelp(int argc, char* argv[])
{
	std::cout << "=================================================================" << std::endl;
	std::cout << "|\t A tool for converting MDB to Shpfile.\tVersion: " + g_version + "\t|" << std::endl;
	std::cout << "=================================================================" << std::endl;
	//std::cout << argv[0] << "\tVersion: 0.1.0 \n"<< std::endl;
	std::cout << "Usage : mdb2shp in out:\n" << std::endl;
	std::cout << "\t in  : input mdb file access.\n" << std::endl;
	std::cout << "\t out : output shp file access.\n" << std::endl;
}

int main(int argc, char *argv[]) {
	
	if(argc < 3) {
		displayHelp(argc, argv);
		return 1;
	}
	int i = 0;
	std::string  path = "E:/ZK";
	//std::string  Filepath = argv[3];
	std::string inputFileName = argv[1];
	std::string outputFileName = argv[2];

	std::string tableName = "default";
	std::string latFieldString = "N";
	std::string longFieldString = "E";
	if(argc > 3){
		tableName = argv[3];
	}
	if(argc > 4){
		latFieldString = argv[4];
	}
	if(argc > 5){
		longFieldString = argv[5];
	}
	//从数据库中获取数据
	NtAccessManager *ntAccessManager = new NtAccessManager(tableName, latFieldString, longFieldString);
	ntAccessManager->testReadMdb(inputFileName);

	std::vector<std::string> fieldNames = ntAccessManager->getFieldNames();
	std::vector<std::vector<double> > latLongS = ntAccessManager->getLatLongs();
	std::vector<std::vector<std::string> > tableValues = ntAccessManager->getDataValues();
	
	std::cout<<"Cow Size:"<<tableValues.size()<<std::endl;
	std::cout<<"Col Size:"<<fieldNames.size()<<std::endl;
	/*for (i = 0; i < fieldNames.size(); i++) {
		std::cout<<fieldNames.at(i)<<std::endl;
	}
	for (i = 0; i < ntAccessManager->_latLongs.size(); i++) {
		std::cout<<ntAccessManager->_latLongs.at(i).at(1)<<std::endl;
	}
	for (i = 0; i < ntAccessManager->_dataValue.size(); i++) {
		std::cout<<ntAccessManager->_dataValue.at(i).at(0)<<std::endl;
	}*/

	// std::vector<std::string> temp;
	// for (int i = 0; i < tableNameS.size(); i++) {
	// 	temp.push_back(path);
	// }
	// tableValues.push_back(temp);
	//将数据写入shp文件
	//std::cout << tableNameS.size() << std::endl;
	autoTracktool *aTool = new autoTracktool();
	// aTool->createShpFileTest();
	aTool->exportPolygonToShpFile(fieldNames,outputFileName, latLongS,tableValues);
	
	return 0;
}
