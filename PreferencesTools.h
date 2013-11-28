#ifndef PREFERENCESTOOLS_H
#define PREFERENCESTOOLS_H

#include <string>
#include <cstdio>

class ToolsException
{
  public:
	ToolsException(const std::string s);
};

std::string getString(FILE *f);
float getFloat(FILE *f);
int getInteger(FILE *f);
bool getBoolean(FILE *f);

#endif /* PREFERENCESTOOLS_H */