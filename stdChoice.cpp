#include "stdChoice.h"
#include <QtGui>

#include <iostream>

#include "treatmentModel.h"
#include "definitions.h"

stdChoice::stdChoice(QWidget* parent, const int& patientId)
	: m_patient(patientId)
{
	if (!parent)
	{
		abort();
	}
	createLayout();
	init();
}

void stdChoice::createLayout()
{
	QGridLayout* mainLayout = new QGridLayout;
	stdLabel = new QLabel(tr("Behandlungsvorlage"));
	m_choice = new QComboBox;
	mainLayout->addWidget(stdLabel, 0, 0);
	mainLayout->addWidget(m_choice, 0, 1);

	addButton = new QPushButton(tr("&Speichern"));
	cancelButton = new QPushButton(tr("&Abbrechen"));
	mainLayout->addWidget(addButton, 1, 0);
	mainLayout->addWidget(cancelButton, 1, 1);

	setLayout(mainLayout);
}

void stdChoice::init()
{
	m_model = new QSqlTableModel(this);
	m_model->setTable("std_treats");
	m_model->select();
	m_choice->setModel(m_model);
	m_choice->setCompleter(m_choice->completer());
	m_choice->setModelColumn(2);

	connect(addButton, SIGNAL(clicked()), this, SLOT(addStdTreatment()));
	connect(cancelButton, SIGNAL(clicked()), this, SLOT(cancel()));
}

void stdChoice::filterModel()
{
	QString filter("name='");
	filter.append(m_choice->currentText());
	filter.append("'");
	m_model->setFilter(filter);
	m_model->select();
}

void stdChoice::unfilterModel()
{
	m_model->setFilter("");
	m_model->select();
}

void stdChoice::addStdTreatment()
{
	//Get the correct stdTreatmentEntry out of the database
	filterModel();
	QSqlRecord treatToAdd = m_model->record(0);
	QString newText = treatToAdd.value(std_treat::treatText).toString();
	QString newDesc = treatToAdd.value(std_treat::treatDesc).toString();
	QString newName = treatToAdd.value(std_treat::treatName).toString();

	//Add a blank treatment to the patient
	TreatmentModel treatments;
	treatments.setTable("treatments");
	treatments.select();
	int treatId = treatments.addNewRelRecord(m_patient);
	treatments.select();

	setResult(treatId);

	//And fill it with live!
	QString id;
	id.setNum(treatId);
	QString filter("id='");
	filter.append(id);
	filter.append("'");
	treatments.setFilter(filter);
	treatments.select();
	QSqlRecord newRecord = treatments.record(0);
	newRecord.setValue(TreatmentName, newName);
	newRecord.setValue(Details, newText);
	newRecord.setValue(Desc, newDesc);

	if (!(treatments.setRecord(0, newRecord)))
	{
		QMessageBox msgBox;
		msgBox.setText("Fehler beim Anlegen der neuen Werte");
		QSqlError last = treatments.lastError();
		msgBox.setInformativeText(last.text());
		msgBox.exec();
	}

	if (!treatments.submit())
	{
		QMessageBox msgBox;
		msgBox.setText("Fehler beim Speichern!");
		QSqlError last = treatments.lastError();
		msgBox.setInformativeText(last.text());
		msgBox.exec();
	}

	//Restore the old form state
	unfilterModel();
	done(treatId);
}

void stdChoice::cancel()
{
	accept();
}
