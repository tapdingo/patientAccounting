#ifndef ACCOUNTINGFORM_H
#define ACCOUNTINGFORM_H

#include <QDialog>
#include <QDate>

class QProgressBar;
class QGridLayout;
class QDateEdit;
class QLabel;
class QPushButton;
class QRadioButton;
class QButtonGroup;
class QSqlRelationalTableModel;

class AccountingForm : public QDialog
{
	Q_OBJECT

	public:
		AccountingForm(const int& pat_id);

	private:
		void createLayout();
		void makeConnections();
		void accPatient();
		bool accountPatient(const int patientRow, const QDate date = QDate::currentDate());
		void initialUpdate();
		void disableButtons();
		void enableButtons();
		QDate getFirstUnaccMonth();
		int getMonthDiff(const QDate& startDate, const QDate& endDate) const;

		QProgressBar* m_progBar;
		QGridLayout* m_layout;
		QDateEdit* m_dateEdit;
		QLabel* m_dateLabel;
		QLabel* m_progLabel;
		QPushButton* m_okButton;
		QPushButton* m_cancelButton;
		QPushButton* m_accountAll;
		QRadioButton* m_thisPatient;
		QRadioButton* m_allPatients;
		QSqlRelationalTableModel* m_patients;
		QSqlRelationalTableModel* m_treats;

		const int& m_patient;

		private slots:
			void performAccounting();
			void accountEverything();
};

#endif
