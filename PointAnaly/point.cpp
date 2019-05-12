#include "point.h"
#include <fstream>
#include <string>
#include <vector>
#include <iostream>
#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/split.hpp>
using namespace std;

void readVertices(string path, vector<vector<zPoint> > &vertices, vector<vector<zSegment> > &segments, double &ox, double &oy)
{
	vertices.clear();
	segments.clear();
	ifstream ifs(path);
	string str;
	vector<string> vec;

	//第一行
	getline(ifs, str);
	boost::split(vec, str, boost::is_any_of(", "), boost::token_compress_on);
	ox = atof(vec[0].c_str());
	oy = atof(vec[1].c_str());

	//第二行
	getline(ifs, str);
	int num = atoi(str.c_str());
	vertices.resize(num);
	segments.resize(num);

	//后面的若干行
	while (getline(ifs, str))
	{
		vector<string> sVec;
		boost::split(sVec, str, boost::is_any_of(", "), boost::token_compress_on);

		int cLabel = atoi(sVec[3].c_str());
		zPoint pt;
		pt.x = atof(sVec[0].c_str());
		pt.y = atof(sVec[1].c_str());
		vertices[cLabel].push_back(pt);
	}

	for (int i = 0; i < vertices.size(); i++)
	{
		segments[i].resize(vertices[i].size());
		for (int j = 0; j < segments[i].size() - 1; j++)
		{
			segments[i][j].src = vertices[i][j];
			segments[i][j].dst = vertices[i][j + 1];
		}
		segments[i][segments[i].size() - 1].src = vertices[i][segments[i].size() - 1];
		segments[i][segments[i].size() - 1].dst = vertices[i][0];
	}
}

void readCheckPoints(string path, vector<zPoint> &checkpts, double ox, double oy)
{
	checkpts.clear();
	ifstream ifs(path);
	string str;
	
	while (getline(ifs, str))
	{
		vector<string> sVec;
		boost::split(sVec, str, boost::is_any_of(", "), boost::token_compress_on);

		zPoint pt;
		pt.x = atof(sVec[0].c_str()) - ox;
		pt.y = atof(sVec[1].c_str()) - oy;
		checkpts.push_back(pt);
	}
}

double ccltDistancePt2Sg(zPoint p, zPoint m, zPoint n)
{
	//pm点乘mn
	double pmDmn = (m.x - p.x)*(n.x - m.x) + (m.y - p.y)*(n.y - m.y);
	//pn点乘mn
	double pnDmn = (n.x - p.x)*(n.x - m.x) + (n.y - p.y)*(n.y - m.y);

	//pm边长
	double pm = sqrt((m.x - p.x)*(m.x - p.x) + (m.y - p.y)*(m.y - p.y));
	//pn边长
	double pn = sqrt((n.x - p.x)*(n.x - p.x) + (n.y - p.y)*(n.y - p.y));
	//mn边长
	double mn = sqrt((n.x - m.x)*(n.x - m.x) + (n.y - m.y)*(n.y - m.y));


	double small_edge = pm;
	if (pn < pm)
	{
		small_edge = pn;
	}

	if (pmDmn * pnDmn >= 1e-6)
	{
		return small_edge;
	}
	else
	{
		double x1 = m.x;
		double y1 = m.y;
		double x2 = n.x;
		double y2 = n.y;
		double x3 = p.x;
		double y3 = p.y;
		double s = fabs(x1*y2 + x2*y3 + x3*y1 - x1*y3 - x2*y1 - x3*y2);
		return fabs(s / mn);
	}
}

void ccltCheckDistances(vector<zPoint> checks, vector<vector<zSegment> > segments, vector<vector<double> > &distances)
{
	distances.clear();
	distances.resize(checks.size());
	for (int i = 0; i < checks.size(); i++)
	{
		zPoint ckPoint = checks[i];
		for (int m = 0; m < segments.size();m++)
		{
			for (int j = 0; j < segments[m].size(); j++)
			{
				double distance = ccltDistancePt2Sg(ckPoint, segments[m][j].src, segments[m][j].dst);
				distances[i].push_back(distance);
			}
		}
		sort(distances[i].begin(), distances[i].end());
	}
}

void readEdges(string path, eString &pts, double &ox, double &oy)
{
	ifstream ifs(path);
	string str;

	while (getline(ifs, str))
	{
		vector<string> sVec;
		boost::split(sVec, str, boost::is_any_of(", "), boost::token_compress_on);

		ePoint pt;
		pt.x = atof(sVec[0].c_str()) - ox;
		pt.y = atof(sVec[1].c_str()) - oy;
		pt.z = atof(sVec[2].c_str());
		pts.push_back(pt);
	}

	//按y值从小到大排序
	sort(pts.begin(), pts.end(), compy);

	for (size_t i = 0; i < pts.size(); i++)
	{
		pts[i].id = i;
	}
}

bool compy(ePoint &a, ePoint &b)
{
	if (a.y < b.y)
	{
		return true;
	}
	return false;
}

bool compid(ePoint &a, ePoint &b)
{
	if (a.id < b.id)
	{
		return true;
	}
	return false;
}

void iterativeBreakLines(eString &oPts, eEdge &whole_edge)
{
	if (oPts.size() < 30)
	{
		whole_edge.push_back(oPts);
		return;
	}
	int pos;
	if (ccltMaxOffset(oPts, pos) < 30)
	{
		whole_edge.push_back(oPts);
		return;
	}

	eString a;
	eString b;
	for (size_t i = 0; i <= pos - oPts.front().id; i++)
	{
		a.push_back(oPts[i]);
	}
	for (size_t i = pos - oPts.front().id + 1; i < oPts.size(); i++)
	{
		b.push_back(oPts[i]);
	}
	iterativeBreakLines(a, whole_edge);
	iterativeBreakLines(b, whole_edge);
}


double ccltMaxOffset(eString &pts, int &pos_maxoft)
{
	if (pts.size() < 4)
		return 0.0;

	sort(pts.begin(), pts.end(), compid);

	zPoint source;
	zPoint target;

	source.x = pts[0].x;
	source.y = pts[0].y;
	target.x = pts[pts.size() - 1].x;
	target.y = pts[pts.size() - 1].y;

	double max_ost = 0.0;
	int max_id = 0;

	for (size_t i = 1; i < pts.size() - 1; i++)
	{
		zPoint cPt;
		cPt.x = pts[i].x;
		cPt.y = pts[i].y;
		double distance = ccltDistancePt2Sg(cPt, source, target);
		if (distance > max_ost)
		{
			max_ost = distance;
			max_id = pts[i].id;
		}
	}
	pos_maxoft = max_id;
	return max_ost;
}

int zrand()
{
	srand(int(time(0)));
	return rand() % 99 + 1;
}