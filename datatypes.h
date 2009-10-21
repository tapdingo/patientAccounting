#ifndef DATATYPES_H
#define DATATYPES_H

#include <QString>

class QLineEdit;

struct DetailTuple
{
	int cost;
	QString detail;
};

struct LayoutTuple
{
	QLineEdit* detField;
	QLineEdit* costDetField;
};

#endif
