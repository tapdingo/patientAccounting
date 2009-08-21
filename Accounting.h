#ifndef ACCOUNTING_H
#define ACCOUNTING_H

#include "definitions.h"

class QSqlRecord;
class QSqlRelationalTableModel;

class PatientAccounter
{
	public:
		PatientAccounter(
				const QSqlRecord& patient,
				QSqlRelationalTableModel& treats);
		void account();

	private:
		void printBill();
		const QSqlRecord& m_patient;
		QSqlRelationalTableModel& m_treats;
};

#endif
