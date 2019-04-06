#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <ctime>
#include <unistd.h>
#include <windows.h>
#include <mmsystem.h>
using namespace std;

class Alarm {

	static const char bell = 7;
	unsigned hh, mm, ss;
	unsigned ah = 25, am = 0, as = 0;	// 默认不响
	int alertCount = 0;					// 还需要响几下

public:
	int alertLength = 10000;			// 闹铃长度

	void setAlarm(unsigned h, unsigned m, unsigned s)
	{
		ah = h;
		am = m;
		as = s;
	}

	void print() const	// const 方法不修改成员变量
	{
		cout << '\r'					// 清除当前行
			 << setw(2) << hh << ':'
			 << setw(2) << mm << ':'
			 << setw(2) << ss << flush;
	}

	void update(time_t cur_sec)
	{
		hh = (cur_sec / 3600 + 8) % 24;
		mm = cur_sec / 60 % 60;
		ss = cur_sec % 60;
	}

	void run()
	{
		time_t last_sec = time(NULL);	// 获取系统时间
		cout << setfill('0');			// 在 cout 的数字前补 0
		while (true) {
			usleep(2000);				// 休息 2000 μs
			/*
			if (alertCount > 0) {
				cout << bell << flush;
				--alertCount;
			}*/
            if (alertCount > 0) {
                PlaySound("Alarm07.wav", NULL, SND_FILENAME | SND_ASYNC);
                alertCount = 0;
            }
			time_t cur_sec = time(NULL);
			if (cur_sec != last_sec) {
				update(cur_sec);
				print();
				last_sec = cur_sec;
				if (ah == hh && am == mm && as == ss) {
					alertCount = alertLength;
				}
			}
		}
	}
};
//#pragma comment(lib, "winmm.lib")
int main(int argc, char **argv)
{
	Alarm a;
	if (argc > 3) {
		a.setAlarm(
			atoi(argv[1]),
			atoi(argv[2]),
			atoi(argv[3])
		);
	}
	a.run();
	return 0;
}
