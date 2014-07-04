/*
 * DoubleParabola.h
 *
 * Author: Zex <top_zlynch@yahoo.com>
 */
#ifndef __DOUBLE_PARABOLA_H_
#define __DOUBLE_PARABOLA_H_

#include <cxcore.h> 
#include <cv.h> 
#include <highgui.h> 
 
typedef std::vector<CvPoint> Points;
//typedef std::vector<int, int> PointsMap;

#define swap_int(a, b) { a ^= b; b ^= a; a ^= b; }

class SingleParabola
{
    int step_x_;
    float a_, b_, c_; 
    Points curve_points_;
    CvPoint start_, end_, vertex_;

public:

    SingleParabola();

    SingleParabola(CvPoint start, CvPoint end, CvPoint vertex, int step = 1); 

    ~SingleParabola()
    {   
    }   

	unsigned int Resize();

    void StepX(int step)
    {   
        step_x_ = step;
		Resize();
    }

	int StepX()
	{
		return step_x_;
	}
    
    Points& CurvePoints()
    {
		Points &ret = curve_points_;
        return ret;
    }

    void StartPt(CvPoint start)
	{
		start_ = start;
		Resize();
	}

    CvPoint& StartPt()
    {
		CvPoint &ret = start_;
        return ret;
    }

	void EndPt(CvPoint end)
	{
		end_ = end;
		Resize();
	}

    CvPoint& EndPt()
    {
		CvPoint &ret = end_;
        return ret;
    }

    CvPoint& VertexPt()
    {
		CvPoint &ret = vertex_;
        return ret;
    }

    CvPoint VertexPt(CvPoint vertex)
	{
		vertex_ = vertex;
	}

    float* ParabConst();
    void CalcParabConst(CvPoint start, CvPoint end);
    void GenParabPoints(CvPoint start, CvPoint end, int &index);
};

class DoubleHalfParabola : public SingleParabola
{
    CvPoint mid1_, mid2_;

public:
	typedef SingleParabola base;

    DoubleHalfParabola(CvPoint start, CvPoint end, CvPoint vertex, int step = 1);

    ~DoubleHalfParabola()
    {
    }

    void GenFullCurve();

	void VertexPt(CvPoint vertex);
	CvPoint VertexPt()
	{
		return base::VertexPt();
	}

    Points FullCurve()
    {
        return CurvePoints();
    }
};

//------------------------------------------------------------------

class FlexablePainter : public DoubleHalfParabola
{
	IplImage *image_;

public:
	typedef DoubleHalfParabola base;

	FlexablePainter(IplImage *image, CvPoint start, CvPoint end, CvPoint vertex, int step_x = 1)
	: DoubleHalfParabola(start, end, vertex, step_x), image_(image)
	{
	}

	~FlexablePainter()
	{
	}

	void VertexPt(CvPoint vertex);
	void PaintFlexable();
	void PaintCross(CvPoint center);
};



#endif /* __DOUBLE_PARABOLA_H_ */
