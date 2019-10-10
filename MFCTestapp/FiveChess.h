#pragma once

#include<vector>
using std::vector;
using std::pair;


struct VCPoint  // VC����ʱ�Ĺ����㣬�ɻ�4����4������4����3����3����ɣ����ȼ����εݼ�
{
	int row;
	int col;
	int pri;  // ���ȼ�
	friend bool operator < (const VCPoint &a,const VCPoint &b)
	{
		return a.pri > b.pri;
	}
	void init(int _r,int _c,int _p)
	{
		row = _r, col = _c, pri = _p;
	}
};

class FiveChess
{
public:
	FiveChess();
	virtual ~FiveChess();

	int nCount; //��¼���˶��ٲ���
	
	void InitChessMap(); //��ʼ������
	void SetKinjite(bool isKinjite); //Ϊtrue��ʾ�н��֣�Ϊfalse��ʾ�޽���

	char GetStatus(int row,int col);  //�õ�chessMap[row][col]��ֵ
	void SetCurrentPoint(int row,int col); //���õ�ǰ�����

	void SetChess(int row,int col,char chessFlag);  //��һ����
	void RetractChess(); //��һ����

	char JudgeWin(char chessFlag); //�ж�chessFlagɫ�������Ƿ�Ӯ��

public:
	vector< pair<int,int> > chessPro; //�����������

	char chessMap[SIZE][SIZE]; //����
	int currentX,currentY; //��ǰ���ӵ�

	bool haveKinjite;  //�������Ƿ����� �н���,��ʵ���Ͻ�����һ��ȥ�жϣ�����ֻ���˻���AI��Ҫ�����ǣ�


	//������ AI ģ��
private:

	//��¼δ�¹��ĵ���ڼ�������˵ļ�ֵ
	int computer[SIZE][SIZE],people[SIZE][SIZE];

	int peoX,peoY;  // AI��������һ��

//	������������ص�,���Բ���һ���������ƿ�,��¼��ǰ�����������ӵ�����
//	���ҡ����ϡ����µ㹹�ɵľ���,���һ���������Ϊ��һ�����λ�ò������������MM(MM�Լ�����)������.
	int left,right,top,bottom;

	//��ĳһλ���ܲ�����������������ͳ��
	int changlian,cheng5,huo4,chong4,tiaochong4,lian3,tiao3,chong3,lian2,tiao2; //
	int chong2,huo1,chong1;

	// ���漸������ ���� �� chessMap[row][col] ���Ӻ� ������� �����͵����� 

     /*********************************************************************/
	// ������chessMap[row][col] ���Ӻ�,�Ƿ��5������),����1��ʾ����������2��ʾ����,���򷵻�0 
	int GetCheng5ChangLian(int row,int col,char chessFlag);

	//  ������chessMap[row][col] ���Ӻ󣬷��ع��ɻ�4������ 
	int GetHuo4(int row,int col,char chessFlag);
	
	// ������chessMap[row][col] ���Ӻ󣬷��ع��ɳ�3�ͳ�4(chong4��tiaochong4)������ 
	pair<int,pair<int,int> > GetChong3Chong4(int row,int col,char chessFlag);

	// ������chessMap[row][col] ���Ӻ󣬷��ع�����2����3������ 
	pair<int,int> GetLian2Lian3(int row,int col,char chessFlag);

	// ������chessMap[row][col] ���Ӻ󣬷��ع�����2����3������ 
	pair<int,int> GetTiao2Tiao3(int row,int col,char chessFlag);

	// ������chessMap[row][col] ���Ӻ��ж�һЩС���͵�����
	void OntherChessType(int row,int col,char chessFlag);

	/*********************************************************************/

	//���� chessMap[row][col] ����ܹ���� chessFlag ɫ�����ܹ�������Щ����
	void Search(int row,int col,char chessFlag);  
    
    // ��¼�Ƿ��б�Ӯ��,Ϊ1��ʾһ��Ӯ�壨���壩��Ϊ2��ʾ����Ӯ�壨�����4����
	// Ϊ3��ʾ����Ӯ�壨����������3��,Ϊ 0 ��ʾû�б�Ӯ��
	int mustWin;   //��¼�Ƿ��б�Ӯ��
	int winPeople,winComputer;  //ͬ���Ǽ�¼��Ӯ�㣬��¼��˭�ı�Ӯ��

	//�����������ƿ�
	void UpdateLimit();
	

    //�õ� chessMap[row][col] ������ chessFlag ɫ����ļ�ֵ
	int GetScoreForComputer(int row,int col,char chessFlag); //վ�ڼ�����ĽǶ�
	int GetScoreForPeople(int row,int col,char chessFlag);   //վ���˵ĽǶ�

	// �������������ĸ���������flagΪ cComputer ��ʾվ�ڼ�����ĽǶȣ�Ϊ cPeople��ʾվ���˵ĽǶ�
	int GetScore(int row,int col,char chessFlag,char flag);

	//�õ������ص�ǰ��ֵ���㣬chessFlag Ϊ cComputer��ʾվ�ڼ�����ĽǶȣ�ΪcPeople��ʾվ���˵ĽǶ�
	pair<int,int> GetCurrentMaxPoint(char chessFlag); 	

	//��ν����Ҫ��Ҫ���Է��Ľ���������ô����Ҫͨ���ӷ���ʵ�֣���GetScore()�е���
	int AddScore(int row,int col,char chessFlag); 
	int isDefend; //�Ƿ���أ��� VCAttack()�о������ڷ�ֹ�Է�VC����ʱʹ�ã����� GetScore()�� AddScore()�ĵ���

	// GameTree�Ĳ����У�flag�ֱ��ʾ������Ƿ��ҵ���Ӯ�㣬
	// ���б�Ӯ�㣬��depthC,depthP��ʾ(���������)��Ӯ�����С���
	// �������ֻ�ǳ���ɸѡ
	// InitGameTree���� �� AgainGameTree��) �ĺ������࣬һ�����ڼ�����ģ�һ�������˵ġ�
	void InitGameTree(int row,int col,bool& flag,int depth,int& depthC,int& depthP,bool& stop,int depthRecord); 
	
	bool PeopleAttack(int r,int c,int depthCFromIGT); //ģ���˹���
	void AgainGameTree(int row,int col,int depth,int& depthC,int& depthP,bool& stop,int& depthRecord); 
	
	bool ComputerAttack(int r,int c,int depthMM); // ģ����������

	bool VCAttack(); // �ж��Ƿ���Խ���VC����
	int IsCombForVC(int row,int col,char chessFlag); // �Ƿ񹹳ɿ���VC�����Ĳ��ϣ���4����3���4
	// type Ϊ1 ��ʾ VCF (��4)������Ϊ2��ʾ VCF��VCT���Ϲ���
	void VCAttackTree(int type,int row,int col,char cOneself,char cOpposite,int depth,bool& flag,int& ansDepth,int depthRecord); // VC ������

	int DEPTH; // �� AI ��̬���� InitGameTree ���������
	void ControlDepth();   // �жϾ��ƣ��Ӷ����� DEPTH
	bool LayOut();   // ��ʼ��ʱ�򲼾�

public:
	char cComputer;  //�����ִ�������ɫ
	char cPeople;    //��ִ�������ɫ

	int grade; // AI�ĵȼ����� CAIThread �е� AI()�иı�

	int comX,comY; //AI�м��������һ��

	//�ڷ������µĹؼ��ĵ����Ӽ��γ�������ͬʱ�����γɽ��֣���ʱ����ʧЧ������ʤ
    //����1˵������Ǻ���ĳ������֣�����2˵�������������Ľ��֣������ǽ���
	int IsKinjite(int row,int col); 
	void ComputerPlay();  // �õ� comX,comY
	void PeoplePlay();   //�õ� peoX,peoY

	void AI();   //

	int depthMM; // ��ȿ���

	int VCDEPTH;  // AI ���� VC �����Ŀ�ʼ���
	int VCDEPTH_END;  // AI ����VC������������
	int VCDEPTH_DIS;  // AI ���� VC ��������� ����ֵ
	
	int VCDE_DEPTH;  // AI ���� VC �����Ŀ�ʼ���
	int VCDE_DEPTH_END; // AI ���� VC ������������
	int VCDE_DEPTH_DIS; // AI ���� VC ��������� ����ֵ

	// �����ǹ���״̬��������ʾ�����淽���û�֪��AIĿǰ��״��
	/*
		0 : ��ʾƨ��û��
		1 ����ʾ��������ڽ��� VCF ����
		2 ����ʾ�����������ֹ�˽��� VCF ����
		3 ����ʾ��������ڽ��� VCF �� VCT ���Ϲ���
	*/
	int AIState;  // �� VCAttack() ������

};


/*
      chessMap[][]Ϊ SPACE ��ʾ��λ��Ϊ�գ�Ϊ BLACK_CHESS ��ʾΪ���壬
	  ΪWHITE_CHESS ��ʾΪ���塣

	  JudgeWin(char chessFlag)���� chessFlag ˵��chessFlagɫ����Ӯ�ˣ�
	  ���� SPACE ��ʾ��δ�ֳ�ʤ�������� PEACE_CHESS����ʾ���塣


  ******   ���������͵Ľ��ܺͽ��ֹ���
  
	1�� ���������������γɵ�5��ͬɫ���ӵ�"��"�� 
	2�� ���������������γɵ�6����6������ͬɫ���ӵ�"��"��
	
	"��"����"����"��"����"����Ϊ�����VC��������������һ��"������"�� 
	1�����ģ�������ĳһ�����߻���������ͬɫ4�Ӳ�����ؽ������������ڴ�4�������ӳ����ϸ���һ�����ӵĽ�������4�ӽ��������� 
	2�����ģ���"����"��ģ�����һ�������γ����������Ҵ��������Ŀ����Եľ��档 
	3�� ��������һ�ſ��γɳ����ľ���Ҳ��Ϊ"��"�� 
	
	"��"ָ����������"����"��"����"�� 
	1��������������ĳһ�����߻���������ͬɫ�������������ڴ����������ӳ�������һ��������һ������һ���������������ӵĽ����������ӽ��������� 
	2���������м�����һ�����ӽ������������������ӳ��߾�������һ�����ӵĽ����������������� 
	
	���֣��Ծ��н�ֹʹ�õ��ŷ���
	  
	
	  1��������ְ���"����"��"����"��"����"�� 
		  1.1 ���������ںڷ���һ�������ӽ������ͬʱ�γɶ�����������Ϻڷ�"��"�ľ��档 
					������������ ���㣩
		  1.2 ���ģ����ںڷ���һ�������ӽ������ͬʱ�γɶ�����������Ϻڷ�"��"�ľ��档 
					�����ģ����� ���㣩

	  2�������޽��֡�
	  
	  "����"��ָĳһ��ͬʱ�߱��������֣�����һ����"��"��һ����"����"�� 

	  ���֣��Է�����Ӧ����ŷ�������"����"��Ϊ�������֡� 

	  ���ֿɽ�������ָ�����������3����󣬰׷���Ӧ�ע�֮ǰ����о��ڷ����β����ڼ�����
	  �������������ִ����һ����Ϊִ����һ�������ڷ������Բ����� 

	  ����������ָ�������������Ϲؼ��ĵ�5����ʱ�������������壬�ð�����������������ѡһ����
	  Ȼ���ټ������ġ�һ��˵��������϶��õ��԰׷�������һ�㣬�������Ժڷ���Ϊ�������ǵ��úڷ����塣 
		  
	   ���������ͬʱ�γɣ�����Ϊʤ��

 ***** ���� ���ֹ����жϷǳ����ӣ�����ֻ����һ�ؽ��֣����ڶ��ؽ��ֳ������жϣ�����������жϡ�
	��˵ QQ ��Ϸ��������Ľ����ж�Ҳ��bug, ������ʹ���һ��bug��

     
	   ������רҵ���VCF��VCT����
	   VCF: victory for continual four  ,�������л��ģ����ģ��������֣�����
	   VCT: victory for continual three, �������л�������
	   �����ۺϳ�Ϊ VC ����
 */