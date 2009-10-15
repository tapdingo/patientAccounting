#include "treatmentModel.h"

#include <QtGui>
#include <QtSql>
#include <QSqlDatabase>
#include <QSqlRelationalTableModel>
#include <iostream>

#include "definitions.h"
int TreatmentModel::addNewRecord()
{
	return 0;
}

bool TreatmentModel::deleteRecord(const int& id)
{
	QSqlDatabase::database().transaction();
	int r = QMessageBox::warning(0, tr("Behandlung entfernen"),
			tr("die Behandlung wirklich entfernen?"),
			QMessageBox::Yes | QMessageBox::No);

	if (r == QMessageBox::No)
	{
		QSqlDatabase::database().rollback();
		return false;
	}
	QSqlQuery query;
	query.exec(QString("DELETE FROM treatments "
				"WHERE id = %1").arg(id));

	this->submitAll();
	if (!this->submitAll())
	{
		QMessageBox msgBox;
		msgBox.setText("Fehler im DatenModell! :(");
		QSqlError last = QSqlDatabase::database().lastError();
		msgBox.setInformativeText(last.text());
		msgBox.exec();
	}
	QSqlDatabase::database().commit();
	return true;
}

int TreatmentModel::addNewRelRecord(const int& id)
{
	//Since the current Model is spoiled, we need a new, clear one
	QSqlTableModel* interimModel= new QSqlTableModel();
	interimModel->setTable("treatments");
	interimModel->select();

	int rc = interimModel->rowCount();

	if (!interimModel->insertRow(rc))
	{
		QMessageBox msgBox;
		msgBox.setText("Konnte die Behandlung nicht anlegen :(");
		msgBox.exec();
		return 0;
	}

	QSqlRecord rec;
	QString idString;
	idString.setNum(id);
	rec = interimModel->record(rc);

	rec.setValue(QString("patient_id"), idString);
	rec.setValue(QString("accounted"), 0);

	interimModel->setRecord(rc, rec);
	if (!interimModel->submitAll())
	{
		QMessageBox msgBox;
		msgBox.setText("Fehler beim Anlegen der Behandlung :(");
		QSqlError last = QSqlDatabase::database().lastError();
		msgBox.setInformativeText(last.text());
		msgBox.exec();
	}
	interimModel->select();

	//Bad Hack to get the new ID
	rc = interimModel->rowCount();
	rec = interimModel->record(rc-1);
	int treatId = rec.value(TreatmentID).toInt();
	return treatId;
}

double TreatmentModel::getOpenBills()
{
	//Since the current Model is spoiled, we need a new, clear one
	QSqlTableModel* interimModel= new QSqlTableModel();
	interimModel->setTable("treatments");
	interimModel->setFilter(QString("accounted = 0"));
	interimModel->select();

	int rc = interimModel->rowCount();
	double openBills = 0;

	if(rc)
	{
		for (int i = 0; i < rc; i++)
		{
			QSqlRecord currentPatient = interimModel->record(i);
			if(!currentPatient.isEmpty())
			{
				openBills += currentPatient.value(Cost).toDouble();
			}
		}
	}

	return openBills;
}
