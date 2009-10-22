#include "TreatmentForm.h"
#include <QtGui>

#include <QDataWidgetMapper>
#include <iostream>

#include <definitions.h>

TreatmentForm::TreatmentForm(
		int id,
		QWidget* parent) : QDialog(parent, Qt::Window)
{
	//Database and stuff...
	m_model = new QSqlRelationalTableModel(this);
	m_model->setTable("treatments");
	m_model->setSort(TreatmentID, Qt::AscendingOrder);
	m_model->setFilter(QString("id = %1").arg(id));
	m_model->select();

	//Nice and cosy...
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


	//Pretty signalling dude!
	connect(closeButton, SIGNAL(clicked()), this, SLOT(accept()));
	connect(saveButton, SIGNAL(clicked()), this, SLOT(saveTreatment()));
	connect(
			details,
			SIGNAL(stateChanged(int)),
			this,
			SLOT(expandHideDetails(int)));
	connect(
			noOfDetails,
			SIGNAL(valueChanged(int)),
			this,
			SLOT(noDetailsChanged(int)));

	//Set unmappable values
	initialUpdate();
}

void TreatmentForm::saveTreatment()
{

	if (!checkForDetails())
	{
		QMessageBox msgBox;
		msgBox.setText("Fehler: Die Detailkosten passen nicht zu den Gesamtkosten!");
		QSqlError last = m_model->lastError();
		msgBox.setInformativeText(last.text());
		msgBox.exec();
		return;
	}

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
	record.setValue(Accounted, accounted->checkState());
	record.setValue(Paid, paid->checkState());

	//DATABASE RAPE AHOY
	//Push the damn detail vector into the poor database
	QString dumpedDetails;
	dumpDetails(dumpedDetails);
	record.setValue(Details, dumpedDetails);

	m_model->setRecord(index, record);
	if (!m_model->submit())
	{
		QMessageBox msgBox;
		msgBox.setText("Fehler beim Speichern!");
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
	mainLayout = new QGridLayout(this);
	setLayout(mainLayout);

	QSqlTableModel* diagnose_model = new QSqlTableModel(this);
	diagnose_model->setTable("diagnoses");
	diagnose_model->select();
	diagnoseLabel = new QLabel(tr("Diagnose"));
	diagnoseComboBox = new QComboBox;
	diagnoseComboBox->setModel(diagnose_model);
	diagnoseComboBox->setCompleter(diagnoseComboBox->completer());
	diagnoseComboBox->setModelColumn(0);
	diagnoseComboBox->setEditable(true);
	mainLayout->addWidget(diagnoseLabel, 0, 0);
	mainLayout->addWidget(diagnoseComboBox, 0, 1);
	mainLayout->setSizeConstraint(QLayout::SetFixedSize);


	dateLabel = new QLabel(tr("Behandlungsdatum"));
	dateEdit = new QDateEdit;
	dateEdit->setCalendarPopup(true);
	dateEdit->setDisplayFormat("dd.MM.yyyy");
	dateLabel->setBuddy(dateEdit);
	mainLayout->addWidget(dateLabel, 1, 0);
	mainLayout->addWidget(dateEdit, 1, 1);

	durationLabel = new QLabel(tr("Dauer (Minuten)"));
	durationField = new QLineEdit;
	durationLabel->setBuddy(durationField);
	connect(durationField, SIGNAL(textChanged(QString)), this, SLOT(durChange()));
	mainLayout->addWidget(durationLabel, 2, 0);
	mainLayout->addWidget(durationField, 2, 1);

	costLabel = new QLabel(tr("Kostenpunkt"));
	costField = new QLineEdit;
	costLabel->setBuddy(costField);
	mainLayout->addWidget(costLabel, 3, 0);
	mainLayout->addWidget(costField, 3, 1);

	nameLabel = new QLabel(tr("Beschreibung"));
	nameField = new QLineEdit;
	nameLabel->setBuddy(nameField);
	mainLayout->addWidget(nameLabel, 4, 0);
	mainLayout->addWidget(nameField, 4, 1);

	telephone = new QRadioButton(tr("Telefonisch"));
	practice = new QRadioButton(tr("Praxis"));
	typeButtons = new QButtonGroup();
	typeButtons->addButton(telephone, Telephone);
	typeButtons->addButton(practice, Practice);
	mainLayout->addWidget(telephone, 5, 0);
	mainLayout->addWidget(practice, 5, 1);

	paid = new QCheckBox("Bereits abgerechnet");
	mainLayout->addWidget(paid, 6, 0);

	accounted = new QCheckBox("Bereits bezahlt");
	mainLayout->addWidget(accounted, 6, 1);

	details = new QCheckBox("Genauere Behandlungsangeben");
	numLabel = new QLabel(tr("Anzahl Detailfelder"));
	noOfDetails = new QSpinBox();
	mainLayout->addWidget(details, 7, 0);

	mainLayout->addWidget(noOfDetails, 7, 1);
	noOfDetails->hide();

	detailsLabel = new QLabel("Details");
	detailsLabel->hide();
	mainLayout->addWidget(detailsLabel, 8, 0);

	costDetailLabel = new QLabel("Kosten-Details");
	costDetailLabel->hide();
	mainLayout->addWidget(costDetailLabel, 8, 1);

	detailFields = new QVBoxLayout;
	mainLayout->addLayout(detailFields, 9, 0);

	costDetailFields = new QVBoxLayout;
	mainLayout->addLayout(costDetailFields, 9, 1);

	saveButton = new QPushButton(tr("&Speichern"));
	saveButton->setEnabled(true);
	mainLayout->addWidget(saveButton, 10, 0);

	closeButton = new QPushButton(tr("Schliessen"));
	closeButton->setDefault(true);
	mainLayout->addWidget(closeButton, 10, 1);

	setLayout(mainLayout);
	setWindowTitle(tr("Behandlung bearbeiten..."));
}

void TreatmentForm::expandHideDetails(int state)
{
	if (state == Qt::Checked)
	{
		//Show some stuff
		noOfDetails->show();
		diagnoseLabel->show();
		detailsLabel->show();
		costDetailLabel->show();

		//Create a new tuple for showing
		LayoutTuple* n_tuple = new LayoutTuple;
		n_tuple->detField = new QLineEdit;
		n_tuple->costDetField = new QLineEdit;
		detailFieldsDesc.push_back(n_tuple);
		detailFields->addWidget(n_tuple->detField);
		costDetailFields->addWidget(n_tuple->costDetField);

		//Set the correct value
		noOfDetails->setValue(1);
	}
	else
	{
		noOfDetails->hide();
		detailsLabel->hide();
		costDetailLabel->hide();

		//Throw all the crap away
		std::vector<LayoutTuple*>::iterator it;
		for (it = detailFieldsDesc.begin(); it != detailFieldsDesc.end(); it++)
		{
			(*it)->detField->hide();
			(*it)->costDetField->hide();
			delete *it;
		}
		detailFieldsDesc.clear();
	}
}

void TreatmentForm::noDetailsChanged(int number)
{
	if (detailFieldsDesc.size() < static_cast<uint32_t>(number))
	{
		//Create a new tuple for showing
		LayoutTuple* n_tuple = new LayoutTuple;
		n_tuple->detField = new QLineEdit;
		n_tuple->costDetField = new QLineEdit;
		detailFieldsDesc.push_back(n_tuple);
		detailFields->addWidget(n_tuple->detField);
		costDetailFields->addWidget(n_tuple->costDetField);

		//RECURSION ALERT!
		noDetailsChanged(number);
	}
	else if (detailFieldsDesc.size() > static_cast<uint32_t>(number))
	{

		LayoutTuple* n_tuple = detailFieldsDesc.back();
		n_tuple->detField->hide();
		n_tuple->costDetField->hide();
		delete n_tuple;
		detailFieldsDesc.pop_back();

		//RECURSION ALERT!
		noDetailsChanged(number);
	}
}

void TreatmentForm::dumpDetails(QString& result)
{
	std::vector<LayoutTuple*>::iterator it;

	for (it = detailFieldsDesc.begin(); it != detailFieldsDesc.end(); it++)
	{
		result.append((*it)->detField->text());
		result.append("||");
		result.append((*it)->costDetField->text());
		result.append(";");
	}
}

void TreatmentForm::reconstructDetailVector(QString& data)
{
	QStringList splitted = data.split(";", QString::SkipEmptyParts);

	for (int i = 0; i < splitted.size(); i++)
	{
		QStringList line_details = splitted[i].split("||", QString::SkipEmptyParts);
		if ( line_details.size() > 0)
		{
			DetailTuple* n_tuple = new DetailTuple;
			n_tuple->detail = line_details[0];
			n_tuple->cost = line_details[1].toFloat();
			detailVector.push_back(n_tuple);
		}
	}

	if (splitted.size() != 0)
	{
		details->setChecked(Qt::Checked);
		noOfDetails->setValue(splitted.size());
	}
}

void TreatmentForm::initialUpdate()
{
	QSqlRecord record = m_model->record(m_mapper->currentIndex());
	if (record.value(Type) == Telephone)
	{
		telephone->setChecked(true);
	}
	else
	{
		practice->setChecked(true);
	}

	paid->setChecked(record.value(Paid).toInt());
	accounted->setChecked(record.value(Accounted).toInt());

	QString details_raw = record.value(Details).toString();
	reconstructDetailVector(details_raw);

	std::vector<DetailTuple*>::iterator it;

	int i = 0;
	for (it = detailVector.begin(); it != detailVector.end(); it ++)
	{
		QString cost;
		cost.setNum((*it)->cost);
		detailFieldsDesc[i]->detField->setText((*it)->detail);
		detailFieldsDesc[i]->costDetField->setText(cost);
		i++;
	}
}

bool TreatmentForm::checkForDetails()
{
	if (details->checkState() != Qt::Checked)
	{
		return true;
	}
	std::vector<LayoutTuple*>::iterator it;

	float total_cost = 0;

	for (it = detailFieldsDesc.begin(); it != detailFieldsDesc.end(); it++)
	{
		total_cost += (*it)->costDetField->text().toFloat();
	}

	if (costField->text().toFloat() == total_cost)
	{
		return true;
	}
	return false;
}
