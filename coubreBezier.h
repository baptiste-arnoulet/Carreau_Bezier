#ifndef COUBREBEZIER_H
#define COUBREBEZIER_H

#include "segment.h"

class CourbeParamtrique : public Segment
{
public:
    CourbeParamtrique(Point p0, Point p1, Point p2, Point p3);
    CourbeParamtrique(Point points[]);
    QVector<Point> *parcours();
    QVector<Point> *parcoursBerstein();
    QVector<Point> *parcoursCarreauBerstein();

private:
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
    ListCarrPoint listCarrPoint;

    Point getCarrpoint(int i, int j);

    Point bezierPoint(float t, Point p0, Point p1, Point p2, Point p3);

    QVector<Point> * discretisation(float nbPoint);
};

#endif // COUBREBEZIER_H