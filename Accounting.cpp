#include "Accounting.h"
#include <QtGui>
#include <QtSql>
#include <iostream>
#include <fstream>

PatientAccounter::PatientAccounter(
		const QSqlRecord& patient, QSqlRelationalTableModel& treats)
: m_patient(patient), m_treats(treats)
{
	QSqlTableModel* miscTable = new QSqlTableModel;
	miscTable->setTable("misc");
	miscTable->select();
	QSqlRecord miscRecord = miscTable->record(0);
	m_billNumber = miscRecord.value(BillNumber).toInt();
	delete miscTable;
}

void PatientAccounter::account()
{
	printBill();
}

void PatientAccounter::printBill()
{
	QString Document;

	//Get The document header
	QString Header;
	QFile header(":forms/header.html");
	if (!header.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		return;
	}

	QTextStream in(&header);
	while (!in.atEnd())
	{
		Header.append(in.readLine());
	}

	Document.append(Header);

	//Add the patient Address
	Document.append("<br>");
	QString patientAddress("Adresse: ");
	Document.append("<br>");
	Document.append("<br>");

	Document.append(patientAddress);

	addPatientHeader(Document);

	Document.append("<br><br>");
	Document.append("F&uuml;r meine Bem&uuml;hungen erlaube ich mir nach \
			der Geb&uuml;hrenordnung f&uuml;r Heilpraktiker \
			(in Anlehnung an die \
			 Geb&uuml;hrenordnung f&uuml;r &Auml;rzte) \
			zu berechnen:<br>");

	addTreatments(Document);

	//Create the bill RTF for this patient
	QString billFile("Abrechnungen/");
	billFile.append(m_patient.value(FirstName).toString());
	billFile.append(m_patient.value(LastName).toString());

	QString monthString;
	monthString.setNum(QDate::currentDate().month());
	QString yearString;
	yearString.setNum(QDate::currentDate().year());

	billFile.append(yearString);
	billFile.append(monthString);
	billFile.append(".rtf");
	std::ofstream outStream;
	outStream.open(billFile.toStdString().c_str());
	outStream << Document.toStdString();

	//Optionally print the document
	//QPrinter printer;
	//QPrintDialog printDialog(&printer, 0);
	//if (printDialog.exec())
	//{
		//QTextDocument textDocument;
		//textDocument.setHtml(Document);
		//textDocument.print(&printer);
	//}

	//Increment the Bill Identifier
	QSqlTableModel* miscTable = new QSqlTableModel;
	miscTable->setTable("misc");
	miscTable->select();
	QSqlRecord miscRecord = miscTable->record(0);
	miscRecord.setValue(BillNumber, m_billNumber + 1);
	miscTable->setRecord(0, miscRecord);
	miscTable->submit();
	delete miscTable;
}

void PatientAccounter::addPatientHeader(QString& Document)
{
	Document.append("<table width=100%><tr>");
	Document.append("<td>Rechnungs Nummer: ");
	QString IdString;
	QString billNumber;
	billNumber.setNum(m_billNumber);
	QString curMonth;
	curMonth.setNum(QDate::currentDate().month());
	QString curYear;
	curYear.setNum(QDate::currentDate().year());

	IdString.append(curMonth);
	IdString.append(curYear);
	IdString.append("/");
	IdString.append(billNumber);
	Document.append(IdString);

	Document.append("<td>");
	Document.append("<td>Datum: "
			+ QDate::currentDate().toString(Qt::SystemLocaleShortDate)
			+ " </td>");
	Document.append("</tr><tr>");
	Document.append("<td>Patient: "
			+ m_patient.value(FirstName).toString() + " "
			+ m_patient.value(LastName).toString() +
			"</td>");
	Document.append("<td>Geb. Datum: "
			+ m_patient.value(DateOfBirth).toString()
			+ " </td>");
	Document.append("</tr><tr>");
	Document.append("<td>Letzte Diagnose: </td>");
	Document.append("<td></td>");
	Document.append("</tr></table>");
}

void PatientAccounter::addTreatments(QString& Document)
{
	Document.append("<table width=100%><tr>");

	uint32_t sum = 0;

	//Iterate over all Treatments
	//\todo Block out already accounted treatments
	for (int i = 0; i < m_treats.rowCount(); i++)
	{
		QSqlRecord treatment = m_treats.record(i);

		if (treatment.value(Accounted).toInt() == 1)
		{
			continue;
		}
		treatment.setValue(QString("accounted"), 1);
		m_treats.setRecord(i, treatment);

		if (!m_treats.submit())
		{
			std::cerr << "Failed to update Treatment! \n";
		}

		int treatCost = treatment.value(Cost).toInt();
		QString costString;
		costString.setNum(treatCost);
		sum += treatCost;

		QDate date = treatment.value(DateOfTreat).toDate();

		Document.append("<td>" + date.toString(Qt::SystemLocaleShortDate) + "</td>");
		Document.append("<td>1(2)</td>");
		Document.append("<td>"
				+ treatment.value(TreatmentName).toString()
				+ "</td>");
		Document.append("<td>" + costString + "&euro;</td>");
		Document.append("</tr><tr>");
	}

	//Create the Final Sum Row
	QString sumString;
	sumString.setNum(sum);
	Document.append("<td><b>Endsumme: </b></td>");
	Document.append("<td></td>");
	Document.append("<td></td>");
	Document.append("<td>" + sumString + " &euro;</td>");
	Document.append("</tr></table>");
}
