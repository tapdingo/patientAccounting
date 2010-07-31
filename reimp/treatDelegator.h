#ifndef TREATDELEGATOR_H
#define TREATDELEGATOR_H

#include <QtGui>

class QSqlTableModel;

class TreatDelegator: public QItemDelegate {

	public:
		TreatDelegator(const QSqlTableModel* model);

	protected:
		virtual void paint(QPainter* p,
				const QStyleOptionViewItem& option,
				const QModelIndex& index) const;

	private:
		const QSqlTableModel* m_model;
};

#endif
