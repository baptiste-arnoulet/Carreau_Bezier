#include <QDebug>
#include <cmath>

#include "coubreBezier.h"

CourbeBezier::CourbeBezier(Point p0, Point p1, Point p2, Point p3, int nbPoint)
{
    listPoint.points[0] = p0;
    listPoint.points[1] = p1;
    listPoint.points[2] = p2;
    listPoint.points[3] = p3;

    this->nbPoint = nbPoint;
}

CourbeBezier::CourbeBezier(QVector<Point> *points, int nbPoint, int n, int m)
{
    this->n = n;
    this->m = m;

    listCarrPoint.vecPoints = points;

    for (int i = 0; i<=n; i++) {
        border1.points[i] = listCarrPoint.vecPoints->value(i);
        border4.points[i] = listCarrPoint.vecPoints->value(i+(m*(n+1)));
    }

    for (int i = 0; i<=m; i++) {
        border2.points[i] = listCarrPoint.vecPoints->value(i*(n+1));
        border3.points[i] = listCarrPoint.vecPoints->value(n+(i*(n+1)));
    }

    listPoint = border1;

    this->nbPoint = nbPoint;
}

// Forme une liste de points afin de dessiner une courbe de Bezier
QVector<Point> *CourbeBezier::parcoursBerstein()
{
    float t;
    QVector<Point> *res = new QVector<Point>;

    for (int i = 0; i < nbPoint; i++) {
        if (i == 0) {
            res->append(listPoint.points[0]);

            t = (float)(i+1)/(float)nbPoint;

            res->append(get2DPoint(n,t));
        } else if (i == nbPoint-1) {
            t = (float)i/(float)nbPoint;

            res->append(get2DPoint(n,t));

            res->append(listPoint.points[n]);
        } else {

            t = (float)i/(float)nbPoint;

            res->append(get2DPoint(n,t));

            t = (float)(i+1)/(float)nbPoint;

            res->append(get2DPoint(n,t));
        }
    }

    return res;
}

//Forme une liste de points pour dessiner un carreau de Bezier
QVector<Point> *CourbeBezier::parcoursCarreauBerstein()
{
    float u,v;
    QVector<Point> *res = new QVector<Point>;

    for (int i = 0; i <= nbPoint; i++) {
        u = ((float)i/(float)nbPoint);

        for (int j = 0; j <= nbPoint; j++) {
            v = ((float)j/(float)nbPoint);

            if (i == 0 && j == 0) {
                 res->append(getCarrpoint(0,0));

                 float u_bis = ((float)(i+1)/(float)nbPoint);

                 listPoint = border1;
                 res->append(get2DPoint(n,u_bis));

                 res->append(getCarrpoint(0,0));

                 float v_bis = ((float)(j+1)/(float)nbPoint);

                 listPoint = border2;
                 res->append(get2DPoint(m,v_bis));
            } else if (i == nbPoint && j == nbPoint) {
                // DO nothing
            } else if (i == nbPoint) {

                //Gestion du coins u = 1 et v = 0
                if (j == 0) {
                    res->append(getCarrpoint(n,0));
                } else {
                    listPoint = border3;
                    res->append(get2DPoint(m,v));
                }

                float v_bis = ((float)(j+1)/(float)nbPoint);

                //Gestion du coins u = 1 et v = 1
                if (v_bis == 1) {
                    res->append(getCarrpoint(n,m));
                } else {
                    listPoint = border3;
                    res->append(get2DPoint(m,v_bis));
                }
            } else if (j == nbPoint) {

                //Gestion du coins u = 0 et v = 1
                if (i == 0) {
                    res->append(getCarrpoint(0,m));
                } else {
                    listPoint = border4;
                    res->append(get2DPoint(n,u));
                }

                float u_bis = ((float)(i+1)/(float)nbPoint);

                //Gestion du coins u = 1 et v = 1
                if (u_bis == 1) {
                    res->append(getCarrpoint(n,m));
                } else {
                    listPoint = border4;
                    res->append(get2DPoint(n,u_bis));
                }
            }

            else if (i == nbPoint-1) {
                //Gestion du coins u = 0.9999 (i=nbPoint-1) et v = 0
                if (j == 0) {
                    listPoint = border1;
                    res->append(get2DPoint(n,u));
                    res->append(getCarrpoint(n,0));
                } else {
                    res->append(get3DPoint(n,m,u,v));
                    listPoint = border3;
                    res->append(get2DPoint(m,v));
                }

                float v_bis = ((float)(j+1)/(float)nbPoint);

                //Gestion du coins u = 0.9999 (i=nbPoint-1) et v = 0
                if (j == 0) {
                    listPoint = border1;
                    res->append(get2DPoint(n,u));
                } else {
                    res->append(get3DPoint(n,m,u,v));
                }

                //Gestion du coins u = 0.9999 (i=nbPoint-1) et v = 0.9999
                if (j == nbPoint-1) {
                    listPoint = border4;
                    res->append(get2DPoint(n,u));
                } else {
                    res->append(get3DPoint(n,m,u,v_bis));
                }

            } else if (j == nbPoint-1) {

                if (i==0) {
                    listPoint = border2;
                    res->append(get2DPoint(m,v));
                } else {
                    res->append(get3DPoint(n,m,u,v));
                }

                float u_bis = ((float)(i+1)/(float)nbPoint);

                res->append(get3DPoint(n,m,u_bis,v));

                if (i==0) {
                    listPoint = border2;
                    res->append(get2DPoint(m,v));

                    res->append(getCarrpoint(0,m));
                } else {
                    res->append(get3DPoint(n,m,u,v));

                    listPoint = border4;
                    res->append(get2DPoint(n,u));
                }


            }

            else if (i == 0) {
                listPoint = border2;
                res->append(get2DPoint(m,v));

                float v_bis = ((float)(j+1)/(float)nbPoint);

                res->append(get2DPoint(m,v_bis));

                res->append(get2DPoint(m,v));

                float u_bis = ((float)(i+1)/(float)nbPoint);

                res->append(get3DPoint(n,m,u_bis,v));
            } else if (j == 0) {
                listPoint = border1;
                res->append(get2DPoint(n,u));

                float v_bis = ((float)(j+1)/(float)nbPoint);

                res->append(get3DPoint(n,m,u,v_bis));

                res->append(get2DPoint(n,u));

                float u_bis = ((float)(i+1)/(float)nbPoint);

                res->append(get2DPoint(n,u_bis));
            }

            //Case where 0<i<1 and 0<j<1
            else {
                res->append(get3DPoint(n,m,u,v));

                float u_bis = ((float)(i+1)/(float)nbPoint);

                res->append(get3DPoint(n,m,u_bis,v));

                res->append(get3DPoint(n,m,u,v));

                float v_bis = ((float)(j+1)/(float)nbPoint);

                res->append(get3DPoint(n,m,u,v_bis));
            }
        }
    }
    return res;
}

// Polynome de berstein
float CourbeBezier::berstein(int i, int n, float t)
{
    return ((facto(n)/(facto(i)*facto(n-i))) * expo(t,i) * expo((1-t),(n-i)));
}

//Recupère un des sommets de controles pour un carreau de degré 3*3
Point CourbeBezier::getCarrpoint(int i, int j)
{
    int index = i + (j*(n+1));
    return listCarrPoint.vecPoints->value(index);
}

//Retourne un point d'une courbe de bezier avec listPoint comme points de contrôle
Point CourbeBezier::get2DPoint(int n, float t)
{
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

    if (coord[0] == 0.0 && coord[1] == 0.0 && coord[2] == 0.0) {
        qDebug() << "0 2D pour t :" << t;
    }

    return p;
}

// retourne un point de la surface de bezier
Point CourbeBezier::get3DPoint(int n, int m, float u, float v)
{
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

    return p;
}
