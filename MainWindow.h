#ifndef MAINFORM_H
#define MAINFORM_H

#include <QMainWindow>

class QDialogButtonBox;
class QTableView;
class QLabel;
class QPushButton;
class QToolBar;
class QWidget;
class QAction;
class QSqlRelationalTableModel;

/*!
 * \brief The Main Window Form Class
 *
 * This Form handles all UI related stuff
 */
class MainWindow : public QMainWindow
{
	Q_OBJECT

	public:
		/*!
		 * \brief Construct the Main Form Object
		 */
		MainWindow();

		QWidget* patientPanel;  //!< The upper panel that holds the patients
		QWidget* dataPanel;  //!< The lower Panel for the patients data
		QToolBar* toolbar;  //!< The toolbar

	private:

		/*!
		 * \brief Construct the patients panel
		 */
		void createPatientPanel();

		/*!
		 * \brief Construct the patient data panel
		 */
		void createDataPanel();

		/*!
		 * \brief Create the button panel
		 */
		void createToolbar();

		/*!
		 * \brief Create the basic actions 
		 */
		void createActions();

		/*!
		 * \brief Create the patient overview
		 */
		void createPatientWidget();

		/*!
		 * \brief Connect to the SQLite Database
		 */
		bool connectToDB();

		/*!
		 * \brief Crate the status bar
		 */
		void createStatusBar();

		QTableView* patientView;  //!< The table that holds the patients
		QLabel* patientLabel;  //!< The label of the patient panel

		QTableView* dataView;  //!< The table for the patients data
		QLabel* dataLabel;  //!< The label of the patients data
		QWidget* patientWidget; //!< The patient data container

		QStatusBar* m_statusBar;  //!< The main status bar

		QSqlRelationalTableModel* patientModel; //!< The model for patient data
		QSqlRelationalTableModel* dataModel; //!< The model for patient treatments

		QAction* newAction;
		QAction* editAction;
		QAction* findAction;
		QAction* deleteAction;
		QAction* accountAction;

		private slots:
			void updateTreatmentView();

		/*!
		 * \brief Add a new Patient to the database
		 *
		 * \todo The new patient has to be edited!
		 */
			void addPatient();

		/*!
		 * \brief Delete a Patient from the database
		 */
			void deletePatient();
};

#endif
