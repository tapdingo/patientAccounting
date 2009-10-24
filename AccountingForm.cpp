#include "AccountingForm.h"

#include <QtGui>
#include <QtSql>
#include <QSqlDatabase>
#include <QSqlRelationalTableModel>
#include <iostream>

AccountingForm::AccountingForm(const int& pat_id) : m_patient(pat_id)
{
	createLayout();
	makeConnections();

	m_patients = new QSqlRelationalTableModel();
	m_patients->setTable("patients");
	m_patients->select();
}


void AccountingForm::createLayout()
{
	m_layout = new QGridLayout(this);

	m_dateLabel = new QLabel(tr("Zu abrechnender Monat: "));
	m_dateEdit = new QDateEdit;
	m_dateEdit->setCalendarPopup(true);
	m_dateEdit->setDisplayFormat("MM.yyyy");
	m_dateLabel->setBuddy(m_dateEdit);
	m_layout->addWidget(m_dateLabel, 0, 0);
	m_layout->addWidget(m_dateEdit, 0, 1);

	m_okButton = new QPushButton("Ok");
	m_cancelButton = new QPushButton("Abbrechen");

	m_thisPatient = new QRadioButton("Nur diesen Patient abrechnen");
	m_allPatients = new QRadioButton("Alle Patienten abrechnen");
	m_layout->addWidget(m_thisPatient, 1, 0);
	m_layout->addWidget(m_allPatients, 1, 1);

	m_layout->addWidget(m_okButton, 3, 0);
	m_layout->addWidget(m_cancelButton, 3, 1);

	m_progLabel = new QLabel("Abrechnung laueft...");
	m_progBar = new QProgressBar();
	m_layout->addWidget(m_progLabel, 2, 0);
	m_layout->addWidget(m_progBar, 2, 1);
	m_progLabel->hide();
	m_progBar->hide();

	m_thisPatient->setChecked(true);
	setLayout(m_layout);
}

void AccountingForm::performAccounting()
{
	m_dateEdit->setEnabled(false);
	m_thisPatient->setEnabled(false);
	m_allPatients->setEnabled(false);
	m_okButton->setEnabled(false);
	m_cancelButton->setEnabled(false);

	//Call the allmighty accounter here
	m_progLabel->show();
	m_progBar->show();

	if (m_thisPatient->isChecked())
	{
	}
	else
	{
		std::cerr << m_patients->rowCount();
		m_progBar->setMaximum(m_patients->rowCount());
		for (int i = 0; i < m_patients->rowCount(); i++)
		{
			m_progBar->setValue(i + 1);
		}
	}

	m_progLabel->setText("Abrechnung beendet");
	m_cancelButton->setEnabled(true);
	m_cancelButton->setText("Beenden");

}

void AccountingForm::makeConnections()
{
	connect(m_okButton, SIGNAL(clicked()), this, SLOT(performAccounting()));
	connect(m_cancelButton, SIGNAL(clicked()), this, SLOT(accept()));
}
