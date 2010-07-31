#include "treatDelegator.h"

#include <QtSql>
#include <QSqlRelationalTableModel>
#include <QSqlDatabase>
#include <iostream>
#include "../definitions.h"

void TreatDelegator::paint(
		QPainter* p,
		const QStyleOptionViewItem& option,
		const QModelIndex& index) const
{
	QItemDelegate::paint(p, option, index);
	QColor fillColor(Qt::red);
	fillColor.setAlpha(100);

	QSqlRecord treat = m_model->record(index.row());
	if (0 == treat.value(Accounted).toInt())
	{
		p->fillRect(option.rect, fillColor);
	}
}

TreatDelegator::TreatDelegator(const QSqlTableModel* model) : m_model(model)
{
}
