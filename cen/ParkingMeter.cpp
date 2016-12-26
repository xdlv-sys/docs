// ParkingMeter.cpp : 定义 DLL 应用程序的导出函数。
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
// 这是导出变量的一个示例
PARKINGMETER_API int nParkingMeter=0;
CParkingMeter * g_PMTest = NULL;
const float g_fEps = 0.001;
// 这是导出函数的一个示例。
PARKINGMETER_API int fnParkingMeter(void)
{
	return 42;
}

// 这是已导出类的构造函数。
// 有关类定义的信息，请参阅 ParkingMeter.h
CParkingMeter::CParkingMeter()
{
	return;
}


/************************************************************************/
/*计算并显示车辆停场信息,此处返回的fMoney为总缴费金额减去预付费后剩余的金额*/
/* iCarType 车类型 0:临时车A,小车 1：临时车B 大车*/
/* pLicense要查询的车牌*/
/* iFreeTime 预付费后免费时间
/* iMatchMethod 0：代表查找车时为完全匹配，并且如有重复车牌，只选择ID最大的车牌，也就是后进场的车，主要给东汇接口使用*/
/*              1:代表模糊查询车牌，将相同车牌和车牌号相差一位的全部查找出,给本公司网上支付使用*/
/*              2:代表查询车牌时，除了匹配车牌外，还匹配ID,主要给本公司网上支付时重新计算金额用，以区分重复车牌的问题*/
/*iCarOrder 为模糊查询车牌时，选择的车牌序号*/
/*pSearchID为在本公司网上支付时，重新计算金额时，用来区分重复车牌,东汇支付重新计算金额时，传空*/
/*pSearchInTime为预留,在本公司网上支付时，传入场时间。东汇支付重新计算金额时，传空*/
/*fMoney查询的车辆要预交费的金额 */
/*pInTime查询的车辆入场时间*/
/*iParkedTime查询的车辆停留时间*/
/*pRtnLicense模糊匹配时，通过序号返回的车牌*/
/*pInPic查询车辆的图片路径*/
/*pID查询车辆的ID*/
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
	//显示入场时间
	CString storeInTime = "";
	CString  stayTime = "";
	CString money = "免费";
	float currCarShouldMony = 0.0;
	BOOL bCheckFree = FALSE;
	float fCheckMny = 0.00;
	CTimeSpan tmFreeSpan(0,0,iFreeTime,0);//预付费后免费时间
	int iLicenColor = -1;
	CString endDate ="";
	CString startDate = "";
	CString strConvert = "";
	bool isInDb;
	bool ret;
	vector<CarInfoIn> carInfoList;
	DEBUG_INFO("GetParkedCarInfo start\n");
	DEBUG_INFO("匹配方式(0:完全匹配,1:模糊匹配，2：ID车牌同匹配)%d\n",iMatchMethod);
	try
	{
		isInDb = DoorMgr::SelectCarInfoByLicense(carInfo, license);

		//如果有这辆车的信息
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
					DEBUG_INFO("查找%s到达相似车辆开头或结束!\n",license);
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

	//如果有入场纪录
	if (ret) 
	{
		//显示入场时间
		storeInTime =  carInfoIn.inOutTime;
		CTime tempStartTime = CommFunc::TimeStrToCTime(storeInTime);
		fCheckMny = carInfoIn.fCheckMny;
		if (iFreeTime == 0)
		{
			bCheckFree = FALSE;
		}
		else if (fCheckMny > 0.00 && carInfoIn.CheckTime != "1900/01/01 00:00:00")//预付费一定时间内免费
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
			DEBUG_INFO("计算车辆：%s作为超期处理,停留时间为 %s\n",license, stayTime);     
		}
		else if (bAsTempCar)
		{
			DEBUG_INFO("计算车辆：%s固定车作为临时车处理,停留时间为 %s\n",license, stayTime);     
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
			strConvert.Format("%.2f",fMoney);//转换进行四舍五入
			fMoney = atof(strConvert);
			strcpy(pRtnLicense,carInfoIn.carLicense);
			strcpy(pInPic,carInfoIn.picPath);
			strcpy(pID,carInfoIn.ID);
			DEBUG_INFO("计算车辆：%s金额%s元入场时间%s停留时间为 %s分钟图片路径%s数据库ID%s\n",pRtnLicense,strConvert,storeInTime,stayTime,pInPic,pID);
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
		DEBUG_INFO("未找到对应入场车辆：%s 的信息\n",license); 
		DEBUG_INFO("GetParkedCarInfo end\n");
		return RESULT_NO_IN_RECORD;
	}
	DEBUG_INFO("GetParkedCarInfo end\n");
	return RESULT_SUCCESS;
}


/************************************************************************/
/*计算并显示车辆停场信息,此处返回的fMoney为总缴费金额减去预付费后剩余的金额*/
/* iCarType 车类型 0:临时车A,小车 1：临时车B 大车*/
/* pLicense要查询的车牌*/
/*pPayTime支付的时间*/
/*fMoney支付的金额*/
/*pSearchID为在本公司网上支付时，重新计算金额时，用来区分重复车牌,东汇支付重新计算金额时，传空*/
/*pSearchInTime为预留,在本公司网上支付时，传入场时间。东汇支付重新计算金额时，传空*/
/*iPayMethod为哪种预支付。0：东汇预支付;1：本公司网上支付*/
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
	float fRemainMoney;//余下应支付的钱
	bool ret; 
	RNRESULT rtn;
	sMoney.Format("%.2f", fMoney);
	DEBUG_INFO("**%s开始支付%s元,当前支付时间%s,SearchID:%s, SearchInTime:%s, iPayMethod(0:东汇,1:网上):%d\n", 
		       sCarLicense, sMoney,sPayTime,sSearchID, sSearchInTime, iPayMethod);
	CarInfoIn carInfoIn;
	try
	{
		if (iPayMethod == 0)//东汇
		{
			ret = DoorMgr::SelectOffLineTempRecordByLicense(carInfoIn, sCarLicense);
		}
		else//网上
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
			DEBUG_INFO("%s支付时间不大于上次支付时间%s,此次未累加写入支付金额\n", sCarLicense,carInfoIn.CheckTime);
			return RESULT_REPEAT_NOTICE_FEE;
		}
	}
	if (iPayMethod == 0)//东汇
	{
		rtn = GetParkedCarInfo(iCarType, pCarLicense, 0, 0, 1, pSearchID, pSearchInTime, fRemainMoney, pRtnInTime, iParkedTm, 
			pRtnLicense, pRtnInPic, pRtnID);
	}
	else//网上
	{
		rtn = GetParkedCarInfo(iCarType, pCarLicense, 0, 2, 1, pSearchID, pSearchInTime, fRemainMoney, pRtnInTime, iParkedTm, 
			pRtnLicense, pRtnInPic, pRtnID);
	}
	if (rtn != RESULT_SUCCESS)
	{
		DEBUG_INFO("%s计算总金额错误\n",sCarLicense);
		return rtn;
	}
	if (fMoney < fRemainMoney + g_fEps)
	{
		try
		{
			if (iPayMethod == 0)//东汇
			{
				ret = DoorMgr::UpdateParkedFee(sCarLicense, sPayTime, fMoney);
			}
			else//网上
			{
				ret = DoorMgr::UpdateParkedFeeAndMatchID(sCarLicense, sPayTime, fMoney, sSearchID);
			}
			if (ret)
			{
				DEBUG_INFO("**%s完成支付%s元\n", sCarLicense, sMoney);
			}
			else
			{
				DEBUG_INFO("**%s支付%s元未成功，无记录\n", sCarLicense, sMoney);
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
		DEBUG_INFO("%s支付金额超出总应缴费额,此次未累加写入支付金额\n", sCarLicense);
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


//得到车位信息
/************************************************************************/
/* pCarLicense通过该车牌号查车位号
/*aCarportInfo:由车牌号得到的相应的所有车位信息
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


//计算要充值车位的付费总额
/************************************************************************/
/* pCarLicense通过该车牌号查车位号
/*pCarportNum:app上车位号下拉框中选择的当前车位
/*aCarportNum：传过来的app车位下拉框中所有的车位
/*iMonthCnt：要充值的月数
/*bPayAllCarport:是对房号下的当前车位充值还是对房号下的所有车位充值，true为所有车位，false只为当前选择的车位
/*aCarportInfo:返回的要充值的车位。如bPayAllCarport为true,为所有房号下的车位；否则，为app上车位号下拉框中选择的当前车位
/*fMoney重新计算用来验证的充值金额
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
				DEBUG_INFO("当前充值车位与数据库中的不匹配\n");
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
				DEBUG_INFO("当前充值车位与数据库中的不匹配\n");
			}
		}
	}
	else
	{
		fMoney = 0;
		DEBUG_INFO("无车位记录!\n");
		return RESULT_NO_IN_RECORD;
	}
	DEBUG_INFO("总应充值额%.2f元\n", fMoney);
	return RESULT_SUCCESS;
}

//包月车充值更新信息
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
		DEBUG_INFO("**%s车位充值成功%.2f元!\n",stuCarportInfo.sCarportNum, stuCarportInfo.fRentMoney * iMonthCnt);
		return RESULT_SUCCESS;
	}
	else
	{
		DEBUG_INFO("**%s车位充值失败!\n",stuCarportInfo.sCarportNum);
		return RESULT_DB_CONNECT_ERROR;
	}
}

//包月车充值
/************************************************************************/
/* pCarLicense通过该车牌号查车位号
/*pRoomNum查找的车牌号对应的房号
/*pCarportNum:app上车位号下拉框中选择的当前车位
/*aCarportNum：传过来的app车位下拉框中所有的车位
/*pPayTime：充值的时间
/*iMonthCnt：要充值的月数
/*fMoney:要充值车位的总金额
/*bPayAllCarport:是对房号下的当前车位充值还是对房号下的所有车位充值，true为所有车位，false只为当前选择的车位
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
	vector<ST_VIEWCARPORTROOMINFO> aCarportInfo;//要充值的车位信息记录
	sMoney.Format("%.2f", fMoney);
	DEBUG_INFO("**%s组号%s当前车位号%s开始包月充值%s元,当前充值时间%s,充值月数:%d, 充值方式%s\n", 
		sCarLicense, sRoomNum, sCarportNum, sMoney,sPayTime,iMonthCnt,bPayAllCarport ? "所有车位":"单车位");
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
			DEBUG_INFO("%s充值时间不大于上次充值时间%s,此次未累加写入充值金额\n", sCarLicense,St_OperateReport.sOperateDate);
			return RESULT_REPEAT_NOTICE_FEE;
		}
	}
	rtn = GetCarportTotalRent(pCarLicense, pCarportNum, aCarportNum, iMonthCnt, bPayAllCarport, aCarportInfo, fTotalMoney);
	if (rtn != RESULT_SUCCESS)
	{
		DEBUG_INFO("%s计算总金额错误\n",sCarLicense);
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
		DEBUG_INFO("%s充值金额与总应充值额不等,此次未写入充值金额\n", sCarLicense);
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

//iCarOrder为相似车牌中的第几个，从1开始
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
	/**//* 下面为获取到Java中对应的实例类中的变量*/
	//获取Java中的实例类
	jclass objcls = (env)->FindClass("xd/dl/job/ParkedCarInfo");
	jobject obj =env->AllocObject(objcls);
	//获取类中每一个变量的定义
	//名字
	jfieldID str = (env)->GetFieldID(objcls,"sInTime","Ljava/lang/String;");
	jfieldID strCarLicense = (env)->GetFieldID(objcls,"sCarLicense","Ljava/lang/String;");
	jfieldID strInPic = (env)->GetFieldID(objcls,"sInPic","Ljava/lang/String;");
	jfieldID strID = (env)->GetFieldID(objcls,"sID","Ljava/lang/String;");
	jfieldID iParkedTimeVal = (env)->GetFieldID(objcls,"iParkedTime","I");
	jfieldID iReturnVal = (env)->GetFieldID(objcls,"iReturn","I");
	jfieldID fMoneyVal = (env)->GetFieldID(objcls,"fMoney","F");
	//给每一个实例的变量付值
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

//查询剩余车位
JNIEXPORT jobjectArray JNICALL Java_xd_dl_job_ParkNative_getLeftParkInfo(JNIEnv *env, jclass)
{
	vector<LeftParkInfo> aLeftParkInfo;
	jstring sParkName;
	DEBUG_INFO("查询剩余车位开始\n");
	//申明一个object数组 
	jobjectArray args = 0;
	//获取object所属类,一般为java/lang/Object就可以了
	jclass objClass = (env)->FindClass("java/lang/Object");
	//新建object数组
	RNRESULT iRtn = g_PMTest->GetLeftParkInfo(aLeftParkInfo);
	if (iRtn == RESULT_DB_CONNECT_ERROR)
	{
		args = (env)->NewObjectArray(aLeftParkInfo.size(), objClass, 0);
		return args;
	}
	args = (env)->NewObjectArray(aLeftParkInfo.size(), objClass, 0);
	if (aLeftParkInfo.size() == 0)
	{
		DEBUG_INFO("查询剩余车位结束，车位记录数为0\n");
		return args;
	}
//	DEBUG_INFO("查询剩余车位记录数为%d\n",aLeftParkInfo.size());
	/**//* 下面为获取到Java中对应的实例类中的变量*/
	//获取Java中的实例类
	jclass objcls = (env)->FindClass("xd/dl/job/LeftParkInfo");
	//获取类中每一个变量的定义
	//名字
	jfieldID strParkName = (env)->GetFieldID(objcls,"sParkName","Ljava/lang/String;");
	jfieldID iSumNum = (env)->GetFieldID(objcls,"iSumNum","I");
	jfieldID iLeftNum = (env)->GetFieldID(objcls,"iLeftNum","I");
	jfieldID iCarProp = (env)->GetFieldID(objcls,"iCarProp","I");
	//给每一个实例的变量付值
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
	DEBUG_INFO("查询剩余车位结束\n");
	return args;
}

//通过车牌得到组号和车位包月信息
JNIEXPORT jobjectArray JNICALL Java_xd_dl_job_ParkNative_getCarportInfo(JNIEnv *env, jclass, jstring sCarLicense)
{

	vector<ST_VIEWCARPORTROOMINFO> aCarportInfo;
	CString strParkName;
	jstring sTemp;
	char *pCarLicense = jstringToWindows(env, sCarLicense);
	DEBUG_INFO("查询%s车相应车位信息开始\n",pCarLicense);
	//申明一个object数组 
	jobjectArray args = 0;
	//获取object所属类,一般为java/lang/Object就可以了
	jclass objClass = (env)->FindClass("java/lang/Object");
	//新建object数组
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
		DEBUG_INFO("查询车位信息结束，相应车位记录数为0\n");
		return args;
	}
	DEBUG_INFO("查询车位信息记录数为%d\n",aCarportInfo.size());
	/**//* 下面为获取到Java中对应的实例类中的变量*/
	//获取Java中的实例类
	jclass objcls = (env)->FindClass("xd/dl/job/ViewCarportRoomInfo");
	//获取类中每一个变量的定义
	//名字
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
				if (aLeftParkInfo[i].iCarProp == 2)//外停车场
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
	
	//给每一个实例的变量付值
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
	DEBUG_INFO("查询车位信息结束\n");
	return args;
}

/************************************************************************/
/* 给包月车充值                                                                     */
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
	int size=env->GetArrayLength(aCarportNum);//得到数组的长度值
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

//将jstring类型转换成windows类型
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
//将windows类型转换成jstring类型
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
