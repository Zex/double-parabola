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
    unsigned int points_count_;
    float a_, b_, c_; 
    Points curve_points_;
    CvPoint start_, end_, vertex_;

public:

    SingleParabola();

    SingleParabola(CvPoint start, CvPoint end, CvPoint vertex, int step = 1); 

    ~SingleParabola()
    {   
    }   

    void StepX(int step)
    {   
        step_x_ = step;
    }
    
    Points& CurvePoints()
    {
		Points &ret = curve_points_;
        return ret;
    }

    CvPoint StartPt()
    {
        return start_;
    }

    CvPoint EndPt()
    {
        return end_;
    }

    CvPoint VertexPt()
    {
        return vertex_;
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

    Points FullCurve()
    {
        return CurvePoints();
    }
};

#endif /* __DOUBLE_PARABOLA_H_ */
