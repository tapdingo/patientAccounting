#include <QApplication>
#include "MainWindow.h"

int main(int argc, char* argv[])
{
	QApplication app(argc, argv);
	MainWindow mainWindow;
	mainWindow.setWindowTitle("Patienten Abrechnung v0.1a");
	mainWindow.show();
	return app.exec();
}
