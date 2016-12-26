JNIEXPORT jobject JNICALL Java_xd_dl_job_ParkNative_getParkedCarInfo
(JNIEnv *env, jclass,jint iCarType, jstring sCarLicense, jint iFreeTime, jint iMatchMethod, jint iCarOrder, jstring sSearchID, jstring sSearchInTime)
1、此接口将以前的jboolean bFuzzyMatch改为了jint iMatchMethod。如是东汇的查询，值设为"0",我们公司的查询设为"1"
2、增加了jstring sSearchID, jstring sSearchInTime,你在调用时，此两参数都传""空字符串。
class  ParkedCarInfo {
    // 
     public String sInTime;

     public float fMoney;
    // 
     public int  iParkedTime;

     public String sInPic;

     public String sCarLicense;
     
     public String sID;

     public int iReturn;
}
ParkedCarInfo类增加了"sID",此为查询到的车的数据库记录ID


JNIEXPORT jint JNICALL Java_xd_dl_job_ParkNative_payParkCarFee
(JNIEnv *env, jclass, jint iCarType, jstring sCarLicense, jstring sPayTime, jfloat fMoney, jstring sID, jstring sInTime, jint iPayMethod)
1、此接口增加了jstring sID, jstring sInTime, jint iPayMethod三参数。东汇调用时sID,sInTime都传""空字符串,iPayMethod传"0"
                                                                    我们自己的支付调用时sID传支付车的数据库记录ID,sInTime传支付车的入场时间，格式为“XXXX-XX-XX XX:XX:XX”,iPayMethod传"1"


JNIEXPORT jobjectArray JNICALL Java_xd_dl_job_ParkNative_getLeftParkInfo(JNIEnv *env, jclass)
1、查询剩余车位,返回一个如下结构的数组
class  LeftParkInfo 
{
	public String sParkName;//名称
	public int iSumNum;//总车位数
	public int iLeftNum;//剩余车位数
	public int iCarProp;//1内停车场;2:外停车场
}



20161212
class ViewCarportRoomInfo
{
	public String sCarportNum;		//车位号
	public String sRoomNum;			//组号
	public String sName;				//姓名	
	public String sAddress;			//地址
	public String sPhoneNumber;		//电话
	public String sPosition;			//位置
	public String	sStartDate;			//有效起始日期
	public String	sEndDate;			//有效结束日期
	public float	fDeposit;			//押金
	public boolean	bTemporary;			//临时
	public String sRemark;			//备注
	public String sRentName;		//包月类型
	public float fRentMoney;		//包月金额
}
//通过车牌得到组号和车位包月信息,此接口返回ViewCarportRoomInfo数组对象
JNIEXPORT jobjectArray JNICALL Java_xd_dl_job_ParkNative_getCarportInfo(JNIEnv *env, jclass, jstring sCarLicense)


//包月车充值
/************************************************************************/
/* sCarLicense通过该车牌号查车位号
/* sRoomNum查找的车牌号对应的房号
/* sCarportNum:app上车位号下拉框中选择的当前车位
/* aCarportNum：传过来的app车位下拉框中所有的车位,为String[]对象
/* sPayTime：充值的时间“格式2016-12-12 15:47:36”
/*iMonthCnt：要充值的月数
/*fMoney:要充值车位的总金额
/*bPayAllCarport:是对房号下的当前车位充值还是对房号下的所有车位充值，true为所有车位，false只为当前选择的车位
/************************************************************************/
JNIEXPORT jint JNICALL Java_xd_dl_job_ParkNative_payCarportRent
(JNIEnv *env, jclass, jstring sCarLicense, jstring sRoomNum, jstring sCarportNum, jobjectArray aCarportNum, jstring sPayTime, jint iMonthCnt, jfloat fMoney, jboolean bPayAllCarport)

此函数返回0代表充值成功

20161226 
class ViewCarportRoomInfo
{
	public String sCarportNum;		//车位号
	public String sRoomNum;			//组号
	public String sName;				//姓名	
	public String sAddress;			//地址
	public String sPhoneNumber;		//电话
	public String sPosition;			//位置
	public String	sStartDate;			//有效起始日期
	public String	sEndDate;			//有效结束日期
	public float	fDeposit;			//押金
	public boolean	bTemporary;			//临时
	public String sRemark;			//备注
	public String sRentName;		//包月类型
	public float fRentMoney;		//包月金额
	public String sParkName;		//停车场名
}
中增加public String sParkName;		//停车场名