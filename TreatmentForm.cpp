#include "TreatmentForm.h"
#include <QtGui>

#include <QDataWidgetMapper>
#include <iostream>

#include <definitions.h>

TreatmentForm::TreatmentForm(
		int id,
		QWidget* parent) : QDialog(parent)
{
	m_model = new QSqlRelationalTableModel(this);
	m_model->setTable("treatments");
	m_model->setSort(TreatmentID, Qt::AscendingOrder);
	m_model->setFilter(QString("id = %1").arg(id));
	m_model->select();


	createLayout();

	m_mapper = new QDataWidgetMapper(this);
	m_mapper->setSubmitPolicy(QDataWidgetMapper::ManualSubmit);
	m_mapper->setModel(m_model);
	m_mapper->addMapping(diagnoseComboBox, Diagnose);
	m_mapper->addMapping(dateEdit, DateOfTreat);
	m_mapper->addMapping(durationField, Duration);
	m_mapper->addMapping(costField, Cost);
	m_mapper->addMapping(nameField, TreatmentName);
	m_mapper->toFirst();

	//Set unmappable values
	QSqlRecord record = m_model->record(m_mapper->currentIndex());
	if (record.value(Type) == Telephone)
	{
		telephone->setChecked(true);
	}
	else
	{
		practice->setChecked(true);
	}

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


	//Explicitly set some values not coveres by the Mapper
	QSqlRecord record = m_model->record(index);
	record.setValue(Diagnose, diagnoseComboBox->currentText());
	record.setValue(Type, typeButtons->checkedId());

	m_model->setRecord(index, record);
	if (!m_model->submit())
	{
		QMessageBox msgBox;
		msgBox.setText("Could not submit ComboBox");
		QSqlError last = m_model->lastError();
		msgBox.setInformativeText(last.text());
		msgBox.exec();
	}
	m_mapper->setCurrentIndex(index);
}

void TreatmentForm::durChange()
{
	double new_cost = (double(durationField->text().toInt()) / 60) * 75;
	QString newCost;
	newCost.setNum(new_cost);
	costField->setText(newCost);
}

void TreatmentForm::createLayout()
{
	QSqlTableModel* diagnose_model = new QSqlTableModel(this);
	diagnose_model->setTable("diagnoses");
	diagnose_model->select();
	diagnoseLabel = new QLabel(tr("Diagnose"));
	diagnoseComboBox = new QComboBox;
	diagnoseComboBox->setModel(diagnose_model);
	diagnoseComboBox->setCompleter(diagnoseComboBox->completer());
	diagnoseComboBox->setModelColumn(0);
	diagnoseComboBox->setEditable(true);


	dateLabel = new QLabel(tr("Behandlungsdatum"));
	dateEdit = new QDateEdit;
	dateEdit->setCalendarPopup(true);
	dateEdit->setDisplayFormat("dd.MM.yyyy");
	dateLabel->setBuddy(dateEdit);

	durationLabel = new QLabel(tr("Dauer (Minuten)"));
	durationField = new QLineEdit;
	durationLabel->setBuddy(durationField);
	connect(durationField, SIGNAL(textChanged(QString)), this, SLOT(durChange()));

	costLabel = new QLabel(tr("Kostenpunkt"));
	costField = new QLineEdit;
	costLabel->setBuddy(costField);

	nameLabel = new QLabel(tr("Beschreibung"));
	nameField = new QLineEdit;
	nameLabel->setBuddy(nameField);

	telephone = new QRadioButton(tr("Telefonisch"));
	practice = new QRadioButton(tr("Praxis"));
	typeButtons = new QButtonGroup();
	typeButtons->addButton(telephone, Telephone);
	typeButtons->addButton(practice, Practice);

	saveButton = new QPushButton(tr("&Speichern"));
	saveButton->setEnabled(true);

	closeButton = new QPushButton(tr("Schliessen"));
	closeButton->setDefault(true);

	QVBoxLayout* fieldLayout = new QVBoxLayout;

	QHBoxLayout* diagnose = new QHBoxLayout;
	diagnose->addWidget(diagnoseLabel);
	diagnose->addWidget(diagnoseComboBox);

	QHBoxLayout* duration = new QHBoxLayout;
	duration->addWidget(durationLabel);
	duration->addWidget(durationField);

	QHBoxLayout* date = new QHBoxLayout;
	date->addWidget(dateLabel);
	date->addWidget(dateEdit);

	QHBoxLayout* cost = new QHBoxLayout;
	cost->addWidget(costLabel);
	cost->addWidget(costField);

	QHBoxLayout* name = new QHBoxLayout;
	name->addWidget(nameLabel);
	name->addWidget(nameField);

	QHBoxLayout* type = new QHBoxLayout;
	name->addWidget(telephone);
	name->addWidget(practice);

	fieldLayout->addLayout(diagnose);
	fieldLayout->addLayout(date);
	fieldLayout->addLayout(duration);
	fieldLayout->addLayout(cost);
	fieldLayout->addLayout(name);
	fieldLayout->addLayout(type);

	QHBoxLayout* buttonLayout = new QHBoxLayout;
	buttonLayout->addWidget(saveButton);
	buttonLayout->addWidget(closeButton);

	QVBoxLayout* mainLayout = new QVBoxLayout;
	mainLayout->addLayout(fieldLayout);
	mainLayout->addLayout(buttonLayout);

	setLayout(mainLayout);

	setWindowTitle(tr("Behandlung bearbeiten..."));
	setFixedHeight(sizeHint().height());
}
