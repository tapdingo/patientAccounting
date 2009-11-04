#ifndef PARSER_H
#define PARSER_H

#include "datatypes.h"
#include <vector>

class QString;

class Parser
{
	public:
		static void reconstructDetails(
				const QString& data,
				std::vector<DetailTuple*>& tgt);

		static void clearDetails(
				std::vector<DetailTuple*>& tgt);
};

#endif
