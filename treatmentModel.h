#ifndef TREATMENT_MODEL_H
#define TREATMENT_MODEL_H

#include "modelAbstraction.h"

class TreatmentModel : public ModelAbstraction
{
	public:
		virtual int addNewRecord();
		virtual int addNewRelRecord(const int& id);
		virtual bool deleteRecord(const int& id);
		virtual double getOpenBills();
		virtual ~TreatmentModel() {};

};

#endif

