#ifndef NTACCESSMANAGER_H
#define NTACCESSMANAGER_H

//#include <QString>
//#include <QMap>
#include <string>
#include <iostream>
#include <vector>

//#include <QtSql>
class NtAccessManager
{
public:
	NtAccessManager(std::string tableName,std::string latFieldString, std::string longFieldString);
	void testReadMdb(std::string mdbFileName);
	/**
	* 获取链接
	*/
	//void connectDB(std::string mdbFileName);
	/**
	* 获取字段列表名
	*/
	//std::vector<std::string> queryTableName();
	std::vector<std::string> getFieldNames(){
		return _fieldNames;
	}
	std::vector<std::vector<std::string> > getDataValues(){
		return _dataValue;
	}
	std::vector<std::vector<double> > getLatLongs(){
		return _latLongs;
	}
	/**
	* 查询数据库经纬度值
	*/
	//std::vector<std::vector<double>> readLatLong();
	/**
	* 查询数据库字段的值
	*/
	
	//std::vector<std::vector<std::string>> readTableValue();
	/**
	* 关闭数据库
	*/
	//void closeConnection();
	std::string _tableName;
	std::string _latFieldString;
	std::string _longFieldString;

	std::vector<std::string> _fieldNames;
	std::vector<std::vector<std::string> > _dataValue;
	std::vector<std::vector<double> > _latLongs;
private:
	// int tableLength = 0; //表长
};

#endif // NTACCESSMANAGER_H
