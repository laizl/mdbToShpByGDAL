#include <ogr_spatialref.h>
#include <ogrsf_frmts.h>
#include <gdal.h>
#include <gdal_priv.h>
#include <cpl_string.h>


#include "autotracktool.h"

autoTracktool::autoTracktool() {

}
int autoTracktool::exportPolygonToShpFile(std::vector<std::string> tableNames, std::string fileName, std::vector<std::vector<double> > polygon2DPointList, std::vector<std::vector<std::string> > tableValues)
{
	const char *pszDriverName = "ESRI Shapefile";
	GDALDriver *gdalDriver;

	OGRRegisterAll();
	//CPLSetConfigOption("GDAL_DATA", "./data/gdaldata");

	CPLSetConfigOption("GDAL_FILENAME_IS_UTF8","YES");
	//CPLSetConfigOption("SHAPE_ENCODING","");
	CPLSetConfigOption("SHAPE_ENCODING","CP936");

	gdalDriver = (OGRSFDriverRegistrar::GetRegistrar()->GetDriverByName(pszDriverName));
	//create shapefile
	GDALDataset* poDstDS = gdalDriver->Create(fileName.c_str(), 0, 0, 0, GDT_Unknown, NULL);
	if (poDstDS == NULL)
	{
		printf("Creation of output file failed.\n");
		return 0;
	}

	//步骤2、创建空间坐标系
	//原始栅格影响空间参考信息直接赋值给矢量文件

	//const char *WKT = poDstDS->GetProjectionRef();
	OGRSpatialReference* oSRS = new OGRSpatialReference("");
	oSRS->SetWellKnownGeogCS("WGS84");

	OGRLayer *poLayer;
	//poLayer = poDstDS->CreateLayer("polygon_out", NULL, wkbPolygon, NULL);
	poLayer = poDstDS->CreateLayer("point_out", oSRS, wkbPoint, NULL);

	if (poLayer == NULL)
	{
		printf("Layer creation failed.\n");
		return 0;
	}
	//将属性字段进行填写
	for (int i = 0; i < tableNames.size(); i++) {
		//printf("%s\n", tableNames[i].c_str());
		//std::cout<<tableNames.at(i)<<std::endl;
		//OGRFieldDefn Field((char*)tableNames[i].data(), OFTString);
		OGRFieldDefn Field(tableNames.at(i).c_str(), OFTString);
		Field.SetWidth(128);
		poLayer->CreateField(&Field);
	}
	//return 0;
	OGRPolygon ogrPolygon;
	//创建一个OGRPoint对象
	std::cout <<"PointList size:" <<polygon2DPointList.size() << std::endl;
	for (int i = 0; i < polygon2DPointList.size(); i++) {
		OGRFeature *poFeature;
		poFeature = OGRFeature::CreateFeature(poLayer->GetLayerDefn());
		for (int j = 0; j < tableNames.size(); j++)
		{
				//std::cout << j << ":" << tableValues[i].size() << std::endl;
//std::cout << 1 << ":" << tableNames[j] << std::endl;
//std::cout << 2 << ":" << tableValues[i][j] << std::endl;
			poFeature->SetField(tableNames[j].data(), tableValues[i][j].data());
			//std::cout << tableValues[i][j].data() << std::endl;
		}
		//设置点的坐标
		//std::cout<<"cart_x:"<<polygon2DPointList.at(i)[0]<<std::endl;
		//std::cout<<"cart_y:"<<polygon2DPointList.at(i)[1]<<std::endl;
		if( polygon2DPointList.at(i)[0]+polygon2DPointList.at(i)[1] > 720 )
		{
			OGRSpatialReference spatialReference;
			spatialReference.importFromEPSG(2383);//XIAN80
			//spatialReference.SetTM(0, 114, 1.0, 38500000, 0);

			OGRSpatialReference* pLonLat = spatialReference.CloneGeogCS();
			//OGRCoordinateTransformation* LonLat2XY = OGRCreateCoordinateTransformation(pLonLat, &spatialReference);
			OGRCoordinateTransformation* LonLat2XY = OGRCreateCoordinateTransformation(&spatialReference, pLonLat);

			double x = polygon2DPointList.at(i)[0];
			double y = polygon2DPointList.at(i)[1];
			// double cart_x = 38465252.023887850;
			// double cart_y = 4296379.277210157;
			
			//printf("平面坐标:%.9lf\t%.9lf\n", x, y);
			if (!LonLat2XY->Transform(1, &x, &y))
			{
				return 1;
			}
			polygon2DPointList.at(i)[0] = x;
			polygon2DPointList.at(i)[1] = y;
			OGRCoordinateTransformation::DestroyCT(LonLat2XY);
			LonLat2XY = NULL;
		}
		//std::cout<<"geo_x:"<<polygon2DPointList.at(i)[0]<<std::endl;
		//std::cout<<"geo_y:"<<polygon2DPointList.at(i)[1]<<std::endl;
		OGRPoint point;
		point.setX(polygon2DPointList.at(i)[0]);
		point.setY(polygon2DPointList.at(i)[1]);
		poFeature->SetGeometry(&point);

		if (poLayer->CreateFeature(poFeature) != OGRERR_NONE)
	    	{
			printf("Failed to create feature in shapefile.\n");
		    	return 0;
		}
		OGRFeature::DestroyFeature(poFeature);
	}
	GDALClose(poDstDS);
	return 1;
}

int autoTracktool::createShpFileTest()
{
	const char *pszDriverName = "ESRI Shapefile";
	OGRSFDriver *poDriver;
	GDALDriver *gdalDriver;

	OGRRegisterAll();

	GDALAllRegister();
	
	CPLSetConfigOption("GDAL_FILENAME_IS_UTF8", "NO");

	CPLSetConfigOption("SHAPE_ENCODING", "");

	//CPLSetConfigOption("SHAPE_ENCODING", "CP936");


	gdalDriver = (OGRSFDriverRegistrar::GetRegistrar()->GetDriverByName(pszDriverName));

	//根据文件名创建输出矢量文件
	GDALDataset* poDstDS = gdalDriver->Create("E:\\test\\polygon.shp", 0, 0, 0, GDT_Unknown, NULL); //创建shp文件
	if (poDstDS == NULL)
	{
		printf("Creation of output file failed.\n");
		return 0;
	}
	OGRLayer *poLayer;
	//poLayer = poDstDS->CreateLayer("point_out", NULL, wkbPoint, NULL); 
	poLayer = poDstDS->CreateLayer("point_out", NULL, wkbPolygon, NULL);
	if (poLayer == NULL)
	{
		printf("Layer creation failed.\n");
		return 0;
	}
	OGRFieldDefn idField("ID", OFTReal);
	OGRFieldDefn firstField("NAME", OFTInteger);
	OGRFieldDefn secondField("X", OFTReal);
	OGRFieldDefn thirdField("Y", OFTReal);
	idField.SetWidth(32);
	firstField.SetWidth(32);
	secondField.SetWidth(32);
	thirdField.SetWidth(32);
	poLayer->CreateField(&idField);
	poLayer->CreateField(&firstField);
	poLayer->CreateField(&secondField);
	poLayer->CreateField(&thirdField);


	int x, y;
	int a = 10, b = 100;
	OGRPolygon ogrPolygon;
	OGRLinearRing ring;
	OGRFeature *poFeature;
	poFeature = OGRFeature::CreateFeature(poLayer->GetLayerDefn());
	ring.addPoint(0, 0);
	ring.addPoint(0, 30);
	ring.addPoint(30, 30);
	ring.addPoint(30, 0);
	ring.closeRings();//首尾点重合形成闭合环  
	OGRPolygon polygon;
	polygon.addRing(&ring);
	poFeature->SetGeometry(&polygon);
	if (poLayer->CreateFeature(poFeature) != OGRERR_NONE)
	{
		printf("Failed to create feature in shapefile.\n");
		return 0;
	}
	OGRFeature::DestroyFeature(poFeature);
	GDALClose(poDstDS);
	return 1;
}
