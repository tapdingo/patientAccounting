#include "MainWindow.h"

#include <QtGui>
#include <QtSql>
#include <QSqlDatabase>
#include <QSqlRelationalTableModel>

MainWindow::MainWindow()
{
	//Connect to the database
	connectToDB();

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

	this->resize(800, 600);

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
	patientView->setColumnHidden(0, true);
	patientView->setShowGrid(false);


	patientLabel = new QLabel(tr("&Patienten"));
	patientLabel->setBuddy(patientView);

	connect(patientView->selectionModel(), SIGNAL(currentRowChanged(const QModelIndex&, 
					const QModelIndex&)), 
			this, SLOT(updateTreatmentView()));
	connect(newAction, SIGNAL(triggered()), this, SLOT(addPatient()));
}

void MainWindow::createDataPanel()
{
	dataPanel = new QWidget();

	dataModel= new QSqlRelationalTableModel(this);
	dataModel->setTable("treatments");
	dataModel->setRelation(0, QSqlRelation("patients", "id", "vorname"));
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
}
