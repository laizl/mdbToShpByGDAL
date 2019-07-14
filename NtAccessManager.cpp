#include <QVector>
#include <iostream>
#include "qt_windows.h"
#include "ntaccessmanager.h"
#include <qapplication.h>
#include <QPluginLoader.h>
#include <qdir.h>
#include <QSqlDatabase>
#include <QSqlQueryModel>
#include <QSqlQuery> 
#include <qsqlrecord.h>
#include <qsqlfield.h>
#include <qvariant.h>



NtAccessManager::NtAccessManager()
{
	//加载数据库驱动文件
	QString path = QDir::currentPath();
	path = path.append("/Release");
	std::cout << path.toStdString() << std::endl;
	QApplication::addLibraryPath(path + QString("/plugins"));
	QPluginLoader loader(path + QString("/plugins/sqldrivers/qsqlodbc.dll"));
	
}

//创建数据库连接
void NtAccessManager::connectDB()
{
	//获取是否存在有数据库连接
	QSqlDatabase dbconn = QSqlDatabase::database("test", false);

	if (dbconn.isValid())//存在连接直接退出
	{
		return;
	}
	
	//创建一连接名为"test"的数据库连接
    dbconn = QSqlDatabase::addDatabase("QODBC", "test");
	std::cout << dbconn.driverName().toStdString() << std::endl;
	QString sDbNm = QString::fromLocal8Bit("E:/mdb和dxf/康定组CZ-SK-06.mdb");
	QString dsn = QString("DRIVER={Microsoft Access Driver (*.mdb, *.accdb)}; FIL={MS Access};DBQ=%1;").arg(sDbNm);//连接字符串
	dbconn.setDatabaseName(dsn);
	dbconn.setUserName("");//设置登陆数据库的用户名
	dbconn.setPassword("");//设置密码
	if (!dbconn.open()) {
		std::cout << "mdb链接失败!" << std::endl;
	}
}
//查询mdb字段名字
std::vector<std::string> NtAccessManager::queryTableName()
{
	std::vector<std::string> tableNames;
	//获取连接名库"test"的连接
	QSqlDatabase dbconn = QSqlDatabase::database("test", false);

	QSqlQuery sql_query(dbconn);

	sql_query.exec(QObject::tr("select * from ZKFC"));

	int fieldCount = sql_query.record().count();
	
	int i;
	QString fieldName;
	for (i = 0; i < fieldCount; i++) {
		tableNames.push_back(std::string(sql_query.record().field(i).name().toLocal8Bit()));
		tableLength ++;
		std::cout << sql_query.record().field(sql_query.record().field(i).name()).type() << std::endl;
	}
	//tableNames.push_back("path");
	//tableLength++;
	return tableNames;
}

//查询数据库经纬度
std::vector<std::vector<double>> NtAccessManager::readLatLong()
{
	std::vector<std::vector<double>> latLongs;
	//获取连接名库"test"的连接
	QSqlDatabase dbconn = QSqlDatabase::database("test", false);

	QSqlQuery sql_query(dbconn);
	sql_query.exec(QObject::tr("SELECT N,E FROM ZKFC"));

	//经纬度依次存入Vector
	while (sql_query.next())
	{
		std::vector<double> temp;
		temp.push_back(sql_query.value(0).toDouble());
	    temp.push_back(sql_query.value(1).toDouble());
		latLongs.push_back(temp);
	}
	return latLongs;
}

//查询数据库所有字段值
std::vector<std::vector<std::string>> NtAccessManager::readTableValue()
{
	std::vector<std::vector<std::string>> tableValues;
	//获取连接名库"test"的连接
	QSqlDatabase dbconn = QSqlDatabase::database("test", false);

	QSqlQuery sql_query(dbconn);
	sql_query.exec(QObject::tr("SELECT * FROM ZKFC"));

	//经纬度依次存入Vector
	while (sql_query.next())
	{
		std::vector<std::string> temp;
		for (int i = 0; i < tableLength; i++) {
			std::string value = sql_query.value(i).toString().toLocal8Bit();
			temp.push_back(value);
		}
		//加入Path
		//temp.push_back("E:/ZK");
		tableValues.push_back(temp);
	}	
	return tableValues;
}

void NtAccessManager::closeConnection() {
	QSqlDatabase dbconn = QSqlDatabase::database("test", false);
	if (dbconn.open()) {
		dbconn.close();
	}
	
}