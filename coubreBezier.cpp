#include <QDebug>
#include <cmath>

#include "coubreBezier.h"

CourbeParamtrique::CourbeParamtrique(Point p0, Point p1, Point p2, Point p3) : Segment()
{
    listPoint.points[0] = p0;
    listPoint.points[1] = p1;
    listPoint.points[2] = p2;
    listPoint.points[3] = p3;
}

CourbeParamtrique::CourbeParamtrique(Point points[])
{
    for (int i =0; i<16; i++) {
        listCarrPoint.points[i] = points[i];
    }
}

QVector<Point> *CourbeParamtrique::parcours()
{
    float t;
    QVector<Point> *res = new QVector<Point>;

    for (int i = 0; i < nbPoint; i++) {
        t = (float)i/(float)nbPoint;
        res->append(bezierPoint(t, listPoint.points[0],listPoint.points[1],listPoint.points[2],listPoint.points[3] ));
    }

    return res;
}

QVector<Point> *CourbeParamtrique::parcoursBerstein()
{
    float t;
    QVector<Point> *res = new QVector<Point>;

    for (int i = 0; i < nbPoint; i++) {
        t = (float)i/(float)nbPoint;

        float * coord = new float[3];
        coord[0] = 0.0;
        coord[1] = 0.0;
        coord[2] = 0.0;

        for (int j = 0; j <= 3; j++) {
            coord[0] += berstein(j,3,t) * listPoint.points[j].getX();
            coord[1] += berstein(j,3,t) * listPoint.points[j].getY();
            coord[2] += berstein(j,3,t) * listPoint.points[j].getZ();
        }

        Point p;
        p.set(coord);

        res->append(p);
    }

    return res;
}

QVector<Point> *CourbeParamtrique::parcoursCarreauBerstein()
{
    float u,v;
    int n = 3, m = 3;
    QVector<Point> *res = new QVector<Point>;

    for (int i = 0; i <= nbPoint; i++) {
        u = ((float)i/(float)nbPoint);

        for (int j = 0; j <= nbPoint; j++) {
            v = ((float)j/(float)nbPoint);

            float * coord = new float[3];
            coord[0] = 0.0;
            coord[1] = 0.0;
            coord[2] = 0.0;

            for (int h = 0; h <= n; h++) {
                for (int k = 0; k <= m; k++) {
                    coord[0] += berstein(h,n,u) * berstein(k,m,v) * getCarrpoint(h,k).getX();
                    coord[1] += berstein(h,n,u) * berstein(k,m,v) * getCarrpoint(h,k).getY();
                    coord[2] += berstein(h,n,u) * berstein(k,m,v) * getCarrpoint(h,k).getZ();
                }
            }

            Point p;
            p.set(coord);

            res->append(p);

        }
    }

    return res;
}

float CourbeParamtrique::berstein(int i, int n, float t)
{
    return ((facto(n)/(facto(i)*facto(n-i))) * expo(t,i) * expo((1-t),(n-i)));
}

Point CourbeParamtrique::getCarrpoint(int i, int j)
{
    int index = i + (j*4);
    return listCarrPoint.points[index];
}

Point CourbeParamtrique::bezierPoint(float t, Point p0, Point p1, Point p2, Point p3)
{
    Point p;

    float * coord = new float[3];
    coord[0] = (1-t)*(1-t)*(1-t)*p0.getX()+3*t*(1-t)*(1-t)*p1.getX()+3*t*t*(1-t)*p2.getX()+t*t*t*p3.getX();
    coord[1] = (1-t)*(1-t)*(1-t)*p0.getY()+3*t*(1-t)*(1-t)*p1.getY()+3*t*t*(1-t)*p2.getY()+t*t*t*p3.getY();
    coord[2] = (1-t)*(1-t)*(1-t)*p0.getZ()+3*t*(1-t)*(1-t)*p1.getZ()+3*t*t*(1-t)*p2.getZ()+t*t*t*p3.getZ();

    p.set(coord);

    return p;
}

QVector<Point> *CourbeParamtrique::discretisation(float nbPoint)
{
    float pourcent = 100/nbPoint;
    qDebug() << "pourcent" << pourcent;

    for (int i = 0; i<nbPoint; i++) {
        Point p = getValue(i*pourcent);
        points->append(p);
    }

    return points;
}
