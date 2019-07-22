#include <iostream>

//#include <QVector>

//#include <QApplication>
//#include <QPluginLoader>
//#include <QDir>
//#include <QSqlDatabase>
//#include <QSqlQueryModel>
//#include <QSqlQuery> 
//#include <QSqlRecord>
//#include <QSqlField>
//#include <QVariant>

//#include "qt_windows.h"
#include <math.h>
#include <mdbtools.h>


#include "NtAccessManager.h"

#undef MDB_BIND_SIZE
#define MDB_BIND_SIZE 200000

#define is_quote_type(x) (x==MDB_TEXT || x==MDB_OLE || x==MDB_MEMO || x==MDB_DATETIME || x==MDB_BINARY || x==MDB_REPID)
#define is_binary_type(x) (x==MDB_OLE || x==MDB_BINARY || x==MDB_REPID)

NtAccessManager::NtAccessManager(std::string tableName,std::string latFieldString, std::string longFieldString)
{
	_tableName = tableName;
	_latFieldString = latFieldString;
	_longFieldString = longFieldString;
	//加载数据库驱动文件
	//QString path = QDir::currentPath();
	//path = path.append("/Release");
	//std::cout << path.toStdString() << std::endl;
	//QApplication::addLibraryPath(path + QString("/plugins"));
	//QPluginLoader loader(path + QString("/plugins/sqldrivers/qsqlodbc.dll"));
	
}

void NtAccessManager::testReadMdb(std::string mdbFileName){
	std::string tempFieldName;
	unsigned int j;
	MdbHandle *mdb;
	MdbTableDef *table;
	MdbColumn *col;
	/* doesn't handle tables > 256 columns.  Can that happen? */
	//char *bound_values [256];
	char **bound_values;
	int  *bound_lens; 
	char delimiter [] = ", ";
	char quote_text = 1;
	int count = 0;
	int started;

	int latIndex = -1;
	int longIndex = -1;
	mdb = mdb_open (mdbFileName.c_str(), MDB_NOFLAGS);
	if (!mdb)
	{
		exit(1);
	}
	table = mdb_read_table_by_name (mdb, (gchar* )_tableName.c_str(), MDB_TABLE);
	if (table)
	{
		mdb_read_columns (table);
		mdb_rewind_table (table);
		
		bound_values = (char **) g_malloc(table->num_cols * sizeof(char *));
		bound_lens = (int *) g_malloc(table->num_cols * sizeof(int));
		for (j = 0; j < table->num_cols; j++) 
		{
			bound_values [j] =  (char *) g_malloc (MDB_BIND_SIZE);
			bound_values [j] [0] = '\0';
			// mdb_bind_column (table, j+1, bound_values[j], NULL);
			mdb_bind_column(table, j+1, bound_values[j], &bound_lens[j]);
		}

		for (int i=0; i<table->num_cols; i++) {
			col = (MdbColumn *)g_ptr_array_index(table->columns,i);
			tempFieldName = col->name;
			_fieldNames.push_back(tempFieldName);
			if(tempFieldName == _latFieldString){
				latIndex = i;
			}
			if(tempFieldName == _longFieldString){
				longIndex = i;
			}
		}
	
		count = 0;
		started = 0;
		while (mdb_fetch_row (table)) 
		{
			std::vector<double> tempLatLongList;
			std::vector<std::string> temList;
			//fprintf (stdout, "{\t\t\t\t/* %6d */\n\t", count);
			for  (j = 0; j < table->num_cols; j++) 
			{
				tempFieldName = bound_values [j];
				if(j==latIndex){
					tempLatLongList.push_back(atof(tempFieldName.c_str()));
				}
				if(j==longIndex){
					tempLatLongList.push_back(atof(tempFieldName.c_str()));
				}
				temList.push_back(tempFieldName);
			}
			_latLongs.push_back(tempLatLongList);
			_dataValue.push_back(temList);
			count++;
		}

		for(j = 0; j < table->num_cols; j++) 
		{
			g_free (bound_values [j]);
		}
		mdb_free_tabledef(table);
	}
	 
	mdb_close (mdb);
}
