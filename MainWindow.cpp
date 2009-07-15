#include "MainWindow.h"

#include <QtGui>

MainWindow::MainWindow()
{
	createPatientPanel();
	createDataPanel();

	createActions();

	createPatientWidget();
	createStatusBar();
	createToolbar();

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
	QHBoxLayout* overall_layout = new QHBoxLayout();
	overall_layout->addLayout(upper_layout);
	overall_layout->addLayout(lower_layout);

	patientWidget->setLayout(overall_layout);
}

void MainWindow::createPatientPanel()
{
	patientPanel = new QWidget();

	patientView = new QTableView();

	patientLabel = new QLabel(tr("&Patienten"));
	patientLabel->setBuddy(patientView);
}

void MainWindow::createDataPanel()
{
	dataPanel = new QWidget();

	dataView = new QTableView();

	dataLabel = new QLabel(tr("Patienten&daten"));
	dataLabel->setBuddy(patientView);
}

void MainWindow::createToolbar()
{
	toolbar = addToolBar(tr("&Patienten"));
	toolbar->addAction(newAction);
	toolbar->addAction(editAction);
	toolbar->addAction(findAction);
	toolbar->addAction(deleteAction);
	toolbar->addAction(accountAction);
}

void MainWindow::createActions()
{
	newAction = new QAction(tr("&Neu"), this);
	newAction->setStatusTip(tr("Legt einen neuen Patienten an"));

	editAction = new QAction(tr("&Bearbeiten"), this);
	editAction->setStatusTip(tr("Bearbeitet einen Patienten"));

	findAction = new QAction(tr("&Suchen"), this);
	findAction->setStatusTip(tr("Sucht einen Patienten"));

	deleteAction = new QAction(tr("&Entfernen"), this);
	deleteAction->setStatusTip(tr("Entfernt einen Patienten"));

	accountAction = new QAction(tr("&Abrechnen"), this);
	accountAction->setStatusTip(tr("Rechnet einen Patienten ab"));
}

void MainWindow::createPatientWidget()
{
	patientWidget = new QWidget();
	setCentralWidget(patientWidget);
}

void MainWindow::createStatusBar()
{
	m_statusBar = statusBar();
}
