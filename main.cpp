#include <QApplication>
#include "MainWindow.h"

int main(int argc, char* argv[])
{
	QApplication app(argc, argv);
	MainWindow mainWindow;
	mainWindow.setWindowTitle("Patienten Abrechnung v0.51");
	mainWindow.show();
	return app.exec();
}
