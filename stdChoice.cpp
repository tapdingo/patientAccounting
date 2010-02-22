#include "stdChoice.h"
#include <QtGui>

#include <iostream>

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
	stdLabel = new QLabel(tr("Standard Behandlung"));
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
	m_choice->setModelColumn(0);

	connect(addButton, SIGNAL(clicked()), this, SLOT(addStdTreatment()));
	connect(cancelButton, SIGNAL(clicked()), this, SLOT(cancel()));
}

void stdChoice::addStdTreatment()
{
	std::cerr << "Patient: " << m_patient;
}

void stdChoice::cancel()
{
	accept();
}
