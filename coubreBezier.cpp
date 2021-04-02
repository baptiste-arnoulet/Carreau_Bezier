#include <QDebug>
#include <cmath>

#include "coubreBezier.h"

CourbeBezier::CourbeBezier(Point p0, Point p1, Point p2, Point p3)
{
    listPoint.points[0] = p0;
    listPoint.points[1] = p1;
    listPoint.points[2] = p2;
    listPoint.points[3] = p3;

    nbPoint = 50;
}

CourbeBezier::CourbeBezier(Point points[])
{
    for (int i =0; i<16; i++) {
        listCarrPoint.points[i] = points[i];
    }

    nbPoint = 50;
}

QVector<Point> *CourbeBezier::parcoursBerstein()
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

QVector<Point> *CourbeBezier::parcoursCarreauBerstein()
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

float CourbeBezier::berstein(int i, int n, float t)
{
    return ((facto(n)/(facto(i)*facto(n-i))) * expo(t,i) * expo((1-t),(n-i)));
}

Point CourbeBezier::getCarrpoint(int i, int j)
{
    int index = i + (j*4);
    return listCarrPoint.points[index];
}
