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
	* 获取链接
	*/
	void connectDB();
	/**
	* 查询数据库表名
	*/
	std::vector<std::string> queryTableName();

	/**
	* 查询数据库经纬度值
	*/
	std::vector<std::vector<double>> readLatLong();
	/**
	* 查询数据库字段的值
	*/
	std::vector<std::vector<std::string>> readTableValue();
	/**
	* 关闭数据库
	*/
	void closeConnection();

private:
	int tableLength = 0; //表长
};

#endif // NTACCESSMANAGER_H
