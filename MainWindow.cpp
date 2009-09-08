#include "MainWindow.h"

#include <QtGui>
#include <QtSql>
#include <QSqlDatabase>
#include <QSqlRelationalTableModel>

#include "definitions.h"
#include "Accounting.h"

MainWindow::MainWindow()
{
	//Connect to the database
	connectToDB();

	printer = new QPrinter;

	createActions();

	createPatientPanel();
	createDataPanel();

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

	this->resize(1000, 600);

}

void MainWindow::createPatientPanel()
{
	patientPanel = new QWidget();

	patientModel= new QSqlRelationalTableModel(this);
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

	connect(patientView->selectionModel(), SIGNAL(currentRowChanged(
					const QModelIndex&,
					const QModelIndex&)),
			this, SLOT(updateTreatmentView()));
	connect(newAction, SIGNAL(triggered()), this, SLOT(addPatient()));
	connect(deleteAction, SIGNAL(triggered()), this, SLOT(deletePatient()));
	connect(editAction, SIGNAL(triggered()), this, SLOT(editPatient()));
	connect(accountAction, SIGNAL(triggered()), this, SLOT(accountPatient()));
}

void MainWindow::createDataPanel()
{
	dataPanel = new QWidget();

	dataModel= new QSqlRelationalTableModel(this);
	dataModel->setTable("treatments");
	dataModel->setRelation(PatientId, QSqlRelation("patients", "id", "vorname"));
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

	connect(newTreatmentAction, SIGNAL(triggered()),
			this, SLOT(addTreatment()));
	connect(editTreatmentAction, SIGNAL(triggered()),
			this, SLOT(editTreatment()));
	connect(deleteTreatmentAction, SIGNAL(triggered()),
			this, SLOT(deleteTreatment()));
}

void MainWindow::createToolbar()
{
	toolbar = addToolBar(tr("&Patienten"));
	toolbar->addAction(newAction);
	toolbar->addAction(editAction);
	toolbar->addAction(findAction);
	toolbar->addAction(deleteAction);
	toolbar->addAction(accountAction);
	toolbar->addAction(newTreatmentAction);
	toolbar->addAction(editTreatmentAction);
	toolbar->addAction(deleteTreatmentAction);
}

void MainWindow::createActions()
{
	newAction = new QAction(tr("&Neu"), this);
	newAction->setStatusTip(tr("Legt einen neuen Patienten an"));
	newAction->setIcon(QIcon(":/img/new.png"));

	editAction = new QAction(tr("&Bearbeiten"), this);
	editAction->setStatusTip(tr("Bearbeitet einen Patienten"));

	findAction = new QAction(tr("&Suchen"), this);
	findAction->setStatusTip(tr("Sucht einen Patienten"));

	deleteAction = new QAction(tr("&Entfernen"), this);
	deleteAction->setStatusTip(tr("Entfernt einen Patienten"));

	accountAction = new QAction(tr("&Abrechnen"), this);
	accountAction->setStatusTip(tr("Rechnet einen Patienten ab"));
	accountAction->setIcon(QIcon(":/img/accounting.png"));

	newTreatmentAction = new QAction(tr("Behandlung Anlegen"), this);
	newTreatmentAction->setStatusTip(tr("Legt eine Behandlung an"));

	editTreatmentAction = new QAction(tr("Behandlung bearbeiten"), this);
	editTreatmentAction->setStatusTip(tr("Bearbeitet eine Behandlung"));

	deleteTreatmentAction = new QAction(tr("Behandlung entfernen"), this);
	deleteTreatmentAction->setStatusTip(tr("Entfernt eine Behandlung"));
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
	QSqlRecord rec;

	int rc = patientModel->rowCount();
	if (!patientModel->insertRow(rc))
	{
		QMessageBox msgBox;
		msgBox.setText("Could not insert row :(");
		msgBox.exec();
		return;
	}

	rec = patientModel->record(rc);
	rec.setValue(QString("gender"), QString("m"));
	patientModel->setRecord(rc, rec);

	if (!patientModel->submitAll())
	{
		QMessageBox msgBox;
		msgBox.setText("Could not submit model :(");
		QSqlError last = QSqlDatabase::database().lastError();
		msgBox.setInformativeText(last.text());
		msgBox.exec();
	}
	patientModel->select();
	updateTreatmentView();

	//Send the new patient for editing purposes to the form
	PatientForm editPatient(patientModel, rc, this);
	editPatient.exec();
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

	int r = QMessageBox::warning(this, tr("Patient entfernen"),
			tr("Patient %1 %2 und alle Behandlungen wirklich entfernen?")
			.arg(record.value(FirstName).toString(), record.value(LastName).toString()),
			QMessageBox::Yes | QMessageBox::No);

	if (r == QMessageBox::No)
	{
		QSqlDatabase::database().rollback();
		return;
	}
	QSqlQuery query;
	query.exec(QString("DELETE FROM treatments "
				"WHERE patient_id = %1").arg(id));

	patientModel->removeRow(index.row());
	patientModel->submitAll();
	QSqlDatabase::database().commit();

	patientModel->select();
	updateTreatmentView();
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
	PatientForm editPatient(patientModel, id - 1, this);
	editPatient.exec();
}

void MainWindow::addTreatment()
{
	QSqlTableModel* interimModel= new QSqlTableModel(this);
	interimModel->setTable("treatments");
	interimModel->select();
	QModelIndex index = patientView->currentIndex();
	if (!index.isValid())
	{
		return;
	}
	QSqlRecord patientRecord = patientModel->record(index.row());
	int id = patientRecord.value(ID).toInt();

	int rc = interimModel->rowCount();

	if (!interimModel->insertRow(rc))
	{
		QMessageBox msgBox;
		msgBox.setText("Could not insert row :(");
		msgBox.exec();
		return;
	}

	QSqlRecord rec;
	QString idString;
	idString.setNum(id);
	rec = interimModel->record(rc);

	rec.setValue(QString("patient_id"), idString);

	interimModel->setRecord(rc, rec);
	if (!interimModel->submitAll())
	{
		QMessageBox msgBox;
		msgBox.setText("Could not submit model :(");
		QSqlError last = QSqlDatabase::database().lastError();
		msgBox.setInformativeText(last.text());
		msgBox.exec();
	}
	interimModel->select();
	updateTreatmentView();

	//Send the new patient for editing purposes to the form
	//Bad Hack to get the new ID
	rec = interimModel->record(rc-1);
	int treatId = rec.value(TreatmentID).toInt() + 1;
	dataModel->select();
	TreatmentForm editTreatment(treatId, this);
	editTreatment.exec();

	dataModel->select();
	patientModel->select();
	updateTreatmentView();
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

	TreatmentForm editTreatment(id, this);
	editTreatment.exec();

	dataModel->select();
	patientModel->select();
	updateTreatmentView();
}

void MainWindow::deleteTreatment()
{
	QModelIndex index = dataView->currentIndex();
	if (!index.isValid())
	{
		return;
	}

	QSqlDatabase::database().transaction();
	QSqlRecord record = dataModel->record(index.row());
	int id = record.value(ID).toInt();

	int r = QMessageBox::warning(this, tr("Behandlung entfernen"),
			tr("die Behandlung wirklich entfernen?"),
			QMessageBox::Yes | QMessageBox::No);

	if (r == QMessageBox::No)
	{
		QSqlDatabase::database().rollback();
		return;
	}
	QSqlQuery query;
	query.exec(QString("DELETE FROM treatments "
				"WHERE id = %1").arg(id));

	dataModel->removeRow(index.row());
	dataModel->submitAll();
	if (!dataModel->submitAll())
	{
		QMessageBox msgBox;
		msgBox.setText("Could not submit model :(");
		QSqlError last = QSqlDatabase::database().lastError();
		msgBox.setInformativeText(last.text());
		msgBox.exec();
	}
	QSqlDatabase::database().commit();
	dataModel->select();
	updateTreatmentView();
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
}
