#ifndef MODEL_ABSTRACTION_H
#define MODEL_ABSTRACTION_H

#include <QSqlRelationalTableModel>

class MainWindow;

class ModelAbstraction : public QSqlRelationalTableModel
{
	public:
	virtual void addNewRecord() = 0;

};

#endif
