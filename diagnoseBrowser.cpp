#include "diagnoseBrowser.h"

#include <QtGui>

#include "definitions.h"

DiagnoseBrowser::DiagnoseBrowser(QWidget* parent) : QDialog(parent)
{
	diagnoseModel = new QSqlTableModel(this);
	diagnoseModel->setTable("diagnoses");
	diagnoseModel->setSort(DiagnoseName, Qt::AscendingOrder);
	diagnoseModel->select();

	diagnoseView = new QTableView();
	diagnoseView->setModel(diagnoseModel);
	diagnoseView->setSelectionMode(QAbstractItemView::SingleSelection);
	diagnoseView->setSelectionBehavior(QAbstractItemView::SelectRows);
	diagnoseView->setShowGrid(false);
	diagnoseView->horizontalHeader()->setStretchLastSection(true);


	createButtons();

	QHBoxLayout* buttonLayout = new QHBoxLayout();
	buttonLayout->addWidget(newButton);
	buttonLayout->addWidget(deleteButton);

	QVBoxLayout* layout = new QVBoxLayout();
	layout->addWidget(diagnoseView);
	layout->addLayout(buttonLayout);

	setLayout(layout);
	setFixedHeight(sizeHint().height());
	setWindowTitle(tr("Diagnosen Browser"));

	connect(newButton, SIGNAL(clicked()), this, SLOT(newDiagnose()));
	connect(deleteButton, SIGNAL(clicked()), this, SLOT(deleteDiagnose()));
}

void DiagnoseBrowser::createButtons()
{
	newButton = new QPushButton(tr("&Neue Diagnose"));
	newButton->setEnabled(true);

	deleteButton = new QPushButton(tr("&Diagnose entfernen"));
	deleteButton->setEnabled(true);
}

void DiagnoseBrowser::newDiagnose()
{
	int rc = diagnoseModel->rowCount();
	if (!diagnoseModel->insertRow(rc))
	{
		QMessageBox msgBox;
		msgBox.setText("Fehler beim Anlegen der Diagnose");
		msgBox.exec();
		return;
	}

}

void DiagnoseBrowser::deleteDiagnose()
{
	QModelIndex index = diagnoseView->currentIndex();
	if(!index.isValid())
	{
		QMessageBox msgBox;
		msgBox.setText("Keine Diagnose ausgewaehlt!");
		msgBox.exec();
		return;
	}
	diagnoseModel->removeRow(index.row());
	diagnoseModel->submitAll();
	diagnoseModel->select();
}
