#ifndef NTACCESSMANAGER_H
#define NTACCESSMANAGER_H

#include <QString>
#include <QMap>

//#include <QtSql>
class NtAccessManager
{
public:
	NtAccessManager();
	/**
	* ��ȡ����
	*/
	void connectDB();
	/**
	* ��ѯ���ݿ����
	*/
	std::vector<std::string> queryTableName();

	/**
	* ��ѯ���ݿ⾭γ��ֵ
	*/
	std::vector<std::vector<double>> readLatLong();
	/**
	* ��ѯ���ݿ��ֶε�ֵ
	*/
	std::vector<std::vector<std::string>> readTableValue();
	/**
	* �ر����ݿ�
	*/
	void closeConnection();

private:
	int tableLength = 0; //��
};

#endif // NTACCESSMANAGER_H
