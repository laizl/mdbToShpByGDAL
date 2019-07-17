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

#include <mdbtools.h>


#include "NtAccessManager.h"

#undef MDB_BIND_SIZE
#define MDB_BIND_SIZE 200000

#define is_quote_type(x) (x==MDB_TEXT || x==MDB_OLE || x==MDB_MEMO || x==MDB_DATETIME || x==MDB_BINARY || x==MDB_REPID)
#define is_binary_type(x) (x==MDB_OLE || x==MDB_BINARY || x==MDB_REPID)

NtAccessManager::NtAccessManager()
{
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
	mdb = mdb_open (mdbFileName.c_str(), MDB_NOFLAGS);
	if (!mdb)
	{
		exit(1);
	}
	table = mdb_read_table_by_name (mdb, "ZKFC", MDB_TABLE);
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

		for (i=0; i<table->num_cols; i++) {
			col=g_ptr_array_index(table->columns,i);
			tempFieldName = col->name;
			_fieldNames.push_back(tempFieldName);
			//if (i)
			//	fputs(delimiter, outfile);
			//fputs(col->name, outfile);
		}
		//fputs(row_delimiter, outfile);

		// fprintf (stdout, "/******************************************************************/\n");
		// fprintf (stdout, "/* THIS IS AN AUTOMATICALLY GENERATED FILE.  DO NOT EDIT IT!!!!!! */\n");
		// fprintf (stdout, "/******************************************************************/\n");
		// fprintf (stdout, "\n");
		// fprintf (stdout, "#include <stdio.h>\n");
		// fprintf (stdout, "#include \"types.h\"\n");
		// fprintf (stdout, "#include \"dump.h\"\n");
		// fprintf (stdout, "\n");
		// fprintf (stdout, "const %s %s_array [] = {\n", "ZKFC", "ZKFC");

		count = 0;
		started = 0;
		while (mdb_fetch_row (table)) 
		{
			if (started != 0)
			{
			fprintf (stdout, ",\n");
			}
			started = 1;
			fprintf (stdout, "{\t\t\t\t/* %6d */\n\t", count);
			for  (j = 0; j < table->num_cols; j++) 
			{
				fprintf (stdout, "\t");
				col = (MdbColumn *)g_ptr_array_index (table->columns, j);
				if  (quote_text && 
					(col->col_type == MDB_TEXT ||
					col->col_type == MDB_MEMO)) 
				{
					fprintf (stdout, "\"%s\"", bound_values [j]);
				} 
				else 
				{
					fprintf (stdout, "%s", bound_values [j]);
				}
				if (j != table->num_cols - 1)
				{
					fprintf (stdout, "%s\n", delimiter);
				}
				else
				{
					fprintf (stdout, "\n");
				}
			}
			fprintf (stdout, "}");
			count++;
		}
		fprintf (stdout, "\n};\n\n");

		for(j = 0; j < table->num_cols; j++) 
		{
			g_free (bound_values [j]);
		}
		mdb_free_tabledef(table);
	}
	 
	mdb_close (mdb);
	fprintf (stdout, "const int %s_array_length = %d;\n", "ZKFC", count);
}
