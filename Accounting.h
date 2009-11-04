#ifndef ACCOUNTING_H
#define ACCOUNTING_H

#include "definitions.h"
#include <inttypes.h>

class QSqlRecord;
class QSqlRelationalTableModel;
class QString;

class PatientAccounter
{
	public:
		PatientAccounter(
				const QSqlRecord& patient,
				QSqlRelationalTableModel& treats);
		void account();

	private:
		void printBill();
		void addPatientHeader(QString& Document);
		bool addTreatments(QString& Document);
		const QSqlRecord& m_patient;
		QSqlRelationalTableModel& m_treats;
		QString addTreatmentRow(
				const QSqlRecord& treatment,
				uint32_t& sum);

		unsigned int m_billNumber;
};

#endif
