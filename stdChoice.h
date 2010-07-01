#ifndef STDCHOICE_H
#define STDCHOICE_H

#include <QDialog>
#include <QtSql>
#include <QSqlDatabase>
#include <QSqlTableModel>

class QComboBox;
class QPushButton;
class QLabel;
class QWidget;

class stdChoice : public QDialog
{
	Q_OBJECT
	public:
		stdChoice(QWidget* parent, const int& patientId);

	private:
		QLabel* stdLabel;
		QComboBox* m_choice;
		QPushButton* addButton;
		QPushButton* cancelButton;
		QSqlTableModel* m_model;
		const int& m_patient;

		void createLayout();
		void init();
		void filterModel();
		void unfilterModel();

		private slots:
			void addStdTreatment();
			void cancel();
};

#endif
