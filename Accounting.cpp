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
		QTextDocument textDocument;
		textDocument.setPlainText(form);
		textDocument.print(&printer);
	}
}
