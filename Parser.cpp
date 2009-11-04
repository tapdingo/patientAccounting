#include "Parser.h"
#include <iostream>

#include <QtGui>

void Parser::reconstructDetails(const QString& data, std::vector<DetailTuple*>& tgt)
{
	QStringList splitted = data.split(";", QString::SkipEmptyParts);

	for (int i = 0; i < splitted.size(); i++)
	{
		QStringList line_details = splitted[i].split("||", QString::SkipEmptyParts);
		if ( line_details.size() > 0)
		{
			DetailTuple* n_tuple = new DetailTuple;
			n_tuple->detail = line_details[0];
			n_tuple->cost = line_details[1].toFloat();
			tgt.push_back(n_tuple);
		}
	}
}

void Parser::clearDetails(std::vector<DetailTuple*>&tgt)
{
	std::vector<DetailTuple*>::iterator it;
	for (it = tgt.begin(); it != tgt.end(); it++)
	{
		delete *it;
	}
	tgt.clear();
}
