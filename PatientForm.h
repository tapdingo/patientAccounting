#ifndef PATIENT_FORM_H
#define PATIENT_FORM_H

#include <QDialog>
#include <QtSql>
#include <QSqlDatabase>
#include <QSqlRelationalTableModel>

class QLabel;
class QLineEdit;
class QPushButton;
class QDataWidgetMapper;

class PatientForm : public QDialog{

	Q_OBJECT

	public:
		PatientForm(QSqlRelationalTableModel* model, QWidget* parent);

		private slots:
			void addPatient();
			void deletePatient();
	private:
			QLabel* firstNameLabel;
			QLineEdit* firstNameField;
			QPushButton* saveButton;
			QPushButton* closeButton;
			QSqlRelationalTableModel* m_model;
			QDataWidgetMapper* m_mapper;
};

#endif
