#include <string>
#include <iostream>
#include "autotracktool.h"
#include "NtAccessManager.h"

void main(int argc, char *argv[]) {
	//将cmd获取路径参数
	/*int i;
	printf("Total %d arguments\n", argc);
	for (i = 0; i<argc; i++)
	{
		printf("Argument %d = %s\n", i + 1, argv[i]);
	}*/
	//从数据库中获取数据
	NtAccessManager *ntAccessManager = new NtAccessManager();
	ntAccessManager->connectDB();
	std::vector<std::string> tableNameS = ntAccessManager->queryTableName();
	std::vector<std::vector<double>> latLongS = ntAccessManager->readLatLong();
	std::vector<std::vector<std::string>> tableValues = ntAccessManager->readTableValue();
	
	for (int i = 0; i < tableValues.size(); i++) {
		for (int j = 0; j < tableValues.size(); j++) {
			std::cout << tableValues[i][j].data() << std::endl;
		}
	}
	
	//将数据写入shp文件
	std::cout << tableNameS.size() << std::endl;
	autoTracktool *aTool = new autoTracktool();
	aTool->createShpFileTest();
	aTool->exportPolygonToShpFile(tableNameS,"E:\\test\\1.shp", latLongS,tableValues);
	
	return ;
}