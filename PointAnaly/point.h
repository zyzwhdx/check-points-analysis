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

void readVertices(string path, vector<vector<zPoint> > &vertices, vector<vector<zSegment> > &segments, double &ox, double &oy);

void readCheckPoints(string path, vector<zPoint> &checkpts, double ox, double oy);

double ccltDistancePt2Sg(zPoint p, zPoint m, zPoint n);

void ccltCheckDistances(vector<zPoint> checks, vector<vector<zSegment> > segments, vector<vector<double> > &distances);