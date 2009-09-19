#ifndef DIAGNOSEBROWSER_H
#define DIAGNOSEBROWSER_H

#include <QDialog>
#include <QtSql>
#include <QSqlDatabase>
#include <QSqlTableModel>

class QTableView;
class QAction;
class QPushButton;

class DiagnoseBrowser : public QDialog{

	Q_OBJECT

	public:
		DiagnoseBrowser(QWidget* parent);

		private slots:
			void newDiagnose();
			void deleteDiagnose();

	private:
		QTableView* diagnoseView;
		QSqlTableModel* diagnoseModel;
		QAction* newDiagnoseAction;
		QAction* deleteDiagnoseAction;

		QPushButton* newButton;
		QPushButton* deleteButton;

		void createButtons();

};



#endif
