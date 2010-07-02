#include "std_treatBrowser.h"

#include <QtGui>

#include "definitions.h"

#include "stdTreatmentForm.h"
#include <iostream>


std_TreatBrowser::std_TreatBrowser(QWidget* parent) : QDialog(parent)
{
	treatmentModel = new QSqlTableModel(this);
	treatmentModel->setTable("std_treats");
	treatmentModel->setSort(DiagnoseName, Qt::AscendingOrder);
	treatmentModel->select();

	treatmentComboBox = new QComboBox();
	treatmentComboBox->setModel(treatmentModel);
	treatmentComboBox->setCompleter(treatmentComboBox->completer());
	treatmentComboBox->setModelColumn(1);
	treatmentComboBox->setEditable(false);

	createButtons();

	QHBoxLayout* buttonLayout = new QHBoxLayout();
	buttonLayout->addWidget(newButton);
	buttonLayout->addWidget(editButton);
	buttonLayout->addWidget(deleteButton);

	QVBoxLayout* layout = new QVBoxLayout();
	layout->addWidget(treatmentComboBox);
	layout->addLayout(buttonLayout);

	setLayout(layout);
	setFixedHeight(sizeHint().height());
	setWindowTitle(tr("Standard Behandlungs Browser"));

	connect(newButton, SIGNAL(clicked()), this, SLOT(newTreatment()));
	connect(editButton, SIGNAL(clicked()), this, SLOT(editTreatment()));
	connect(deleteButton, SIGNAL(clicked()), this, SLOT(deleteTreatment()));
}

void std_TreatBrowser::createButtons()
{
	newButton = new QPushButton(tr("&Neue Behandlung"));
	newButton->setEnabled(true);

	editButton = new QPushButton(tr("&Behandlung bearbeiten"));
	editButton->setEnabled(true);

	deleteButton = new QPushButton(tr("&Behandlung entfernen"));
	deleteButton->setEnabled(true);
}

void std_TreatBrowser::newTreatment()
{
	int rc = treatmentModel->rowCount();
	if (!treatmentModel->insertRow(rc))
	{
		QMessageBox msgBox;
		msgBox.setText("Fehler beim Anlegen der Behandlung");
		msgBox.exec();
		return;
	}
	treatmentModel->submitAll();
	QSqlRecord newTreat = treatmentModel->record(rc);
	newTreat.setValue(std_treat::treatName, "CHANGEME");
	treatmentModel->setRecord(rc, newTreat);
	treatmentModel->submitAll();
	uint32_t treatId = getStdTreatId("CHANGEME");
	std::cerr << "TreatToEdit: " << treatId << std::endl;
	stdTreatmentForm editStdTreat(treatId, this);
	editStdTreat.exec();
	treatmentModel->submitAll();
	treatmentModel->select();
}

void std_TreatBrowser::deleteTreatment()
{
	uint32_t treatId = getStdTreatId(treatmentComboBox->currentText());
	QSqlDatabase::database().transaction();
	int r = QMessageBox::warning(0, tr("Standard Behandlung entfernen"),
			tr("die Behandlung wirklich entfernen?"),
			QMessageBox::Yes | QMessageBox::No);

	if (r == QMessageBox::No)
	{
		QSqlDatabase::database().rollback();
		return;
	}
	QSqlQuery query;
	query.exec(QString("DELETE FROM std_treats "
				"WHERE id = %1").arg(treatId));
	QSqlDatabase::database().commit();
	treatmentModel->submitAll();
	treatmentModel->select();
}

void std_TreatBrowser::editTreatment()
{
	uint32_t treatId = getStdTreatId(treatmentComboBox->currentText());
	stdTreatmentForm editStdTreat(treatId, this);
	editStdTreat.exec();
	treatmentModel->submitAll();
	treatmentModel->select();
}

uint32_t std_TreatBrowser::getStdTreatId(const QString& name)
{
	QSqlTableModel* model = new QSqlTableModel(this);
	model->setTable("std_treats");
	model->select();
	QString filter("name='");
	filter.append(name);
	filter.append("'");
	model->setFilter(filter);
	model->select();
	if (model->rowCount() > 1)
	{
		std::cerr << "ERROR, DOUBLE NAMES!";
	}
	QSqlRecord treatment = model->record(0);
	return treatment.value(std_treat::treatId).toInt();
}
