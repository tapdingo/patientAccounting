#ifndef TREATMENTFORM_H
#define TREATMENTFORM_H

#include <QDialog>
#include <QtSql>
#include <QSqlDatabase>
#include <QSqlRelationalTableModel>

#include "definitions.h"

class QDataWidgetMapper;
class QLabel;
class QLineEdit;
class QPushButton;
class QComboBox;

class TreatmentForm : public QDialog
{
	Q_OBJECT

	public:
		TreatmentForm(QSqlRelationalTableModel* model,
				int id,
				QWidget* parent);

		private slots:
			void saveTreatment();

	private:
		QLabel* patientIDLabel;
		QLineEdit* idField;
		QLabel* costLabel;
		QLineEdit* costField;
		QLabel* nameLabel;
		QLineEdit* nameField;
		QLabel* patientLabel;
		QLineEdit* patientBox;

		QSqlRelationalTableModel* m_model;
		QDataWidgetMapper* m_mapper;

		QPushButton* saveButton;
		QPushButton* closeButton;
};

#endif
