#ifndef DEFINITIONS_H
#define DEFINITIONS_H


namespace Constants
{
	const int STUNDENSATZ = 75;
	const double diag_part = 0.25;
	const double anam_part = 0.5;
	const double treat_part = 0.25;
}

enum {
	HouseNumber = 0,
	Street = 1,
	City = 2,
	PostalCode = 3,
	Parent = 4,
	Gender = 5,
	Phone = 6,
	DateOfBirth = 7,
	FirstName = 8,
	LastName = 9,
	ID = 10
};

namespace std_treat {

	enum {

		treatName = 0,
		treatText = 1
	};

};

enum {
	Details = 0,
	Type = 1,
	Duration = 2,
	Diagnose = 3,
	DurationOfTreat = 4,
	DateOfTreat = 5,
	Paid = 6,
	Accounted = 7,
	TreatmentID = 8,
	PatientId = 9,
	Cost = 10,
	TreatmentName = 11
};

enum {
	DiagnoseName = 0,
};

enum {
	BillNumber = 0,
};

enum{
	Telephone = 0,
	Practice = 1,
};

#endif
