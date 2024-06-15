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
const string KnownPointName = "c02";    //��֪������
const double KnownNumberHeight = 33.83300;//��֪��߳�
const double SettlementRateLimit = 0.04;  //���������޲�
const vector<string>RegionName = {"���Ƹ��¸����ִ����޹�˾��̨��ҵ��Һ�廯��Ʒ��������׮��������������۲�ɹ���һ��",
"���Ƹ��¸����ִ����޹�˾��̨��ҵ��Һ�廯��Ʒ�������̷�׮��������������۲�ɹ���һ��"};    //��������
const string ProjectName = "���Ƹ��¸����ִ����޹�˾��̨��ҵ��Һ�廯��Ʒ�������̳����۲�";  //��������
const string CompanyName = "���Ƹ۸ۿڹ�������о�Ժ���޹�˾";                          //��˾����
const string NewestMeasureData = "";                //����һ�ڵĲ�������
const vector<string>NewAddPoint = {};   //�¼ӵĵ�
const vector<string>NewAddPointBelongGZW = {};  //�¼ӵĵ������Ĺ�����
const vector<int>NewAddPointBelongQY = {};    //�¼ӵĵ�����������

//�����ڻ�Ϊ�ꡢ�¡���
vector<int>splitData(const string data);

//����ʱ����
int calculateTimeInterval(const vector<int>f, const vector<int>b);

//UTF-8�����ʽ�ַ��� ת��ͨsting����
std::string UTF8_To_string(const std::string& str);

//��վ
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
//������
class cj_point
{
public:
	vector<string> cl_data; //��������
	vector<double> cl_height; //�����߳�
	int Cl_Frequency;    //��������
	vector<double> SettlementAmount;  //������
	vector<double> AccumulateSettlementAmount;  //�ۻ�������
	vector<double> SettlementSpeed;  //�����ٶ�
	string name;     //�����������

	cj_point(int n, string name)
	{
		this->Cl_Frequency = n;
		this->name = name;
	}
	//���������
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
	//�����ۻ�������
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

	//��������ٶ�
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
						//����ʱ����
						double interval_time = calculateTimeInterval(f_time_result, b_time_result);
						//��������ٶ�
						res.push_back(SettlementAmount[i] / interval_time);
					}
				}
			}
		}
		this->SettlementSpeed = res;
		return res;
	}
};
//������
class GZW
{
public:
	string name;    //����������
	int SettlementPointNum; //�����ĳ�������
	int frequency;        //�ù������������
	vector<cj_point>ContainSettlementPoint; //�����ĳ�����
	vector<double>AverageSettlementAmount; //ÿ�ڵ�ƽ��������
	vector<double>AverageSettlementRate;   //ÿ�ڵ�ƽ�������ٶ�
	vector<double>AverageAccumulateSettlementAmount; //ÿ�ڵ�ƽ���ۻ�������


	GZW(string n/*����������*/,  int f/*��������*/)
	{
		this->name = n;
		this->frequency = f;
	}
	//����ÿ�ڵ�ƽ��������
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
	//����ÿ�ڵ�ƽ�������ٶ�
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
	//����ÿ�ڵ�ƽ���ۻ�������
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
	//�������һ������ۻ�������
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
	//�������һ�ڵ���С�ۻ�������
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
	//�������һ�ڵ��������ٶ�
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
	//�������һ�ڵ���С�����ٶ�
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
//����
class QY
{
public:
	string name;  //��������
	string EngineeringName;  //��������
	string CompanyName;       //��˾����

	vector<GZW>ContainGZW;//�����Ĺ�����
	double MaxAverageAccumulateSettlementAmount; //���ƽ���ۼƳ�����
	double MinAverageAccumulateSettlementAmount; //��Сƽ���ۼƳ�����
	string MaxAASettlementAmountGZW;     //���ƽ���ۼƳ�����������
	string MinAASettlementAmountGZW;    //��Сƽ���ۼƳ�����������
	double MaxAverageSettlementRate; //���ƽ�������ٶ�
	double MinAverageSettlementRate; //��Сƽ�������ٶ�
	string MaxASettlementRateGZW;     //���ƽ�������ٶȹ�����
	string MinASettlementRateGZW;    //��Сƽ�������ٶȹ�����
	string MaxSettlementRatePoint;   //�������ٶȵ�
	double MaxSettlementRate;     //�������ٶ�
	string MaxSettlementRateGZW;   //�������ٶȹ�����
	
	QY(string n/*��������*/, string EN/*��������*/, string CN/*��˾����*/)
	{
		this->name = n;
		this->EngineeringName = EN;
		this->CompanyName = CN;
	}

	//��������ƽ���ۻ�������
	void calculateMaxAverageAccumulateSettlementAmount()
	{
		double res = -1;
		string s;  //���ƽ���ۼƳ���������Ӧ�Ĺ�����
		for (int i = 0; i < ContainGZW.size(); i++)
		{
			double rate = ContainGZW[i].frequency;   //�ù���������Ƶ��
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
	//������С��ƽ���ۼƳ�����
	void calculateMinAverageAccumulateSettlementAmount()
	{
		double res = 1000000;
		string s;  //��Сƽ���ۼƳ���������Ӧ�Ĺ�����
		for (int i = 0; i < ContainGZW.size(); i++)
		{
			double rate = ContainGZW[i].frequency;   //�ù���������Ƶ��
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
	//�������ƽ�������ٶ�
	void calculateMaxAverageSettlementRate()
	{
		double res = -1;
		string s;  //���ƽ�������ٶ�����Ӧ�Ĺ�����
		for (int i = 0; i < ContainGZW.size(); i++)
		{
			double rate = ContainGZW[i].frequency;   //�ù���������Ƶ��
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
	//������Сƽ�������ٶ�
	void calculateMinAverageSettlementRate()
	{
		double res = 1000000;
		string s;  //��Сƽ�������ٶ�����Ӧ�Ĺ�����
		for (int i = 0; i < ContainGZW.size(); i++)
		{
			double rate = ContainGZW[i].frequency;   //�ù���������Ƶ��
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
	//�����������ٶ�
	void calculateMaxSettlementRate()
	{
		double res = -1;
		string s;  //�������ٶȵ�
		string s2; //�������ٶȹ�����
		for (int i = 0; i < ContainGZW.size(); i++)
		{
			int ContainPointNum = ContainGZW[i].ContainSettlementPoint.size(); //�ù������������ĵ���
			int frequency = ContainGZW[i].frequency;    //�ù��������������
			if (ContainPointNum == 0)
			{
				cout << "�ù�����û�м���" << endl;
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
	//ͳ�Ƴ��޵Ĺ�����
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
//��Ŀ
class Project
{
public:
	vector<QY>qy; //������������
	int RegionNum;  //��������
	vector<unordered_set<string>>ContainGZW;  //�������Ĺ�����
	vector<unordered_map<string, unordered_set<string>>>ContainCJP;//�������ĳ�����
	vector<unordered_map<string, vector<double>>>ContainHeight;  //�������������ĸ߳�
	vector<vector<string>>GZW_order;      //��˳��װ�Ĺ�����
	vector<unordered_map<string, vector<string>>>CJP_order;  //��˳��װ�ĳ�����
	Project(int RN)
	{
		this->RegionNum = RN;
		/*this->ContainGZW.resize(this->RegionNum);
		this->ContainCJP.resize(this->RegionNum);
		this->ContainHeight.resize(this->RegionNum);*/
	}
	//���س����������
	void LoadData(string path/*������������ļ�·��*/)
	{
		xlnt::workbook wb;
		wb.load(path);


		//����ѭ��
		for (int i = 0; i < RegionNum; i++)
		{
			this->ContainGZW.push_back(unordered_set<string>());
			this->ContainCJP.push_back(unordered_map<string, unordered_set<string>>());
			this->ContainHeight.push_back(unordered_map<string, vector<double>>());
			this->GZW_order.push_back(vector<string>());
			this->CJP_order.push_back(unordered_map<string, vector<string>>());
			vector<GZW>structures;  //������
			vector<string>time;
			auto ws = wb.sheet_by_index(i);
			int c1 = 0;
			int	StartQs = 0;   //��¼ÿ�����Ŀ�ʼ����
			int SheetNum = 0;   //��¼�����ı����
			int CurrentSheetTitleRow = 0;   //��¼Ŀǰ�ı��������
			string CurrentGZWName = "";     //Ŀǰ�Ĺ���������
			string CurrentCJPName = "";     //Ŀǰ�ĳ���������
			for (auto row : ws.rows(false))
			{
				int c2 = 0;
				int c3 = StartQs;  //ͳ�Ƹ̹߳۲����
				for (auto cell : row)
				{
					//����һ�У������������й۲����ڶ���
					if (c1 == 0 && cell.has_value())
					{
						time.push_back(UTF8_To_string(cell.to_string()));
					}
					//�����۲�������һ��
					else if (UTF8_To_string(cell.to_string()) == "�۲�����")
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
					//����������������һ��
					else if (UTF8_To_string(cell.to_string()) == "����������")
					{
						CurrentSheetTitleRow = c1;
						break;
					}
					//�洢������
					else if (cell.has_value()&&UTF8_To_string(ws.cell(c2 + 1, CurrentSheetTitleRow+1).to_string()) == "����������")
					{
						CurrentGZWName = UTF8_To_string(cell.to_string());    //Ŀǰ�Ĺ���������
						if (this->ContainGZW[i].count(CurrentGZWName) == 0)
						{
							this->ContainGZW[i].emplace(CurrentGZWName);
							this->ContainCJP[i].emplace(CurrentGZWName, unordered_set<string>());
							this->GZW_order[i].push_back(CurrentGZWName);
						}
					}
					//�洢������
					else if (cell.has_value() && UTF8_To_string(ws.cell(c2 + 1, CurrentSheetTitleRow+1).to_string()) == "���")
					{
						CurrentCJPName = UTF8_To_string(cell.to_string()); //Ŀǰ�ĳ���������
						if (this->ContainCJP[i][CurrentGZWName].count(CurrentCJPName) == 0)
						{
							this->ContainCJP[i][CurrentGZWName].emplace(CurrentCJPName);
							this->ContainHeight[i].emplace(CurrentCJPName, vector<double>());
							this->ContainHeight[i][CurrentCJPName].resize(time.size(), -100);
							this->CJP_order[i][CurrentGZWName].push_back(CurrentCJPName);
						}
					}
					//��������̼߳���
					else if (UTF8_To_string(ws.cell(c2 + 1, CurrentSheetTitleRow+1).to_string()).find("ֵ") != string::npos)
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
			//����������Ĺ�����
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
			//����һ������
			QY CurrentQY(RegionName[i], ProjectName, CompanyName);
			CurrentQY.ContainGZW = structures;
			this->qy.push_back(CurrentQY);
		}
		//���
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
	//�Թ涨�ļ�����ָ��ַ���
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
	//��ȡ��վ��Ϣ
	void read_station_information(cz& station, const vector<string>station_line)
	{
		int flag = 0;
		for (int i = 0; i < station_line.size(); i++)
		{
			flag++;
			//�ָ��ַ���
			vector<string>s;
			splitString(s, '|', ' ', station_line[i]);
			//��һ�κ���
			if (flag == 1)
			{
				station.b_point = s[5];
				station.first_b_num = stod(s[9]);
				station.first_b_distance = stod(s[12]);
			}
			//��һ��ǰ��
			else if (flag == 2)
			{
				station.f_point = s[5];
				station.first_f_num = stod(s[9]);
				station.first_f_distance = stod(s[12]);
			}
			//�ڶ���ǰ��
			else if (flag == 3)
			{
				station.second_f_num = stod(s[9]);
				station.second_f_distance = stod(s[12]);
			}
			//�ڶ��κ���
			else if (flag == 4)
			{
				station.second_b_num = stod(s[9]);
				station.second_b_distance = stod(s[12]);
			}
		}
	}
	//��ȡˮ׼ԭʼ����
	void readFile(string path, vector<cz>& d)
	{
		//���ļ�
		std::ifstream file(path);
		std::string line;
		vector<string>data;  //��¼��������
		int flag = 0;//�ж��ǲ���һ����վ��
		int line_num = 0;//��¼��������
		vector<string>station_information; //��¼һ����վ������
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
					//����
					for (int i = 0; i < station_information.size(); i++)
					{
						cout << station_information[i] << endl;
					}
					//��ʼ����һ����վ����Ϣ
					cz station;
					read_station_information(station, station_information);
					d.push_back(station);  //����վװ��
					flag = 0;    //����վ������Ϊ0
					station_information.clear(); //����¼��վ���ݵ������ÿ�
				}
				std::cout << line << std::endl;
			}
			////��ȡ�պϲ�
			//vector<string>SplitedInformation;
			//splitString(SplitedInformation, '|', ' ', data[line_num - 3]);
			//difference = stod(SplitedInformation[6]);
			file.close();
		}
		else
		{
			std::cout << "�޷���Ҫ��ȡ���ļ�" << std::endl;
		}
	}
	//����պϲ�
	double calculateClosureError(vector<cz>& d)
	{
		double res = 0;
		for (int i = 0; i < d.size(); i++)
		{
			res += d[i].calculateUnDistributionDifference();
		}
		return res;
	}
	//��·�߳��ȷ���պϲ�
	void allocationClosureError(const string path, vector<cz>& d)
	{
		//����ˮ׼·���ܳ���
		double all_length = 0;
		double AFDistance = 0;
		double ABDistance = 0;
		//����պϲ�
		double ClosureError = calculateClosureError(d);
		cout << "��վ����:" << d.size() << endl;
		//�����ۻ����Ӿ���
		for (int i = 0; i < d.size(); i++)
		{
			ABDistance += d[i].calculate_b_distance();
		}
		//�����ۻ�ǰ�Ӿ���
		for (int i = 0; i < d.size(); i++)
		{
			AFDistance += d[i].calculate_f_distance();
		}
		cout << "�ۻ����Ӿ���:" << ABDistance << endl;
		cout << "�ۻ�ǰ�Ӿ���" << AFDistance << endl;
		for (int i = 0; i < d.size(); i++)
		{
			all_length += d[i].calculate_b_distance() + d[i].calculate_f_distance();
		}
		//����ÿǧ�ױպϲ�
		double ClosureError_km = -ClosureError / (all_length / 1000);
		//����ÿ����վ�ĸ߲������
		for (int i = 0; i < d.size(); i++)
		{
			d[i].calculate_v(ClosureError_km);
			//����ÿ���������ĸ߳�
			if (i == 0)
			{
				d[i].calculae_front_point_height(KnownNumberHeight);
			}
			else
			{
				d[i].calculae_front_point_height(d[i - 1].front_point_height);
			}
		}


		//��ÿ����ĸ߳�д���ı�
		std::ofstream outfile(path); // ����ofstream���󣬲����ļ�

		if (outfile.is_open()) // ����ļ��Ƿ�ɹ���	
		{
			outfile << d[0].b_point << "," << KnownNumberHeight << std::endl; // ����׼��д��
			//�������д��
			for (int i = 0; i < d.size(); i++)
			{
				outfile << d[i].f_point << "," << std::fixed << std::setprecision(3) << d[i].front_point_height << std::endl;
			}
			outfile.close(); // �ر��ļ�
		}
		else
		{
			std::cout << "�޷���Ҫд����ļ�" << std::endl;
		}
		cout << "�������" << endl;
	}
	//������һ�����ݼ�����Ŀ
	void addNewestData(vector<cz>& d,string path1/*ԭʼˮ׼����·��*/,string path2/*д��ˮ׼�����ļ�·��*/,int flag/*�ж���û���µ�,1���У�0��û��*/)
	{
		readFile(path1, d);
		allocationClosureError(path2, d);
		//�洢����һ�ڵĳ�����
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
		//������µ�
		if (flag)
		{
			if (NewAddPoint.size() == 0)
			{
				cout << "������¼ӵĵ�" << endl;
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
	//������ֱ���
	void calculateVariable()
	{
		//������������ۻ��������������ٶ�
		for (int i = 0; i < this->qy.size(); i++)
		{
			for (int j = 0; j < this->qy[i].ContainGZW.size(); j++)
			{
				for (int k = 0; k < this->qy[i].ContainGZW[j].ContainSettlementPoint.size(); k++)
				{
					//���������
					this->qy[i].ContainGZW[j].ContainSettlementPoint[k].calculateSettlementAmount();
					//�����ۻ�������
					this->qy[i].ContainGZW[j].ContainSettlementPoint[k].calculateASettlementAmount();
					//��������ٶ�
					this->qy[i].ContainGZW[j].ContainSettlementPoint[k].calculateSettlementSpeed();
				}
			}
		}
		//����
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

