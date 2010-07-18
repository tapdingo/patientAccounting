#ifndef STDTREATMENTFORM_H
#define STDTREATMENTFORM_H

#include <QDialog>
#include <QtSql>
#include <QSqlDatabase>
#include <QSqlRelationalTableModel>
#include <QVector>

#include "definitions.h"
#include "patientModel.h"
#include "datatypes.h"
#include <vector>

class QDataWidgetMapper;
class QLabel;
class QLineEdit;
class QPushButton;
class QComboBox;
class QDateEdit;
class QRadioButton;
class QButtonGroup;
class QCheckBox;
class QSpinBox;
class QGridLayout;
class QVBoxLayout;
class QWidget;
class QTextEdit;

class stdTreatmentForm : public QDialog
{
	Q_OBJECT

	public:
		stdTreatmentForm(
				int id,
				QWidget* parent);

		private slots:
			void saveStdTreatment();
			void expandHideDetails(int state);
			void noDetailsChanged(int number);
			void updateDetails(const QString& newCost);

	private:
		QLabel* diagnoseLabel;
		QComboBox* diagnoseComboBox;
		QLabel* dateLabel;
		QDateEdit* dateEdit;
		QLabel* durationLabel;
		QLineEdit* durationField;
		QLabel* nameLabel;
		QLineEdit* nameField;
		QLabel* patientLabel;
		QLineEdit* patientBox;

		QLabel* descLabel;
		QTextEdit* descEdit;

		QCheckBox* details;
		QLabel* numLabel;
		QSpinBox* noOfDetails;
		QLabel* detailsLabel;
		QLabel* costDetailLabel;
		QVBoxLayout* detailLayout;

		QSqlRelationalTableModel* m_model;
		QDataWidgetMapper* m_mapper;

		QPushButton* saveButton;
		QPushButton* closeButton;

		QVBoxLayout* detailFields;
		QVBoxLayout* costDetailFields;
		QGridLayout* mainLayout;
		QWidget* extension;

		std::vector<DetailTuple*> detailVector;
		std::vector<LayoutTuple*> detailFieldsDesc;

		void createLayout();

		void dumpDetails(QString& result);
		void reconstructDetailVector(QString& data);
		void initialUpdate();
		bool checkForDetails();
};

#endif
