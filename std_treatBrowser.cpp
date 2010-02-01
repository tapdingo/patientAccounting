#include "std_treatBrowser.h"

#include <QtGui>

#include "definitions.h"


std_TreatBrowser::std_TreatBrowser(QWidget* parent) : QDialog(parent)
{
	treatmentModel = new QSqlTableModel(this);
	treatmentModel->setTable("std_treats");
	treatmentModel->setSort(DiagnoseName, Qt::AscendingOrder);
	treatmentModel->select();

	treatmentView = new QTableView();
	treatmentView->setModel(treatmentModel);
	treatmentView->setSelectionMode(QAbstractItemView::SingleSelection);
	treatmentView->setSelectionBehavior(QAbstractItemView::SelectRows);
	treatmentView->setShowGrid(false);
	treatmentView->horizontalHeader()->setStretchLastSection(true);


	createButtons();

	QHBoxLayout* buttonLayout = new QHBoxLayout();
	buttonLayout->addWidget(newButton);
	buttonLayout->addWidget(deleteButton);

	QVBoxLayout* layout = new QVBoxLayout();
	layout->addWidget(treatmentView);
	layout->addLayout(buttonLayout);

	setLayout(layout);
	setFixedHeight(sizeHint().height());
	setWindowTitle(tr("Standard Behandlungs Browser"));

	connect(newButton, SIGNAL(clicked()), this, SLOT(newTreatment()));
	connect(deleteButton, SIGNAL(clicked()), this, SLOT(deleteTreatment()));
}

void std_TreatBrowser::createButtons()
{
	newButton = new QPushButton(tr("&Neue Behandlung"));
	newButton->setEnabled(true);

	deleteButton = new QPushButton(tr("&Behandlung entfernen"));
	deleteButton->setEnabled(true);
}

void std_TreatBrowser::newTreatment()
{
	int rc = treatmentModel->rowCount();
	if (!treatmentModel->insertRow(rc))
	{
		QMessageBox msgBox;
		msgBox.setText("Fehler beim Anlegen der treatment");
		msgBox.exec();
		return;
	}

}

void std_TreatBrowser::deleteTreatment()
{
	QModelIndex index = treatmentView->currentIndex();
	if(!index.isValid())
	{
		QMessageBox msgBox;
		msgBox.setText("Keine treatment ausgewaehlt!");
		msgBox.exec();
		return;
	}
	treatmentModel->removeRow(index.row());
	treatmentModel->submitAll();
	treatmentModel->select();
}
