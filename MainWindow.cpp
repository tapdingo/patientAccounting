#include "MainWindow.h"

#include <QtGui>
#include <QtSql>
#include <QSqlDatabase>
#include <QSqlRelationalTableModel>

#include "definitions.h"
#include "Accounting.h"
#include "diagnoseBrowser.h"
#include "std_treatBrowser.h"
#include "patientModel.h"
#include "treatmentModel.h"
#include "AccountingForm.h"
#include "stdChoice.h"
#include "treatModifier.h"
#include "treatDelegator.h"

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

	createToolBar();

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
	patientView->selectRow(0);
}

void MainWindow::createPatientPanel()
{
	patientPanel = new QWidget();

	patientModel= new PatientModel();
	patientModel->setTable("patients");
	patientModel->setSort(LastName, Qt::AscendingOrder);

	//Nice Header Info
	patientModel->setHeaderData(FirstName, Qt::Horizontal, tr("Vorname"));
	patientModel->setHeaderData(LastName, Qt::Horizontal, tr("Nachname"));
	patientModel->setHeaderData(DateOfBirth, Qt::Horizontal, tr("Geburtsdatum"));
	patientModel->setHeaderData(Phone, Qt::Horizontal, tr("Telefon"));

	patientModel->select();

	patientView = new QTableView();

	patientView->setModel(patientModel);
	patientView->resizeColumnsToContents();
	patientView->setSelectionMode(
			QAbstractItemView::SingleSelection);
	patientView->setSelectionBehavior(
			QAbstractItemView::SelectRows);
	//patientView->horizontalHeader()->setStretchLastSection(true);
	patientView->verticalHeader()->hide();
	patientView->setShowGrid(false);
	patientView->sortByColumn(LastName, Qt::AscendingOrder);

	//Hide some unneccessary info here
	patientView->setColumnHidden(HouseNumber, true);
	patientView->setColumnHidden(Street, true);
	patientView->setColumnHidden(City, true);
	patientView->setColumnHidden(PostalCode, true);
	patientView->setColumnHidden(Parent, true);
	patientView->setColumnHidden(Gender, true);
	patientView->setColumnHidden(ID, true);

	patientLabel = new QLabel(tr("&Patienten"));
	patientLabel->setBuddy(patientView);

	patientView->horizontalHeader()->moveSection(9,0);
	patientView->horizontalHeader()->moveSection(9,1);
	patientView->horizontalHeader()->moveSection(9,2);
	patientView->resizeColumnsToContents();
	patientView->setSortingEnabled(true);

	patientView->resize(patientView->sizeHint());

	//Disable Editing in the View
	patientView->setEditTriggers(QAbstractItemView::NoEditTriggers);
}

void MainWindow::createDataPanel()
{
	dataPanel = new QWidget();

	dataModel= new TreatmentModel();
	dataModel->setTable("treatments");
	dataModel->setRelation(
			PatientId,
			QSqlRelation("patients", "id", "firstname"));
	dataModel->setSort(DateOfTreat, Qt::DescendingOrder);

	//Nice header info ahead!
	dataModel->setHeaderData(Diagnose, Qt::Horizontal, tr("Diagnose"));
	dataModel->setHeaderData(DateOfTreat, Qt::Horizontal, tr("Datum"));
	dataModel->setHeaderData(Paid, Qt::Horizontal, tr("Bezahlt?"));
	dataModel->setHeaderData(Accounted, Qt::Horizontal, tr("Abgerechnet?"));
	dataModel->setHeaderData(Cost, Qt::Horizontal, tr("Kostenpunkt"));

	dataModel->select();

	dataView = new QTableView();

	TreatDelegator* delegator = new TreatDelegator(dataModel);
	dataView->setItemDelegate(delegator);

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

	//Hide some unneccessary info here
	dataView->setColumnHidden(Details, true);
	dataView->setColumnHidden(Type, true);
	dataView->setColumnHidden(Duration, true);
	dataView->setColumnHidden(DurationOfTreat, true);
	dataView->setColumnHidden(TreatmentID, true);
	dataView->setColumnHidden(PatientId, true);
	dataView->setColumnHidden(Desc, true);
	dataView->setColumnHidden(Paid, true);
	dataView->setColumnHidden(Accounted, true);
	dataView->horizontalHeader()->moveSection(6, 0);
	patientView->sortByColumn(DateOfTreat, Qt::AscendingOrder);


	//Disable Editing in the View
	dataView->setEditTriggers(QAbstractItemView::NoEditTriggers);
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

	newStdTreatmentAction = new QAction(tr("&Behandlung anlegen"), this);
	newStdTreatmentAction->setStatusTip(tr("Legt eine Behandlung an"));

	editTreatmentAction = new QAction(tr("&Bearbeiten"), this);
	editTreatmentAction->setStatusTip(tr("Bearbeitet eine Behandlung"));

	deleteTreatmentAction = new QAction(tr("Entfernen"), this);
	deleteTreatmentAction->setStatusTip(tr("Entfernt eine Behandlung"));

	browseDiagnosesAction = new QAction(tr("&Diagnosen Browser"), this);
	browseDiagnosesAction->setStatusTip(tr("Startet den Diagnosen Browser"));

	browseTreatmentsAction = new QAction(tr("&Behandlungs Vorlagen Browser"), this);
	browseTreatmentsAction->setStatusTip(tr("Startet den Behandlungs Vorlagen Browser"));

	//Accounting related Actions
	accountAction = new QAction(tr("&Einzelabrechnung"), this);
	accountAction->setStatusTip(tr("Rechnet einen Patienten ab"));
	accountAction->setIcon(QIcon(":/img/accounting.png"));

	accountAllAction = new QAction(tr("&Alles Abrechnen"), this);
	accountAllAction->setStatusTip(tr("Rechnet alle Patienten ab"));
	accountAllAction->setIcon(QIcon(":/img/accounting.png"));

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
	connect(newStdTreatmentAction, SIGNAL(triggered()),
			this, SLOT(addStdTreatment()));
	connect(editTreatmentAction, SIGNAL(triggered()),
			this, SLOT(editTreatment()));
	connect(deleteTreatmentAction, SIGNAL(triggered()),
			this, SLOT(deleteTreatment()));
	connect(deleteTreatmentAction, SIGNAL(triggered()),
			this, SLOT(deleteTreatment()));
	connect(browseDiagnosesAction, SIGNAL(triggered()),
			this, SLOT(browseDiagnoses()));
	connect(browseTreatmentsAction, SIGNAL(triggered()),
			this, SLOT(browseTreatments()));
	connect(changeAccounted, SIGNAL(clicked()), this, SLOT(toggleAccounted()));

	//Accounting related connections
	connect(accountAction, SIGNAL(triggered()), this, SLOT(accountPatient()));
	connect(accountAllAction, SIGNAL(triggered()), this, SLOT(accountAll()));

	//Misc related connections
	connect(aboutAction, SIGNAL(triggered()), this, SLOT(about()));
	connect(aboutQTAction, SIGNAL(triggered()), qApp, SLOT(aboutQt()));
	connect(accCheckbox, SIGNAL(stateChanged(int)), this,
			SLOT(setAccFilter(int)));
	connect(monthCheckbox, SIGNAL(stateChanged(int)), this,
			SLOT(setMonthFilter(int)));
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
	treatmentMenu->addAction(newStdTreatmentAction);
	treatmentMenu->addAction(editTreatmentAction);
	treatmentMenu->addAction(deleteTreatmentAction);
	treatmentMenu->addAction(browseDiagnosesAction);
	treatmentMenu->addAction(browseTreatmentsAction);

	//Accounting Menu
	accountingMenu = menuBar()->addMenu(tr("&Abrechnung"));
	accountingMenu->addAction(accountAction);
	accountingMenu->addAction(accountAllAction);

	//Misc Menu
	miscMenu = menuBar()->addMenu(tr("&Sonstiges"));
	miscMenu->addAction(aboutAction);
	miscMenu->addAction(aboutQTAction);
}

void MainWindow::createToolBar()
{
	QToolBar* fileToolBar = new QToolBar("Filtering");
	addToolBar(Qt::BottomToolBarArea, fileToolBar);
	accCheckbox = new QCheckBox("Nur nicht abgerechnete anzeigen", this);
	fileToolBar->addWidget(accCheckbox);

	monthCheckbox = new QCheckBox("Nur ab angegebenem Monat anzeigen", this);
	fileToolBar->addWidget(monthCheckbox);

	month = new QDateEdit();
	month->setDisplayFormat(QString("MM.yyyy"));
	fileToolBar->addWidget(month);

	QToolBar* topToolBar = new QToolBar("SpecialActions");
	addToolBar(Qt::TopToolBarArea, topToolBar);
	changeAccounted = new QPushButton("Abrechnungszustand aendern");
	topToolBar->addWidget(changeAccounted);
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

		//Get the treatment view state
		dataModel->setFilter(QString("1"));
		dataModel->select();

		QString filterString;
		if (m_acc_state == Qt::Checked)
		{
			filterString.append(" AND accounted = 0");
		}

		if (m_month_state == Qt::Checked)
		{
			filterString.append(" AND dateoftreat > '");
			filterString.append(month->date().toString("yyyy-MM-dd"));
			filterString.append("'");
		}

		//construct the filter string
		QString patientId;
		patientId.setNum(id);
		QString filter("patient_id = ");
		filter.append(patientId);
		filter.append(filterString);
		dataModel->setFilter(filter);
	}
	dataModel->select();
	dataView->horizontalHeader()->setVisible(dataModel->rowCount() > 0);
	setDataColors();
	patientView->setCurrentIndex(index);
}

void MainWindow::addPatient()
{
	int rc = dynamic_cast<PatientModel*>(patientModel)->addNewRecord();
	updateTreatmentView();

	//Send the new patient for editing purposes to the form
	if (rc)
	{
		//WARNING! As long as the patients are sorted by name, the new record
		//will always be row 0
		//This is a very ugly hack, maybe i'll fix it...
		filterPatientModel("lastname='__!!__'");
		QSqlRecord record = patientModel->record(0);
		record.setValue(QString("lastname"), "");
		patientModel->setRecord(0, record);
		patientModel->submit();
		clearPatientModel();
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

	PatientModel* int_model = dynamic_cast<PatientModel*>(patientModel);
	if (int_model->deleteRecord(id))
	{
		patientModel->removeRow(index.row());
	}
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

void MainWindow::addStdTreatment()
{
	QModelIndex index = patientView->currentIndex();

	if(index.isValid())
	{
		QSqlRecord record = patientModel->record(index.row());
		int id = record.value("id").toInt();
		stdChoice add(this, id);
		add.exec();
		int returned = add.result();

		TreatmentForm editTreatment(returned, this);
		editTreatment.exec();

		dataModel->select();
		patientModel->select();
		updateTreatmentView();
		updateStatusBar();
	}
}

void MainWindow::editTreatment()
{
	const int id = getCurrentTreatId();
	if (-1 == id)
	{
		return;
	}

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
	if (dynamic_cast<TreatmentModel*>(dataModel)->deleteRecord(id))
	{
		dataModel->removeRow(index.row());
	}
	dataModel->select();
	updateTreatmentView();
	updateStatusBar();
	QModelIndex patient = patientView->currentIndex();
	patientView->setCurrentIndex(patient);
}

void MainWindow::accountPatient()
{
	QModelIndex index = patientView->currentIndex();
	if (!index.isValid())
	{
		return;
	}
	QSqlRecord record = patientModel->record(index.row());

	AccountingForm* acc_window = new AccountingForm(record.value(ID).toInt());
	acc_window->exec();

	//Dunno if i have to delete this here...
	delete acc_window;

	updateTreatmentView();
	updateStatusBar();
}

void MainWindow::browseDiagnoses()
{
	DiagnoseBrowser browser(this);
	browser.exec();
}

void MainWindow::browseTreatments()
{
	std_TreatBrowser browser(this);
	browser.exec();
}

void MainWindow::about()
{
		QMessageBox msgBox;
		msgBox.about(this, "About PatientAccounting",
				"PatientAccounting is a programm to manage and"
				"account patients\n\n"
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

void MainWindow::setAccFilter(int state)
{
	m_acc_state = state;
	updateTreatmentView();
}

void MainWindow::setMonthFilter(int state)
{
	m_month_state = state;
	updateTreatmentView();
}

void MainWindow::setDataColors()
{
	QModelIndex index = dataModel->index(1, 0);
	dataModel->setData(index, QColor(Qt::red), Qt::BackgroundColorRole);
	dataModel->setData(index, Qt::red, Qt::TextColorRole);

	index = patientView->currentIndex();
	dataModel->setData(index, QColor(Qt::red), Qt::BackgroundColorRole);
	dataModel->setData(index, Qt::red, Qt::TextColorRole);
}

void MainWindow::toggleAccounted()
{
	const int id = getCurrentTreatId();

	if (-1 == id)
	{
		return;
	}

	const QModelIndex treatment = dataView->currentIndex();
	const int row = treatment.row();

	TreatModifier modifier;
	QSqlRecord curTreat = dataModel->record(row);
	const int newValue = curTreat.value(Accounted).toInt() ^ 2;
	modifier.setAccounted(&curTreat, newValue);
	dataModel->setRecord(row, curTreat);
	dataModel->submit();

	dataModel->select();
	patientModel->select();
	updateTreatmentView();
	updateStatusBar();
}

int MainWindow::getCurrentTreatId() const
{
	QModelIndex patient = patientView->currentIndex();
	QModelIndex index = dataView->currentIndex();
	if (!index.isValid())
	{
		return -1;
	}
	QSqlRecord record = dataModel->record(index.row());

	//GET THE TREATMENTS A PRIMARY KEY
	int id = record.value(TreatmentID).toInt();

	return id;
}

void MainWindow::filterPatientModel(const QString value)
{
	patientModel->setFilter(value);
	patientModel->select();
}

void MainWindow::clearPatientModel()
{
	patientModel->setFilter("");
	patientModel->select();
}

void MainWindow::accountAll()
{
	AccountingForm* acc_window = new AccountingForm();
	acc_window->exec();

	//Dunno if i have to delete this here...
	delete acc_window;

	updateTreatmentView();
	updateStatusBar();
}
