// ParkingMeter.cpp : ���� DLL Ӧ�ó���ĵ���������
//

#include "stdafx.h"
#include <atltime.h>
#include "ParkingMeter.h"
#include "SqlDataBase.h"
#include "Log.h"
#include "DoorMgr.h"

using namespace std;
const int g_iNightStartHour = 8;
const int g_iNightEndHour = 18;
CString g_sDbIP;
CString g_sDbName;
CString g_sDbUserID;
CString g_sDbPwd;
// ���ǵ���������һ��ʾ��
PARKINGMETER_API int nParkingMeter=0;
CParkingMeter * g_PMTest = NULL;
const float g_fEps = 0.001;
// ���ǵ���������һ��ʾ����
PARKINGMETER_API int fnParkingMeter(void)
{
	return 42;
}

// �����ѵ�����Ĺ��캯����
// �й��ඨ�����Ϣ������� ParkingMeter.h
CParkingMeter::CParkingMeter()
{
	return;
}


/************************************************************************/
/*���㲢��ʾ����ͣ����Ϣ,�˴����ص�fMoneyΪ�ܽɷѽ���ȥԤ���Ѻ�ʣ��Ľ��*/
/* iCarType ������ 0:��ʱ��A,С�� 1����ʱ��B ��*/
/* pLicenseҪ��ѯ�ĳ���*/
/* iFreeTime Ԥ���Ѻ����ʱ��
/* iMatchMethod 0��������ҳ�ʱΪ��ȫƥ�䣬���������ظ����ƣ�ֻѡ��ID���ĳ��ƣ�Ҳ���Ǻ�����ĳ�����Ҫ������ӿ�ʹ��*/
/*              1:����ģ����ѯ���ƣ�����ͬ���ƺͳ��ƺ����һλ��ȫ�����ҳ�,������˾����֧��ʹ��*/
/*              2:�����ѯ����ʱ������ƥ�䳵���⣬��ƥ��ID,��Ҫ������˾����֧��ʱ���¼������ã��������ظ����Ƶ�����*/
/*iCarOrder Ϊģ����ѯ����ʱ��ѡ��ĳ������*/
/*pSearchIDΪ�ڱ���˾����֧��ʱ�����¼�����ʱ�����������ظ�����,����֧�����¼�����ʱ������*/
/*pSearchInTimeΪԤ��,�ڱ���˾����֧��ʱ�����볡ʱ�䡣����֧�����¼�����ʱ������*/
/*fMoney��ѯ�ĳ���ҪԤ���ѵĽ�� */
/*pInTime��ѯ�ĳ����볡ʱ��*/
/*iParkedTime��ѯ�ĳ���ͣ��ʱ��*/
/*pRtnLicenseģ��ƥ��ʱ��ͨ����ŷ��صĳ���*/
/*pInPic��ѯ������ͼƬ·��*/
/*pID��ѯ������ID*/
/************************************************************************/
RNRESULT CParkingMeter::GetParkedCarInfo(int iCarType, char* pLicense, int iFreeTime, int iMatchMethod, int iCarOrder, char* pSearchID, 
										 char* pSearchInTime, float& fMoney, char* pInTime, int& iParkedTime, char* pRtnLicense, char* pInPic, char* pID)
{
	CString license(pLicense);
	CString sSearchID(pSearchID);
	bool isOutDate = false;
	bool bAsTempCar = false;
	CString strTemp;
	CarInfoIn carInfoIn;
	CarInfo carInfo;
	CString carType = "";
	//��ʾ�볡ʱ��
	CString storeInTime = "";
	CString  stayTime = "";
	CString money = "���";
	float currCarShouldMony = 0.0;
	BOOL bCheckFree = FALSE;
	float fCheckMny = 0.00;
	CTimeSpan tmFreeSpan(0,0,iFreeTime,0);//Ԥ���Ѻ����ʱ��
	int iLicenColor = -1;
	CString endDate ="";
	CString startDate = "";
	CString strConvert = "";
	bool isInDb;
	bool ret;
	vector<CarInfoIn> carInfoList;
	DEBUG_INFO("GetParkedCarInfo start\n");
	DEBUG_INFO("ƥ�䷽ʽ(0:��ȫƥ��,1:ģ��ƥ�䣬2��ID����ͬƥ��)%d\n",iMatchMethod);
	try
	{
		isInDb = DoorMgr::SelectCarInfoByLicense(carInfo, license);

		//���������������Ϣ
		if (isInDb)
		{
			isOutDate = DoorMgr::IsExpire(carInfo.carLicense, carInfo.roomNum, startDate, endDate);
			bAsTempCar = DoorMgr::IsAsTempCar(carInfo.carLicense, carInfo.roomNum);
		}
		if (iMatchMethod == 1)
		{
			ret = DoorMgr::SelectOffLineTempRecordFuzzyMatch(carInfoList, license);
			if (ret)
			{
				if (iCarOrder > carInfoList.size() || iCarOrder < 1)
				{		
					fMoney = 0;
					strcpy(pInTime,"0000-00-00 00:00:00");
					iParkedTime = 0;
					strcpy(pRtnLicense,"");
					strcpy(pInPic,"");
					strcpy(pID,"");
					DEBUG_INFO("����%s�������Ƴ�����ͷ�����!\n",license);
					DEBUG_INFO("GetParkedCarInfo end\n");
					return RESULT_REACH_END_RECORD;
				}
				else
				{
					carInfoIn = carInfoList[iCarOrder - 1];
				}
			}
		}
		else if (iMatchMethod == 0)
		{
			ret = DoorMgr::SelectOffLineTempRecordByLicense(carInfoIn, license);
		}
		else
		{
			ret = DoorMgr::SelectOffLineTempRecordByLicenseAndID(carInfoIn, license, sSearchID);
		}
	}
	catch (_com_error &e)
	{
		fMoney = 0;
		strcpy(pInTime,"0000-00-00 00:00:00");
		iParkedTime = 0;
		strcpy(pRtnLicense,"");
		strcpy(pInPic,"");
		strcpy(pID,"");
		DEBUG_INFO("GetParkedCarInfo%s\n", (char*)(e.Description()));
		DEBUG_INFO("GetParkedCarInfo end\n");
		return RESULT_DB_CONNECT_ERROR;
	}
	DEBUG_INFO("GetParkedCarInfo 2\n");

	//������볡��¼
	if (ret) 
	{
		//��ʾ�볡ʱ��
		storeInTime =  carInfoIn.inOutTime;
		CTime tempStartTime = CommFunc::TimeStrToCTime(storeInTime);
		fCheckMny = carInfoIn.fCheckMny;
		if (iFreeTime == 0)
		{
			bCheckFree = FALSE;
		}
		else if (fCheckMny > 0.00 && carInfoIn.CheckTime != "1900/01/01 00:00:00")//Ԥ����һ��ʱ�������
		{
			CTime tempCheckTime = CommFunc::TimeStrToCTime(carInfoIn.CheckTime);
			if (CTime::GetCurrentTime() - tempCheckTime < tmFreeSpan)
			{
				bCheckFree = TRUE;
			}
		}

		stayTime = CommFunc::TimeSpaceToNow(tempStartTime);

		storeInTime.Replace("/", "-");
		if (isOutDate)
		{
			DEBUG_INFO("���㳵����%s��Ϊ���ڴ���,ͣ��ʱ��Ϊ %s\n",license, stayTime);     
		}
		else if (bAsTempCar)
		{
			DEBUG_INFO("���㳵����%s�̶�����Ϊ��ʱ������,ͣ��ʱ��Ϊ %s\n",license, stayTime);     
		}
		else
		{
		}
		if (!isInDb || isOutDate || carInfo.sort == "2" || bAsTempCar)
		{
			if(!GetFeeParameter())
			{
				DEBUG_INFO("GetParkedCarInfo end\n");
				return RESULT_GET_FEE_PARAMETER_ERROR;
			}
			if (!SetFeeTypeMode())
			{
				DEBUG_INFO("GetParkedCarInfo end\n");
				return RESULT_NO_MORE_HOUR_FEE_TYPE_SET;
			}
			DEBUG_INFO("GetParkedCarInfo CalcMoney start\n");
			currCarShouldMony = CalcMoney(false, tempStartTime, iCarType, money, carInfo.sort, bCheckFree, fCheckMny);
			DEBUG_INFO("GetParkedCarInfo CalcMoney end\n");
			strcpy(pInTime,storeInTime);
			iParkedTime = atoi(stayTime);
			fMoney = atof(money);
			fMoney = fMoney - carInfoIn.fCheckMny;
			strConvert.Format("%.2f",fMoney);//ת��������������
			fMoney = atof(strConvert);
			strcpy(pRtnLicense,carInfoIn.carLicense);
			strcpy(pInPic,carInfoIn.picPath);
			strcpy(pID,carInfoIn.ID);
			DEBUG_INFO("���㳵����%s���%sԪ�볡ʱ��%sͣ��ʱ��Ϊ %s����ͼƬ·��%s���ݿ�ID%s\n",pRtnLicense,strConvert,storeInTime,stayTime,pInPic,pID);
			if (fMoney < 0)
			{
				fMoney = 0;
			}
		}
		else
		{
			fMoney = 0;
			strcpy(pInTime,storeInTime);
			iParkedTime = atoi(stayTime);
			strcpy(pRtnLicense,carInfoIn.carLicense);
			strcpy(pInPic,carInfoIn.picPath);
			strcpy(pID,"");
		}
	}
	else
	{
		fMoney = 0;
		strcpy(pInTime,"0000-00-00 00:00:00");
		iParkedTime = 0;
		strcpy(pRtnLicense,"");
		strcpy(pInPic,"");
		strcpy(pID,"");
		DEBUG_INFO("δ�ҵ���Ӧ�볡������%s ����Ϣ\n",license); 
		DEBUG_INFO("GetParkedCarInfo end\n");
		return RESULT_NO_IN_RECORD;
	}
	DEBUG_INFO("GetParkedCarInfo end\n");
	return RESULT_SUCCESS;
}


/************************************************************************/
/*���㲢��ʾ����ͣ����Ϣ,�˴����ص�fMoneyΪ�ܽɷѽ���ȥԤ���Ѻ�ʣ��Ľ��*/
/* iCarType ������ 0:��ʱ��A,С�� 1����ʱ��B ��*/
/* pLicenseҪ��ѯ�ĳ���*/
/*pPayTime֧����ʱ��*/
/*fMoney֧���Ľ��*/
/*pSearchIDΪ�ڱ���˾����֧��ʱ�����¼�����ʱ�����������ظ�����,����֧�����¼�����ʱ������*/
/*pSearchInTimeΪԤ��,�ڱ���˾����֧��ʱ�����볡ʱ�䡣����֧�����¼�����ʱ������*/
/*iPayMethodΪ����Ԥ֧����0������Ԥ֧��;1������˾����֧��*/
/************************************************************************/
RNRESULT CParkingMeter::PayParkCarFee(int iCarType, char * pCarLicense, char * pPayTime, float fMoney, char* pSearchID, char* pSearchInTime, int iPayMethod)
{
	CString sCarLicense(pCarLicense);
	CString sPayTime(pPayTime);
	CString sSearchID(pSearchID);
	CString sSearchInTime(pSearchInTime);
	CString sMoney;
	char pRtnInTime[100];
	char pRtnLicense[100];
	char pRtnInPic[200];
	char pRtnID[100];
	int iParkedTm;
	float fRemainMoney;//����Ӧ֧����Ǯ
	bool ret; 
	RNRESULT rtn;
	sMoney.Format("%.2f", fMoney);
	DEBUG_INFO("**%s��ʼ֧��%sԪ,��ǰ֧��ʱ��%s,SearchID:%s, SearchInTime:%s, iPayMethod(0:����,1:����):%d\n", 
		       sCarLicense, sMoney,sPayTime,sSearchID, sSearchInTime, iPayMethod);
	CarInfoIn carInfoIn;
	try
	{
		if (iPayMethod == 0)//����
		{
			ret = DoorMgr::SelectOffLineTempRecordByLicense(carInfoIn, sCarLicense);
		}
		else//����
		{
			ret = DoorMgr::SelectOffLineTempRecordByLicenseAndID(carInfoIn, sCarLicense, sSearchID);
		}
	}
	catch (_com_error &e)
	{
		DEBUG_INFO("PayParkCarFee SelectOffLineTempRecordByLicense %s\n", (char*)(e.Description()));
		return RESULT_DB_CONNECT_ERROR;
	}
	if (ret) 
	{
		COleDateTime tempPreCheckTime =  CommFunc::TimeStrToCOleDateTime(carInfoIn.CheckTime);
		COleDateTime tempCurrCheckTime = CommFunc::TimeStrToCOleDateTime(sPayTime);
		if (tempCurrCheckTime <= tempPreCheckTime)
		{
			DEBUG_INFO("%s֧��ʱ�䲻�����ϴ�֧��ʱ��%s,�˴�δ�ۼ�д��֧�����\n", sCarLicense,carInfoIn.CheckTime);
			return RESULT_REPEAT_NOTICE_FEE;
		}
	}
	if (iPayMethod == 0)//����
	{
		rtn = GetParkedCarInfo(iCarType, pCarLicense, 0, 0, 1, pSearchID, pSearchInTime, fRemainMoney, pRtnInTime, iParkedTm, 
			pRtnLicense, pRtnInPic, pRtnID);
	}
	else//����
	{
		rtn = GetParkedCarInfo(iCarType, pCarLicense, 0, 2, 1, pSearchID, pSearchInTime, fRemainMoney, pRtnInTime, iParkedTm, 
			pRtnLicense, pRtnInPic, pRtnID);
	}
	if (rtn != RESULT_SUCCESS)
	{
		DEBUG_INFO("%s�����ܽ�����\n",sCarLicense);
		return rtn;
	}
	if (fMoney < fRemainMoney + g_fEps)
	{
		try
		{
			if (iPayMethod == 0)//����
			{
				ret = DoorMgr::UpdateParkedFee(sCarLicense, sPayTime, fMoney);
			}
			else//����
			{
				ret = DoorMgr::UpdateParkedFeeAndMatchID(sCarLicense, sPayTime, fMoney, sSearchID);
			}
			if (ret)
			{
				DEBUG_INFO("**%s���֧��%sԪ\n", sCarLicense, sMoney);
			}
			else
			{
				DEBUG_INFO("**%s֧��%sԪδ�ɹ����޼�¼\n", sCarLicense, sMoney);
				return RESULT_NO_IN_RECORD;
			}
		}
		catch (_com_error &e)
		{
			DEBUG_INFO("PayParkCarFee UpdateParkedFee %s\n", (char*)(e.Description()));
			return RESULT_DB_CONNECT_ERROR;
		}
	}
	else
	{
		DEBUG_INFO("%s֧��������Ӧ�ɷѶ�,�˴�δ�ۼ�д��֧�����\n", sCarLicense);
		return RESULT_REPEAT_NOTICE_FEE;
	}
	return RESULT_SUCCESS;
}


RNRESULT CParkingMeter::GetLeftParkInfo(vector<LeftParkInfo>& aLeftParkInfo)
{
	aLeftParkInfo.clear();
	try
	{
		int ret = DoorMgr::SelectLeftParkInfo(aLeftParkInfo);
	}
	catch (_com_error &e)
	{
		return RESULT_DB_CONNECT_ERROR;
	}
	return RESULT_SUCCESS;
}


//�õ���λ��Ϣ
/************************************************************************/
/* pCarLicenseͨ���ó��ƺŲ鳵λ��
/*aCarportInfo:�ɳ��ƺŵõ�����Ӧ�����г�λ��Ϣ
/************************************************************************/

RNRESULT CParkingMeter::GetCarportInfo(vector<ST_VIEWCARPORTROOMINFO>& aCarportInfo, char * pCarLicense)
{
	CString sCarLicense(pCarLicense);
	aCarportInfo.clear();
	vector<CString> aRoomNum;
	try
	{
		DoorMgr::GetRoomNumWithCarLicense(aRoomNum, sCarLicense);
		if (aRoomNum.size() > 0)
		{
			DoorMgr::GetViewCarportRoom(aCarportInfo, aRoomNum[0]);
		}
	}
	catch (_com_error &e)
	{
		return RESULT_DB_CONNECT_ERROR;
	}
	return RESULT_SUCCESS;
}


//����Ҫ��ֵ��λ�ĸ����ܶ�
/************************************************************************/
/* pCarLicenseͨ���ó��ƺŲ鳵λ��
/*pCarportNum:app�ϳ�λ����������ѡ��ĵ�ǰ��λ
/*aCarportNum����������app��λ�����������еĳ�λ
/*iMonthCnt��Ҫ��ֵ������
/*bPayAllCarport:�ǶԷ����µĵ�ǰ��λ��ֵ���ǶԷ����µ����г�λ��ֵ��trueΪ���г�λ��falseֻΪ��ǰѡ��ĳ�λ
/*aCarportInfo:���ص�Ҫ��ֵ�ĳ�λ����bPayAllCarportΪtrue,Ϊ���з����µĳ�λ������Ϊapp�ϳ�λ����������ѡ��ĵ�ǰ��λ
/*fMoney���¼���������֤�ĳ�ֵ���
*/
/************************************************************************/
RNRESULT CParkingMeter::GetCarportTotalRent(char * pCarLicense, char * pCarportNum, vector<CString> & aCarportNum, int iMonthCnt, bool bPayAllCarport,vector<ST_VIEWCARPORTROOMINFO>& aCarportInfo, float & fMoney)
{
	ST_VIEWCARPORTROOMINFO stuCarportInfo;
	CString sCarportNum(pCarportNum);
	fMoney = 0;
	int i, j;
	int iRtn;
	bool bFind;
	int iCount = 0;
// 	for (i = 0; i < aCarportNum.size(); i++)
// 	{
// 		DEBUG_INFO("aCarportNum %s\n", aCarportNum[i]);
// 	}
	iRtn = GetCarportInfo(aCarportInfo, pCarLicense);
	if (iRtn != RESULT_SUCCESS)
	{
		return iRtn;
	}
	if (aCarportInfo.size() > 0)
	{
		if (bPayAllCarport)
		{
			for (i = 0; i < aCarportNum.size(); i++)
			{
				for (j = 0; j < aCarportInfo.size(); j++)
				{
					if (aCarportNum[i] == aCarportInfo[j].sCarportNum)
					{
						fMoney = fMoney + aCarportInfo[j].fRentMoney * iMonthCnt;
						iCount++;
					}
				}
			}
			if (aCarportInfo.size() != iCount)
			{
				aCarportInfo.clear();
				DEBUG_INFO("��ǰ��ֵ��λ�����ݿ��еĲ�ƥ��\n");
			}
		}
		else
		{
			bFind = false;
			for (i = 0; i < aCarportInfo.size(); i++)
			{
				if (aCarportInfo[i].sCarportNum == sCarportNum)
				{
					stuCarportInfo = aCarportInfo[i];
					fMoney = fMoney + aCarportInfo[i].fRentMoney * iMonthCnt;
					bFind = true;
					break;
				}
			}
			if (bFind)
			{
				aCarportInfo.clear();
				aCarportInfo.push_back(stuCarportInfo);
			}
			else
			{
				aCarportInfo.clear();
				DEBUG_INFO("��ǰ��ֵ��λ�����ݿ��еĲ�ƥ��\n");
			}
		}
	}
	else
	{
		fMoney = 0;
		DEBUG_INFO("�޳�λ��¼!\n");
		return RESULT_NO_IN_RECORD;
	}
	DEBUG_INFO("��Ӧ��ֵ��%.2fԪ\n", fMoney);
	return RESULT_SUCCESS;
}

//���³���ֵ������Ϣ
RNRESULT CParkingMeter::UpdateCarportRentInfo(ST_VIEWCARPORTROOMINFO stuCarportInfo, CString sPayTime, int iMonthCnt)
{
	BOOL bIsRes;
	BOOL bIsOperateReport;
	CString sData;
	CString sEndData;
	CString strTemp;
	COleDateTime m_cNowStartDateBak;
	ST_OPERATEREPORT St_OperateReport;
	St_OperateReport.sRoomNum=stuCarportInfo.sRoomNum;
	St_OperateReport.sOperateDate = sPayTime;
	St_OperateReport.nOperateType = DELAY_OPERATOR_TYPE;
	St_OperateReport.sOperator = "App";
	St_OperateReport.sChargeCompany = "";
	St_OperateReport.nCheckMode = NET_CHECK_MODE;
	bIsRes = DoorMgr::UpDateCarportInfoDelayData(stuCarportInfo.sCarportNum, stuCarportInfo.sEndDate, iMonthCnt);
	St_OperateReport.sCarportNum=stuCarportInfo.sCarportNum;
	St_OperateReport.sOriginalStartDate = stuCarportInfo.sStartDate;
	CTime tmLastStartDate = CommFunc::TimeStrToCTime(stuCarportInfo.sEndDate) + CTimeSpan(0,0,0,1);
	St_OperateReport.sLastStartDate = tmLastStartDate.Format("%Y-%m-%d %H:%M:%S");
	St_OperateReport.sOriginalEndDate = stuCarportInfo.sEndDate;
	St_OperateReport.sLastEndDate = CommFunc::TimeStrAddMonth(stuCarportInfo.sEndDate,iMonthCnt);
	St_OperateReport.fDeposit=stuCarportInfo.fDeposit;
	St_OperateReport.fOperateMoney = stuCarportInfo.fRentMoney * iMonthCnt;

	bIsOperateReport = DoorMgr::InsertOperateReportInfo(St_OperateReport);
	if (bIsRes && bIsOperateReport)
	{
		DEBUG_INFO("**%s��λ��ֵ�ɹ�%.2fԪ!\n",stuCarportInfo.sCarportNum, stuCarportInfo.fRentMoney * iMonthCnt);
		return RESULT_SUCCESS;
	}
	else
	{
		DEBUG_INFO("**%s��λ��ֵʧ��!\n",stuCarportInfo.sCarportNum);
		return RESULT_DB_CONNECT_ERROR;
	}
}

//���³���ֵ
/************************************************************************/
/* pCarLicenseͨ���ó��ƺŲ鳵λ��
/*pRoomNum���ҵĳ��ƺŶ�Ӧ�ķ���
/*pCarportNum:app�ϳ�λ����������ѡ��ĵ�ǰ��λ
/*aCarportNum����������app��λ�����������еĳ�λ
/*pPayTime����ֵ��ʱ��
/*iMonthCnt��Ҫ��ֵ������
/*fMoney:Ҫ��ֵ��λ���ܽ��
/*bPayAllCarport:�ǶԷ����µĵ�ǰ��λ��ֵ���ǶԷ����µ����г�λ��ֵ��trueΪ���г�λ��falseֻΪ��ǰѡ��ĳ�λ
/************************************************************************/

RNRESULT CParkingMeter::PayCarportRent(char * pCarLicense, char* pRoomNum, char* pCarportNum, vector<CString> & aCarportNum, char * pPayTime, int iMonthCnt, float fMoney, bool bPayAllCarport)
{
	CString sCarLicense(pCarLicense);
	CString sPayTime(pPayTime);
	CString sRoomNum(pRoomNum);
	CString sCarportNum(pCarportNum);
	CString sMoney;
	float fTotalMoney = 0;
	ST_OPERATEREPORT St_OperateReport;
	bool ret; 
	int i;
	RNRESULT rtn;
	vector<ST_VIEWCARPORTROOMINFO> aCarportInfo;//Ҫ��ֵ�ĳ�λ��Ϣ��¼
	sMoney.Format("%.2f", fMoney);
	DEBUG_INFO("**%s���%s��ǰ��λ��%s��ʼ���³�ֵ%sԪ,��ǰ��ֵʱ��%s,��ֵ����:%d, ��ֵ��ʽ%s\n", 
		sCarLicense, sRoomNum, sCarportNum, sMoney,sPayTime,iMonthCnt,bPayAllCarport ? "���г�λ":"����λ");
	try
	{
		ret = DoorMgr::GetOperateReport(St_OperateReport, sRoomNum, sCarportNum, DELAY_OPERATOR_TYPE, NET_CHECK_MODE);
	}
	catch (_com_error &e)
	{
		DEBUG_INFO("PayCarportRent GetOperateReport %s\n", (char*)(e.Description()));
		return RESULT_DB_CONNECT_ERROR;
	}
	if (ret) 
	{
		COleDateTime tempPreCheckTime =  CommFunc::TimeStrToCOleDateTime(St_OperateReport.sOperateDate);
		COleDateTime tempCurrCheckTime = CommFunc::TimeStrToCOleDateTime(sPayTime);
		if (tempCurrCheckTime <= tempPreCheckTime)
		{
			DEBUG_INFO("%s��ֵʱ�䲻�����ϴγ�ֵʱ��%s,�˴�δ�ۼ�д���ֵ���\n", sCarLicense,St_OperateReport.sOperateDate);
			return RESULT_REPEAT_NOTICE_FEE;
		}
	}
	rtn = GetCarportTotalRent(pCarLicense, pCarportNum, aCarportNum, iMonthCnt, bPayAllCarport, aCarportInfo, fTotalMoney);
	if (rtn != RESULT_SUCCESS)
	{
		DEBUG_INFO("%s�����ܽ�����\n",sCarLicense);
		return rtn;
	}
	if (fabs(fMoney - fTotalMoney) < g_fEps)
	{
		try
		{
			for (i = 0; i  < aCarportInfo.size(); i++)
			{
				rtn = UpdateCarportRentInfo(aCarportInfo[i], sPayTime, iMonthCnt);
				if (rtn != RESULT_SUCCESS)
				{
					return RESULT_DB_CONNECT_ERROR_OR_NO_IN_RECORD;
				}
			}
		}
		catch (_com_error &e)
		{
			DEBUG_INFO("PayCarportRent UpdateCarportRentInfo %s\n", (char*)(e.Description()));
			return RESULT_DB_CONNECT_ERROR;
		}
	}
	else
	{
		DEBUG_INFO("%s��ֵ�������Ӧ��ֵ���,�˴�δд���ֵ���\n", sCarLicense);
		return RESULT_PAY_RENT_NO_EQUAL;
	}
	return RESULT_SUCCESS;
}

JNIEXPORT jint JNICALL Java_xd_dl_job_ParkNative_initialized
(JNIEnv *env, jclass, jstring sDbIP, jstring sDbName, jstring sDbUserID, jstring sDbPwd)
{
	char * pDbIP;
	char * pDbName;
	char * pDbUserID;
	char * pDbPwd;
	InitializeCriticalSection((LPCRITICAL_SECTION)&gLogMutex);
	DEBUG_INFO("Java_xd_dl_job_ParkNative_initialized Start!\n");
	pDbIP = jstringToWindows(env, sDbIP);
	pDbName = jstringToWindows(env, sDbName);	
	pDbUserID = jstringToWindows(env, sDbUserID);
	pDbPwd = jstringToWindows(env, sDbPwd);
	if (g_PMTest == NULL)
	{
		g_PMTest=new CParkingMeter();
	}
	RNRESULT rtn = g_PMTest->InitializeEnv( pDbIP,pDbName,pDbUserID,pDbPwd );
	free(pDbIP);
	free(pDbName);
	free(pDbUserID);
	free(pDbPwd);
	DEBUG_INFO("Java_xd_dl_job_ParkNative_initialized End!\n");
	return rtn;
}

JNIEXPORT jint JNICALL Java_xd_dl_job_ParkNative_unitialized
(JNIEnv *, jclass)
{
	DEBUG_INFO("Java_xd_dl_job_ParkNative_Unitialized Start!\n");
	if (g_PMTest != NULL)
	{
		delete g_PMTest;
		g_PMTest = NULL;
	}
	DEBUG_INFO("Java_xd_dl_job_ParkNative_Unitialized End!\n");
	DeleteCriticalSection((LPCRITICAL_SECTION)&gLogMutex);
	return RESULT_SUCCESS;
}

//iCarOrderΪ���Ƴ����еĵڼ�������1��ʼ
JNIEXPORT jobject JNICALL Java_xd_dl_job_ParkNative_getParkedCarInfo
(JNIEnv *env, jclass,jint iCarType, jstring sCarLicense, jint iFreeTime, jint iMatchMethod, jint iCarOrder, jstring sSearchID, jstring sSearchInTime)
{
	char pInTime[100];
	char pRtnLicense[100];
	char pInPic[200];
	char pID[100];
	int iParkedTm;
	float fMoney;
	jstring sInTime;
	jstring sRtnLicense;
	jstring sInPic;
	jstring sID;
	char *pCarLicense = jstringToWindows(env, sCarLicense);
	char *pSearchID = jstringToWindows(env, sSearchID);
	char *pSearchInTime = jstringToWindows(env, sSearchInTime);
	RNRESULT rtn = g_PMTest->GetParkedCarInfo(iCarType, pCarLicense, iFreeTime, iMatchMethod, iCarOrder, pSearchID, pSearchInTime, 
		           fMoney, pInTime, iParkedTm, pRtnLicense,pInPic,pID);
	sInTime = WindowsTojstring(env,pInTime);
	sRtnLicense = WindowsTojstring(env,pRtnLicense);
	sInPic = WindowsTojstring(env,pInPic);
	sID = WindowsTojstring(env,pID);
	free(pCarLicense);
	free(pSearchID);
	free(pSearchInTime);
	/**//* ����Ϊ��ȡ��Java�ж�Ӧ��ʵ�����еı���*/
	//��ȡJava�е�ʵ����
	jclass objcls = (env)->FindClass("xd/dl/job/ParkedCarInfo");
	jobject obj =env->AllocObject(objcls);
	//��ȡ����ÿһ�������Ķ���
	//����
	jfieldID str = (env)->GetFieldID(objcls,"sInTime","Ljava/lang/String;");
	jfieldID strCarLicense = (env)->GetFieldID(objcls,"sCarLicense","Ljava/lang/String;");
	jfieldID strInPic = (env)->GetFieldID(objcls,"sInPic","Ljava/lang/String;");
	jfieldID strID = (env)->GetFieldID(objcls,"sID","Ljava/lang/String;");
	jfieldID iParkedTimeVal = (env)->GetFieldID(objcls,"iParkedTime","I");
	jfieldID iReturnVal = (env)->GetFieldID(objcls,"iReturn","I");
	jfieldID fMoneyVal = (env)->GetFieldID(objcls,"fMoney","F");
	//��ÿһ��ʵ���ı�����ֵ
	(env)->SetObjectField(obj,str,sInTime);
	(env)->SetObjectField(obj,strCarLicense,sRtnLicense);
	(env)->SetObjectField(obj,strInPic,sInPic);
	(env)->SetObjectField(obj,strID,sID);
	(env)->SetIntField(obj,iParkedTimeVal,iParkedTm);
	(env)->SetIntField(obj,iReturnVal,rtn);
	(env)->SetFloatField(obj,fMoneyVal,fMoney);
	return obj;
}


JNIEXPORT jint JNICALL Java_xd_dl_job_ParkNative_payParkCarFee
(JNIEnv *env, jclass, jint iCarType, jstring sCarLicense, jstring sPayTime, jfloat fMoney, jstring sID, jstring sInTime, jint iPayMethod)
{
	char *pCarLicense = jstringToWindows(env, sCarLicense);
	char *pPayTime = jstringToWindows(env, sPayTime);
	char *pID = jstringToWindows(env, sID);
	char *pInTime = jstringToWindows(env, sInTime);
	RNRESULT rtn = g_PMTest->PayParkCarFee(iCarType, pCarLicense, pPayTime, fMoney, pID, pInTime, iPayMethod);
	free(pCarLicense);
	free(pPayTime);
	free(pID);
	free(pInTime);
	return rtn;
}

//��ѯʣ�೵λ
JNIEXPORT jobjectArray JNICALL Java_xd_dl_job_ParkNative_getLeftParkInfo(JNIEnv *env, jclass)
{
	vector<LeftParkInfo> aLeftParkInfo;
	jstring sParkName;
	DEBUG_INFO("��ѯʣ�೵λ��ʼ\n");
	//����һ��object���� 
	jobjectArray args = 0;
	//��ȡobject������,һ��Ϊjava/lang/Object�Ϳ�����
	jclass objClass = (env)->FindClass("java/lang/Object");
	//�½�object����
	RNRESULT iRtn = g_PMTest->GetLeftParkInfo(aLeftParkInfo);
	if (iRtn == RESULT_DB_CONNECT_ERROR)
	{
		args = (env)->NewObjectArray(aLeftParkInfo.size(), objClass, 0);
		return args;
	}
	args = (env)->NewObjectArray(aLeftParkInfo.size(), objClass, 0);
	if (aLeftParkInfo.size() == 0)
	{
		DEBUG_INFO("��ѯʣ�೵λ��������λ��¼��Ϊ0\n");
		return args;
	}
//	DEBUG_INFO("��ѯʣ�೵λ��¼��Ϊ%d\n",aLeftParkInfo.size());
	/**//* ����Ϊ��ȡ��Java�ж�Ӧ��ʵ�����еı���*/
	//��ȡJava�е�ʵ����
	jclass objcls = (env)->FindClass("xd/dl/job/LeftParkInfo");
	//��ȡ����ÿһ�������Ķ���
	//����
	jfieldID strParkName = (env)->GetFieldID(objcls,"sParkName","Ljava/lang/String;");
	jfieldID iSumNum = (env)->GetFieldID(objcls,"iSumNum","I");
	jfieldID iLeftNum = (env)->GetFieldID(objcls,"iLeftNum","I");
	jfieldID iCarProp = (env)->GetFieldID(objcls,"iCarProp","I");
	//��ÿһ��ʵ���ı�����ֵ
	for (int i = 0; i < aLeftParkInfo.size(); i++)
	{
		sParkName= WindowsTojstring(env,aLeftParkInfo[i].sParkName.GetBuffer());
		jobject obj =env->AllocObject(objcls);
		(env)->SetObjectField(obj,strParkName,sParkName);
		(env)->SetIntField(obj,iSumNum,aLeftParkInfo[i].iSumNum);
		(env)->SetIntField(obj,iLeftNum,aLeftParkInfo[i].iLeftNum);
		(env)->SetIntField(obj,iCarProp,aLeftParkInfo[i].iCarProp);
		(env)->SetObjectArrayElement(args, i, obj);
	}
	DEBUG_INFO("��ѯʣ�೵λ����\n");
	return args;
}

//ͨ�����Ƶõ���źͳ�λ������Ϣ
JNIEXPORT jobjectArray JNICALL Java_xd_dl_job_ParkNative_getCarportInfo(JNIEnv *env, jclass, jstring sCarLicense)
{

	vector<ST_VIEWCARPORTROOMINFO> aCarportInfo;
	CString strParkName;
	jstring sTemp;
	char *pCarLicense = jstringToWindows(env, sCarLicense);
	DEBUG_INFO("��ѯ%s����Ӧ��λ��Ϣ��ʼ\n",pCarLicense);
	//����һ��object���� 
	jobjectArray args = 0;
	//��ȡobject������,һ��Ϊjava/lang/Object�Ϳ�����
	jclass objClass = (env)->FindClass("java/lang/Object");
	//�½�object����
	RNRESULT iRtn = g_PMTest->GetCarportInfo(aCarportInfo, pCarLicense);
	for (int i = 0; i < aCarportInfo.size(); i++)
	{
		aCarportInfo[i].sStartDate.Replace("/", "-");
		aCarportInfo[i].sEndDate.Replace("/", "-");
	}
	free(pCarLicense);
	if (iRtn == RESULT_DB_CONNECT_ERROR)
	{
		args = (env)->NewObjectArray(aCarportInfo.size(), objClass, 0);
		return args;
	}
	args = (env)->NewObjectArray(aCarportInfo.size(), objClass, 0);
	if (aCarportInfo.size() == 0)
	{
		DEBUG_INFO("��ѯ��λ��Ϣ��������Ӧ��λ��¼��Ϊ0\n");
		return args;
	}
	DEBUG_INFO("��ѯ��λ��Ϣ��¼��Ϊ%d\n",aCarportInfo.size());
	/**//* ����Ϊ��ȡ��Java�ж�Ӧ��ʵ�����еı���*/
	//��ȡJava�е�ʵ����
	jclass objcls = (env)->FindClass("xd/dl/job/ViewCarportRoomInfo");
	//��ȡ����ÿһ�������Ķ���
	//����
	jfieldID sCarportNum = (env)->GetFieldID(objcls,"sCarportNum","Ljava/lang/String;");
	jfieldID sRoomNum = (env)->GetFieldID(objcls,"sRoomNum","Ljava/lang/String;");
	jfieldID sName = (env)->GetFieldID(objcls,"sName","Ljava/lang/String;");
	jfieldID sAddress = (env)->GetFieldID(objcls,"sAddress","Ljava/lang/String;");
	jfieldID sPhoneNumber = (env)->GetFieldID(objcls,"sPhoneNumber","Ljava/lang/String;");
	jfieldID sPosition = (env)->GetFieldID(objcls,"sPosition","Ljava/lang/String;");
	jfieldID sStartDate = (env)->GetFieldID(objcls,"sStartDate","Ljava/lang/String;");
	jfieldID sEndDate = (env)->GetFieldID(objcls,"sEndDate","Ljava/lang/String;");
	jfieldID fDeposit = (env)->GetFieldID(objcls,"fDeposit","F");
	jfieldID bTemporary = (env)->GetFieldID(objcls,"bTemporary","Z");
	jfieldID sRemark = (env)->GetFieldID(objcls,"sRemark","Ljava/lang/String;");
	jfieldID sRentName = (env)->GetFieldID(objcls,"sRentName","Ljava/lang/String;");
	jfieldID fRentMoney = (env)->GetFieldID(objcls,"fRentMoney","F");
	jfieldID sParkName = (env)->GetFieldID(objcls,"sParkName","Ljava/lang/String;");

	if (aCarportInfo.size() > 0)
	{
		vector<LeftParkInfo> aLeftParkInfo;
		iRtn = g_PMTest->GetLeftParkInfo(aLeftParkInfo);
		if (iRtn == RESULT_SUCCESS && aLeftParkInfo.size() > 0)
		{
			bool bFind = false;
			for (int i = 0; i < aLeftParkInfo.size(); i++)
			{
				if (aLeftParkInfo[i].iCarProp == 2)//��ͣ����
				{
					strParkName = aLeftParkInfo[i].sParkName;
					bFind = true;
					break;
				}
			}
			if (!bFind)
			{
				strParkName = aLeftParkInfo[0].sParkName;
			}
		}
	}
	
	//��ÿһ��ʵ���ı�����ֵ
	for (int i = 0; i < aCarportInfo.size(); i++)
	{
		jobject obj =env->AllocObject(objcls);
		sTemp= WindowsTojstring(env,aCarportInfo[i].sCarportNum.GetBuffer());
		(env)->SetObjectField(obj,sCarportNum,sTemp);
		sTemp= WindowsTojstring(env,aCarportInfo[i].sRoomNum.GetBuffer());
		(env)->SetObjectField(obj,sRoomNum,sTemp);
		sTemp= WindowsTojstring(env,aCarportInfo[i].sName.GetBuffer());
		(env)->SetObjectField(obj,sName,sTemp);
		sTemp= WindowsTojstring(env,aCarportInfo[i].sAddress.GetBuffer());
		(env)->SetObjectField(obj,sAddress,sTemp);
		sTemp= WindowsTojstring(env,aCarportInfo[i].sPhoneNumber.GetBuffer());
		(env)->SetObjectField(obj,sPhoneNumber,sTemp);
		sTemp= WindowsTojstring(env,aCarportInfo[i].sPosition.GetBuffer());
		(env)->SetObjectField(obj,sPosition,sTemp);
		sTemp= WindowsTojstring(env,aCarportInfo[i].sStartDate.GetBuffer());
		(env)->SetObjectField(obj,sStartDate,sTemp);
		sTemp= WindowsTojstring(env,aCarportInfo[i].sEndDate.GetBuffer());
		(env)->SetObjectField(obj,sEndDate,sTemp);
		(env)->SetFloatField(obj,fDeposit,aCarportInfo[i].fDeposit);
		(env)->SetBooleanField(obj,bTemporary,aCarportInfo[i].bTemporary);
		sTemp= WindowsTojstring(env,aCarportInfo[i].sRemark.GetBuffer());
		(env)->SetObjectField(obj,sRemark,sTemp);
		sTemp= WindowsTojstring(env,aCarportInfo[i].sRentName.GetBuffer());
		(env)->SetObjectField(obj,sRentName,sTemp);
		(env)->SetFloatField(obj,fRentMoney,aCarportInfo[i].fRentMoney);
		sTemp= WindowsTojstring(env,strParkName.GetBuffer());
		(env)->SetObjectField(obj,sParkName,sTemp);
		(env)->SetObjectArrayElement(args, i, obj);
	}
	DEBUG_INFO("��ѯ��λ��Ϣ����\n");
	return args;
}

/************************************************************************/
/* �����³���ֵ                                                                     */
/************************************************************************/
JNIEXPORT jint JNICALL Java_xd_dl_job_ParkNative_payCarportRent
(JNIEnv *env, jclass, jstring sCarLicense, jstring sRoomNum, jstring sCarportNum, jobjectArray aCarportNum, jstring sPayTime, jint iMonthCnt, jfloat fMoney, jboolean bPayAllCarport)
{
	char *pCarLicense = jstringToWindows(env, sCarLicense);
	char *pRoomNum = jstringToWindows(env, sRoomNum);
	char *pCarportNum = jstringToWindows(env, sCarportNum);
	char *pPayTime = jstringToWindows(env, sPayTime);
	vector<CString> vecCarportNum;
	char *pTemp;
	int size=env->GetArrayLength(aCarportNum);//�õ�����ĳ���ֵ
	int i=0;
	for(i=0;i<size;i++)
	{
		jstring jsTemp = (jstring)env->GetObjectArrayElement(aCarportNum,i);
		pTemp = jstringToWindows(env, jsTemp);
		CString strTemp(pTemp);
		vecCarportNum.push_back(strTemp);
	}
	RNRESULT rtn = g_PMTest->PayCarportRent(pCarLicense, pRoomNum, pCarportNum, vecCarportNum, pPayTime, iMonthCnt, fMoney, bPayAllCarport);
	free(pCarLicense);
	free(pPayTime);
	free(pRoomNum);
	free(pCarportNum);
	return rtn;
}

//��jstring����ת����windows����
char* jstringToWindows( JNIEnv  *env, jstring jstr )
{
	int length = (env)->GetStringLength(jstr );
	const jchar* jcstr = (env)->GetStringChars(jstr, 0 );
	char* rtn = (char*)malloc( length*2+1 );
	int size = 0;
	size = WideCharToMultiByte( CP_ACP, 0, (LPCWSTR)jcstr, length, rtn,(length*2+1), NULL, NULL );
	if( size <= 0 )
		return NULL;
	(env)->ReleaseStringChars(jstr, jcstr );
	rtn[size] = 0;
	return rtn;
}
//��windows����ת����jstring����
jstring WindowsTojstring( JNIEnv* env, char* str )
{
	jstring rtn = 0;
	int slen = strlen(str);
	unsigned short * buffer = 0;
	if( slen == 0 )
		rtn = (env)->NewStringUTF(str ); 
	else
	{
		int length = MultiByteToWideChar( CP_ACP, 0, (LPCSTR)str, slen, NULL, 0 );
		buffer = (unsigned short *)malloc( length*2 + 1 );
		if( MultiByteToWideChar( CP_ACP, 0, (LPCSTR)str, slen, (LPWSTR)buffer, length ) >0 )
			rtn = (env)->NewString(  (jchar*)buffer, length );
	}
	if( buffer )
		free( buffer );
	return rtn;
}
