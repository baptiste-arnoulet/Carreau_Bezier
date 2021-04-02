#ifndef MYOPENGLWIDGET_H
#define MYOPENGLWIDGET_H

#include <QObject>
#include <QWidget>

#include <QKeyEvent>
#include <QTimer>
#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLBuffer>
#include <QOpenGLShaderProgram>

#include "point.h"
#include "coubreBezier.h"

class myOpenGLWidget : public QOpenGLWidget,
			   protected QOpenGLFunctions
{
	Q_OBJECT

public:
	explicit myOpenGLWidget(QWidget *parent = nullptr);
	~myOpenGLWidget();

    enum class Mode_aff {
        points = 0, lines = 1
    } mode_aff;

    void setU(float u);
    void setV(float v);
    void showPolyedre(int i);

public slots:

signals:  // On ne les implémente pas, elles seront générées par MOC ;
		  // les paramètres seront passés aux slots connectés.

protected slots:
	void onTimeout();

protected:
	void initializeGL() override;
	void doProjection();
	void resizeGL(int w, int h) override;
	void paintGL() override;
	void keyPressEvent(QKeyEvent *ev) override;
	void keyReleaseEvent(QKeyEvent *ev) override;
	void mousePressEvent(QMouseEvent *ev) override;
	void mouseReleaseEvent(QMouseEvent *ev) override;
	void mouseMoveEvent(QMouseEvent *ev) override;

private:
	double m_angle = 0;
	QTimer *m_timer = nullptr;
	double m_ratio = 1;
    int nbPoint = 20;
    int n, m;
    float p = 1.0/(float)nbPoint;

    // Coordonnées paramétriques
    float u, v;

    struct Point3D{
        double x,y,z;
    };


	//RR matrices utiles
	QMatrix4x4 m_modelView;
	QMatrix4x4 m_projection;
	QMatrix4x4 m_model;

    QVector<Point> *allControlPoints;
    QVector<Point> *allBezierPoints;

    Point getBezierPoint(float u, float v);

	QOpenGLShaderProgram *m_program;
    QOpenGLBuffer m_vbo;

    CourbeBezier *cp;

    void makeGLSegment(Point start, Point end);
    void makeGLBezierCurve(Point p0, Point p1, Point p2, Point p3, int presision);
    void makeGLBezierCarr(QVector<Point> *points, int presision);
    void makeGLForm();
	void tearGLObjects();
    void prepareOpenGl(Point p, QVector<Point> controlPoints, QVector<Point> bezierPoints);
};


#endif // MYOPENGLWIDGET_H
