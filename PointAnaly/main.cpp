#include <liblas\liblas.hpp>
#include <fstream>
#include <iostream>
#include <string>
#include "LasIO.h"
#include <pcl/io/pcd_io.h>
#include <pcl/point_types.h>
#include <pcl/visualization/cloud_viewer.h>
#include <pcl/filters/passthrough.h>
#include <pcl/io/pcd_io.h>
#include <pcl/point_cloud.h>
#include <pcl/kdtree/kdtree_flann.h>
#include <vector>
#include "dataIo.h"
#include <pcl/sample_consensus/method_types.h>
#include <pcl/sample_consensus/model_types.h>
#include <pcl/segmentation/sac_segmentation.h>
#include <pcl/filters/extract_indices.h>
#include "point.h"

using namespace pcl;
using namespace std;

void main()
{
	//LasIO las;
	//pcl::PointCloud<pcl::PointXYZI> PCcloud = las.transToPcd(0);
	////采用深层复制，所以从此PCcloud作废，以下均使用智能指针
	//MinBound bound = las.getMinBound();
	//pcl::PointCloud<pcl::PointXYZI>::ConstPtr cloud = PCcloud.makeShared();

	vector<vector<zPoint> > vertices;//边缘顶点们，每个环一个vector
	vector<vector<zSegment> > segments;//边缘线段们，每个环一个vector
	vector<zPoint> checks;   //二维的检查点们
	vector<vector<double> > distances;//检查点到所有边的距离
	double ox, oy;
	readVertices("D:\\exp\\h4\\vertices.txt", vertices, segments, ox, oy);

	readCheckPoints("D:\\exp\\h4\\CheckEdge.txt", checks, ox, oy);

	ccltCheckDistances(checks, segments, distances);

	for (int i = 0; i < checks.size(); i++)
	{
		cout << distances[i][0] << endl;
	}

	ofstream ofs("check.txt");
	for (int i = 0; i < checks.size(); i++)
	{
		ofs << to_string(ox + checks[i].x) << "," << to_string(oy + checks[i].y) << "," << to_string(100 * distances[i][0]) << endl;
	}
	ofs.close();

	cout << "end of program" << endl;
	cin.get();
	cin.get();
}