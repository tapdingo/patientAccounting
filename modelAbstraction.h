#ifndef MODEL_ABSTRACTION_H
#define MODEL_ABSTRACTION_H

#include <QSqlRelationalTableModel>

class MainWindow;

class ModelAbstraction : public QSqlRelationalTableModel
{
	public:
	virtual int addNewRecord() = 0;
	virtual bool deleteRecord(const int& id) = 0;
	virtual ~ModelAbstraction() {};

};

#endif
