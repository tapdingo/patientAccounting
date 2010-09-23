#include "Accounting.h"
#include <QtGui>
#include <QtSql>
#include <iostream>
#include <fstream>
#include <sys/types.h>
#include <sys/stat.h>
#include "Parser.h"

PatientAccounter::PatientAccounter(
		const QSqlRecord& patient,
		QSqlRelationalTableModel& treats)
: m_patient(patient), m_treats(treats)
{
	QSqlTableModel* miscTable = new QSqlTableModel;
	miscTable->setTable("misc");
	miscTable->select();
	QSqlRecord miscRecord = miscTable->record(0);
	m_billNumber = miscRecord.value(BillNumber).toInt();
	delete miscTable;
}

void PatientAccounter::account(const QDate& date)
{
	if (m_treats.rowCount() == 0)
	{
		//Nothing to account, simple return
		return;
	}
	printBill(date);
}

void PatientAccounter::printBill(const QDate& date)
{
	QString Document;

	addRTFHeader(Document);
	addDocumentHeader(Document);
	addPatientHeader(Document);
	addInfoText(Document);

	QString billFile = createBillFile(date);
	//std::cerr << "Target: " << billFile.toStdString() << std::endl;
	if (!addTreatments(Document))
	{
		//Again nothing to account, simple return
		return;
	}
	addDocumentFooter(Document);
	finishRTF(Document);

	//Get The document Footer
	std::ofstream outStream;
	outStream.open(billFile.toStdString().c_str());
	outStream << Document.toStdString();

	//Increment the Bill Identifier
	QSqlTableModel* miscTable = new QSqlTableModel;
	miscTable->setTable("misc");
	miscTable->select();
	QSqlRecord miscRecord = miscTable->record(0);
	miscRecord.setValue(BillNumber, m_billNumber + 1);
	miscTable->setRecord(0, miscRecord);
	miscTable->submit();
	delete miscTable;
}

void PatientAccounter::addPatientHeader(QString& Document)
{
	Document.append("{\\fs24 "
			+ m_patient.value(FirstName).toString() + " "
			+ m_patient.value(LastName).toString() +
			"\\line ");
	//Patient Address
	QString patientAddress;
	patientAddress.append(generatePatientAddress());
	Document.append(patientAddress);
	Document.append("\\line \\line \\par}");

	Document.append("{\\par \\fs20 {\\trowd \\cellx4250 \\cellx8500 Rechnungs Nummer: ");
	QString IdString;
	QString billNumber;
	billNumber.setNum(m_billNumber);
	QString curMonth;
	curMonth.setNum(QDate::currentDate().month());
	QString curYear;
	curYear.setNum(QDate::currentDate().year());

	IdString.append(curMonth);
	IdString.append(curYear);
	IdString.append("/");
	IdString.append(billNumber);
	Document.append(IdString);

	Document.append(" \\line Bitte bei Bezahlung angeben! \\line\\ql\\intbl\\cell ");

	//Current Date
	Document.append(" Datum: "
			+ QDate::currentDate().toString("dd.MM.yyyy")
			+ "\\qr\\intbl\\cell");
	Document.append(" \\row ");
	Document.append("\\trowd \\cellx4250 \\cellx8500 Patient: ");

	//Patient Name
	Document.append(m_patient.value(FirstName).toString() + " "
			+ m_patient.value(LastName).toString() +
			"\\ql\\intbl\\cell ");

	//Patient Birth Date
	Document.append("Geb. Datum: "
			+ QDate::fromString(m_patient.value(DateOfBirth).toString(), "yyyy-MM-dd").toString("dd.MM.yyyy")
			+ "\\qr\\intbl\\cell ");
	Document.append("\\row} \\par}");
}

bool PatientAccounter::addTreatments(QString& Document)
{
	bool treated = false;

	 double sum = 0;

	//Iterate over all Treatments
	for (int i = 0; i < m_treats.rowCount(); i++)
	{
		QSqlRecord treatment = m_treats.record(i);

		if (treatment.value(Accounted).toInt() == 1 ||
				treatment.value(Accounted).toInt() == 2)
		{
			continue;
		}

		treatment.setValue(QString("accounted"), 2);
		m_treats.setRecord(i, treatment);
		treated = true;

		if (!m_treats.submit())
		{
			std::cerr << "Failed to update Treatment! \n";
		}

		Document.append(addTreatmentRow(treatment, sum));
	}

	//Create the Final Sum Row
	QString sumString;
	sumString.setNum(sum, 'f', 2);
	Document.append("{\\b Endsumme:  ");
	Document.append(sumString + " Euro \\qr \\par} \\par");
	return treated;
}

QString PatientAccounter::addTreatmentRow(
		const QSqlRecord& treatment,
		double& sum)
{
	QString treatmentRow;
	double treatCost = treatment.value(Cost).toDouble();
	QString costString;
	costString.setNum(treatCost, 'f', 2);
	sum += treatCost;

	QDate date = treatment.value(DateOfTreat).toDate();

	//Get The Details of this treatments
	std::vector<DetailTuple*> details;
	Parser::reconstructDetails(treatment.value(Details).toString(), details);

	QString typeString("undefined");

	if (treatment.value(Type).toInt() == Telephone)
	{
		typeString = "Telefonische Behandlung";
	}
	else
	{
		typeString = "Praxis Behandlung";
	}

	treatmentRow.append("\\fs20 {{\\trowd \\trkeep \\cellx1000 \\cellx8500  ");
	treatmentRow.append(date.toString(Qt::SystemLocaleShortDate) + " \\intbl\\cell ");

	float totalCost = treatment.value(Cost).toDouble();

	//Add all the lovely details
	std::vector<DetailTuple*>::iterator it;
	for (it = details.begin(); it != details.end(); it++)
	{
		treatmentRow.append((*it)->detail + "\\ql \\par ");
		float treatFloat = ((*it)->cost / 100)  * totalCost ;
		QString floatString;
		floatString.setNum(treatFloat, 'f', 2);
		treatmentRow.append(floatString + " Euro \\qr \\par ");
	}

	treatmentRow.append("\\line " + treatment.value(Desc).toString() + "\\ql \\par");
	treatmentRow.append("\\line \\line Zeitaufwand inkl. Fallanalysearbeiten: ");
	treatmentRow.append(treatment.value(Duration).toString() + "min \\line ");
	treatmentRow.append("Diagnose: " + treatment.value(Diagnose).toString());

	treatmentRow.append(" \\ql \\par \\intbl\\cell \\row}} \\par");

	//Clean up the mess...
	Parser::clearDetails(details);
	return treatmentRow;
}

QString PatientAccounter::generatePatientAddress()
{
	QString address;

	address.append(m_patient.value(Street).toString());
	address.append(" ");
	address.append(m_patient.value(HouseNumber).toString());
	address.append("\\line ");
	address.append(m_patient.value(PostalCode).toString());
	address.append(" ");
	address.append(m_patient.value(City).toString());

	return address;
}

void PatientAccounter::addRTFHeader(QString& document) const
{
	document.append("{\\rtf \\ansicpg1252 {\\fonttbl {\fswiss}}");
}

void PatientAccounter::addFooter(QString& document) const
{
}

void PatientAccounter::addDocumentHeader(QString& document) const
{
	QString Header;
	QFile header(":forms/header.html");
	if (!header.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		return;
	}

	QTextStream in(&header);
	while (!in.atEnd())
	{
		Header.append(in.readLine());
	}
	document.append(Header);
}

void PatientAccounter::addDocumentFooter(QString& document) const
{
	QString Footer;
	QFile footer(":forms/footer.html");
	if (!footer.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		return;
	}

	Footer.append("{\\footer ");
	QTextStream inf(&footer);
	while (!inf.atEnd())
	{
		Footer.append(inf.readLine());
	}
	Footer.append("}");
	document.append(Footer);
}

void PatientAccounter::finishRTF(QString& document) const
{
	document.append("}");
}

void PatientAccounter::addInfoText(QString& document) const
{
	QString Text;
	QFile Info(":forms/infoText.html");
	if (!Info.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		return;
	}
	QTextStream info(&Info);
	while (!info.atEnd())
	{
		Text.append(info.readLine());
	}
	document.append(Text);
}

QString PatientAccounter::createBillFile(const QDate& date)
{
	// \todo This emethod actually needs the current accounted date
	// instead of the current actual date Otherwise this only shows the month
	// and year when the accounting happened
	//Create the bill RTF for this patient
	//Format shall be: Abrechnungen/Date/patientName.rtf
	QString monthString;
	monthString.setNum(date.month());
	QString yearString;
	yearString.setNum(date.year());

	QString billFile("Abrechnungen/");

	billFile.append(yearString);
	billFile.append(monthString);
	mkdir(billFile.toStdString().c_str(), 0777);
	billFile.append("/");

	//\todo What to do when two people have the same name?
	//Possible: if_exists: add: _no. to the fileName
	billFile.append(m_patient.value(FirstName).toString());
	billFile.append(m_patient.value(LastName).toString());

	billFile.append(".rtf");
	return billFile;
}
