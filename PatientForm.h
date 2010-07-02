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
class QCheckBox;

class PatientForm : public QDialog{

	Q_OBJECT

	public:
		PatientForm(int id, QWidget* parent);

		private slots:
			void savePatient();
			void parentCheckToggle(int state);
	private:
			QLabel* firstNameLabel;
			QLineEdit* firstNameField;

			QLabel* lastNameLabel;
			QLineEdit* lastNameField;

			QLabel* phoneLabel;
			QLineEdit* phoneField;

			QCheckBox* parentCheck;
			QLabel* parentLabel;
			QLineEdit* parentField;

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

			void finishLayout();
			void showParentAddition();
			void hideParentAddition();
};

#endif
