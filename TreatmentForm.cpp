#include "TreatmentForm.h"
#include <QtGui>

#include <QDataWidgetMapper>
#include <iostream>

TreatmentForm::TreatmentForm(
		int id,
		QWidget* parent) : QDialog(parent)
{
	m_model = new QSqlRelationalTableModel(this);
	m_model->setTable("treatments");
	m_model->setSort(TreatmentID, Qt::AscendingOrder);
	m_model->setFilter(QString("id = %1").arg(id));
	m_model->select();

	costLabel = new QLabel(tr("Kostenpunkt"));
	costField = new QLineEdit;
	costLabel->setBuddy(costField);

	nameLabel = new QLabel(tr("Beschreibung"));
	nameField = new QLineEdit;
	nameLabel->setBuddy(nameField);

	m_mapper = new QDataWidgetMapper(this);
	m_mapper->setSubmitPolicy(QDataWidgetMapper::ManualSubmit);
	m_mapper->setModel(m_model);
	m_mapper->addMapping(costField, Cost);
	m_mapper->addMapping(nameField, TreatmentName);
	m_mapper->toFirst();

	saveButton = new QPushButton(tr("&Speichern"));
	saveButton->setEnabled(true);

	closeButton = new QPushButton(tr("Schliessen"));
	closeButton->setDefault(true);

	QVBoxLayout* fieldLayout = new QVBoxLayout;

	QHBoxLayout* cost = new QHBoxLayout;
	cost->addWidget(costLabel);
	cost->addWidget(costField);

	QHBoxLayout* name = new QHBoxLayout;
	name->addWidget(nameLabel);
	name->addWidget(nameField);

	fieldLayout->addLayout(cost);
	fieldLayout->addLayout(name);

	QHBoxLayout* buttonLayout = new QHBoxLayout;
	buttonLayout->addWidget(saveButton);
	buttonLayout->addWidget(closeButton);

	QVBoxLayout* mainLayout = new QVBoxLayout;
	mainLayout->addLayout(fieldLayout);
	mainLayout->addLayout(buttonLayout);

	setLayout(mainLayout);

	setWindowTitle(tr("Behandlung bearbeiten..."));
	setFixedHeight(sizeHint().height());

	connect(closeButton, SIGNAL(clicked()), this, SLOT(accept()));
	connect(saveButton, SIGNAL(clicked()), this, SLOT(saveTreatment()));
}

void TreatmentForm::saveTreatment()
{
	int index = m_mapper->currentIndex();
	if (!m_mapper->submit())
	{
		QMessageBox msgBox;
		msgBox.setText("Could not submit treatment");
		QSqlError last = m_model->lastError();
		msgBox.setInformativeText(last.text());
		msgBox.exec();
	}
	m_mapper->setCurrentIndex(index);
}
