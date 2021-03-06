//R. Raffin, M1 Informatique, "Surfaces 3D"
//tiré de CC-BY Edouard.Thiel@univ-amu.fr - 22/01/2019

#include "myopenglwidget.h"
#include <QDebug>
#include <QSurfaceFormat>
#include <QMatrix4x4>

#include <iostream>

#include "segment.h"
#include "coubreBezier.h"

static const QString vertexShaderFile   = ":/basic.vsh";
static const QString fragmentShaderFile = ":/basic.fsh";


myOpenGLWidget::myOpenGLWidget(QWidget *parent) :
	QOpenGLWidget(parent)
{
	qDebug() << "init myOpenGLWidget" ;

    allControlPoints = new QVector<Point>;
    allBezierPoints = new QVector<Point>;

    mode_aff = Mode_aff::lines;

	QSurfaceFormat sf;
	sf.setDepthBufferSize(24);
	sf.setSamples(16);  // nb de sample par pixels : suréchantillonnage por l'antialiasing, en décalant à chaque fois le sommet
						// cf https://www.khronos.org/opengl/wiki/Multisampling et https://stackoverflow.com/a/14474260
	setFormat(sf);

	setEnabled(true);  // événements clavier et souris
	setFocusPolicy(Qt::StrongFocus); // accepte focus
	setFocus();                      // donne le focus

	m_timer = new QTimer(this);
	m_timer->setInterval(50);  // msec
	connect (m_timer, SIGNAL(timeout()), this, SLOT(onTimeout()));
}

myOpenGLWidget::~myOpenGLWidget()
{
	qDebug() << "destroy GLArea";

	delete m_timer;

	// Contrairement aux méthodes virtuelles initializeGL, resizeGL et repaintGL,
	// dans le destructeur le contexte GL n'est pas automatiquement rendu courant.
	makeCurrent();
	tearGLObjects();
	doneCurrent();
}


void myOpenGLWidget::initializeGL()
{
	qDebug() << __FUNCTION__ ;
	initializeOpenGLFunctions();
	glEnable(GL_DEPTH_TEST);

    // Point de controle
    Point p0;
    p0.set(new float[3]{-0.75 ,-0.5, 0.75});

    Point p1;
    p1.set(new float[3]{-0.25,0.0,0.75});

    Point p2;
    p2.set(new float[3]{0.25,0.0,0.75});

    Point p3;
    p3.set(new float[3]{0.75,-0.5,0.75});

    Point p4;
    p4.set(new float[3]{-0.75 ,0.0, 0.25});

    Point p5;
    p5.set(new float[3]{-0.25,0.5,0.25});

    Point p6;
    p6.set(new float[3]{0.25,0.5,0.25});

    Point p7;
    p7.set(new float[3]{0.75,0.0,0.25});

    Point p8;
    p8.set(new float[3]{-0.75 ,0.0, -0.25});

    Point p9;
    p9.set(new float[3]{-0.25,0.5,-0.25});

    Point p10;
    p10.set(new float[3]{0.25,0.5,-0.25});

    Point p11;
    p11.set(new float[3]{0.75,0.0,-0.25});

    Point p12;
    p12.set(new float[3]{-0.75 ,-0.5, -0.75});

    Point p13;
    p13.set(new float[3]{-0.25,0.0,-0.75});

    Point p14;
    p14.set(new float[3]{0.25,0.0,-0.75});

    Point p15;
    p15.set(new float[3]{0.75,-0.5,-0.75});

    polyedre1 = new QVector<Point>{p0,p1,p2,p3,p4,p5,p6,p7,p8,p9,p10,p11,p12,p13,p14,p15};
    polyedre2 = new QVector<Point>{p0,p1,p2,p4,p5,p6,p8,p9,p10,p12,p13,p14,p15};
    polyedre3 = new QVector<Point>{p0,p1,p2,p4,p5,p6,p8,p9,p10};

    p0.set(new float[3]{-1.0 ,-1.4, 0.0});
    p1.set(new float[3]{-0.4,1.2,0.0});
    p2.set(new float[3]{1.3,0.8,0.0});
    p3.set(new float[3]{1.0,-0.7,0.0});
    p4.set(new float[3]{-1.0 ,-0.8, -1.0});
    p5.set(new float[3]{-0.4,1.4,-1.0});
    p6.set(new float[3]{1.3,1.1,-1.0});
    p7.set(new float[3]{1.0,-0.5,-1.0});
    p8.set(new float[3]{-1.0 ,-0.8, -2.0});
    p9.set(new float[3]{-0.4,1.5,-2.0});
    p10.set(new float[3]{1.3,1.1,-2.0});
    p11.set(new float[3]{1.0,-0.4,-2.0});
    p12.set(new float[3]{-1.0 ,-1.4, -3.0});
    p13.set(new float[3]{-0.4,1.2,-3.0});
    p14.set(new float[3]{1.3,0.8,-3.0});
    p15.set(new float[3]{1.0,-0.7,-3.0});

    polyedre4 = new QVector<Point>{p0,p1,p2,p3,p4,p5,p6,p7,p8,p9,p10,p11,p12,p13,p14,p15};

    n = 3;
    m = 3;

    currentPolyedre = polyedre1;

    //Point carrPoints2[12] = {p0,p1,p2,p4,p5,p6,p8,p9,p10,p12,p13,p14};
    //Point carrPoints[16] = {p0,p1,p2,p3,p4,p5,p6,p7,p8,p9,p10,p11,p12,p13,p14,p15};

    makeGLForm();

	//shaders
	m_program = new QOpenGLShaderProgram(this);
	m_program->addShaderFromSourceFile(QOpenGLShader::Vertex, vertexShaderFile);  // compile
	m_program->addShaderFromSourceFile(QOpenGLShader::Fragment, fragmentShaderFile);

	if (! m_program->link()) {  // édition de lien des shaders dans le shader program
		qWarning("Failed to compile and link shader program:");
		qWarning() << m_program->log();
	}
}

void myOpenGLWidget::doProjection()
{
	//m_mod.setToIdentity();
	//modelMatrix.ortho( -aratio, aratio, -1.0f, 1.0f, -1.0f, 1.0f );
}

// Pour dessiner un segment
void myOpenGLWidget::makeGLSegment(Point start, Point end)
{
	//1 Nos objets géométriques
    Point a, b;
	float * coord = new float[3];

    coord[0] = start.getX();
    coord[1] = start.getY();
    coord[2] = start.getZ();

    a.set (coord);

    coord[0] = end.getX();
    coord[1] = end.getY();
    coord[2] = end.getZ();

    b.set(coord);

    Segment *s = new Segment();
    s->setStart(a);
    s->setEnd(b);

    delete [] coord;

    allControlPoints->append(s->getStart());
    allControlPoints->append(s->getEnd());
}

// Pour dessiner une courbe de bezier avec 4 point de contrôles
void myOpenGLWidget::makeGLBezierCurve(Point p0, Point p1, Point p2, Point p3, int presision)
{
    cp = new CourbeBezier(p0, p1, p2, p3, presision);
    allBezierPoints->append(*cp->parcoursBerstein());
}

// Pour dessiner un carreau de bezier de degrés 3 * 3
void myOpenGLWidget::makeGLBezierCarr(QVector<Point> *points, int presision)
{
    cp = new CourbeBezier(points, presision,n,m);
    allBezierPoints->append(*cp->parcoursCarreauBerstein());
}

void myOpenGLWidget::makeGLForm()
{
    allControlPoints->clear();
    allBezierPoints->clear();

    for (int j = 0; j <= m; j++) {
        for (int i = 0; i<n; i++) {
            makeGLSegment(currentPolyedre->value(((n+1)*j)+i), currentPolyedre->value(((n+1)*j)+i+1));
        }
    }

    for (int i = 0; i <= n; i++) {
        for (int j = 0; j<m; j++) {
            makeGLSegment(currentPolyedre->value(((n+1)*j)+i), currentPolyedre->value(((n+1)*(j+1))+i));
        }
    }

    // Carreau de bezier
    makeGLBezierCarr(currentPolyedre, nbPoint);

    prepareOpenGl(getBezierPoint(u,v), *allControlPoints, *allBezierPoints);
}


void myOpenGLWidget::tearGLObjects()
{
    m_vbo.destroy();
}

void myOpenGLWidget::prepareOpenGl(Point p, QVector<Point> controlPoints, QVector<Point> bezierPoints)
{
    QVector<GLfloat> vertData;

    vertData.append(p.getX());
    vertData.append(p.getY());
    vertData.append(p.getZ());
    vertData.append(1.0);
    vertData.append(0.0);
    vertData.append(0.0);


    GLfloat * colors = new GLfloat[3];
    colors[0] = 1.0;
    colors[1] = 0.0;
    colors[2] = 0.0;

    for (Point p : controlPoints) {
        vertData.append(p.getX());
        vertData.append(p.getY());
        vertData.append(p.getZ());
        vertData.append(colors[0]);
        vertData.append(colors[1]);
        vertData.append(colors[2]);
    }

    colors[3] = 0.0;
    colors[4] = 1.0;
    colors[5] = 0.0;

    for (Point p : bezierPoints) {
        vertData.append(p.getX());
        vertData.append(p.getY());
        vertData.append(p.getZ());
        vertData.append(colors[3]);
        vertData.append(colors[4]);
        vertData.append(colors[5]);
    }

    m_vbo.create();
    m_vbo.bind();

    m_vbo.allocate(vertData.constData(), vertData.count() * sizeof(GLfloat));
}

Point myOpenGLWidget::getBezierPoint(float u, float v)
{
    return cp->get3DPoint(n,m,u,v);
}


void myOpenGLWidget::resizeGL(int w, int h)
{
	qDebug() << __FUNCTION__ << w << h;

	//C'est fait par défaut
	glViewport(0, 0, w, h);

	m_ratio = (double) w / h;
	//doProjection();
}

void myOpenGLWidget::paintGL()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    QMatrix4x4 m_modelView;
    QMatrix4x4 m_projection;
    QMatrix4x4 m_model;

	m_program->bind(); // active le shader program

    m_modelView.setToIdentity();
    m_modelView.lookAt(QVector3D(0.0f, 0.0f, 3.0f),    // Camera Position
                       QVector3D(0.0f, 0.0f, 0.0f),    // Point camera looks towards
                       QVector3D(0.0f, 1.0f, 0.0f));   // Up vector

    m_projection.setToIdentity ();
    m_projection.perspective(70.0, width() / height(), 0.1, 100.0); //ou m_ratio

    //m_model.translate(0, 0, -3.0);

    // Rotation de la scène pour l'animation
    m_model.rotate(m_angle, 0, 1, 0);

    QMatrix4x4 m = m_projection * m_modelView * m_model;

	m_program->setUniformValue("matrix", m);

	m_program->setAttributeBuffer("posAttr", GL_FLOAT, 0, 3, 6 * sizeof(GLfloat));
	m_program->setAttributeBuffer("colAttr", GL_FLOAT, 3 * sizeof(GLfloat), 3, 6 * sizeof(GLfloat));
	m_program->enableAttributeArray("posAttr");
	m_program->enableAttributeArray("colAttr");

    glPointSize (10.0f);
    glDrawArrays(GL_POINTS, 0, 1);

    glLineWidth(2.0f);
    glDrawArrays(GL_LINES, 1, allControlPoints->size());

    glPointSize (5.0f);
    if (mode_aff == Mode_aff::lines) {
        glDrawArrays(GL_LINES, 1 + allControlPoints->size(), allBezierPoints->size());
    } else if (mode_aff == Mode_aff::points) {
        glDrawArrays(GL_POINTS, 1 + allControlPoints->size(), allBezierPoints->size());
    }

	m_program->disableAttributeArray("posAttr");
	m_program->disableAttributeArray("colAttr");

	m_program->release();
}

void myOpenGLWidget::setU(float u)
{
    if (u <= 0 || u >= 1) return;

    this->u = u;

    makeGLForm();

    update();
}

void myOpenGLWidget::setV(float v)
{
    if (v <= 0 || v >= 1) return;
    this->v = v;

    makeGLForm();

    update();
}

void myOpenGLWidget::showPolyedre(int i)
{
    switch (i) {
        case 1 :
            currentPolyedre = polyedre1;
            n = 3;
            m = 3;
            break;
        case 2 :
            currentPolyedre = polyedre2;
            n = 2;
            m = 3;
            break;
        case 3 :
            currentPolyedre = polyedre3;
            n = 2;
            m = 2;
            break;
        case 4 :
            currentPolyedre = polyedre4;
            n = 3;
            m = 3;
            break;
    }

    makeGLForm();
    update();
}

void myOpenGLWidget::toggleDisplayMode()
{
    if (mode_aff == Mode_aff::points) {
        mode_aff = Mode_aff::lines;
    } else {
        mode_aff = Mode_aff::points;
    }
    update();
}

void myOpenGLWidget::keyPressEvent(QKeyEvent *ev)
{
	qDebug() << __FUNCTION__ << ev->text();

    switch(ev->key()) {
		case Qt::Key_A :
            if (m_timer->isActive()) {
                m_angle = 0;
				m_timer->stop();
            } else m_timer->start();
			break;
        case Qt::Key_F :
            if (nbPoint<=3) break;
            nbPoint -= 1;

            makeGLForm();

            update();
            break;
        case Qt::Key_G :
            nbPoint += 1;

            makeGLForm();

            update();
            break;
		case Qt::Key_R :
			break;
	}
}

void myOpenGLWidget::keyReleaseEvent(QKeyEvent *ev)
{
	qDebug() << __FUNCTION__ << ev->text();
}

void myOpenGLWidget::mousePressEvent(QMouseEvent *ev)
{
	qDebug() << __FUNCTION__ << ev->x() << ev->y() << ev->button();
}

void myOpenGLWidget::mouseReleaseEvent(QMouseEvent *ev)
{
	qDebug() << __FUNCTION__ << ev->x() << ev->y() << ev->button();
}

void myOpenGLWidget::mouseMoveEvent(QMouseEvent *ev)
{
    qDebug() << __FUNCTION__ << ev->x() << ev->y();
}

void myOpenGLWidget::onTimeout()
{
    m_angle += 1;
    if (m_angle > 360) m_angle -= 360;
	update();
}





