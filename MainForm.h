#ifndef MAINFORM_H
#define MAINFORM_H

#include <QWidget>

class QDialogButtonBox;
class QTableView;
class QLabel;

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
		QDialogButtonBox* buttonBox;  //!< The control button box

	private:

		/*!
		 * \brief Construct the patients panel
		 */
		void createPatientPanel();

		QTableView* patientView;  //!< The table that holds the patients
		QLabel* patientLabel;  //!< The label of the patient panel
};

#endif
