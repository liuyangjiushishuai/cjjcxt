#include"szjs.h"






std::string UTF8_To_string(const std::string& str)
{
	int nwLen = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, NULL, 0);

	wchar_t* pwBuf = new wchar_t[nwLen + 1];//һ��Ҫ��1����Ȼ�����β��
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
double SaveThreeDecimal(double OriginalData)
{
	string c_str2 = to_string(OriginalData);
	string c_str = to_string(abs(OriginalData));
	/*cout << c_str << endl;*/
	/*cout << c_str2 << endl;*/
	vector<int>z;
	string t;
	bool check = false;
	int count = 0;
	for (int i = 0; i < c_str.size(); i++)
	{
		if (!check)
		{
			if (c_str[i] == '.')
			{
				z.push_back(stoi(t));
				check = true;
			}
			else
			{
				t += c_str[i];
			}
		}
		else
		{
			if (count < 4)
			{
				string T;
				T += c_str[i];
				z.push_back(stoi(T));
				count++;
			}
			else
			{
				break;
			}
		}
	}
	for (int i = z.size() - 1; i >= 0; i--)
	{
		if (i == 4)
		{
			if (z[i] >= 5)
			{
				z[i - 1] += 1;
			}
		}
		else if (i > 0 && i < 4)
		{
			if (z[i] >= 10)
			{
				z[i] = 0;
				z[i - 1]++;
			}
			else
			{
				break;
			}
		}
	}
	string t2;
	if (c_str2[0] == '-')
	{
		t2 += '-';
	}
	for (int i = 0; i < 4; i++)
	{
		if (i == 0)
		{
			t2 += to_string(z[i]);
			t2 += '.';
		}
		else
		{
			t2 += to_string(z[i]);
		}
	}

	int count2 = 0;
	for (int i = 0; i < t2.size(); i++)
	{
		if (isdigit(t2[i]))
		{
			if (t2[i] == '0')
			{
				count2++;
			}
		}
	}
	if (count2 == 4)
	{
		if (t2[0] == '-')
		{
			t2.erase(t2.begin());
		}
	}
	double res = stod(t2);
	return res;
}
//�����ڻ�Ϊ�ꡢ�¡���
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
//����ʱ����
int calculateTimeInterval(const vector<int>f, const vector<int>b)
{
	int f_year = f[0];
	int f_mon = f[1];
	int f_day = f[2];
	int b_year = b[0];
	int b_mon = b[1];
	int b_day = b[2];
	int res = 0;

	//��������ͬ
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
	else //�����ݲ�ͬ
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
