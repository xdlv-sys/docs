JNIEXPORT jobject JNICALL Java_xd_dl_job_ParkNative_getParkedCarInfo
(JNIEnv *env, jclass,jint iCarType, jstring sCarLicense, jint iFreeTime, jint iMatchMethod, jint iCarOrder, jstring sSearchID, jstring sSearchInTime)
1���˽ӿڽ���ǰ��jboolean bFuzzyMatch��Ϊ��jint iMatchMethod�����Ƕ���Ĳ�ѯ��ֵ��Ϊ"0",���ǹ�˾�Ĳ�ѯ��Ϊ"1"
2��������jstring sSearchID, jstring sSearchInTime,���ڵ���ʱ��������������""���ַ�����
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
ParkedCarInfo��������"sID",��Ϊ��ѯ���ĳ������ݿ��¼ID


JNIEXPORT jint JNICALL Java_xd_dl_job_ParkNative_payParkCarFee
(JNIEnv *env, jclass, jint iCarType, jstring sCarLicense, jstring sPayTime, jfloat fMoney, jstring sID, jstring sInTime, jint iPayMethod)
1���˽ӿ�������jstring sID, jstring sInTime, jint iPayMethod���������������ʱsID,sInTime����""���ַ���,iPayMethod��"0"
                                                                    �����Լ���֧������ʱsID��֧���������ݿ��¼ID,sInTime��֧�������볡ʱ�䣬��ʽΪ��XXXX-XX-XX XX:XX:XX��,iPayMethod��"1"


JNIEXPORT jobjectArray JNICALL Java_xd_dl_job_ParkNative_getLeftParkInfo(JNIEnv *env, jclass)
1����ѯʣ�೵λ,����һ�����½ṹ������
class  LeftParkInfo 
{
	public String sParkName;//����
	public int iSumNum;//�ܳ�λ��
	public int iLeftNum;//ʣ�೵λ��
	public int iCarProp;//1��ͣ����;2:��ͣ����
}



20161212
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
}
//ͨ�����Ƶõ���źͳ�λ������Ϣ,�˽ӿڷ���ViewCarportRoomInfo�������
JNIEXPORT jobjectArray JNICALL Java_xd_dl_job_ParkNative_getCarportInfo(JNIEnv *env, jclass, jstring sCarLicense)


//���³���ֵ
/************************************************************************/
/* sCarLicenseͨ���ó��ƺŲ鳵λ��
/* sRoomNum���ҵĳ��ƺŶ�Ӧ�ķ���
/* sCarportNum:app�ϳ�λ����������ѡ��ĵ�ǰ��λ
/* aCarportNum����������app��λ�����������еĳ�λ,ΪString[]����
/* sPayTime����ֵ��ʱ�䡰��ʽ2016-12-12 15:47:36��
/*iMonthCnt��Ҫ��ֵ������
/*fMoney:Ҫ��ֵ��λ���ܽ��
/*bPayAllCarport:�ǶԷ����µĵ�ǰ��λ��ֵ���ǶԷ����µ����г�λ��ֵ��trueΪ���г�λ��falseֻΪ��ǰѡ��ĳ�λ
/************************************************************************/
JNIEXPORT jint JNICALL Java_xd_dl_job_ParkNative_payCarportRent
(JNIEnv *env, jclass, jstring sCarLicense, jstring sRoomNum, jstring sCarportNum, jobjectArray aCarportNum, jstring sPayTime, jint iMonthCnt, jfloat fMoney, jboolean bPayAllCarport)

�˺�������0�����ֵ�ɹ�

20161226 
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
������public String sParkName;		//ͣ������