#include <string>
#include <iostream>
#include "autotracktool.h"
#include "NtAccessManager.h"

void main(int argc, char *argv[]) {
	
	/*if (argc <= 3) {
		std::cout << "参数不全" << std::endl;
		return;
	}*/
	std::string  path = "E:/ZK";
	//std::string  Filepath = argv[3];
	//从数据库中获取数据
	NtAccessManager *ntAccessManager = new NtAccessManager();
	ntAccessManager->connectDB();
	std::vector<std::string> tableNameS = ntAccessManager->queryTableName();
	std::vector<std::vector<double>> latLongS = ntAccessManager->readLatLong();
	std::vector<std::vector<std::string>> tableValues = ntAccessManager->readTableValue();
	
	std::vector<std::string> temp;
	for (int i = 0; i < tableNameS.size(); i++) {
		temp.push_back(path);
	}
	tableValues.push_back(temp);
	//将数据写入shp文件
	//std::cout << tableNameS.size() << std::endl;
	autoTracktool *aTool = new autoTracktool();
	aTool->createShpFileTest();
	aTool->exportPolygonToShpFile(tableNameS,"Filepath", latLongS,tableValues);
	
	return ;
}