#ifndef DATEVIEW_H
#define DATEVIEW_H

#include <View.h>
#include <Bitmap.h>
#include <String.h>
#include <StringView.h>
#include <string>
#include <ctime>

class DatePreferences;

class DateView: public BView
{
	time_t	last_time;
	char tmp_buf[15];
	BBitmap *LoadImage(const char *file_name);
  private:
  	friend DatePreferences;
	
	BString *path_image[2], OutString[3];
	BBitmap *Background, *b_image[2];
	BStringView *s_view[3];
	uint32 antialiasing_view[3];
	
	void applyChanges(void);
	void applySettings(bool changes = true);
  public:
	DateView(void);
	DateView(BMessage *archive);
	~DateView();
	
	status_t Archive(BMessage *archive, bool deep = true) const;
	static BArchivable *Instantiate(BMessage *archive);
	
	void AttachedToWindow(void);
	void Pulse(void);
	void MessageReceived(BMessage *msg);
};

#endif /* DATEVIEW_H */