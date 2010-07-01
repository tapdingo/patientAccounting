#include "std_treatBrowser.h"

#include <QtGui>

#include "definitions.h"

#include "stdTreatmentForm.h"


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
}

void std_TreatBrowser::deleteTreatment()
{
}

void std_TreatBrowser::editTreatment()
{
		stdTreatmentForm editStdTreat(treatmentComboBox->currentIndex() + 1, this);
		editStdTreat.exec();
}

