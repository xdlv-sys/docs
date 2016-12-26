package xd.dl.job;
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

class  LeftParkInfo 
{
	public String sParkName;
	public int iSumNum;
	public int iLeftNum;
	public int iCarProp;
}

class ViewCarportRoomInfo
{
	public String sCarportNum;		//��λ��
	public String sRoomNum;			//���
	public String sName;				//����	
	public String sAddress;			//��ַ
	public String sPhoneNumber;		//�绰
	public String sPosition;			//λ��
	public String	sStartDate;			//��Ч��ʼ����
	public String	sEndDate;			//��Ч��������
	public float	fDeposit;			//Ѻ��
	public boolean	bTemporary;			//��ʱ
	public String sRemark;			//��ע
	public String sRentName;		//��������
	public float fRentMoney;		//���½��
	public String sParkName;		//ͣ������
}

public class ParkNative
{
    public native static int initialized( String pDbIP,String pDbName,String pDbUserID,String pDbPwd );
    public native static int unitialized();
    public native static ParkedCarInfo getParkedCarInfo(int iCarType, String pLicense, int iFreeTime, int iMatchMethod, int iCarOrder, String pSearchID, String pSearchInTime);
    public native static int payParkCarFee(int iCarType, String pCarLicese, String pPayTime, float fMoney, String pSearchID, String pSearchInTime, int iPayMethod);
    public native static LeftParkInfo[] getLeftParkInfo();
    public native static ViewCarportRoomInfo[] getCarportInfo(String pLicense);
    public native static int payCarportRent(String pCarLicense, String pRoomNum, String pCarportNum, String[] aCarportNum, String pPayTime, int iMonthCnt, float fMoney, boolean bPayAllCarport);

    static {
        System.loadLibrary("ParkingMeter");
    }
   
    public static void main(String[] args) {

	int iInitRtn = initialized("193.168.1.100","hjcsfbz","sa","sa");
	System.out.println("iInitRtn=" + iInitRtn);
//	ParkedCarInfo obj = getParkedCarInfo(0,"��A12366",15, 0, 2, "", "");
//	System.out.println("CarLicense = "+obj.sCarLicense+ " money = "+obj.fMoney + " intime =" +  obj.sInTime + " parkedtime=" + obj.iParkedTime + " InPic = "+obj.sInPic + " ID = "+obj.sID + " return val =" + obj.iReturn); 
//
//	obj = getParkedCarInfo(0,"��A12367",15, 0, 3, "", "");
//	System.out.println("CarLicense = "+obj.sCarLicense+ " money = "+obj.fMoney + " intime =" +  obj.sInTime + " parkedtime=" + obj.iParkedTime + " InPic = "+obj.sInPic + " ID = "+obj.sID + " return val =" + obj.iReturn); 
//
//	obj = getParkedCarInfo(0,"��A12366",15, 1, 0, "", "");
//	System.out.println("CarLicense = "+obj.sCarLicense+ " money = "+obj.fMoney + " intime =" +  obj.sInTime + " parkedtime=" + obj.iParkedTime + " InPic = "+obj.sInPic + " ID = "+obj.sID + " return val =" + obj.iReturn); 
//
//	obj = getParkedCarInfo(0,"��A12366",15, 1, 1, "", "");
//	System.out.println("CarLicense = "+obj.sCarLicense+ " money = "+obj.fMoney + " intime =" +  obj.sInTime + " parkedtime=" + obj.iParkedTime + " InPic = "+obj.sInPic + " ID = "+obj.sID + " return val =" + obj.iReturn); 
//
//	obj = getParkedCarInfo(0,"��A12366",15, 1, 2, "", "");
//	System.out.println("CarLicense = "+obj.sCarLicense+ " money = "+obj.fMoney + " intime =" +  obj.sInTime + " parkedtime=" + obj.iParkedTime + " InPic = "+obj.sInPic + " ID = "+obj.sID + " return val =" + obj.iReturn); 
//
//	obj = getParkedCarInfo(0,"��A12366",15, 1, 3, "", "");
//	System.out.println("CarLicense = "+obj.sCarLicense+ " money = "+obj.fMoney + " intime =" +  obj.sInTime + " parkedtime=" + obj.iParkedTime + " InPic = "+obj.sInPic + " ID = "+obj.sID + " return val =" + obj.iReturn); 
//
//	obj = getParkedCarInfo(0,"��A12366",15, 1, 4, "", "");
//	System.out.println("CarLicense = "+obj.sCarLicense+ " money = "+obj.fMoney + " intime =" +  obj.sInTime + " parkedtime=" + obj.iParkedTime + " InPic = "+obj.sInPic + " ID = "+obj.sID + " return val =" + obj.iReturn); 
//	obj = getParkedCarInfo(0,"��A12366",15, 1, 5, "", "");
//	System.out.println("CarLicense = "+obj.sCarLicense+ " money = "+obj.fMoney + " intime =" +  obj.sInTime + " parkedtime=" + obj.iParkedTime + " InPic = "+obj.sInPic + " ID = "+obj.sID + " return val =" + obj.iReturn); 
//	payParkCarFee(0,"��A12366","2016-11-18 14:45:08",.6f, "79853", "2016/11/17 10:57:16", 1);
//	payParkCarFee(0,"��A12368","2016-11-18 14:45:08",.6f, "79754", "2016/11/17 11:17:29", 1);
//	payParkCarFee(0,"��A12466","2016-11-18 14:45:08",.6f, "79751", "2016/11/17 11:47:29", 1);
//	LeftParkInfo[] aobject;
//	aobject=getLeftParkInfo();
//	for(int i = 0; i < aobject.length; i++)
//	{
//		System.out.println("ParkName = "+aobject[i].sParkName+ " SumNum = "+aobject[i].iSumNum + " LeftNum =" +  aobject[i].iLeftNum + " iCarProp=" + aobject[i].iCarProp); 
//  }

	String aCarportNum[] = new String[] { "102ǰ��", "����20", "����24", "����25" };
	ViewCarportRoomInfo[] aCarportObj;
	aCarportObj=getCarportInfo("��A12344");
	for(int i = 0; i < aCarportObj.length; i++)
	{
		System.out.println("sCarportNum = "+aCarportObj[i].sCarportNum + " sRoomNum = "+aCarportObj[i].sRoomNum + " sName = "+aCarportObj[i].sName 
		 + " sAddress = "+aCarportObj[i].sAddress + " sPhoneNumber = "+aCarportObj[i].sPhoneNumber + " sPosition = "+aCarportObj[i].sPosition + " sStartDate = "+aCarportObj[i].sStartDate
		  + " sEndDate = "+aCarportObj[i].sEndDate + " fDeposit = "+aCarportObj[i].fDeposit + " bTemporary = "+aCarportObj[i].bTemporary
		   + " sRemark =" +  aCarportObj[i].sRemark + " sRentName=" + aCarportObj[i].sRentName + " fRentMoney=" + aCarportObj[i].fRentMoney + " sParkName = "+aCarportObj[i].sParkName); 
  }
	payCarportRent("��A12344", "102", "����20", aCarportNum, "2016-12-12 09:32:49", 3, 3, false);
	payCarportRent("��A12344", "102", "����20", aCarportNum, "2016-12-12 09:32:50", 3, 4.0f, false);
	payCarportRent("��A12344", "102", "����20", aCarportNum, "2016-12-12 09:32:51", 3, 12, true);
	payCarportRent("��A12344", "102", "����20", aCarportNum, "2016-12-12 09:32:52", 3, 3, true);
	
	unitialized();
	System.out.println("unitialized1");
// 	iInitRtn = initialized("193.168.1.100","hjcsfbz","sa","sa");
// 	System.out.println("iInitRtn=" + iInitRtn);
// 	obj = getParkedCarInfo(0,"��A12377",15);
// 	payParkCarFee(0,"��A12377","2016-8-25 8:56:56",0.5f);
// 	System.out.println("money = "+obj.fMoney + "intime =" +  obj.sInTime + "parkedtime=" + obj.iParkedTime + "return val =" + obj.iReturn); 
// 	unitialized();
// 
// 	System.out.println("unitialized2");
// 	iInitRtn = initialized("193.168.1.100","hjcsfbz","sa","sa");
// 	obj = getParkedCarInfo(0,"��A12366",15);
// 	payParkCarFee(0,"��A12366","2016-8-25 8:56:56",0.5f);
// 	System.out.println("money = "+obj.fMoney + "intime =" +  obj.sInTime + "parkedtime=" + obj.iParkedTime + "return val =" + obj.iReturn); 
// 	unitialized();
// 
// 	iInitRtn = initialized("193.168.1.100","hjcsfbz","sa","sa");
// 	obj = getParkedCarInfo(0,"��A12399",15);
// 	payParkCarFee(0,"��A12399","2016-8-25 8:56:56",0.5f);
// 	System.out.println("money = "+obj.fMoney + "intime =" +  obj.sInTime + "parkedtime=" + obj.iParkedTime + "return val =" + obj.iReturn); 
// 	unitialized();
// 
// 	iInitRtn = initialized("193.168.1.100","hjcsfbz","sa","sa");
// 	obj = getParkedCarInfo(0,"��A12311",15);
// 	payParkCarFee(0,"��A12311","2016-8-25 8:56:56",0.5f);
// 	System.out.println("money = "+obj.fMoney + "intime =" +  obj.sInTime + "parkedtime=" + obj.iParkedTime + "return val =" + obj.iReturn); 
// 	unitialized();
// 
// 	iInitRtn = initialized("193.168.1.100","hjcsfbz","sa","sa");
// 	obj = getParkedCarInfo(0,"��A12322",15);
// 	payParkCarFee(0,"��A12322","2016-8-25 8:56:56",0.5f);
// 	System.out.println("money = "+obj.fMoney + "intime =" +  obj.sInTime + "parkedtime=" + obj.iParkedTime + "return val =" + obj.iReturn); 
// 	unitialized();
// 
// 	iInitRtn = initialized("193.168.1.100","hjcsfbz","sa","sa");
// 	obj = getParkedCarInfo(0,"��A12333",15);
// 	payParkCarFee(0,"��A12333","2016-8-25 8:56:56",0.5f);
// 	System.out.println("money = "+obj.fMoney + "intime =" +  obj.sInTime + "parkedtime=" + obj.iParkedTime + "return val =" + obj.iReturn); 
// 	unitialized();

    }
}