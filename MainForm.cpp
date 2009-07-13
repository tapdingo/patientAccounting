#include "MainForm.h"

#include <QTableView>
#include <QLabel>
#include <QVBoxLayout>

MainForm::MainForm()
{
	createPatientPanel();

	QVBoxLayout* upper_layout = new QVBoxLayout();
	upper_layout->addWidget(patientPanel);
	upper_layout->addWidget(patientLabel);
	upper_layout->addWidget(patientView);

	this->setLayout(upper_layout);
}

void MainForm::createPatientPanel()
{
	patientPanel = new QWidget();

	patientView = new QTableView();

	patientLabel = new QLabel(tr("&Patienten"));
	patientLabel->setBuddy(patientView);
}
