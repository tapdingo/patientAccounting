#include "PatientForm.h"
#include <QtGui>

#include "definitions.h"

#include <iostream>

PatientForm::PatientForm(
		int id,
		QWidget* parent) : QDialog(parent)
{
	m_model = new QSqlRelationalTableModel(this);
	m_model->setTable("patients");
	m_model->setFilter(QString("id = %1").arg(id));
	m_model->select();

	firstNameLabel = new QLabel(tr("Vorname"));
	firstNameField = new QLineEdit;
	firstNameLabel->setBuddy(firstNameField);

	lastNameLabel = new QLabel(tr("Nachname"));
	lastNameField = new QLineEdit;
	lastNameLabel->setBuddy(lastNameField);

	streetLabel = new QLabel(tr("Straße"));
	streetField = new QLineEdit;
	streetLabel->setBuddy(streetField);

	houseNumLabel = new QLabel(tr("Hausnummer"));
	houseNumField = new QLineEdit;
	houseNumLabel->setBuddy(houseNumField);

	postalLabel = new QLabel(tr("Postleitzahl"));
	postalField = new QLineEdit;
	postalLabel->setBuddy(postalField);

	cityLabel = new QLabel(tr("Stadt"));
	cityField = new QLineEdit;
	cityLabel->setBuddy(cityField);

	phoneLabel = new QLabel(tr("Telefon"));
	phoneField = new QLineEdit;
	phoneLabel->setBuddy(phoneField);

	birthdayLabel = new QLabel(tr("Geburtstag"));
	birthdayEdit = new QDateEdit;
	birthdayEdit->setCalendarPopup(true);
	birthdayEdit->setDisplayFormat("dd.MM.yyyy");
	birthdayLabel->setBuddy(birthdayEdit);

	genderLabel = new QLabel(tr("Geschlecht"));
	genderField = new QLineEdit;
	genderLabel->setBuddy(genderField);

	m_mapper = new QDataWidgetMapper(this);
	m_mapper->setSubmitPolicy(QDataWidgetMapper::ManualSubmit);
	m_mapper->setModel(m_model);
	m_mapper->addMapping(firstNameField, FirstName);
	m_mapper->addMapping(lastNameField, LastName);
	m_mapper->addMapping(phoneField, Phone);
	m_mapper->addMapping(birthdayEdit, DateOfBirth);
	m_mapper->addMapping(genderField, Gender);
	m_mapper->addMapping(houseNumField, HouseNumber);
	m_mapper->addMapping(streetField, Street);
	m_mapper->addMapping(cityField, City);
	m_mapper->addMapping(postalField, PostalCode);
	m_mapper->toFirst();

	saveButton = new QPushButton(tr("&Speichern"));
	saveButton->setEnabled(true);

	closeButton = new QPushButton(tr("Schliessen"));
	closeButton->setDefault(true);

	QGridLayout* mainLayout = new QGridLayout(this);

	mainLayout->addWidget(firstNameLabel, 0, 0);
	mainLayout->addWidget(firstNameField, 0, 1);

	mainLayout->addWidget(lastNameLabel, 1, 0);
	mainLayout->addWidget(lastNameField, 1, 1);

	mainLayout->addWidget(lastNameLabel, 2, 0);
	mainLayout->addWidget(lastNameField, 2, 1);

	mainLayout->addWidget(phoneLabel, 3, 0);
	mainLayout->addWidget(phoneField, 3, 1);

	mainLayout->addWidget(streetLabel, 4, 0);
	mainLayout->addWidget(streetField, 4, 1);

	mainLayout->addWidget(houseNumLabel, 5, 0);
	mainLayout->addWidget(houseNumField, 5, 1);

	mainLayout->addWidget(postalLabel, 6, 0);
	mainLayout->addWidget(postalField, 6, 1);

	mainLayout->addWidget(cityLabel, 7, 0);
	mainLayout->addWidget(cityField, 7, 1);

	mainLayout->addWidget(genderLabel, 8, 0);
	mainLayout->addWidget(genderField, 8, 1);

	mainLayout->addWidget(birthdayLabel, 9, 0);
	mainLayout->addWidget(birthdayEdit, 9, 1);

	mainLayout->addWidget(saveButton, 10, 0);
	mainLayout->addWidget(closeButton, 10, 1);

	setLayout(mainLayout);

	setWindowTitle(tr("Patient bearbeiten..."));
	setFixedHeight(sizeHint().height());

	connect(closeButton, SIGNAL(clicked()), this, SLOT(accept()));
	connect(saveButton, SIGNAL(clicked()), this, SLOT(savePatient()));
}

void PatientForm::savePatient()
{
	int index = m_mapper->currentIndex();
	if (!m_mapper->submit())
	{
		QMessageBox msgBox;
		msgBox.setText("Could not submit Patient");
		QSqlError last = m_model->lastError();
		msgBox.setInformativeText(last.text());
		msgBox.exec();
	}
	m_mapper->setCurrentIndex(index);
	accept();
}

