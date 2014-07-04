/*
 * DoubleParabola.cpp
 *
 * Author: Zex <top_zlynch@yahoo.com>
 */
#include "DoubleParabola.h" 

SingleParabola::SingleParabola()
{
}

SingleParabola::SingleParabola(CvPoint start, CvPoint end, CvPoint vertex, int step)
: a_(0.0), b_(0.0), c_(0.0), step_x_(step)
{
	start_ = start;
	end_ = end;
	vertex_ = vertex;
	points_count_ = (end_.x - start_.x)/step_x_;
	curve_points_.resize(points_count_);

	curve_points_[0].x = start_.x;
	curve_points_[0].y = start_.y;
	curve_points_[curve_points_.size()-1].x = end_.x;
	curve_points_[curve_points_.size()-1].y = end_.y;
}

float* SingleParabola::ParabConst()
{
	float *ret = new float[3];

	ret[0] = a_;
	ret[1] = b_;
	ret[2] = c_;

	return ret;
}

void SingleParabola::CalcParabConst(CvPoint start, CvPoint end)
{
	a_ = (start.y-vertex_.y)/pow((float)(end.x-vertex_.x), 2);
	b_ = -a_ * (end.x-start.x);
	c_ = 0;//y-start.y;
}

void SingleParabola::GenParabPoints(CvPoint start, CvPoint end, int &index)
{
	if (2 > curve_points_.size()) return;

	while (index < points_count_-1 && curve_points_[index-1].x <= end.x)
	{
		curve_points_[index].x = curve_points_[index-1].x + step_x_;
		float ix = curve_points_[index].x - start.x;
		curve_points_[index].y = start.y + (a_*ix*ix + b_*ix + c_);
	    index++;
	}
}

DoubleHalfParabola::DoubleHalfParabola(CvPoint start, CvPoint end, CvPoint vertex, int step)
: SingleParabola(start, end, vertex, step)
{
	VertexPt(vertex);
}

void DoubleHalfParabola::GenFullCurve()
{
	int index = 1;

	CalcParabConst(StartPt(), mid1_);
	GenParabPoints(StartPt(), base::VertexPt(), index);

	CalcParabConst(mid2_, EndPt());
	GenParabPoints(mid2_, EndPt(), index);
}

void DoubleHalfParabola::VertexPt(CvPoint vertex)
{
	mid1_.x = vertex.x + vertex.x - StartPt().x;
	mid1_.y = StartPt().y;

	mid2_.x = vertex.x - (EndPt().x - vertex.x);
	mid2_.y = StartPt().y;

	base::VertexPt(vertex);
}

//// ----------test---------->
//IplImage *image = 0;
//
//void on_mouse(int event, int x, int y, int flags, void *param) 
//{
//	DoubleHalfParabola *dhp = (DoubleHalfParabola*)param;//(cvPoint(200, 200), cvPoint(500, 200), cvPoint(x, y), 10);
//	dhp->VertexPt(cvPoint(x, y)); 
//
//	if (event == CV_EVENT_MOUSEMOVE) //修改控制点坐标 
//	{
//		cvZero(image);
//
//		dhp->GenFullCurve();
//
//		for (Points::iterator it = dhp->CurvePoints().begin(); it != dhp->CurvePoints().end(); it++)
//		{
//			cvCircle(image, *it, 0, CV_RGB(150,100,100), 1);
//		}
//	
//	    cvLine(image, cvPoint(x-10, y-10), cvPoint(x+10, y+10), CV_RGB(150,100,100), 1, 8, 0); 
//	    cvLine(image, cvPoint(x+10, y-10), cvPoint(x-10, y+10), CV_RGB(150,100,100), 1, 8, 0);
//	}  
//	else if (event == CV_EVENT_LBUTTONDOWN)
//	{
//	}
//
//	cvShowImage("Tinny Tiger", image); 
//} 
// 
//int main(int argc, char* argv[]) 
//{ 
//	DoubleHalfParabola *dhp = new DoubleHalfParabola(cvPoint(200, 200), cvPoint(500, 200), cvPoint(250, 200), 10);
//	CvSize image_sz = cvSize(1000,1000);  
//	image = cvCreateImage(image_sz , 8, 3); 
//	cvNamedWindow("Tinny Tiger", CV_WINDOW_AUTOSIZE); 
//	cvSetMouseCallback("Tinny Tiger", on_mouse, dhp); 
//	//cvResizeWindow("Tinny Tiger",500,500); 
// 
//	cvShowImage("Tinny Tiger", image); 
//	cvWaitKey(0);
//
//	delete dhp; 
//	return 0; 
//}
//// ----------test----------|
 
//-----------------------------
//	float a = (start.y-y)/pow((float)(end.x-x), 2);
//	float b = -a * (end.x-start.x);
//	float c = 0;//y-start.y;
//	while (i < ptc-1 && pts[i-1].x <= end.x)
//	{
//		pts[i].x = pts[i-1].x+step_x;
//		float ix = pts[i].x-start.x;
//		pts[i].y = start.y+(a*ix*ix + b*ix + c);
//	    i++;
//	}
//-----------------------------
//	int step_x = 10, i = 1;
//	int ptc = (end_x-start_x)/step_x;
//	Points pts(ptc);
//
//	pts[0].x = start_x;
//	pts[0].y = start_y;
//	pts[pts.size()-1].x = end_x;
//	pts[pts.size()-1].y = start_y;
//
//	int mid1_x = x + x - start_x, mid1_y = start_y;
//	int mid2_x = x - (end_x - x), mid2_y = start_y;
//	float a, b, c;
//
//	a = (start_y-y)/pow((float)(mid1_x-x), 2);
//	b = -a * (mid1_x-start_x);
//	c = 0;
//
//	while (i < ptc-1 && pts[i-1].x <= x)
//	{
//		pts[i].x = pts[i-1].x+step_x;
//		float ix = pts[i].x-start_x;
//		pts[i].y = start_y+(a*ix*ix + b*ix + c);
//	    i++;
//	}
////-----------------------------
//	a = (mid2_y-y)/pow((float)(end_x-x), 2);
//	b = -a * (end_x-mid2_x);
//	c = 0;
//
//	while (i < ptc-1 && pts[i-1].x <= end_x)
//	{
//		pts[i].x = pts[i-1].x+step_x;
//		float ix = pts[i].x-mid2_x;
//		pts[i].y = mid2_y+(a*ix*ix + b*ix + c);
//	    i++;
//	}
////-----------------------------
