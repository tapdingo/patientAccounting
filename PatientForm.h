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
class QDateEdit;
class QComboBox;

class PatientForm : public QDialog{

	Q_OBJECT

	public:
		PatientForm(int id, QWidget* parent);

		private slots:
			void savePatient();
	private:
			QLabel* firstNameLabel;
			QLineEdit* firstNameField;

			QLabel* lastNameLabel;
			QLineEdit* lastNameField;

			QLabel* phoneLabel;
			QLineEdit* phoneField;

			QLabel* birthdayLabel;
			QDateEdit* birthdayEdit;

			QLabel* houseNumLabel;
			QLineEdit* houseNumField;

			QLabel* streetLabel;
			QLineEdit* streetField;

			QLabel* postalLabel;
			QLineEdit* postalField;

			QLabel* cityLabel;
			QLineEdit* cityField;

			QPushButton* saveButton;
			QPushButton* closeButton;
			QSqlRelationalTableModel* m_model;
			QDataWidgetMapper* m_mapper;
};

#endif
