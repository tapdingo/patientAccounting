#ifndef MAINFORM_H
#define MAINFORM_H

#include <QMainWindow>
#include "PatientForm.h"
#include "TreatmentForm.h"

class QDialogButtonBox;
class QTableView;
class QLabel;
class QPushButton;
class QToolBar;
class QWidget;
class QAction;
class QSqlRelationalTableModel;
class QPrinter;

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

		/*!
		 * \brief Create the different Menus
		 */
		void createMenus();

		/*!
		 * \brief Interlink the Actions with the slots
		 */
		void connectSlots();

		void updateStatusBar();

		QTableView* patientView;  //!< The table that holds the patients
		QLabel* patientLabel;  //!< The label of the patient panel

		QTableView* dataView;  //!< The table for the patients data
		QLabel* dataLabel;  //!< The label of the patients data
		QWidget* patientWidget; //!< The patient data container

		QStatusBar* m_statusBar;  //!< The main status bar

		QSqlRelationalTableModel* patientModel; //!< The model for patient data
		QSqlRelationalTableModel* dataModel; //!< The model for patient treatments


		QMenu* patientMenu;  //!< The Menu to add/edit/delete Patients
		QMenu* treatmentMenu;  //!< The Menu to add/edit/delete Treatments
		QMenu* accountingMenu;  //!< The Menu to Account Patients
		QMenu* miscMenu;  //!< Miscallenous Menu

		QAction* newAction;
		QAction* editAction;
		QAction* findAction;
		QAction* deleteAction;
		QAction* browseDiagnosesAction;
		QAction* accountAction;
		QAction* aboutAction;
		QAction* aboutQTAction;

		QAction* newTreatmentAction;
		QAction* editTreatmentAction;
		QAction* deleteTreatmentAction;
		QPrinter* printer;

		QLabel* unaccountedPatients;

		private slots:

			/*!
			 * \brief Update the patient Treatments view
			 */
			void updateTreatmentView();

		/*!
		 * \brief Add a new Patient to the database
		 *
		 */
			void addPatient();

		/*!
		 * \brief Delete a Patient from the database
		 */
			void deletePatient();

		/*!
		 * \brief Edit a patient in the database
		 */
			void editPatient();

		/*!
		 * \brief Add a new Patient to the database
		 *
		 */
			void addTreatment();

		/*!
		 * \brief Delete a Patient from the database
		 */
			void deleteTreatment();

		/*!
		 * \brief Edit a patient in the database
		 */
			void editTreatment();

		/*!
		 * \brief Account a patient
		 */
		void accountPatient();

		/*!
		 * \brief Opens the Diagnose browser
		 */
		void browseDiagnoses();

		/*!
		 * \brief Opens the About Window
		 */
		void about();

};

#endif
