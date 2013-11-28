#include <Application.h>
#include <ClassInfo.h>
#include <Menu.h>
#include <String.h>
#include <Path.h>
#include <FindDirectory.h>
#include <File.h>
#include <Alert.h>
#include <string>
#include "DatePreferences.h"
#include "PreferencesTools.h"

enum 
{
	BACKGROUNG_AM = 0, BACKGROUNG_PM, FONTDAY, FONTMONTH, FONTYEAR, POSITIONDAY,
	POSITIONMONTH, POSITIONYEAR, DAYVISIBLE, MONTHVISIBLE, YEARVISIBLE, 
	DAYSTRING, MONTHSTRING, YEARSTRING
};

struct 
{
	char *name;
	int type;
} static const VariableInfo[] = 
{
	{ "Background_AM"	, BACKGROUNG_AM},
	{ "Background_PM"	, BACKGROUNG_PM},
	{ "FontDay"			, FONTDAY},
	{ "FontMonth"		, FONTMONTH},
	{ "FontYear"		, FONTYEAR},
	{ "PositionDay"		, POSITIONDAY},
	{ "PositionMonth"	, POSITIONMONTH},
	{ "PositionYear"	, POSITIONYEAR},
	{ "DayVisible"		, DAYVISIBLE},
	{ "MonthVisible"	, MONTHVISIBLE},
	{ "YearVisible"		, YEARVISIBLE},
	{ "DayString"		, DAYSTRING},
	{ "MonthString"		, MONTHSTRING},
	{ "YearString"		, YEARSTRING}
};

extern const char *settings_file;

static const char *default_settings = 
	"Background_AM \"\"\n"
	"Background_PM \"\"\n"
	"FontDay \"Courier10 BT\" \"Bold Italic\" 20.1 150 50 60 0 \"Off\"\n"
	"FontMonth \"Courier10 BT\" \"Bold Italic\" 10.1 150 50 60 0 \"Off\"\n"
	"FontYear \"Courier10 BT\" \"Bold Italic\" 10.1 150 50 60 0 \"Off\"\n"
	"PositionDay 14 10\n"
	"PositionMonth 10 35\n"
	"PositionYear 30 35\n"
	"DayVisible \"On\"\n"
	"MonthVisible \"On\"\n"
	"YearVisible \"On\"\n"
	"DayString \"%d\"\n"
	"MonthString \"%b\"\n"
	"YearString \"%y\"";

class DateException
{
	public:
		DateException(std::string s)
		{
			std::string m = "\"Date\" application error:\n\t" + s;
			(new BAlert("Simple Date", m.c_str(),"OK"))->Go();		
		}
};

DatePreferences::DatePreferences(): 
	BWindow(BRect(20, 20, 230, 130), "Date", B_TITLED_WINDOW, B_NOT_ZOOMABLE | B_NOT_RESIZABLE)
{
	BPath path;
	SetPulseRate(500000);

	date = new DateView();
	
	if (date)
	{
		AddChild(date);
	
		if (find_directory(B_USER_SETTINGS_DIRECTORY, &path) == B_OK)
		{
			path.Append(settings_file);
			
			if (path.InitCheck() != B_OK || 
				BFile(path.Path(), B_READ_ONLY).InitCheck() != B_OK)
			{
				saveSettings(path.Path());				
			}
				
			loadSettings(path.Path());
		}
	
		ResizeTo(date->Bounds().Width(), date->Bounds().Height());
	}
}

DatePreferences::~DatePreferences()
{
}

bool DatePreferences::QuitRequested(void)
{
	be_app->PostMessage(B_QUIT_REQUESTED);
	return(TRUE);
}

void DatePreferences::loadSettings(const char *fileName)
{
	FILE *f = fopen(fileName, "rt");
	int x = 0, y = 0;
	char tmp[15] = "";
	
	if (!f)
		throw ToolsException("Settings file not present");

	for (unsigned i = 0; i < sizeof(VariableInfo)/sizeof(VariableInfo[0]); i++)
	{
		if (fscanf(f,"%s",tmp) == 1 && strcmp(tmp, VariableInfo[i].name) == 0)
		{
			switch (VariableInfo[i].type)
			{
				case BACKGROUNG_AM:
					date->path_image[0]->SetTo(getString(f).c_str());
					break;
				case BACKGROUNG_PM:
					date->path_image[1]->SetTo(getString(f).c_str());
					break;					
				case FONTDAY:
					setViewSettings(f, 0);
					break;
				case FONTMONTH:
					setViewSettings(f, 1);
					break;
				case FONTYEAR:
					setViewSettings(f, 2);
					break;
				case POSITIONDAY:
					x = getInteger(f);
					y = getInteger(f);
					date->s_view[0]->MoveTo(BPoint(x, y));
					break;
				case POSITIONMONTH:
					x = getInteger(f);
					y = getInteger(f);
					date->s_view[1]->MoveTo(BPoint(x, y));
					break;
				case POSITIONYEAR:
					x = getInteger(f);
					y = getInteger(f);
					date->s_view[2]->MoveTo(BPoint(x, y));
					break;
				case DAYVISIBLE:
					getBoolean(f)? date->s_view[0]->Show(): date->s_view[0]->Hide();
					break;
				case MONTHVISIBLE:
					getBoolean(f)? date->s_view[1]->Show(): date->s_view[1]->Hide();
					break;
				case YEARVISIBLE:
					getBoolean(f)? date->s_view[2]->Show(): date->s_view[2]->Hide();
					break;
				case DAYSTRING:
					date->OutString[0].SetTo(getString(f).c_str());
					break;
				case MONTHSTRING:
					date->OutString[1].SetTo(getString(f).c_str());
					break;
				case YEARSTRING:
					date->OutString[2].SetTo(getString(f).c_str());
					break;
				default:
					fclose(f);
					throw ToolsException("Undefined variable type");
					break;
			}
		}
		else
			{
				fclose(f);
				throw ToolsException("Expected " + std::string(VariableInfo[i].name) + "variable");
			}
	}
		
	fclose(f);
	
	date->applySettings();
}

void DatePreferences::saveSettings(const char *fileName)
{
	FILE *f = fopen(fileName, "wt");
				
	if (f)
	{
		fprintf(f, "%s", default_settings);
		fclose(f);
	}
	else
		throw ToolsException("Can't create settings file\n"+
			std::string(fileName));
}

void DatePreferences::setFont(BView *v, const char *sFamily, const char *sStyle)
{
	font_family family;
	uint32 flags;
	font_style style;

	for (int32 i = 0; i < count_font_families(); i++)
	{
		if (get_font_family(i, &family, &flags) == B_OK && 
			strcmp(family, sFamily) == 0)
		{			
			for (int32 j = 0; j < count_font_styles(family); j++)
			{
				if (get_font_style(family, j, &style, &flags) == B_OK &&
					strcmp(style, sStyle) == 0)
				{
					BFont font;
					v->GetFont(&font);
					font.SetFamilyAndStyle(family, style);
					v->SetFont(&font);
				}
			}
		}
	}
}

void DatePreferences::setViewSettings(FILE *f, unsigned i)
{
	char tmp[15] = "";
	sprintf(tmp, "%d", i);

	if (i > sizeof(date->s_view)/sizeof(date->s_view[0]))
		throw DateException(std::string("Undefined StringView #") + tmp);

	std::string s = "", name = "", style = "";
	uint8 r = 0, g = 0, b = 0, a = 0;
	
	if (date->s_view[i])
	{
		name = getString(f);
		style = getString(f);
		setFont(date->s_view[i], name.c_str(), style.c_str());
		date->s_view[i]->SetFontSize(getFloat(f));
		r = getInteger(f);
		g = getInteger(f);
		b = getInteger(f);
		a = getInteger(f);
		date->s_view[i]->SetHighColor(r, g, b, a);
		date->s_view[i]->SetViewColor(B_TRANSPARENT_32_BIT);
		date->antialiasing_view[i] = getBoolean(f) ? 
			B_FORCE_ANTIALIASING: B_DISABLE_ANTIALIASING;
	}
	else
	{
		throw DateException("StringView #" + std::string(tmp) + " is NULL");
	}
}