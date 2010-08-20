#ifndef DATATYPES_H
#define DATATYPES_H

#include <QString>

class QLineEdit;
class QTextEdit;

struct DetailTuple
{
	float cost;
	QString detail;
};

struct LayoutTuple
{
	QTextEdit* detField;
	QLineEdit* costDetField;
};

#endif
