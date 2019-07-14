#include <string>
#include <iostream>
#include "autotracktool.h"
#include "NtAccessManager.h"

int main(int argc, char *argv[]) {
	
	/*if (argc <= 3) {
		std::cout << "������ȫ" << std::endl;
		return;
	}*/
	std::string  path = "E:/ZK";
	//std::string  Filepath = argv[3];
	//�����ݿ��л�ȡ����
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
	//������д��shp�ļ�
	//std::cout << tableNameS.size() << std::endl;
	autoTracktool *aTool = new autoTracktool();
	aTool->createShpFileTest();
	aTool->exportPolygonToShpFile(tableNameS,"Filepath", latLongS,tableValues);
	
	return 0;
}
