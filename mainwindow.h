#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	explicit MainWindow(QWidget *parent = 0);
	~MainWindow();

private:
	Ui::MainWindow *ui;
    bool toggled = false;

protected slots:
    void setU(int u);
    void setV(int v);
    void toggleDisplayMode();
};

#endif // MAINWINDOW_H
