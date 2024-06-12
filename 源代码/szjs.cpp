#include"szjs.h"






std::string UTF8_To_string(const std::string& str)
{
	int nwLen = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, NULL, 0);

	wchar_t* pwBuf = new wchar_t[nwLen + 1];//一定要加1，不然会出现尾巴
	memset(pwBuf, 0, nwLen * 2 + 2);

	MultiByteToWideChar(CP_UTF8, 0, str.c_str(), str.length(), pwBuf, nwLen);

	int nLen = WideCharToMultiByte(CP_ACP, 0, pwBuf, -1, NULL, NULL, NULL, NULL);

	char* pBuf = new char[nLen + 1];
	memset(pBuf, 0, nLen + 1);

	WideCharToMultiByte(CP_ACP, 0, pwBuf, nwLen, pBuf, nLen, NULL, NULL);

	std::string retStr = pBuf;

	delete[]pBuf;
	delete[]pwBuf;

	pBuf = NULL;
	pwBuf = NULL;

	return retStr;
}
//将日期化为年、月、日
vector<int>splitData(const string data)
{
	vector<int>res;
	string temper;
	for (int i = 0; i < data.size(); i++)
	{
		if (isdigit(static_cast<unsigned char>(data[i])))
		{
			temper += data[i];
		}
		else
		{
			if (temper.size()!=0)
			{
				res.push_back(stoi(temper));
				temper.clear();
			}
		}
	}
	return res;
}
//计算时间间隔
int calculateTimeInterval(const vector<int>f, const vector<int>b)
{
	int f_year = f[0];
	int f_mon = f[1];
	int f_day = f[2];
	int b_year = b[0];
	int b_mon = b[1];
	int b_day = b[2];
	int res = 0;

	//如果年份相同
	if (f_year == b_year)
	{
		for (int i = f_mon; i <= b_mon - 1; i++)
		{
			if (i == 1 || i == 3 || i == 5 || i == 7 || i == 8 || i == 10 || i == 12)
			{
				res += 31;
			}
			else if (i == 4 || i == 6 || i == 9 || i == 11)
			{
				res += 30;
			}
			else
			{
				if ((f_year % 100) % 2 != 0)
				{
					res += 28;
				}
				else
				{
					if ((f_year % 100) % 4 == 0)
					{
						res += 29;
					}
					else
					{
						res += 28;
					}
				}
			}
		}
		res -= f_day;
		res += b_day;
	}
	else //如果年份不同
	{
		for (int i = f_mon; i <= 12; i++)
		{
			if (i == 1 || i == 3 || i == 5 || i == 7 || i == 8 || i == 10 || i == 12)
			{
				res += 31;
			}
			else if (i == 4 || i == 6 || i == 9 || i == 11)
			{
				res += 30;
			}
			else
			{
				if ((f_year % 100) % 2 != 0)
				{
					res += 28;
				}
				else
				{
					if ((f_year % 100) % 4 == 0)
					{
						res += 29;
					}
					else
					{
						res += 28;
					}
				}
			}
		}
		for (int i = 1; i <= b_mon - 1; i++)
		{
			if (i == 1 || i == 3 || i == 5 || i == 7 || i == 8 || i == 10 || i == 12)
			{
				res += 31;
			}
			else if (i == 4 || i == 6 || i == 9 || i == 11)
			{
				res += 30;
			}
			else
			{
				if ((b_year % 100) % 2 != 0)
				{
					res += 28;
				}
				else
				{
					if ((b_year % 100) % 4 == 0)
					{
						res += 29;
					}
					else
					{
						res += 28;
					}
				}
			}
		}
		res -= f_day;
		res += b_day;
		for (int i = f_year + 1; i <= b_year - 1; i++)
		{
			if ((i % 100) % 2 != 0)
			{
				res += 365;
			}
			else
			{
				if ((i % 100) % 4 == 0)
				{
					res += 366;
				}
				else
				{
					res += 365;
				}
			}
		}
	}
	return res;
}
