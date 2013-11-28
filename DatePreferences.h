#ifndef DATEPREFERENCES_H
#define DATEPREFERENCES_H

#include <Window.h>
#include <View.h>
#include <MenuField.h>
#include <cstdio>
#include "DateView.h"

class DatePreferences: public BWindow
{
	DateView *date;
	
	void loadSettings(const char *fileName);
	void saveSettings(const char *fileName);
	void setFont(BView *v, const char *sFamily, const char *sStyle);
	void setViewSettings(FILE *f, unsigned i);
  public:

	DatePreferences();
	~DatePreferences();
	
	bool QuitRequested(void);
};

#endif /* DATEPREFERENCES_H */