#include <Windows.h>
#include <string>

using namespace std;

class Clock
{
	public:
		string time_now(SYSTEMTIME win_time)
		{
			int ora = win_time.wHour;
			int perc = win_time.wMinute;
			int masodperc = win_time.wSecond;
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