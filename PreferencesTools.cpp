#include <Alert.h>
#include "PreferencesTools.h"

ToolsException::ToolsException(const std::string s)
{
	std::string m = "Settings file error:\n\t" + s;
	(new BAlert("Simple Date", m.c_str(),"OK"))->Go();
}

std::string getString(FILE *f)
{
	std::string s = "";
	char c = 0;
	
	while ( !feof(f) && isspace(c = fgetc(f)) ) ;
	
	if (c != '\"')
		throw ToolsException("Expected \" (begin string)");
	
	while ( !feof(f) && (c = fgetc(f)) != '\"' && c != '\n')
	{
		if (!feof(f) && c == '\\')
			c = fgetc(f);
			
		s += c;
	}
		
	if (c != '\"')
		throw ToolsException("Expected \" (end string)");
		
	return std::string(s.c_str());
}

float getFloat(FILE *f)
{
	float x = 0.0;
	
	if (fscanf(f, "%f", &x) != 1)
		throw ToolsException("Expected integer value");
	
	return x;
}

int getInteger(FILE *f)
{
	int x = 0;
	
	if (fscanf(f, "%d", &x) != 1)
		throw ToolsException("Expected integer value");
	
	return x;
}

bool getBoolean(FILE *f)
{
	std::string s = getString(f);
	
	if (s != "On" && s != "Off")
		throw ToolsException("Expected boolean value (\"On\"/\"Off\")");
		
	return (s == "On")? true: false;
}