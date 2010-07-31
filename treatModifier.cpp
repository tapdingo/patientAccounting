#include "treatModifier.h"

#include <QtSql>

#include "definitions.h"

void TreatModifier::setAccounted(QSqlRecord* record, const int& value)
{
	record->setValue(Accounted, value);
}
