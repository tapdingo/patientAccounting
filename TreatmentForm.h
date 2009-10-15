#ifndef TREATMENTFORM_H
#define TREATMENTFORM_H

#include <QDialog>
#include <QtSql>
#include <QSqlDatabase>
#include <QSqlRelationalTableModel>

#include "definitions.h"
#include "patientModel.h"

class QDataWidgetMapper;
class QLabel;
class QLineEdit;
class QPushButton;
class QComboBox;
class QDateEdit;
class QRadioButton;
class QButtonGroup;

class TreatmentForm : public QDialog
{
	Q_OBJECT

	public:
		TreatmentForm(
				int id,
				QWidget* parent);

		private slots:
			void saveTreatment();
			void durChange();

	private:
		QLabel* diagnoseLabel;
		QComboBox* diagnoseComboBox;
		QLabel* dateLabel;
		QDateEdit* dateEdit;
		QLabel* patientIDLabel;
		QLineEdit* idField;
		QLabel* durationLabel;
		QLineEdit* durationField;
		QLabel* costLabel;
		QLineEdit* costField;
		QLabel* nameLabel;
		QLineEdit* nameField;
		QLabel* patientLabel;
		QLineEdit* patientBox;

		QRadioButton* telephone;
		QRadioButton* practice;
		QButtonGroup* typeButtons;

		QSqlRelationalTableModel* m_model;
		QDataWidgetMapper* m_mapper;

		QPushButton* saveButton;
		QPushButton* closeButton;

		void createLayout();
};

#endif
