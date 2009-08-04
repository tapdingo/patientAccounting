#include "PatientForm.h"
#include <QtGui>

PatientForm::PatientForm(QSqlRelationalTableModel* model,  QWidget* parent) : QDialog(parent)
{
	m_model = new QSqlRelationalTableModel(this);
	m_model->setTable("patients");
	m_model->select();


	m_mapper = new QDataWidgetMapper(this);
	m_mapper->setSubmitPolicy(QDataWidgetMapper::AutoSubmit);
	m_mapper->setModel(m_model);
	m_mapper->addMapping(firstNameField, 0);
	m_mapper->toLast();

	firstNameLabel = new QLabel(tr("Vorname"));;
	firstNameField = new QLineEdit;
	firstNameLabel->setBuddy(firstNameField);

	saveButton = new QPushButton(tr("&Speichern"));
	saveButton->setEnabled(true);

	closeButton = new QPushButton(tr("Schliessen"));
	closeButton->setDefault(true);

	QHBoxLayout* fieldLayout = new QHBoxLayout;
	fieldLayout->addWidget(firstNameLabel);
	fieldLayout->addWidget(firstNameField);

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
	connect(saveButton, SIGNAL(clicked()), this, SLOT(addPatient()));
}

void PatientForm::addPatient()
{
	int row = m_mapper->currentIndex();
	QMessageBox msgBox;
	QString text;
	msgBox.setText(text.setNum(row));
	msgBox.setStandardButtons(QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
	msgBox.setDefaultButton(QMessageBox::Save);
	int ret = msgBox.exec();

	m_model->removeRow(row);
	//m_model->insertRow(row);
	//if(!m_mapper->submit())
	//{
		//QMessageBox msgBox;
		//QString text;
		//msgBox.setText("ERROR");
		//msgBox.setStandardButtons(QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
		//msgBox.setDefaultButton(QMessageBox::Save);
	//}
	firstNameField->clear();
	closeButton->setFocus();
}

void PatientForm::deletePatient()
{
}
