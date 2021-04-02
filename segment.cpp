#include <cmath>
#include <QDebug>

#include "segment.h"

Segment::Segment()
{
    nbPoint = 50;
	pointList = new Point[2];
    points = new QVector<Point>;
}

Segment::~Segment()
{
	delete [] pointList;
	pointList = nullptr;
}

Segment::Segment(const Segment & s)
{
	pointList = new Point[2];

	for (unsigned i=0; i<2; ++i)
		pointList[i] = s.pointList[i];

}

void Segment::setStart(const Point & p)
{
	pointList[0] = p;
}

void Segment::setEnd(const Point & p)
{
	pointList[1] = p;
}

void Segment::setN(unsigned r, const Point & p)
{
	if (r>1)
		r=1;
	pointList[r] = p;
}

Point Segment::getN(unsigned r) const
{
	if (r>1)
		r=1;

	return pointList[r];
}

Point Segment::getStart() const
{
	return getN(0);
}


Point Segment::getEnd() const
{
    return getN(1);
}

Point Segment::getValue(float p)
{
    if (p < 0 || p > 100) {
        qDebug() << "Mauvais P";
        float * coord = new float[3]{0.0 ,0.0, 0.0};

        Point res;
        res.set(coord);
        return res;
    } else {
        float lenghtX = getEnd().getX() - getStart().getX();
        float lenghtY = getEnd().getY() - getStart().getY();
        float lenghtZ = getEnd().getZ() - getStart().getZ();

        float coefX = lenghtX * (p/100);
        float coefY = lenghtY * (p/100);
        float coefZ = lenghtZ * (p/100);

        float * coord = new float[3]{getStart().getX() + coefX, getStart().getY() + coefY, getStart().getZ() + coefZ};
        Point res;
        res.set(coord);
        return res;
    }
}

QVector<Point> *Segment::discretisation()
{
    float pourcent = 100/nbPoint;

    for (int i = 0; i<nbPoint; i++) {
        Point p = getValue(i*pourcent);
        points->append(p);
    }

    return points;
}

QVector<Point> *Segment::parcours()
{
    float t;
    QVector<Point> *res = new QVector<Point>;

    for (int i = 0; i < nbPoint; i++) {
        t = (float)i/(float)nbPoint;
        res->append(points->value(i));
    }

    return res;
}

float Segment::length() const
{
	float res=0.0f;

	for (unsigned i=0; i<3; ++i)
		res += pow((pointList[1]).getN(i) - (pointList[0]).getN(i), 2.0f);

	return sqrt(res);
}

Segment& Segment::operator= (const Segment &s)
{
	for (unsigned i=0; i<2; ++i)
		pointList[i] = s.pointList[i];

return *this;
}

std::ostream& operator<<(std::ostream& out, const Segment& s)
{
	return out << s.pointList[0] << " -- " << s.pointList[1];
}
