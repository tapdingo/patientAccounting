#ifndef MAINFORM_H
#define MAINFORM_H

#include <QWidget>

class QDialogButtonBox;
class QTableView;
class QLabel;
class QPushButton;

/*!
 * \brief The Main Window Form Class
 *
 * This Form handles all UI related stuff
 */
class MainForm : public QWidget
{
	Q_OBJECT

	public:
		/*!
		 * \brief Construct the Main Form Object
		 */
		MainForm();

		QWidget* patientPanel;  //!< The upper panel that holds the patients
		QWidget* dataPanel;  //!< The lower Panel for the patients data
		QDialogButtonBox* buttonBox;  //!< The control button box

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
		void createButtonPanel();

		QTableView* patientView;  //!< The table that holds the patients
		QLabel* patientLabel;  //!< The label of the patient panel

		QTableView* dataView;  //!< The table for the patients data
		QLabel* dataLabel;  //!< The label of the patients data

		QPushButton* addPatient;  //!< Button for adding patients
		QPushButton* editPatient;  //!< Button for editing patients
		QPushButton* findPatient;  //!< Button for finding patients
		QPushButton* deletePatient;  //!< Button for deleting patients
};

#endif
