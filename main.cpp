#include <QApplication>
#include "MainForm.h"

int main(int argc, char* argv[])
{
	QApplication app(argc, argv);
	MainForm mainForm;
	mainForm.setWindowTitle("Patienten Abrechnung v0.1a");
	mainForm.show();
	return app.exec();
}
