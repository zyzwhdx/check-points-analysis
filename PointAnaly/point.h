#ifndef _POINT_H_
#define _POINT_H_

#include <vector>
#include <string>
using namespace std;

struct zPoint
{
	double x;
	double y;
};

struct zSegment
{
	zPoint src;
	zPoint dst;
};

struct ePoint
{
	int id;
	double x;
	double y;
	double z;
	ePoint()
	{
		x = y = z = 0.0;
		id = 0;
	}
};

//一串边缘点定义一个string
typedef vector<ePoint> eString;

//一串string定义一个edge
typedef vector<eString> eEdge;

void readVertices(string path, vector<vector<zPoint> > &vertices, vector<vector<zSegment> > &segments, double &ox, double &oy);

void readCheckPoints(string path, vector<zPoint> &checkpts, double ox, double oy);

double ccltDistancePt2Sg(zPoint p, zPoint m, zPoint n);

void ccltCheckDistances(vector<zPoint> checks, vector<vector<zSegment> > segments, vector<vector<double> > &distances);

double ccltMaxOffset(eString &pts, int &pos_maxoft);

void readEdges(string path, eString &pts, double &ox, double &oy);

void iterativeBreakLines(eString &oPts, eEdge &whole_edge);

bool compy(ePoint &a, ePoint &b);

bool compid(ePoint &a, ePoint &b);

int zrand();

#endif