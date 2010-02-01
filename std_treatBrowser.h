#ifndef STD_TREATBROWSER_H
#define STD_TREATBROWSER_H

#include <QDialog>
#include <QtSql>
#include <QSqlDatabase>
#include <QSqlTableModel>

class QTableView;
class QAction;
class QPushButton;

class std_TreatBrowser :public QDialog{
	Q_OBJECT

	public:
		std_TreatBrowser(QWidget* parent);

		private slots:
			void newTreatment();
			void deleteTreatment();

	private:
		QTableView* treatmentView;
		QSqlTableModel* treatmentModel;
		QAction* newTreatmentAction;
		QAction* deleteTreatmentAction;

		QPushButton* newButton;
		QPushButton* deleteButton;

		void createButtons();

};

#endif
