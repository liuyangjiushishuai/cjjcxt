#ifndef SZJS_H_
#define SZJS_H_



#include<iostream>
#include<string>
#include<vector>
#include <fstream>
#include <iomanip>
#include<cctype>
#include <xlnt/xlnt.hpp>
#include <wchar.h>
#include <Windows.h>
#include<unordered_map>
#include<unordered_set>
#include<algorithm>





using namespace std;
using namespace xlnt;
const string KnownPointName = "c02";    //已知点名称
const double KnownNumberHeight = 33.83300;//已知点高程
const double SettlementRateLimit = 0.04;  //沉降速率限差
const vector<string>RegionName = {"连云港新富海仓储有限公司旗台作业区液体化工品罐区工程桩基础构筑物沉降观测成果表（一）",
"连云港新富海仓储有限公司旗台作业区液体化工品罐区工程非桩基础构筑物沉降观测成果表（一）"};    //区域名称
const string ProjectName = "连云港新富海仓储有限公司旗台作业区液体化工品罐区工程沉降观测";  //工程名称
const string CompanyName = "连云港港口工程设计研究院有限公司";                          //公司名称
const string NewestMeasureData = "";                //最新一期的测量日期
const vector<string>NewAddPoint = {};   //新加的点
const vector<string>NewAddPointBelongGZW = {};  //新加的点所属的构筑物
const vector<int>NewAddPointBelongQY = {};    //新加的点所属的区域

//将日期化为年、月、日
vector<int>splitData(const string data);

//计算时间间隔
int calculateTimeInterval(const vector<int>f, const vector<int>b);

//UTF-8编码格式字符串 转普通sting类型
std::string UTF8_To_string(const std::string& str);

//测站
class cz
{
public:
	string b_point;
	string f_point;
	double first_b_num;
	double first_f_num;
	double second_f_num;
	double second_b_num;
	double first_b_distance;
	double first_f_distance;
	double second_f_distance;
	double second_b_distance;
	double front_point_height;
	double difference_v;
	cz()
	{

	}
	cz(const string b_point, const string f_point, const double first_b_num,
		const double first_f_num, const double second_f_num, const double second_b_num,
		const double first_b_distance, const double first_f_distance, const double second_f_distance,
		const double second_b_distance)
	{
		this->b_point = b_point;
		this->f_point = f_point;
		this->first_b_num = first_b_num;
		this->first_f_num = first_f_num;
		this->second_f_num = second_f_num;
		this->second_b_num = second_b_num;
		this->first_b_distance = first_b_distance;
		this->first_f_distance = first_f_distance;
		this->second_f_distance = second_f_distance;
		this->second_b_distance = second_b_distance;
	}
	double calculateUnDistributionDifference()
	{
		double res = ((first_b_num - first_f_num) + (second_b_num - second_f_num)) / 2;
		return res;
	}
	double calculate_b_distance()
	{
		return (first_b_distance + second_b_distance) / 2;
	}
	double calculate_f_distance()
	{
		return (first_f_distance + second_f_distance) / 2;
	}
	double calculate_v(const double difference_km)
	{
		double res = (((calculate_b_distance() + calculate_f_distance())) / 1000) * difference_km;
		this->difference_v = res;
		return res;
	}
	double calculae_front_point_height(const double BackPointHeight)
	{
		double res = BackPointHeight + calculateUnDistributionDifference() + difference_v;
		this->front_point_height = res;
		return res;
	}
};
//沉降点
class cj_point
{
public:
	vector<string> cl_data; //测量日期
	vector<double> cl_height; //测量高程
	int Cl_Frequency;    //测量次数
	vector<double> SettlementAmount;  //沉降量
	vector<double> AccumulateSettlementAmount;  //累积沉降量
	vector<double> SettlementSpeed;  //沉降速度
	string name;     //沉降点的名称

	cj_point(int n, string name)
	{
		this->Cl_Frequency = n;
		this->name = name;
	}
	//计算沉降量
	vector<double>calculateSettlementAmount()
	{
		vector<double>res;
		for (int i = 0; i < Cl_Frequency; i++)
		{
			if (i == 0)
			{
				if (cl_height[i] != -100)
				{
					res.push_back(0);
				}
				else
				{
					res.push_back(-1000);
				}
			}
			else
			{
				if (cl_height[i] == -100)
				{
					res.push_back(-1000);
				}
				else
				{
					bool check = false;
					for (int j = i - 1; j >= 0; j--)
					{
						if (cl_height[j] == -100)
						{
							continue;
						}
						else
						{
							check = true;
							res.push_back(cl_height[i] - cl_height[j]);
							break;
						}
					}
					if (!check)
					{
						res.push_back(0);
					}
				}
			}
		}
		this->SettlementAmount = res;
		return res;
	}
	//计算累积沉降量
	vector<double>calculateASettlementAmount()
	{
		vector<double>res;
		for (int i = 0; i < Cl_Frequency; i++)
		{
			if (i == 0)
			{
				if (cl_height[i] != -100)
				{
					res.push_back(0);
				}
				else
				{
					res.push_back(-1000);
				}
			}
			else
			{
				if (cl_height[i] == -100)
				{
					res.push_back(-1000);
				}
				else
				{
					double CurrentASettlementAmount = 0;
					CurrentASettlementAmount += this->SettlementAmount[i];
					for (int j = i-1; j>=0; j--)
					{
						if (cl_height[j] != -100)
						{
							CurrentASettlementAmount += res[j];
							break;
						}
					}
					res.push_back(CurrentASettlementAmount);
				}
			}
		}
		this->AccumulateSettlementAmount = res;
		return res;
	}

	//计算沉降速度
	vector<double>calculateSettlementSpeed()
	{
		vector<double>res;
		for (int i = 0; i < Cl_Frequency; i++)
		{
			if (i == 0)
			{
				if (cl_height[i] != -100)
				{
					res.push_back(0);
				}
				else
				{
					res.push_back(-1000);
				}
			}
			else
			{
				if (cl_height[i] == -100)
				{
					res.push_back(-1000);
				}
				else
				{
					string f_time = "";
					string b_time = cl_data[i];
					for (int j = i - 1; j >= 0; j--)
					{
						if (cl_height[j] != -100)
						{
							f_time = cl_data[j];
							break;
						}
					}
					if (f_time == "")
					{
						res.push_back(0);
					}
					else
					{
						auto f_time_result = splitData(f_time);
						auto b_time_result = splitData(b_time);
						//计算时间间隔
						double interval_time = calculateTimeInterval(f_time_result, b_time_result);
						//计算沉降速度
						res.push_back(SettlementAmount[i] / interval_time);
					}
				}
			}
		}
		this->SettlementSpeed = res;
		return res;
	}
};
//构筑物
class GZW
{
public:
	string name;    //构筑物名称
	int SettlementPointNum; //包含的沉降点数
	int frequency;        //该构筑物测量期数
	vector<cj_point>ContainSettlementPoint; //包含的沉降点
	vector<double>AverageSettlementAmount; //每期的平均沉降量
	vector<double>AverageSettlementRate;   //每期的平均沉降速度
	vector<double>AverageAccumulateSettlementAmount; //每期的平均累积沉降量


	GZW(string n/*构筑物名称*/,  int f/*测量期数*/)
	{
		this->name = n;
		this->frequency = f;
	}
	//计算每期的平均沉降量
	void calculateEachIssueAverageSettlementAmount()
	{
		for (int i = 0; i < frequency; i++)
		{
			double CurrentAllSettlementAmount = 0;
			for (int j = 0; j < ContainSettlementPoint.size(); j++)
			{
				CurrentAllSettlementAmount += ContainSettlementPoint[j].SettlementAmount[i];
			}
			AverageSettlementAmount.push_back(CurrentAllSettlementAmount / SettlementPointNum);
		}
	}
	//计算每期的平均沉降速度
	void calculateEachIssueAverageSettlementSpeed()
	{
		for (int i = 0; i < frequency; i++)
		{
			double CurrentSettlementSpeed = 0;
			for (int j = 0; j < ContainSettlementPoint.size(); j++)
			{
				CurrentSettlementSpeed += ContainSettlementPoint[j].SettlementSpeed[i];
			}
			AverageSettlementRate.push_back(CurrentSettlementSpeed / SettlementPointNum);
		}
	}
	//计算每期的平均累积沉降量
	void calcualteEachIssueAverageAccumulateSettlementAmount()
	{
		for (int i = 0; i < frequency; i++)
		{
			double CurrentAccumulateSettlementAmount = 0;
			for (int j = 0; j < ContainSettlementPoint.size(); j++)
			{
				CurrentAccumulateSettlementAmount += ContainSettlementPoint[j].AccumulateSettlementAmount[i];
			}
			AverageAccumulateSettlementAmount.push_back(CurrentAccumulateSettlementAmount / SettlementPointNum);
		}
	}
	//计算最后一期最大累积沉降量
	double calcualteMaxAccumulateSettlementAmount()
	{
		double res = -1;
		for (int i = 0; i < ContainSettlementPoint.size(); i++)
		{
			double CurrentAccumulateSettlementAmount = ContainSettlementPoint[i].AccumulateSettlementAmount[frequency - 1];
			if (CurrentAccumulateSettlementAmount > res)
			{
				res = CurrentAccumulateSettlementAmount;
			}
		}
		return res;
	}
	//计算最后一期的最小累积沉降量
	double calcualteMinAccumulateSettlementAmount()
	{
		double res = 1000000;
		for (int i = 0; i < ContainSettlementPoint.size(); i++)
		{
			double CurrentAccumulateSettlementAmount = ContainSettlementPoint[i].AccumulateSettlementAmount[frequency - 1];
			if (CurrentAccumulateSettlementAmount < res)
			{
				res = CurrentAccumulateSettlementAmount;
			}
		}
		return res;
	}
	//计算最后一期的最大沉降速度
	double calcualteMaxSettlementRate()
	{
		double res = -1;
		for (int i = 0; i < ContainSettlementPoint.size(); i++)
		{
			double CurrentSettlementRate = ContainSettlementPoint[i].SettlementSpeed[frequency - 1];
			if (CurrentSettlementRate > res)
			{
				res = CurrentSettlementRate;
			}
		}
		return res;
	}
	//计算最后一期的最小沉降速度
	double calcualteMinSettlementRate()
	{
		double res = 1000000;
		for (int i = 0; i < ContainSettlementPoint.size(); i++)
		{
			double CurrentSettlementRate = ContainSettlementPoint[i].SettlementSpeed[frequency - 1];
			if (CurrentSettlementRate < res)
			{
				res = CurrentSettlementRate;
			}
		}
		return res;
	}
};
//区域
class QY
{
public:
	string name;  //区域名称
	string EngineeringName;  //工程名称
	string CompanyName;       //公司名称

	vector<GZW>ContainGZW;//包含的构筑物
	double MaxAverageAccumulateSettlementAmount; //最大平均累计沉降量
	double MinAverageAccumulateSettlementAmount; //最小平均累计沉降量
	string MaxAASettlementAmountGZW;     //最大平均累计沉降量构筑物
	string MinAASettlementAmountGZW;    //最小平均累计沉降量构筑物
	double MaxAverageSettlementRate; //最大平均沉降速度
	double MinAverageSettlementRate; //最小平均沉降速度
	string MaxASettlementRateGZW;     //最大平均沉降速度构筑物
	string MinASettlementRateGZW;    //最小平均沉降速度构筑物
	string MaxSettlementRatePoint;   //最大沉降速度点
	double MaxSettlementRate;     //最大沉降速度
	string MaxSettlementRateGZW;   //最大沉降速度构筑物
	
	QY(string n/*区域名称*/, string EN/*工程名称*/, string CN/*公司名称*/)
	{
		this->name = n;
		this->EngineeringName = EN;
		this->CompanyName = CN;
	}

	//计算最大的平均累积沉降量
	void calculateMaxAverageAccumulateSettlementAmount()
	{
		double res = -1;
		string s;  //最大平均累计沉降量所对应的构筑物
		for (int i = 0; i < ContainGZW.size(); i++)
		{
			double rate = ContainGZW[i].frequency;   //该构筑物所测频率
			double CurrentAverageAccumulateSettlementAmount = ContainGZW[i].AverageAccumulateSettlementAmount[rate - 1];
			if (CurrentAverageAccumulateSettlementAmount > res)
			{
				res = CurrentAverageAccumulateSettlementAmount;
				s = ContainGZW[i].name;
			}
		}
		this->MaxAverageAccumulateSettlementAmount = res;
		this->MaxAASettlementAmountGZW = s;
	}
	//计算最小的平均累计沉降量
	void calculateMinAverageAccumulateSettlementAmount()
	{
		double res = 1000000;
		string s;  //最小平均累计沉降量所对应的构筑物
		for (int i = 0; i < ContainGZW.size(); i++)
		{
			double rate = ContainGZW[i].frequency;   //该构筑物所测频率
			double CurrentAverageAccumulateSettlementAmount = ContainGZW[i].AverageAccumulateSettlementAmount[rate - 1];
			if (CurrentAverageAccumulateSettlementAmount < res)
			{
				res = CurrentAverageAccumulateSettlementAmount;
				s = ContainGZW[i].name;
			}
		}
		this->MinAverageAccumulateSettlementAmount = res;
		this->MinAASettlementAmountGZW = s;
	}
	//计算最大平均沉降速度
	void calculateMaxAverageSettlementRate()
	{
		double res = -1;
		string s;  //最大平均沉降速度所对应的构筑物
		for (int i = 0; i < ContainGZW.size(); i++)
		{
			double rate = ContainGZW[i].frequency;   //该构筑物所测频率
			double CurrentAverageSettlementRate = ContainGZW[i].AverageSettlementRate[rate - 1];
			if (CurrentAverageSettlementRate > res)
			{
				res = CurrentAverageSettlementRate;
				s = ContainGZW[i].name;
			}
		}
		this->MaxAverageSettlementRate = res;
		this->MaxASettlementRateGZW = s;
	}
	//计算最小平均沉降速度
	void calculateMinAverageSettlementRate()
	{
		double res = 1000000;
		string s;  //最小平均沉降速度所对应的构筑物
		for (int i = 0; i < ContainGZW.size(); i++)
		{
			double rate = ContainGZW[i].frequency;   //该构筑物所测频率
			double CurrentAverageSettlementRate = ContainGZW[i].AverageSettlementRate[rate - 1];
			if (CurrentAverageSettlementRate < res)
			{
				res = CurrentAverageSettlementRate;
				s = ContainGZW[i].name;
			}
		}
		this->MinAverageSettlementRate = res;
		this->MinASettlementRateGZW = s;
	}
	//计算最大沉降速度
	void calculateMaxSettlementRate()
	{
		double res = -1;
		string s;  //最大沉降速度点
		string s2; //最大沉降速度构筑物
		for (int i = 0; i < ContainGZW.size(); i++)
		{
			int ContainPointNum = ContainGZW[i].ContainSettlementPoint.size(); //该构筑物所包含的点数
			int frequency = ContainGZW[i].frequency;    //该构筑物所测的期数
			if (ContainPointNum == 0)
			{
				cout << "该构筑物没有监测点" << endl;
				system("pause");
				exit(1);
			}
			for (int j = 0; j < ContainPointNum; j++)
			{
				double CurrentSettlementRate = ContainGZW[i].ContainSettlementPoint[j].SettlementSpeed[frequency - 1];
				if (CurrentSettlementRate > res)
				{
					res = CurrentSettlementRate;
					s = ContainGZW[i].ContainSettlementPoint[j].name;
					s2 = ContainGZW[i].name;
				}
			}
		}
		this->MaxSettlementRate = res;
		this->MaxSettlementRatePoint = s;
		this->MaxSettlementRateGZW = s2;
	}
	//统计超限的构筑物
	int countMoreLimitGZW()
	{
		int res = 0;
		for (int i = 0; i < ContainGZW.size(); i++)
		{
			double CurrentMaxSettlementRate = ContainGZW[i].calcualteMaxSettlementRate();
			if (CurrentMaxSettlementRate >= SettlementRateLimit)
			{
				res++;
			}
		}
		return res;
	}
};
//项目
class Project
{
public:
	vector<QY>qy; //所包含的区域
	int RegionNum;  //区域数量
	vector<unordered_set<string>>ContainGZW;  //所包含的构筑物
	vector<unordered_map<string, unordered_set<string>>>ContainCJP;//所包含的沉降点
	vector<unordered_map<string, vector<double>>>ContainHeight;  //沉降点所包含的高程
	vector<vector<string>>GZW_order;      //按顺序装的构筑物
	vector<unordered_map<string, vector<string>>>CJP_order;  //按顺序装的沉降点
	Project(int RN)
	{
		this->RegionNum = RN;
		/*this->ContainGZW.resize(this->RegionNum);
		this->ContainCJP.resize(this->RegionNum);
		this->ContainHeight.resize(this->RegionNum);*/
	}
	//加载沉降监测数据
	void LoadData(string path/*沉降监测数据文件路径*/)
	{
		xlnt::workbook wb;
		wb.load(path);


		//区域循环
		for (int i = 0; i < RegionNum; i++)
		{
			this->ContainGZW.push_back(unordered_set<string>());
			this->ContainCJP.push_back(unordered_map<string, unordered_set<string>>());
			this->ContainHeight.push_back(unordered_map<string, vector<double>>());
			this->GZW_order.push_back(vector<string>());
			this->CJP_order.push_back(unordered_map<string, vector<string>>());
			vector<GZW>structures;  //构筑物
			vector<string>time;
			auto ws = wb.sheet_by_index(i);
			int c1 = 0;
			int	StartQs = 0;   //记录每个表格的开始期数
			int SheetNum = 0;   //记录读到的表格数
			int CurrentSheetTitleRow = 0;   //记录目前的表标题行数
			string CurrentGZWName = "";     //目前的构筑物名称
			string CurrentCJPName = "";     //目前的沉降点名称
			for (auto row : ws.rows(false))
			{
				int c2 = 0;
				int c3 = StartQs;  //统计高程观测次数
				for (auto cell : row)
				{
					//读第一行，将该区域所有观测日期读入
					if (c1 == 0 && cell.has_value())
					{
						time.push_back(UTF8_To_string(cell.to_string()));
					}
					//读到观测日期这一行
					else if (UTF8_To_string(cell.to_string()) == "观测日期")
					{
						SheetNum++;
						if (SheetNum == 2)
						{
							StartQs += 3;
						}
						else if (SheetNum>2)
						{
							StartQs += 2;
						}
						break;
					}
					//读到构筑物名称这一行
					else if (UTF8_To_string(cell.to_string()) == "构筑物名称")
					{
						CurrentSheetTitleRow = c1;
						break;
					}
					//存储构筑物
					else if (cell.has_value()&&UTF8_To_string(ws.cell(c2 + 1, CurrentSheetTitleRow+1).to_string()) == "构筑物名称")
					{
						CurrentGZWName = UTF8_To_string(cell.to_string());    //目前的构筑物名称
						if (this->ContainGZW[i].count(CurrentGZWName) == 0)
						{
							this->ContainGZW[i].emplace(CurrentGZWName);
							this->ContainCJP[i].emplace(CurrentGZWName, unordered_set<string>());
							this->GZW_order[i].push_back(CurrentGZWName);
						}
					}
					//存储沉降点
					else if (cell.has_value() && UTF8_To_string(ws.cell(c2 + 1, CurrentSheetTitleRow+1).to_string()) == "点号")
					{
						CurrentCJPName = UTF8_To_string(cell.to_string()); //目前的沉降点名称
						if (this->ContainCJP[i][CurrentGZWName].count(CurrentCJPName) == 0)
						{
							this->ContainCJP[i][CurrentGZWName].emplace(CurrentCJPName);
							this->ContainHeight[i].emplace(CurrentCJPName, vector<double>());
							this->ContainHeight[i][CurrentCJPName].resize(time.size(), -100);
							this->CJP_order[i][CurrentGZWName].push_back(CurrentCJPName);
						}
					}
					//将沉降点高程加入
					else if (UTF8_To_string(ws.cell(c2 + 1, CurrentSheetTitleRow+1).to_string()).find("值") != string::npos)
					{
						if (cell.has_value())
						{
							try
							{
								this->ContainHeight[i][CurrentCJPName][c3]=stod(UTF8_To_string(cell.to_string()));
							}
							catch (const std::exception&)
							{

							}
						}
						c3++;
					}
					c2++;
				}
				c1++;
			}
			//创建该区域的构筑物
			for (int j=0;j<this->GZW_order[i].size();j++)
			{
				string CurrentGZWName =this->GZW_order[i][j];
				GZW CurrentGZW(CurrentGZWName, time.size());
				CurrentGZW.SettlementPointNum = this->ContainCJP[i][CurrentGZWName].size();
				for (int k=0; k<this->CJP_order[i][CurrentGZWName].size();k++)
				{
					string CurrentCJPName = this->CJP_order[i][CurrentGZWName][k];
					cj_point CurrentCJP(time.size(), CurrentCJPName);
					CurrentCJP.cl_height = this->ContainHeight[i][CurrentCJPName];
					CurrentCJP.cl_data = time;
					CurrentGZW.ContainSettlementPoint.push_back(CurrentCJP);
				}
				structures.push_back(CurrentGZW);
			}
			//创建一个区域
			QY CurrentQY(RegionName[i], ProjectName, CompanyName);
			CurrentQY.ContainGZW = structures;
			this->qy.push_back(CurrentQY);
		}
		//检测
		for (int i = 0; i < this->qy.size(); i++)
		{
			for (int j = 0; j < this->qy[i].ContainGZW.size(); j++)
			{
				cout << this->qy[i].ContainGZW[j].name << endl;
				for (int k = 0; k < this->qy[i].ContainGZW[j].ContainSettlementPoint.size(); k++)
				{
					cout << this->qy[i].ContainGZW[j].ContainSettlementPoint[k].name << " ";
					for (int m = 0; m < this->qy[i].ContainGZW[j].ContainSettlementPoint[k].cl_height.size(); m++)
					{
						cout << std::fixed << std::setprecision(3) << this->qy[i].ContainGZW[j].ContainSettlementPoint[k].cl_height[m] << " ";
					}
					cout << endl;
				}
			}
		}
	}
	//以规定的间隔符分割字符串
	void splitString(vector<string>& s, const char FSplitStr, const char SSplitStr, const string& str)
	{
		string temper;
		for (int i = 0; i < str.size(); i++)
		{
			if (str[i] == FSplitStr || str[i] == SSplitStr)
			{
				if (temper != "")
				{
					s.push_back(temper);
					temper.clear();
				}
			}
			else
			{
				temper += str[i];
			}
		}
		s.push_back(temper);
	}
	//读取测站信息
	void read_station_information(cz& station, const vector<string>station_line)
	{
		int flag = 0;
		for (int i = 0; i < station_line.size(); i++)
		{
			flag++;
			//分割字符串
			vector<string>s;
			splitString(s, '|', ' ', station_line[i]);
			//第一次后视
			if (flag == 1)
			{
				station.b_point = s[5];
				station.first_b_num = stod(s[9]);
				station.first_b_distance = stod(s[12]);
			}
			//第一次前视
			else if (flag == 2)
			{
				station.f_point = s[5];
				station.first_f_num = stod(s[9]);
				station.first_f_distance = stod(s[12]);
			}
			//第二次前视
			else if (flag == 3)
			{
				station.second_f_num = stod(s[9]);
				station.second_f_distance = stod(s[12]);
			}
			//第二次后视
			else if (flag == 4)
			{
				station.second_b_num = stod(s[9]);
				station.second_b_distance = stod(s[12]);
			}
		}
	}
	//读取水准原始数据
	void readFile(string path, vector<cz>& d)
	{
		//打开文件
		std::ifstream file(path);
		std::string line;
		vector<string>data;  //记录所有数据
		int flag = 0;//判断是不是一个测站的
		int line_num = 0;//记录数据行数
		vector<string>station_information; //记录一个测站的数据
		if (file.is_open())
		{
			while (std::getline(file, line))
			{
				line_num++;
				data.push_back(line);
				if (line_num <= 4)
				{
					continue;
				}
				if (line.find("#") != string::npos || line.find("Measurement repeated") != string::npos
					|| line.find("Station repeated") != string::npos || line.find("End-Line") != string::npos
					|| line.find("Cont-Line") != string::npos || line.find("Sh") != string::npos ||
					line.find("Db") != string::npos || line.find("Start-Line") != string::npos || line.find(":") == string::npos)
				{
					continue;
				}
				flag++;
				if (flag >= 1 && flag <= 4)
				{
					station_information.push_back(line);
				}
				else if (flag == 5)
				{
					//测试
					for (int i = 0; i < station_information.size(); i++)
					{
						cout << station_information[i] << endl;
					}
					//开始记入一个测站的信息
					cz station;
					read_station_information(station, station_information);
					d.push_back(station);  //将测站装入
					flag = 0;    //将测站计数置为0
					station_information.clear(); //将记录测站数据的容器置空
				}
				std::cout << line << std::endl;
			}
			////读取闭合差
			//vector<string>SplitedInformation;
			//splitString(SplitedInformation, '|', ' ', data[line_num - 3]);
			//difference = stod(SplitedInformation[6]);
			file.close();
		}
		else
		{
			std::cout << "无法打开要读取的文件" << std::endl;
		}
	}
	//计算闭合差
	double calculateClosureError(vector<cz>& d)
	{
		double res = 0;
		for (int i = 0; i < d.size(); i++)
		{
			res += d[i].calculateUnDistributionDifference();
		}
		return res;
	}
	//按路线长度分配闭合差
	void allocationClosureError(const string path, vector<cz>& d)
	{
		//计算水准路线总长度
		double all_length = 0;
		double AFDistance = 0;
		double ABDistance = 0;
		//计算闭合差
		double ClosureError = calculateClosureError(d);
		cout << "测站总数:" << d.size() << endl;
		//计算累积后视距离
		for (int i = 0; i < d.size(); i++)
		{
			ABDistance += d[i].calculate_b_distance();
		}
		//计算累积前视距离
		for (int i = 0; i < d.size(); i++)
		{
			AFDistance += d[i].calculate_f_distance();
		}
		cout << "累积后视距离:" << ABDistance << endl;
		cout << "累积前视距离" << AFDistance << endl;
		for (int i = 0; i < d.size(); i++)
		{
			all_length += d[i].calculate_b_distance() + d[i].calculate_f_distance();
		}
		//计算每千米闭合差
		double ClosureError_km = -ClosureError / (all_length / 1000);
		//计算每个测站的高差改正数
		for (int i = 0; i < d.size(); i++)
		{
			d[i].calculate_v(ClosureError_km);
			//计算每个点改正后的高程
			if (i == 0)
			{
				d[i].calculae_front_point_height(KnownNumberHeight);
			}
			else
			{
				d[i].calculae_front_point_height(d[i - 1].front_point_height);
			}
		}


		//将每个点的高程写入文本
		std::ofstream outfile(path); // 创建ofstream对象，并打开文件

		if (outfile.is_open()) // 检查文件是否成功打开	
		{
			outfile << d[0].b_point << "," << KnownNumberHeight << std::endl; // 将基准点写入
			//将待测点写入
			for (int i = 0; i < d.size(); i++)
			{
				outfile << d[i].f_point << "," << std::fixed << std::setprecision(3) << d[i].front_point_height << std::endl;
			}
			outfile.close(); // 关闭文件
		}
		else
		{
			std::cout << "无法打开要写入的文件" << std::endl;
		}
		cout << "计算完成" << endl;
	}
	//将最新一期数据加入项目
	void addNewestData(vector<cz>& d,string path1/*原始水准数据路径*/,string path2/*写入水准数据文件路径*/,int flag/*判断有没有新点,1：有，0：没有*/)
	{
		readFile(path1, d);
		allocationClosureError(path2, d);
		//存储最新一期的沉降点
		unordered_map<string, double>NewestCJP;
		for (int i = 0; i < d.size(); i++)
		{
			string name = d[i].f_point;
			double height = d[i].front_point_height;
			if (name == KnownPointName)
			{
				continue;
			}
			NewestCJP[name] = height;
		}
		for (int j = 0; j < RegionNum; j++)
		{
			for (int k = 0; k < this->qy[j].ContainGZW.size(); k++)
			{
				for (int m = 0; m < this->qy[j].ContainGZW[k].ContainSettlementPoint.size(); m++)
				{
					this->qy[j].ContainGZW[k].ContainSettlementPoint[m].cl_data.push_back(NewestMeasureData);
					this->qy[j].ContainGZW[k].ContainSettlementPoint[m].Cl_Frequency++;
					this->qy[j].ContainGZW[k].ContainSettlementPoint[m].cl_height.push_back(-100);
				}
				this->qy[j].ContainGZW[k].frequency++;
			}
		}
		for (int i = 0; i < d.size(); i++)
		{
			auto CurrentCJPName = d[i].f_point;
			auto CurrentHeight = d[i].front_point_height;
			for (int j = 0; j < RegionNum; j++)
			{
				for (int k = 0; k < this->qy[j].ContainGZW.size(); k++)
				{
					for (int m = 0; m < this->qy[j].ContainGZW[k].ContainSettlementPoint.size(); m++)
					{
						if (this->qy[j].ContainGZW[k].ContainSettlementPoint[m].name == CurrentCJPName)
						{
							this->qy[j].ContainGZW[k].ContainSettlementPoint[m].cl_height[this->qy[j].ContainGZW[k].frequency-1]=CurrentHeight;
						}
					}
				}
			}
		}
		//如果有新点
		if (flag)
		{
			if (NewAddPoint.size() == 0)
			{
				cout << "请加入新加的点" << endl;
				system("pause");
				exit(1);
			}
			for (int i = 0; i < NewAddPoint.size(); i++)
			{
				string CJPName = NewAddPoint[i];
				int BelongQY = NewAddPointBelongQY[i];
				string BelongGZW = NewAddPointBelongGZW[i];
				cj_point CJP(this->qy[0].ContainGZW[0].frequency, CJPName);
				CJP.cl_data = this->qy[0].ContainGZW[0].ContainSettlementPoint[0].cl_data;
				CJP.cl_height.resize(this->qy[0].ContainGZW[0].frequency, -100);
				CJP.cl_height[this->qy[0].ContainGZW[0].ContainSettlementPoint[0].cl_data.size() - 1] = NewestCJP[CJPName];
				if (this->ContainGZW[BelongQY].count(BelongGZW) == 0)
				{
					GZW gzw(BelongGZW, this->qy[0].ContainGZW[0].frequency);
					gzw.ContainSettlementPoint.push_back(CJP);
					gzw.SettlementPointNum++;
					this->ContainGZW[BelongQY].emplace(BelongGZW);
					this->ContainCJP[BelongQY].emplace(BelongGZW, unordered_set<string>{CJPName});
					this->ContainHeight[BelongQY].emplace(CJPName, vector<double>(this->qy[0].ContainGZW[0].ContainSettlementPoint[0].cl_data.size(), -100));
					this->ContainHeight[BelongQY][CJPName][this->qy[0].ContainGZW[0].ContainSettlementPoint[0].cl_data.size() - 1] = NewestCJP[CJPName];
					this->GZW_order[BelongQY].push_back(BelongGZW);
					this->CJP_order[BelongQY].emplace(BelongGZW, vector<string>{CJPName});
					this->qy[BelongQY].ContainGZW.push_back(gzw);
				}
				else
				{
					this->ContainCJP[BelongQY][BelongGZW].emplace(CJPName);
					this->ContainHeight[BelongQY].emplace(CJPName, vector<double>(this->qy[0].ContainGZW[0].ContainSettlementPoint[0].cl_data.size(), -100));
					this->ContainHeight[BelongQY][CJPName][this->qy[0].ContainGZW[0].ContainSettlementPoint[0].cl_data.size() - 1] = NewestCJP[CJPName];
					this->CJP_order[BelongQY][BelongGZW].push_back(CJPName);
					for (int j = 0; j < this->qy[BelongQY].ContainGZW.size(); j++)
					{
						if (this->qy[BelongQY].ContainGZW[j].name == BelongGZW)
						{
							this->qy[BelongQY].ContainGZW[j].ContainSettlementPoint.push_back(CJP);
							this->qy[BelongQY].ContainGZW[j].SettlementPointNum++;
							break;
						}
					}
				}
			}
		}
	}
	//计算各种变量
	void calculateVariable()
	{
		//计算沉降量、累积沉降量、沉降速度
		for (int i = 0; i < this->qy.size(); i++)
		{
			for (int j = 0; j < this->qy[i].ContainGZW.size(); j++)
			{
				for (int k = 0; k < this->qy[i].ContainGZW[j].ContainSettlementPoint.size(); k++)
				{
					//计算沉降量
					this->qy[i].ContainGZW[j].ContainSettlementPoint[k].calculateSettlementAmount();
					//计算累积沉降量
					this->qy[i].ContainGZW[j].ContainSettlementPoint[k].calculateASettlementAmount();
					//计算沉降速度
					this->qy[i].ContainGZW[j].ContainSettlementPoint[k].calculateSettlementSpeed();
				}
			}
		}
		//测试
		for (int i = 0; i < this->qy.size(); i++)
		{
			for (int j = 0; j < this->qy[i].ContainGZW.size(); j++)
			{
				cout << this->qy[i].ContainGZW[j].name << endl;
				for (int k = 0; k < this->qy[i].ContainGZW[j].ContainSettlementPoint.size(); k++)
				{
					cout << this->qy[i].ContainGZW[j].ContainSettlementPoint[k].name << " ";
					for (int m = 0; m < this->qy[i].ContainGZW[j].ContainSettlementPoint[k].cl_height.size(); m++)
					{
						cout << this->qy[i].ContainGZW[j].ContainSettlementPoint[k].cl_height[m] << " " <<
							this->qy[i].ContainGZW[j].ContainSettlementPoint[k].SettlementAmount[m] << " " <<
							this->qy[i].ContainGZW[j].ContainSettlementPoint[k].AccumulateSettlementAmount[m] << " " <<
							this->qy[i].ContainGZW[j].ContainSettlementPoint[k].SettlementSpeed[m] << " ";
					}
					cout << endl;
				}
			}
		}
	}
};





#endif 

