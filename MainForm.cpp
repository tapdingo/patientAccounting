#include "MainForm.h"

#include <QTableView>
#include <QLabel>
#include <QVBoxLayout>
#include <QDialogButtonBox>
#include <QPushButton>

MainForm::MainForm()
{
	createPatientPanel();
	createDataPanel();
	createButtonPanel();

	//Upper section Layout
	QVBoxLayout* upper_layout = new QVBoxLayout();
	upper_layout->addWidget(patientPanel);
	upper_layout->addWidget(patientLabel);
	upper_layout->addWidget(patientView);

	//Lower section Layout
	QVBoxLayout* lower_layout = new QVBoxLayout();
	lower_layout->addWidget(dataPanel);
	lower_layout->addWidget(dataLabel);
	lower_layout->addWidget(dataView);

	//Assemble all layouts
	QVBoxLayout* overall_layout = new QVBoxLayout();
	overall_layout->addLayout(upper_layout);
	overall_layout->addLayout(lower_layout);
	overall_layout->addWidget(buttonBox);

	this->setLayout(overall_layout);
}

void MainForm::createPatientPanel()
{
	patientPanel = new QWidget();

	patientView = new QTableView();

	patientLabel = new QLabel(tr("&Patienten"));
	patientLabel->setBuddy(patientView);
}

void MainForm::createDataPanel()
{
	dataPanel = new QWidget();

	dataView = new QTableView();

	dataLabel = new QLabel(tr("Patienten&daten"));
	dataLabel->setBuddy(patientView);
}

void MainForm::createButtonPanel()
{
	addPatient = new QPushButton(tr("&Neu"));
	editPatient = new QPushButton(tr("&Bearbeiten"));
	findPatient = new QPushButton(tr("&Suche"));
	deletePatient = new QPushButton(tr("&Loeschen"));
	buttonBox = new QDialogButtonBox(Qt::Horizontal);

	buttonBox->addButton(addPatient, QDialogButtonBox::ActionRole);
	buttonBox->addButton(editPatient, QDialogButtonBox::ActionRole);
	buttonBox->addButton(findPatient, QDialogButtonBox::ActionRole);
	buttonBox->addButton(deletePatient, QDialogButtonBox::ActionRole);
}
