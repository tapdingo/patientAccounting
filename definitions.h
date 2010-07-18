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
		treatDesc = 0,
		treatId = 1,
		treatName = 2,
		treatText = 3
	};

};

enum {
	Desc = 0,
	Details = 1,
	Type = 2,
	Duration = 3,
	Diagnose = 4,
	DurationOfTreat = 5,
	DateOfTreat = 6,
	Paid = 7,
	Accounted = 8,
	TreatmentID = 9,
	PatientId = 10,
	Cost = 11,
	TreatmentName = 12
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
