#include "MainWindow.h"

#include <QtGui>
#include <QtSql>
#include <QSqlDatabase>
#include <QSqlRelationalTableModel>

#include "definitions.h"
#include "Accounting.h"
#include "diagnoseBrowser.h"
#include "patientModel.h"
#include "treatmentModel.h"

#include <iostream>

MainWindow::MainWindow()
{
	//Connect to the database
	connectToDB();

	printer = new QPrinter;

	createActions();
	createMenus();

	createPatientPanel();
	createDataPanel();

	createPatientWidget();
	createStatusBar();

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


	unaccountedPatients = new QLabel("Offene Rechnungsbetraege: 000000000 ");
	unaccountedPatients->setMinimumSize(unaccountedPatients->sizeHint());

	statusBar()->addWidget(unaccountedPatients);

	connectSlots();

	this->resize(1000, 600);
	updateStatusBar();

}

void MainWindow::createPatientPanel()
{
	patientPanel = new QWidget();

	patientModel= new PatientModel();
	patientModel->setTable("patients");
	patientModel->select();

	patientView = new QTableView();
	patientView->setModel(patientModel);
	patientView->resizeColumnsToContents();
	patientView->setSelectionMode(
			QAbstractItemView::SingleSelection);
	patientView->setSelectionBehavior(
			QAbstractItemView::SelectRows);
	patientView->horizontalHeader()->setStretchLastSection(true);
	patientView->verticalHeader()->hide();
	patientView->setShowGrid(false);

	patientLabel = new QLabel(tr("&Patienten"));
	patientLabel->setBuddy(patientView);

}

void MainWindow::createDataPanel()
{
	dataPanel = new QWidget();

	dataModel= new TreatmentModel();
	dataModel->setTable("treatments");
	dataModel->setRelation(PatientId, QSqlRelation("patients", "id", "firstname"));
	dataModel->select();

	dataView = new QTableView();
	dataView->setModel(dataModel);
	dataView->resizeColumnsToContents();
	dataView->setSelectionMode(
			QAbstractItemView::SingleSelection);
	dataView->setSelectionBehavior(
			QAbstractItemView::SelectRows);
	dataView->horizontalHeader()->setStretchLastSection(true);
	dataView->verticalHeader()->hide();
	dataView->setShowGrid(false);

	dataLabel = new QLabel(tr("Patienten&daten"));
	dataLabel->setBuddy(patientView);

}

void MainWindow::createActions()
{
	//Patient related Actions
	newAction = new QAction(tr("&Neu"), this);
	newAction->setStatusTip(tr("Legt einen neuen Patienten an"));

	editAction = new QAction(tr("&Bearbeiten"), this);
	editAction->setStatusTip(tr("Bearbeitet einen Patienten"));

	findAction = new QAction(tr("&Suchen"), this);
	findAction->setStatusTip(tr("Sucht einen Patienten"));

	deleteAction = new QAction(tr("&Entfernen"), this);
	deleteAction->setStatusTip(tr("Entfernt einen Patienten"));

	//Treatment related Actions
	newTreatmentAction = new QAction(tr("&Anlegen"), this);
	newTreatmentAction->setStatusTip(tr("Legt eine Behandlung an"));

	editTreatmentAction = new QAction(tr("&Bearbeiten"), this);
	editTreatmentAction->setStatusTip(tr("Bearbeitet eine Behandlung"));

	deleteTreatmentAction = new QAction(tr("Entfernen"), this);
	deleteTreatmentAction->setStatusTip(tr("Entfernt eine Behandlung"));

	browseDiagnosesAction = new QAction(tr("&Diagnosen Browser"), this);
	browseDiagnosesAction->setStatusTip(tr("Startet den Diagnosen Browser"));

	//Accounting related Actions
	accountAction = new QAction(tr("&Abrechnen"), this);
	accountAction->setStatusTip(tr("Rechnet einen Patienten ab"));
	accountAction->setIcon(QIcon(":/img/accounting.png"));

	//Misc related Actions
	aboutAction = new QAction(tr("About"), this);
	aboutAction->setStatusTip(tr("Ueber dieses Programm..."));

	aboutQTAction = new QAction(tr("About QT"), this);
	aboutQTAction->setStatusTip(tr("Ueber QT..."));

}

void MainWindow::connectSlots()
{
	//Patient related connections
	connect(patientView->selectionModel(), SIGNAL(currentRowChanged(
					const QModelIndex&,
					const QModelIndex&)),
			this, SLOT(updateTreatmentView()));
	connect(newAction, SIGNAL(triggered()), this, SLOT(addPatient()));
	connect(deleteAction, SIGNAL(triggered()), this, SLOT(deletePatient()));
	connect(editAction, SIGNAL(triggered()), this, SLOT(editPatient()));

	//Treatment related connections
	connect(newTreatmentAction, SIGNAL(triggered()),
			this, SLOT(addTreatment()));
	connect(editTreatmentAction, SIGNAL(triggered()),
			this, SLOT(editTreatment()));
	connect(deleteTreatmentAction, SIGNAL(triggered()),
			this, SLOT(deleteTreatment()));
	connect(deleteTreatmentAction, SIGNAL(triggered()),
			this, SLOT(deleteTreatment()));
	connect(browseDiagnosesAction, SIGNAL(triggered()),
			this, SLOT(browseDiagnoses()));

	//Accounting related connections
	connect(accountAction, SIGNAL(triggered()), this, SLOT(accountPatient()));

	//Misc related connections
	connect(aboutAction, SIGNAL(triggered()), this, SLOT(about()));
	connect(aboutQTAction, SIGNAL(triggered()), qApp, SLOT(aboutQt()));
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

void MainWindow::createMenus()
{

	//Patient Menu
	patientMenu = menuBar()->addMenu(tr("&Patienten"));
	patientMenu->addAction(newAction);
	patientMenu->addAction(editAction);
	patientMenu->addAction(findAction);
	patientMenu->addAction(deleteAction);

	//Treatment Menu
	treatmentMenu = menuBar()->addMenu(tr("&Behandlungen"));
	treatmentMenu->addAction(newTreatmentAction);
	treatmentMenu->addAction(editTreatmentAction);
	treatmentMenu->addAction(deleteTreatmentAction);
	treatmentMenu->addAction(browseDiagnosesAction);

	//Accounting Menu
	accountingMenu = menuBar()->addMenu(tr("&Abrechnung"));
	accountingMenu->addAction(accountAction);

	//Misc Menu
	miscMenu = menuBar()->addMenu(tr("&Sonstiges"));
	miscMenu->addAction(aboutAction);
	miscMenu->addAction(aboutQTAction);

}

bool MainWindow::connectToDB()
{
	QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
	db.setDatabaseName("data/patientdata");
	if (!db.open())
	{
		QMessageBox::critical(0, QObject::tr("Datenbank Fehler!"),
				db.lastError().text());
		return false;
	}
	return true;
}


void MainWindow::updateTreatmentView()
{
	QModelIndex index = patientView->currentIndex();

	if(index.isValid())
	{
		QSqlRecord record = patientModel->record(index.row());
		int id = record.value("id").toInt();
		dataModel->setFilter(QString("patient_id = %1").arg(id));
	}

	dataModel->select();
	dataView->horizontalHeader()->setVisible(dataModel->rowCount() > 0);
}

void MainWindow::addPatient()
{
	int rc = dynamic_cast<PatientModel*>(patientModel)->addNewRecord();
	updateTreatmentView();

	//Send the new patient for editing purposes to the form
	if (rc)
	{
		QSqlRecord record = patientModel->record(rc);
		int id = record.value("id").toInt();
		PatientForm editPatient(id, this);
		editPatient.exec();
	}
	patientModel->select();
}

void MainWindow::deletePatient()
{
	QModelIndex index = patientView->currentIndex();
	if (!index.isValid())
	{
		return;
	}

	QSqlDatabase::database().transaction();
	QSqlRecord record = patientModel->record(index.row());
	int id = record.value(ID).toInt();

	dynamic_cast<PatientModel*>(patientModel)->deleteRecord(id);
	patientModel->removeRow(index.row());
	updateTreatmentView();
	updateStatusBar();
}

void MainWindow::editPatient()
{
	QModelIndex index = patientView->currentIndex();
	if (!index.isValid())
	{
		return;
	}
	QSqlRecord record = patientModel->record(index.row());

	int id = record.value(ID).toInt();

	//Why this - 1 is needed, no one knows!
	//Counting should always start at 0
	PatientForm editPatient(id, this);
	editPatient.exec();
	patientModel->select();
}

void MainWindow::addTreatment()
{
	QModelIndex index = patientView->currentIndex();
	if (!index.isValid())
	{
		return;
	}
	QSqlRecord patientRecord = patientModel->record(index.row());
	int id = patientRecord.value(ID).toInt();
	int treatId = dynamic_cast<TreatmentModel*>(dataModel)->addNewRelRecord(id);

	//Send the new patient for editing purposes to the form
	if (treatId)
	{
		std::cerr << treatId;
		dataModel->select();
		TreatmentForm editTreatment(treatId, this);
		editTreatment.exec();
	}

	dataModel->select();
	patientModel->select();
	updateTreatmentView();
	updateStatusBar();
}

void MainWindow::editTreatment()
{
	QModelIndex index = dataView->currentIndex();
	if (!index.isValid())
	{
		return;
	}
	QSqlRecord record = dataModel->record(index.row());

	//GET THE TREATMENTS A PRIMARY KEY
	int id = record.value(TreatmentID).toInt();

	std::cerr << id;
	TreatmentForm editTreatment(id, this);
	editTreatment.exec();

	dataModel->select();
	patientModel->select();
	updateTreatmentView();
	updateStatusBar();
}

void MainWindow::deleteTreatment()
{
	QModelIndex index = dataView->currentIndex();
	if (!index.isValid())
	{
		return;
	}

	QSqlRecord record = dataModel->record(index.row());
	int id = record.value(ID).toInt();
	dynamic_cast<TreatmentModel*>(dataModel)->deleteRecord(id);
	dataModel->removeRow(index.row());
	dataModel->select();
	updateTreatmentView();
	updateStatusBar();
}

void MainWindow::accountPatient()
{
	QModelIndex index = patientView->currentIndex();
	if (!index.isValid())
	{
		return;
	}
	QSqlRecord record = patientModel->record(index.row());

	PatientAccounter acc(record, *(dataModel));
	acc.account();
	updateStatusBar();
}

void MainWindow::browseDiagnoses()
{
	DiagnoseBrowser browser(this);
	browser.exec();
}

void MainWindow::about()
{
		QMessageBox msgBox;
		msgBox.about(this, "About PatientAccounting",
				"PatientAccounting is a programm to manage and account patients\n\n"
				"It was developed open source. "
				"The GPL applies\n\n"
				"Support is not foressen ;)\n");
}

void MainWindow::updateStatusBar()
{
	double openBills = dynamic_cast<TreatmentModel*>(dataModel)->getOpenBills();
	QString openBillsText;
	openBillsText.setNum(openBills);
	QString statusBarText("Offene Rechnungsbetraege: ");
	statusBarText.append(openBillsText);
	statusBarText.append(" Euro");
	unaccountedPatients->setText(statusBarText);
}
