#ifndef NTACCESSMANAGER_H
#define NTACCESSMANAGER_H

//#include <QString>
//#include <QMap>

//#include <QtSql>
class NtAccessManager
{
public:
	NtAccessManager();
	void testReadMdb(std::string mdbFileName);
	/**
	* 获取链接
	*/
	//void connectDB(std::string mdbFileName);
	/**
	* 获取字段列表名
	*/
	//std::vector<std::string> queryTableName();
	std::vector<std::string> getFieldNames();
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

	std::vector<std::string> _fieldNames;
	std::vector<std::vector<std::string> > _dataValue;
	std::vector<std::vector<double> > _latLongS;
private:
	int tableLength = 0; //表长
};

#endif // NTACCESSMANAGER_H
