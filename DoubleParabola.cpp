/*
 * DoubleParabola.cpp
 *
 * Author: Zex <top_zlynch@yahoo.com>
 */
#include "DoubleParabola.h" 

SingleParabola::SingleParabola(CvPoint start, CvPoint end, CvPoint vertex, int step)
: a_(0.0), b_(0.0), c_(0.0), step_x_(step)
{
	start_ = start;
	end_ = end;
	vertex_ = vertex;

	Resize();
}

unsigned int SingleParabola::Resize()
{
	unsigned int sz = abs(end_.x - start_.x)+1;
	sz /= step_x_;

	sz = 3 > sz ? 3 : sz;

	curve_points_.resize(sz);

	curve_points_.at(0).x = start_.x;
	curve_points_.at(0).y = start_.y;
	curve_points_.at(curve_points_.size()-1).x = end_.x;
	curve_points_.at(curve_points_.size()-1).y = end_.y;

	return curve_points_.size();
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

void SingleParabola::GenParabPoints(CvPoint start, CvPoint end, unsigned int &index)
{
	if (2 > curve_points_.size()) return;

	while (index < curve_points_.size() && curve_points_[index-1].x <= end.x)
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

//void DoubleHalfParabola::PointsRotation(Points &dst_points, Points &src_points, float theta, CvPoint center)
//{
//	unsigned int point_number = dst_points.size();
//
//	for (unsigned int i = 0; i < point_number; i++)
//	{
//		int x = src_points[i].x - center.x;
//		int y = src_points[i].y - center.y;
//
//		int s = (int)(cos(theta)*x - sin(theta)*y);
//		int t = (int)(sin(theta)*x + cos(theta)*y);
//
//		dst_points[i].x = s + center.x;
//		dst_points[i].y = t + center.y;
//	}
//}

void DoubleHalfParabola::GenFullCurve()
{
	unsigned int index = 1;

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

Points DoubleHalfParabola::GetFixedCurve(float theta, CvPoint center)
{
	Points src = CurvePoints();
//	Points dst(src.size());
//
//	PointsRotation(dst, src, -theta, center);
//	PointsRotation(src, dst, theta, center);

	return src;
}


void FlexablePainter::VertexPt(CvPoint vertex)
{
	if (StartPt().x > vertex.x)
		vertex.x = StartPt().x;
	else if (EndPt().x < vertex.x)
		vertex.x = EndPt().x;

	base::VertexPt(vertex);
}

void FlexablePainter::PaintFlexable()
{
	if (!image_) return;

	GenFullCurve();
	Points pts = CurvePoints();

	for (Points::iterator it = pts.begin(); it != pts.end(); it++)
	{
		cvCircle(image_, *it, 1, CV_RGB(100,200,150), 2);
	}

	PaintCross(base::VertexPt());

//	Points dst(pts.size());
//	PointsRotation(dst, pts, 45, cvPoint(450, 700));
//
//	for (Points::iterator it = dst.begin(); it != dst.end(); it++)
//	{
//		cvCircle(image_, *it, 1, CV_RGB(100,150,210), 2);
//	}
//
//	PointsRotation(pts, dst, -30, cvPoint(450, 700));
//
//	for (Points::iterator it = pts.begin(); it != pts.end(); it++)
//	{
//		cvCircle(image_, *it, 1, CV_RGB(100,50,210), 2);
//	}
}

void FlexablePainter::PaintCross(CvPoint center)
{
    cvLine(image_, cvPoint(center.x-10, center.y-10), cvPoint(center.x+10, center.y+10), CV_RGB(150,100,100), 1, 8, 0); 
    cvLine(image_, cvPoint(center.x+10, center.y-10), cvPoint(center.x-10, center.y+10), CV_RGB(150,100,100), 1, 8, 0);
}

// ----------test---------->
//IplImage *image = 0;
//
//void on_mouse(int event, int x, int y, int flags, void *param) 
//{
//	FlexablePainter *fp = (FlexablePainter*)param;//(cvPoint(200, 200), cvPoint(500, 200), cvPoint(x, y), 10);
//	fp->VertexPt(cvPoint(x, y)); 
//
//	if (event == CV_EVENT_MOUSEMOVE) //淇敼鎺у埗鐐瑰潗鏍?
//	{
//		cvZero(image);
//		fp->PaintFlexable();
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
//	CvSize image_sz = cvSize(2000, 800);  
//	image = cvCreateImage(image_sz , 8, 3);
//
//	FlexablePainter *fp= new FlexablePainter(image, cvPoint(200, 800), cvPoint(700, 800), cvPoint(250, 800), 10);
//	cvNamedWindow("Tinny Tiger", CV_WINDOW_AUTOSIZE); 
//	cvSetMouseCallback("Tinny Tiger", on_mouse, fp); 
//	//cvResizeWindow("Tinny Tiger",500,500); 
// 
//	cvShowImage("Tinny Tiger", image); 
//	cvWaitKey(0);
//
//	delete fp;
//
//	return 0; 
//}
// ----------test----------|
 
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
