

/*��ˮ׼�պ�·�ߣ�����֮�䲻���пո񣬼򵥱պϲ�ķ���*/

#include"szjs.h"
//int main0()
//{
//	//��ȡ�����ļ�
//	string data_path = "D:/½�ϲ�������/ˮ׼�պϲ����/����/XFH-��.txt";//ԭʼ�ļ�·��
//	string save_path = "D:/½�ϲ�������/ˮ׼�պϲ����/����/���/�߳�.txt"; //���ս���ļ�·��
//	vector<cz>cz_information;
//	readFile(data_path, cz_information);
//	//����պϲ�
//	allocationClosureError(save_path, cz_information);
//	return 0;
//}
int main()
{
	Project P(2);
	P.LoadData("D:/ce/ceshi.xlsx");
	P.calculateVariable();
    return 0;
}