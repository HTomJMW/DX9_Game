#include <Windows.h>
#include <string>

using namespace std;

class Clock {
	public:
	SYSTEMTIME t;
	string time_now()
	{
		GetLocalTime(&t);
		int ora = t.wHour;
		int perc = t.wMinute;
		int masodperc = t.wSecond;
		string h = to_string(ora);
		string m = to_string(perc);
		string s = to_string(masodperc);
		if (ora < 10) { h = "0" + h; }
		if (perc < 10) { m = "0" + m; }
		if (masodperc < 10) { s = "0" + s; }
		string current_time = h + ":" + m + ":" + s;
		return current_time;
	}
};