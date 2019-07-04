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
	//�������ݿ������ļ�
	QString path = QDir::currentPath();
	path = path.append("/Release");
	std::cout << path.toStdString() << std::endl;
	QApplication::addLibraryPath(path + QString("/plugins"));
	QPluginLoader loader(path + QString("/plugins/sqldrivers/qsqlodbc.dll"));
	
}

//�������ݿ�����
void NtAccessManager::connectDB()
{
	//��ȡ�Ƿ���������ݿ�����
	QSqlDatabase dbconn = QSqlDatabase::database("test", false);

	if (dbconn.isValid())//��������ֱ���˳�
	{
		return;
	}
	
	//����һ������Ϊ"test"�����ݿ�����
    dbconn = QSqlDatabase::addDatabase("QODBC", "test");
	std::cout << dbconn.driverName().toStdString() << std::endl;
	QString sDbNm = QString::fromLocal8Bit("E:/mdb��dxf/������CZ-SK-06.mdb");
	QString dsn = QString("DRIVER={Microsoft Access Driver (*.mdb, *.accdb)}; FIL={MS Access};DBQ=%1;").arg(sDbNm);//�����ַ���
	dbconn.setDatabaseName(dsn);
	dbconn.setUserName("");//���õ�½���ݿ���û���
	dbconn.setPassword("");//��������
	if (!dbconn.open()) {
		std::cout << "mdb����ʧ��!" << std::endl;
	}
}
//��ѯmdb�ֶ�����
std::vector<std::string> NtAccessManager::queryTableName()
{
	std::vector<std::string> tableNames;
	//��ȡ��������"test"������
	QSqlDatabase dbconn = QSqlDatabase::database("test", false);

	QSqlQuery sql_query(dbconn);

	sql_query.exec(QObject::tr("select * from ZKGK"));

	int fieldCount = sql_query.record().count();

	int i;
	QString fieldName;
	for (i = 0; i < fieldCount; i++) {
		//fieldName = sql_query.record().fieldName(i).toUtf8();
		tableNames.push_back(std::string(sql_query.record().field(i).name().toLocal8Bit()));
		//std::cout<<sql_query.record().field(i).type()<<std::endl;
		tableLength ++;
	}
	
	return tableNames;
}

//��ѯ���ݿ⾭γ��
std::vector<std::vector<double>> NtAccessManager::readLatLong()
{
	std::vector<std::vector<double>> latLongs;
	//��ȡ��������"test"������
	QSqlDatabase dbconn = QSqlDatabase::database("test", false);

	QSqlQuery sql_query(dbconn);
	sql_query.exec(QObject::tr("SELECT N,E FROM ZKGK"));

	//��γ�����δ���Vector
	while (sql_query.next())
	{
		std::vector<double> temp;
		temp.push_back(sql_query.value(0).toDouble());
	    temp.push_back(sql_query.value(1).toDouble());
		latLongs.push_back(temp);
	}
	return latLongs;
}

//��ѯ���ݿ������ֶ�ֵ
std::vector<std::vector<std::string>> NtAccessManager::readTableValue()
{
	std::vector<std::vector<std::string>> tableValues;
	//��ȡ��������"test"������
	QSqlDatabase dbconn = QSqlDatabase::database("test", false);

	QSqlQuery sql_query(dbconn);
	sql_query.exec(QObject::tr("SELECT * FROM ZKGK"));

	//��γ�����δ���Vector
	while (sql_query.next())
	{
		std::vector<std::string> temp;
		for (int i = 0; i < tableLength; i++) {
			std::string value = sql_query.value(i).toString().toLocal8Bit();
			temp.push_back(value);
		}
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