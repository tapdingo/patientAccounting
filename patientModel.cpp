#include "patientModel.h"

#include <QtGui>
#include <QtSql>
#include <QSqlDatabase>

#include "definitions.h"

int PatientModel::addNewRecord()
{
	QSqlRecord rec;

	int rc = this->rowCount();
	if (!this->insertRow(rc))
	{
		QMessageBox msgBox;
		msgBox.setText("Fehler beim Anlegen des Patienten :(");
		msgBox.exec();
		return 0;
	}

	rec = this->record(rc);
	rec.setValue(QString("gender"), QString("m"));
	this->setRecord(rc, rec);

	if (!this->submitAll())
	{
		QMessageBox msgBox;
		msgBox.setText("Could not submit model :(");
		QSqlError last = QSqlDatabase::database().lastError();
		msgBox.setInformativeText(last.text());
		msgBox.exec();
	}
	this->select();
	return rc;
}

void PatientModel::deleteRecord(const int& id)
{
	//QSqlRecord record = this->record(index.row());
	int r = QMessageBox::warning(0, tr("Patient entfernen"),
			tr("Wollen sie den Patient und alle seine Behandlungen wirklich entfernen?"),
			QMessageBox::Yes | QMessageBox::No);

	if (r == QMessageBox::No)
	{
		QSqlDatabase::database().rollback();
		return;
	}
	QSqlQuery query;
	query.exec(QString("DELETE FROM treatments "
				"WHERE patient_id = %1").arg(id));

	//this->removeRow(index.row());
	this->submitAll();
	QSqlDatabase::database().commit();

	this->select();
}
