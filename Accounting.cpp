#include "Accounting.h"
#include <QtGui>
#include <QtSql>


PatientAccounter::PatientAccounter(
		const QSqlRecord& patient, QSqlRelationalTableModel& treats)
: m_patient(patient), m_treats(treats)
{
}

void PatientAccounter::account()
{
	printBill();
}

void PatientAccounter::printBill()
{
	QPrinter printer;
	QPrintDialog printDialog(&printer, 0);
	if (printDialog.exec())
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

		QTextDocument textDocument;
		textDocument.setHtml(Document);
		textDocument.print(&printer);
	}
}

void PatientAccounter::addPatientHeader(QString& Document)
{
	Document.append("<table width=100%><tr>");
	Document.append("<td>Rechnungs Nummer: </td>");
	Document.append("<td>Datum: </td>");
	Document.append("</tr><tr>");
	Document.append("<td>Patient: "
			+ m_patient.value(FirstName).toString() + " "
			+ m_patient.value(LastName).toString() +
			"</td>");
	Document.append("<td>Geb. Datum: " + m_patient.value(DateOfBirth).toString() + " </td>");
	Document.append("</tr><tr>");
	Document.append("<td>Diagnose: </td>");
	Document.append("<td></td>");
	Document.append("</tr></table>");
}

void PatientAccounter::addTreatments(QString& Document)
{
	Document.append("<table width=100%><tr>");

	//Iterate over all Treatments
	//\todo Block out already accounted treatments
	for (int i = 0; i < m_treats.rowCount(); i++)
	{
		Document.append("<td>Datum </td>");
		Document.append("<td>1(2)</td>");
		Document.append("<td>Name</td>");
		Document.append("<td>Kosten</td>");
		Document.append("</tr><tr>");
	}

	//Create the Final Sum Row
	Document.append("<td><b>Endsumme: </b></td>");
	Document.append("<td></td>");
	Document.append("<td></td>");
	Document.append("<td>120 €</td>");
	Document.append("</tr></table>");
}
