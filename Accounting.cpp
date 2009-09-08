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
		QTextDocument textDocument;
		textDocument.setHtml(Document);
		textDocument.print(&printer);
	}
}
