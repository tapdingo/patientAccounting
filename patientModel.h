#ifndef PATIENT_MODEL_H
#define PATIENT_MODEL_H

#include "modelAbstraction.h"

class MainWindow;

class PatientModel : public ModelAbstraction
{
	public:
	virtual int addNewRecord();
	virtual void deleteRecord(const int& id);

};

#endif
