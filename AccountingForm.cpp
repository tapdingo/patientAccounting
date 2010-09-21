#include "AccountingForm.h"

#include <QtGui>
#include <QtSql>
#include <QSqlDatabase>
#include <QSqlRelationalTableModel>
#include <iostream>

#include "Accounting.h"
#include "definitions.h"

AccountingForm::AccountingForm(const int& pat_id) : m_patient(pat_id)
{
	createLayout();
	makeConnections();

	m_patients = new QSqlRelationalTableModel();
	m_patients->setTable("patients");
	m_patients->select();

	m_treats = new QSqlRelationalTableModel();
	m_treats->setTable("treatments");
	m_treats->select();

	initialUpdate();
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
	m_accountAll = new QPushButton("Alles abrechnen...");

	m_thisPatient = new QRadioButton("Nur diesen Patient abrechnen");
	m_allPatients = new QRadioButton("Alle Patienten abrechnen");
	m_layout->addWidget(m_thisPatient, 1, 0);
	m_layout->addWidget(m_allPatients, 1, 1);

	m_layout->addWidget(m_okButton, 3, 0);
	m_layout->addWidget(m_cancelButton, 3, 1);
	m_layout->addWidget(m_accountAll, 3, 2);

	m_progLabel = new QLabel("Abrechnung laueft...");
	m_progBar = new QProgressBar();
	m_layout->addWidget(m_progLabel, 2, 0);
	m_layout->addWidget(m_progBar, 2, 1);
	m_progLabel->hide();
	m_progBar->hide();

	m_allPatients->setChecked(true);
	setLayout(m_layout);
}

void AccountingForm::disableButtons()
{
	m_dateEdit->setEnabled(false);
	m_thisPatient->setEnabled(false);
	m_allPatients->setEnabled(false);
	m_okButton->setEnabled(false);
	m_cancelButton->setEnabled(false);
}

void AccountingForm::enableButtons()
{
	m_dateEdit->setEnabled(true);
	m_thisPatient->setEnabled(true);
	m_allPatients->setEnabled(true);
	m_okButton->setEnabled(true);
	m_cancelButton->setEnabled(true);
}

void AccountingForm::performAccounting()
{
	disableButtons();

	//Call the allmighty accounter here
	m_progLabel->show();
	m_progBar->show();

	if (m_thisPatient->isChecked())
	{
		QString filter("id = ");
		QString patId;
		patId.setNum(m_patient);
		filter.append(patId);
		m_patients->setFilter(filter);
		m_patients->select();

		accountPatient(0);
		m_progBar->setMaximum(1);
		m_progBar->setValue(1);
	}
	else
	{
		m_progBar->setMaximum(m_patients->rowCount());
		for (int i = 0; i < m_patients->rowCount(); i++)
		{
			QSqlRecord curPat = m_patients->record(i);
			QString patName = curPat.value(FirstName).toString();
			patName.append(" ");
			patName.append(curPat.value(LastName).toString());
			m_progLabel->setText(patName);
			accountPatient(i);
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
	connect(m_accountAll, SIGNAL(clicked()), this, SLOT(accountEverything()));
}

bool AccountingForm::accountPatient(const int patientRow, const QDate date)
{
	QSqlRecord patient = m_patients->record(patientRow);
	QString filter("patient_id = ");
	filter.append(patient.value(ID).toString());

	QDate curDate;
	curDate.setDate(date.year(), date.month(), 1);
	QDate futDate;
	futDate.setDate(date.year(), date.month(), date.daysInMonth());
	QString filterString;
	filterString.append(" AND ");
	filterString.append("dateoftreat BETWEEN '");
	filterString.append(curDate.toString("yyyy-MM-dd"));
	filterString.append("' and '");
	filterString.append(futDate.toString("yyyy-MM-dd"));
	filterString.append("'");

	filter.append(filterString);
	m_treats->setFilter(filter);
	m_treats->select();

	PatientAccounter accounter(patient, *m_treats);
	accounter.account(date);

	return true;
}

void AccountingForm::initialUpdate()
{
	QSqlRecord latestTreat = m_treats->record(m_treats->rowCount() - 1);
	if (latestTreat.isEmpty())
	{
		return;
	}

	QString lastDate = latestTreat.value("dateoftreat").toString();
	QStringList splittedDate = lastDate.split("-");
	QDate curDate(splittedDate[0].toInt(), splittedDate[1].toInt(), splittedDate[2].toInt());
	m_dateEdit->setDate(curDate);
}

void AccountingForm::accountEverything()
{
	disableButtons();
	m_progLabel->show();
	m_progBar->show();

	QDate firstUnaccMonth = getFirstUnaccMonth();
	m_progBar->setMaximum(m_patients->rowCount());

	//get all Patients...
	for (int i = 0; i < m_patients->rowCount(); i++)
	{
		QSqlRecord curPat = m_patients->record(i);
		QString patName = curPat.value(FirstName).toString();
		patName.append(" ");
		patName.append(curPat.value(LastName).toString());
		m_progLabel->setText(patName);
		std::cerr << "Accounting Patient: " << patName.toStdString() << std::endl;
		m_progBar->setValue(i + 1);

		//Iterate over all open accounting dates for this patient
		QDate curDate = QDate::currentDate();
		int yearDiff = curDate.year() - firstUnaccMonth.year();
		int monthDiff = curDate.month() - firstUnaccMonth.month();
		std::cerr << "First unacc: " << firstUnaccMonth.toString().toStdString() << std::endl;
		std::cerr << "Cur: " << curDate.toString().toStdString() << std::endl;
		std::cerr << "Months Difference: " << getMonthDiff(firstUnaccMonth, curDate) << std::endl;
		QDate tempDate = firstUnaccMonth;
		while (tempDate < curDate)
		{
			//std::cerr << "Accounting: " << tempDate.toString().toStdString() << std::endl;
			accountPatient(i, tempDate);
			tempDate = tempDate.addMonths(1);
		}

		//Update the view of the form
		update();
	}
	std::cerr << "Accounting everything finished" << std::endl;
	//PatientAccounter* accounter = new PatientAccounter(patient, *m_treats);
	//accounter->account();
	//delete accounter;
	enableButtons();
}

QDate AccountingForm::getFirstUnaccMonth()
{
	QString filterString("");
	filterString.append("accounted = 0");
	m_treats->setFilter(filterString);
	m_treats->select();
	QSqlRecord latestTreat = m_treats->record(0);

	QString dateString = latestTreat.value("dateoftreat").toString();
	std::cerr << "DateString: " << dateString.toStdString() << std::endl;
	QDate date = QDate::fromString(dateString, "yyyy-MM-dd");
	std::cerr << date.toString().toStdString() << " found!" << std::endl;
	date.setDate(date.year(), date.month(), 1);
	return date;
}

int AccountingForm::getMonthDiff(const QDate& startDate, const QDate& endDate) const
{
	const int yearDiff =  endDate.year() - startDate.year();
	const int monthDiff = endDate.month() - startDate.month();

	//Both dates are in the same year
	if (0 == yearDiff)
	{
		return monthDiff;
	}

	std::cerr << "Year Diff: " << yearDiff << std::endl;

	int dateDiff = endDate.month();
	for (int i = 0; i < yearDiff; i ++)
	{
		dateDiff += 12;
	}
	dateDiff += (12 - startDate.month());

	return dateDiff;
}
