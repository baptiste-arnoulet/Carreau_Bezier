/* R. Raffin
 * M1 Informatique, Aix-Marseille Université
 * Fenêtre principale
 * Au cas où, l'UI contient une barre de menu, une barre de status, une barre d'outils (cf QMainWindow).
 * Une zone est laissée libre à droite du Widget OpenGL pour mettre de futurs contrôles ou informations.
 */

#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow)
{
	ui->setupUi(this);

    connect(this->ui->_btn_poly_1, &QPushButton::clicked, this->ui->openGLWidget, [this] { this->ui->openGLWidget->showPolyedre(1); });
    connect(this->ui->_btn_poly_2, &QPushButton::clicked, this->ui->openGLWidget, [this] { this->ui->openGLWidget->showPolyedre(2); });
    connect(this->ui->_btn_poly_3, &QPushButton::clicked, this->ui->openGLWidget, [this] { this->ui->openGLWidget->showPolyedre(3); });
}

MainWindow::~MainWindow()
{
	delete ui;
}

void MainWindow::setU(int u)
{
    this->ui->_u_lbl->setText("u = " + QString::number(u/100.f));
    this->ui->openGLWidget->setU(u/100.f);
}

void MainWindow::setV(int v)
{
    this->ui->_v_lbl->setText("t = " + QString::number(v/100.f));
    this->ui->openGLWidget->setV(v/100.f);
}
