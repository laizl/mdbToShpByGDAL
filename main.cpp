#include <string>
#include <iostream>
#include "autotracktool.h"
#include "NtAccessManager.h"

void main(int argc, char *argv[]) {
	//��cmd��ȡ·������
	/*int i;
	printf("Total %d arguments\n", argc);
	for (i = 0; i<argc; i++)
	{
		printf("Argument %d = %s\n", i + 1, argv[i]);
	}*/
	//�����ݿ��л�ȡ����
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
	
	//������д��shp�ļ�
	std::cout << tableNameS.size() << std::endl;
	autoTracktool *aTool = new autoTracktool();
	aTool->createShpFileTest();
	aTool->exportPolygonToShpFile(tableNameS,"E:\\test\\1.shp", latLongS,tableValues);
	
	return ;
}