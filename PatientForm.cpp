#include "PatientForm.h"
#include <QtGui>

#include "definitions.h"

PatientForm::PatientForm(
		int id,
		QWidget* parent) : QDialog(parent)
{
	m_model = new QSqlRelationalTableModel(this);
	m_model->setTable("patients");
	m_model->setFilter(QString("id = %1").arg(id));
	m_model->select();

	firstNameLabel = new QLabel(tr("Vorname"));;
	firstNameField = new QLineEdit;
	firstNameLabel->setBuddy(firstNameField);

	lastNameLabel = new QLabel(tr("Nachname"));;
	lastNameField = new QLineEdit;
	lastNameLabel->setBuddy(lastNameField);

	phoneLabel = new QLabel(tr("Telefon"));;
	phoneField = new QLineEdit;
	phoneLabel->setBuddy(phoneField);

	birthdayLabel = new QLabel(tr("Geburtstag"));
	birthdayEdit = new QDateEdit;
	birthdayEdit->setCalendarPopup(true);
	birthdayEdit->setDisplayFormat("dd.MM.yyyy");
	birthdayLabel->setBuddy(birthdayEdit);

	genderLabel = new QLabel(tr("Geschlecht"));;
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
	m_mapper->toFirst();

	saveButton = new QPushButton(tr("&Speichern"));
	saveButton->setEnabled(true);

	closeButton = new QPushButton(tr("Schliessen"));
	closeButton->setDefault(true);

	QVBoxLayout* fieldLayout = new QVBoxLayout;

	QHBoxLayout* firstName = new QHBoxLayout;
	firstName->addWidget(firstNameLabel);
	firstName->addWidget(firstNameField);

	QHBoxLayout* lastName = new QHBoxLayout;
	lastName->addWidget(lastNameLabel);
	lastName->addWidget(lastNameField);

	QHBoxLayout* phone = new QHBoxLayout;
	phone->addWidget(phoneLabel);
	phone->addWidget(phoneField);

	QHBoxLayout* birthday = new QHBoxLayout;
	birthday->addWidget(birthdayLabel);
	birthday->addWidget(birthdayEdit);

	QHBoxLayout* gender = new QHBoxLayout;
	gender->addWidget(genderLabel);
	gender->addWidget(genderField);

	fieldLayout->addLayout(firstName);
	fieldLayout->addLayout(lastName);
	fieldLayout->addLayout(phone);
	fieldLayout->addLayout(birthday);
	fieldLayout->addLayout(gender);

	QHBoxLayout* buttonLayout = new QHBoxLayout;
	buttonLayout->addWidget(saveButton);
	buttonLayout->addWidget(closeButton);

	QVBoxLayout* mainLayout = new QVBoxLayout;
	mainLayout->addLayout(fieldLayout);
	mainLayout->addLayout(buttonLayout);

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
}

