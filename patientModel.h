#ifndef PATIENT_MODEL_H
#define PATIENT_MODEL_H

#include "modelAbstraction.h"

class MainWindow;

class PatientModel : public ModelAbstraction
{
	public:
	virtual int addNewRecord();
	virtual bool deleteRecord(const int& id);
	virtual ~PatientModel() {};

};

#endif
