#include "Accounting.h"
#include <QtGui>
#include <QtSql>
#include <iostream>
#include <fstream>
#include "Parser.h"

PatientAccounter::PatientAccounter(
		const QSqlRecord& patient,
		QSqlRelationalTableModel& treats)
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
	if (m_treats.rowCount() == 0)
	{
		//Nothing to account, simple return
		return;
	}
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
	Document.append("<br>");

	addPatientHeader(Document);

	Document.append("<br><br>");
	Document.append("F&uuml;r meine Bem&uuml;hungen erlaube ich mir nach \
			der Geb&uuml;hrenordnung f&uuml;r Heilpraktiker \
			(in Anlehnung an die \
			 Geb&uuml;hrenordnung f&uuml;r &Auml;rzte) \
			zu berechnen:<br>");

	if (!addTreatments(Document))
	{
		//Again nothing to account, simple return
		return;
	}

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

	//Current Date
	Document.append("<td>Datum: "
			+ QDate::currentDate().toString(Qt::SystemLocaleShortDate)
			+ " </td>");
	Document.append("</tr><tr>");

	//Patient Name
	Document.append("<td>Patient: "
			+ m_patient.value(FirstName).toString() + " "
			+ m_patient.value(LastName).toString() +
			"</td>");

	//Patient Birth Date
	Document.append("<td>Geb. Datum: "
			+ m_patient.value(DateOfBirth).toString()
			+ " </td>");
	Document.append("</tr><tr>");

	//Patient Address
	QString patientAddress("Adresse: ");
	patientAddress.append(generatePatientAddress());
	Document.append("<td>");
	Document.append(patientAddress);

	Document.append("</td>");
	Document.append("<td></td>");
	Document.append("</tr></table>");
}

bool PatientAccounter::addTreatments(QString& Document)
{
	bool treated = false;
	Document.append("<table width=100%><tr>");

	uint32_t sum = 0;

	Document.append("<br>");
	Document.append("<td><b>Datum </b></td>");
	Document.append("<td></td>");
	Document.append("<td><b>Diagnose</b></td>");
	Document.append("<td><b>Behandlungsdetails</b></td>");
	Document.append("<td><b>Kostenpunkt</b></td>");
	Document.append("</tr>");

	//Iterate over all Treatments
	for (int i = 0; i < m_treats.rowCount(); i++)
	{
		QSqlRecord treatment = m_treats.record(i);

		if (treatment.value(Accounted).toInt() == 1 ||
				treatment.value(Accounted).toInt() == 2)
		{
			continue;
		}

		treatment.setValue(QString("accounted"), 2);
		m_treats.setRecord(i, treatment);
		treated = true;

		if (!m_treats.submit())
		{
			std::cerr << "Failed to update Treatment! \n";
		}

		Document.append(addTreatmentRow(treatment, sum));
	}

	//Create the Final Sum Row
	QString sumString;
	sumString.setNum(sum);
	Document.append("<td><b>Endsumme: </b></td>");
	Document.append("<td></td>");
	Document.append("<td></td>");
	Document.append("<td></td>");
	Document.append("<td>" + sumString + " &euro;</td>");
	Document.append("</tr></table>");


	return treated;
}

QString PatientAccounter::addTreatmentRow(
		const QSqlRecord& treatment,
		uint32_t& sum)
{
	QString treatmentRow;
	int treatCost = treatment.value(Cost).toInt();
	QString costString;
	costString.setNum(treatCost);
	sum += treatCost;

	QDate date = treatment.value(DateOfTreat).toDate();

	//Get The Details of this treatments
	std::vector<DetailTuple*> details;
	Parser::reconstructDetails(treatment.value(Details).toString(), details);

	QString typeString("undefined");

	if (treatment.value(Type).toInt() == Telephone)
	{
		typeString = "Telefonische Behandlung";
	}
	else
	{
		typeString = "Praxis Behandlung";
	}

	treatmentRow.append("<td>" + date.toString(Qt::SystemLocaleShortDate) + "</td>");
	treatmentRow.append("<td>1(2)</td>");
	treatmentRow.append("<td>");
	treatmentRow.append(treatment.value(Diagnose).toString());
	treatmentRow.append("</td><td>");

	//Add all the lovely details
	std::vector<DetailTuple*>::iterator it;
	for (it = details.begin(); it != details.end(); it++)
	{
		treatmentRow.append((*it)->detail);
		treatmentRow.append(", ");
	}
	treatmentRow.append(typeString);

	treatmentRow.append("</td>");
	treatmentRow.append("<td>" + costString + "&euro;</td>");
	treatmentRow.append("</tr><tr>");

	//Clean up the mess...
	Parser::clearDetails(details);

	return treatmentRow;
}

QString PatientAccounter::generatePatientAddress()
{
	QString address;

	address.append(m_patient.value(Street).toString());
	address.append(" ");
	address.append(m_patient.value(HouseNumber).toString());
	address.append(", ");
	address.append(m_patient.value(PostalCode).toString());
	address.append(" ");
	address.append(m_patient.value(City).toString());

	return address;
}
