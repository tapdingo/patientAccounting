#ifndef TREATMENTFORM_H
#define TREATMENTFORM_H

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
			void expandHideDetails(int state);
			void noDetailsChanged(int number);
			void warnPaid(int state);
			void warnAccounted(int state);
			void updateDetails(const QString& newCost);
			void std_treatChanged(const QString& newstd_treat);

	private:
		QLabel* std_treatLabel;
		QComboBox* std_treatComboBox;
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

		QCheckBox* details;
		QLabel* numLabel;
		QSpinBox* noOfDetails;
		QLabel* detailsLabel;
		QLabel* costDetailLabel;
		QVBoxLayout* detailLayout;

		QCheckBox* paid;
		QCheckBox* accounted;

		QRadioButton* telephone;
		QRadioButton* practice;
		QButtonGroup* typeButtons;

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
		void restorePaid();

		int paidState;
		int accountedState;
};
#endif
