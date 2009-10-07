#ifndef TREATMENT_MODEL_H
#define TREATMENT_MODEL_H

#include "modelAbstraction.h"

class TreatmentModel : public ModelAbstraction
{
	public:
		virtual int addNewRecord();
		virtual void deleteRecord(const int& id);

};

#endif

