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
		void addRTFHeader(QString& document) const;
		void addFooter(QString& document) const;
		void addDocumentHeader(QString& document) const;
		void addDocumentFooter(QString& document) const;
		void addInfoText(QString& document) const;
		void finishRTF(QString& document) const;
		void printBill();
		void addPatientHeader(QString& Document);
		bool addTreatments(QString& Document);
		const QSqlRecord& m_patient;
		QSqlRelationalTableModel& m_treats;
		QString addTreatmentRow(
				const QSqlRecord& treatment,
				double& sum);

		QString generatePatientAddress();

		unsigned int m_billNumber;
};

#endif
