#include <Application.h>
#include "DatePreferences.h"

extern const char *app_sig = "application/x-vnd.Be-SimpleDate";
extern const char *settings_file = "Date_settings";
extern const char *resources_name = "from OBOS_UI_v2_2 image";

class DateApp: public BApplication
{
  public:
	DateApp(void): BApplication(app_sig)
	{
		DatePreferences *win = new DatePreferences();
		if (win)
			win->Show();	
	}
	~DateApp() {}
};

int main(int argc, char *argv[]) try
{
	DateApp *app = new DateApp();
	
	if (app)
	{
		app->Run();
		delete app;
	}

	return 0;
}
catch (...)
{
}
