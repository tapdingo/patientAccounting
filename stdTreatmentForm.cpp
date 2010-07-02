#include "stdTreatmentForm.h"
#include <QtGui>

#include <QDataWidgetMapper>
#include <iostream>

#include <definitions.h>
#include "Parser.h"

stdTreatmentForm::stdTreatmentForm(
		int id,
		QWidget* parent) : QDialog(parent, Qt::Window)
{
	//Database and stuff...
	m_model = new QSqlRelationalTableModel(this);
	m_model->setTable("std_treats");
	m_model->setSort(std_treat::treatId, Qt::AscendingOrder);
	m_model->setFilter(QString("id = %1").arg(id));
	m_model->select();

	//Nice and cosy...
	createLayout();

	m_mapper = new QDataWidgetMapper(this);
	m_mapper->setSubmitPolicy(QDataWidgetMapper::ManualSubmit);
	m_mapper->setModel(m_model);
	m_mapper->addMapping(nameField, std_treat::treatName);
	m_mapper->toFirst();

	//Pretty signalling dude!
	connect(closeButton, SIGNAL(clicked()), this, SLOT(accept()));
	connect(saveButton, SIGNAL(clicked()), this, SLOT(saveStdTreatment()));
	connect(
			details,
			SIGNAL(stateChanged(int)),
			this,
			SLOT(expandHideDetails(int)));
	connect(
			noOfDetails,
			SIGNAL(valueChanged(int)),
			this,
			SLOT(noDetailsChanged(int)));

	//Set unmappable values
	initialUpdate();
}

void stdTreatmentForm::saveStdTreatment()
{

	if (!checkForDetails())
	{
		QMessageBox msgBox;
		msgBox.setText("Fehler: Die Detailkosten passen nicht zu den Gesamtkosten!");
		QSqlError last = m_model->lastError();
		msgBox.setInformativeText(last.text());
		msgBox.exec();
		return;
	}

	int index = m_mapper->currentIndex();

	if (!m_mapper->submit())
	{
		QMessageBox msgBox;
		msgBox.setText("Could not submit treatment");
		QSqlError last = m_model->lastError();
		msgBox.setInformativeText(last.text());
		msgBox.exec();
		return;
	}


	//Explicitly set some values not covered by the Mapper
	QSqlRecord record = m_model->record(index);
	int id =  record.value(std_treat::treatId).toInt();

	//DATABASE RAPE AHOY
	//Push the damn detail vector into the poor database
	QString dumpedDetails;
	dumpDetails(dumpedDetails);
	record.setValue(std_treat::treatText, dumpedDetails);

	m_model->setRecord(0, record);
	if (!m_model->submit())
	{
		QMessageBox msgBox;
		msgBox.setText("Fehler beim Speichern!");
		QSqlError last = m_model->lastError();
		msgBox.setInformativeText(last.text());
		msgBox.exec();
	}
	m_mapper->setCurrentIndex(index);

	//Close the window
	accept();
}


void stdTreatmentForm::createLayout()
{
	mainLayout = new QGridLayout(this);
	setLayout(mainLayout);

	nameLabel = new QLabel(tr("Beschreibung"));
	nameField = new QLineEdit;
	nameLabel->setBuddy(nameField);
	mainLayout->addWidget(nameLabel, 5, 0);
	mainLayout->addWidget(nameField, 5, 1);

	details = new QCheckBox("Genauere Behandlungsangeben");
	numLabel = new QLabel(tr("Anzahl Detailfelder"));
	noOfDetails = new QSpinBox();
	mainLayout->addWidget(details, 8, 0);

	mainLayout->addWidget(noOfDetails, 8, 1);
	noOfDetails->hide();

	detailsLabel = new QLabel("Details");
	detailsLabel->hide();
	mainLayout->addWidget(detailsLabel, 9, 0);

	costDetailLabel = new QLabel("Kosten-Details");
	costDetailLabel->hide();
	mainLayout->addWidget(costDetailLabel, 9, 1);

	detailFields = new QVBoxLayout;
	mainLayout->addLayout(detailFields, 10, 0);

	costDetailFields = new QVBoxLayout;
	mainLayout->addLayout(costDetailFields, 10, 1);

	saveButton = new QPushButton(tr("&Speichern"));
	saveButton->setEnabled(true);
	mainLayout->addWidget(saveButton, 11, 0);

	closeButton = new QPushButton(tr("&Abbrechen"));
	closeButton->setDefault(true);
	mainLayout->addWidget(closeButton, 11, 1);

	setLayout(mainLayout);
	setWindowTitle(tr("Behandlung bearbeiten..."));
}

void stdTreatmentForm::expandHideDetails(int state)
{
	if (state == Qt::Checked)
	{
		//Show some stuff
		noOfDetails->show();
		detailsLabel->show();
		costDetailLabel->show();

		//Create a new tuple for showing
		LayoutTuple* n_tuple = new LayoutTuple;
		n_tuple->detField = new QLineEdit;
		n_tuple->costDetField = new QLineEdit;
		detailFieldsDesc.push_back(n_tuple);
		detailFields->addWidget(n_tuple->detField);
		costDetailFields->addWidget(n_tuple->costDetField);

		//Set the correct value
		noOfDetails->setValue(1);
	}
	else
	{
		noOfDetails->hide();
		detailsLabel->hide();
		costDetailLabel->hide();

		//Throw all the crap away
		std::vector<LayoutTuple*>::iterator it;
		for (it = detailFieldsDesc.begin(); it != detailFieldsDesc.end(); it++)
		{
			(*it)->detField->hide();
			(*it)->costDetField->hide();
			delete *it;
		}
		detailFieldsDesc.clear();
	}
}

void stdTreatmentForm::noDetailsChanged(int number)
{
	if (detailFieldsDesc.size() < static_cast<uint32_t>(number))
	{
		//Create a new tuple for showing
		LayoutTuple* n_tuple = new LayoutTuple;
		n_tuple->detField = new QLineEdit;
		n_tuple->costDetField = new QLineEdit;
		detailFieldsDesc.push_back(n_tuple);
		detailFields->addWidget(n_tuple->detField);
		costDetailFields->addWidget(n_tuple->costDetField);

		//RECURSION ALERT!
		noDetailsChanged(number);
	}
	else if (detailFieldsDesc.size() > static_cast<uint32_t>(number))
	{

		LayoutTuple* n_tuple = detailFieldsDesc.back();
		n_tuple->detField->hide();
		n_tuple->costDetField->hide();
		delete n_tuple;
		detailFieldsDesc.pop_back();

		//RECURSION ALERT!
		noDetailsChanged(number);
	}
}

void stdTreatmentForm::dumpDetails(QString& result)
{
	std::vector<LayoutTuple*>::iterator it;

	for (it = detailFieldsDesc.begin(); it != detailFieldsDesc.end(); it++)
	{
		result.append((*it)->detField->text());
		result.append("||");
		result.append((*it)->costDetField->text());
		result.append(";");
	}
}

void stdTreatmentForm::reconstructDetailVector(QString& data)
{
	QStringList splitted = data.split(";", QString::SkipEmptyParts);

	if (splitted.empty())
	{
		std::cerr << "EMPTY";
		return;
	}

	for (int i = 0; i < splitted.size(); i++)
	{
		QStringList line_details = splitted[i].split("||", QString::SkipEmptyParts);
		if ( line_details.size() > 0)
		{
			DetailTuple* n_tuple = new DetailTuple;
			n_tuple->detail = line_details[0];
			n_tuple->cost = line_details[1].toFloat();
			detailVector.push_back(n_tuple);
		}
	}

	if (splitted.size() != 0)
	{
		details->setChecked(Qt::Checked);
		noOfDetails->setValue(splitted.size());
	}
}

void stdTreatmentForm::initialUpdate()
{
	QSqlRecord record = m_model->record(m_mapper->currentIndex());
	QString details_raw = record.value(std_treat::treatText).toString();

	reconstructDetailVector(details_raw);
	std::vector<DetailTuple*>::iterator it;

	int i = 0;
	for (it = detailVector.begin(); it != detailVector.end(); it ++)
	{
		QString cost;
		cost.setNum((*it)->cost);
		detailFieldsDesc[i]->detField->setText((*it)->detail);
		detailFieldsDesc[i]->costDetField->setText(cost);
		i++;
	}

	//Set the default values for a new treatment
	if (detailVector.empty())
	{
		details->setChecked(Qt::Checked);
		noOfDetails->setValue(3);
		detailFieldsDesc[0]->detField->setText("Anamese");
		detailFieldsDesc[1]->detField->setText("Diagnose");
		detailFieldsDesc[2]->detField->setText("Mittelfindung");
	}
}

bool stdTreatmentForm::checkForDetails()
{
	if (details->checkState() != Qt::Checked)
	{
		return true;
	}
	std::vector<LayoutTuple*>::iterator it;

	int total_cost = 0;

	for (it = detailFieldsDesc.begin(); it != detailFieldsDesc.end(); it++)
	{
		total_cost += (*it)->costDetField->text().toInt();
	}

	return total_cost == 100;
}

void stdTreatmentForm::updateDetails(const QString& newCost)
{
	//The Number of details is no longer the default number,
	//Don't mess with the number, damnit!
	if (noOfDetails->value() == 3)
	{
		if (detailFieldsDesc.size() > 2)
		{
		}
		return;
	}
}

