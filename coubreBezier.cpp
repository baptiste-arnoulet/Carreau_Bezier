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
    int n = 3;
    QVector<Point> *res = new QVector<Point>;

    for (int i = 1; i < nbPoint-1; i++) {
        t = (float)i/(float)nbPoint;

        float * coord = new float[3];
        coord[0] = 0.0;
        coord[1] = 0.0;
        coord[2] = 0.0;

        for (int j = 0; j <= n; j++) {
            coord[0] += berstein(j,n,t) * listPoint.points[j].getX();
            coord[1] += berstein(j,n,t) * listPoint.points[j].getY();
            coord[2] += berstein(j,n,t) * listPoint.points[j].getZ();
        }

        Point p;
        p.set(coord);

        res->append(p);

        t = (float)(i+1)/(float)nbPoint;

        coord[0] = 0.0;
        coord[1] = 0.0;
        coord[2] = 0.0;

        for (int j = 0; j <= n; j++) {
            coord[0] += berstein(j,n,t) * listPoint.points[j].getX();
            coord[1] += berstein(j,n,t) * listPoint.points[j].getY();
            coord[2] += berstein(j,n,t) * listPoint.points[j].getZ();
        }

        Point p2;
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

    for (int i = 1; i < nbPoint; i++) {
        u = ((float)i/(float)nbPoint);

        for (int j = 1; j < nbPoint; j++) {
            v = ((float)j/(float)nbPoint);


            if (i < nbPoint-1) {

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


                float u_bis = ((float)(i+1)/(float)nbPoint);

                coord[0] = 0.0;
                coord[1] = 0.0;
                coord[2] = 0.0;

                for (int h = 0; h <= n; h++) {
                    for (int k = 0; k <= m; k++) {
                        coord[0] += berstein(h,n,u_bis) * berstein(k,m,v) * getCarrpoint(h,k).getX();
                        coord[1] += berstein(h,n,u_bis) * berstein(k,m,v) * getCarrpoint(h,k).getY();
                        coord[2] += berstein(h,n,u_bis) * berstein(k,m,v) * getCarrpoint(h,k).getZ();
                    }
                }

                Point p2;
                p2.set(coord);

                res->append(p2);

            }

            if (j < nbPoint-1) {

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

                float v_bis = ((float)(j+1)/(float)nbPoint);

                coord[0] = 0.0;
                coord[1] = 0.0;
                coord[2] = 0.0;

                for (int h = 0; h <= n; h++) {
                    for (int k = 0; k <= m; k++) {
                        coord[0] += berstein(h,n,u) * berstein(k,m,v_bis) * getCarrpoint(h,k).getX();
                        coord[1] += berstein(h,n,u) * berstein(k,m,v_bis) * getCarrpoint(h,k).getY();
                        coord[2] += berstein(h,n,u) * berstein(k,m,v_bis) * getCarrpoint(h,k).getZ();
                    }
                }

                Point p2;
                p2.set(coord);

                res->append(p2);

            }

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
