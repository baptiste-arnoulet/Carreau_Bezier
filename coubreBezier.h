#ifndef COUBREBEZIER_H
#define COUBREBEZIER_H

#include "segment.h"

class CourbeBezier
{
public:
    CourbeBezier(Point p0, Point p1, Point p2, Point p3, int nbPoint);
    CourbeBezier(Point points[], int nbPoint);
    QVector<Point> *parcoursBerstein();
    QVector<Point> *parcoursCarreauBerstein();

private:
    int nbPoint;

    struct ListPoint {
        Point points[4];
    };

    struct ListCarrPoint {
        Point points[16];
    };

    float facto(float a) {
        float res = 1;
        for (int i = 1; i<=a ;i++) {
            res *= i;
        }
        return res;
    }

    float expo (float a, int i) {
        if (a == 0) {
            return 0.0;
        } else if (i == 0) {
            return 1.0;
        }

        float res = 1;
        for (int j = 0; j<i; j++) {
            res *= a;
        }
        return res;
    }

    float berstein(int i, int n, float t);

    ListPoint listPoint;
    ListPoint border1;
    ListPoint border2;
    ListPoint border3;
    ListPoint border4;
    ListCarrPoint listCarrPoint;

    Point getCarrpoint(int i, int j);
    Point get2DPoint(int n, float t);
    Point get3DPoint(int n, int m, float u, float v);
};

#endif // COUBREBEZIER_H
