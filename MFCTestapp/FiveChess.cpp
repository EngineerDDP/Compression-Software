#include "StdAfx.h"
#include "FiveChess.h"

#include<iostream>
#include<algorithm>
using namespace std;

FiveChess::FiveChess()
{
	InitChessMap();
	haveKinjite = false; //Ĭ�����޽���
}

FiveChess::~FiveChess()
{
	chessPro.clear();
	vector< pair<int,int> >(chessPro).swap(chessPro); 
}

void FiveChess::InitChessMap()
{
	memset(chessMap,SPACE,sizeof(chessMap));
	chessPro.clear();
	currentX = -1; //��ʼλ��
	currentY = -1;
	nCount = 0;
}

char FiveChess::GetStatus(int row,int col)
{
	return chessMap[row][col];
}


//�������޽���
void FiveChess::SetKinjite(bool isKinjite) 
{
	haveKinjite = isKinjite;
}

//���õ�ǰ���ӵ�
void FiveChess::SetCurrentPoint(int row,int col)
{
	currentX = row;
	currentY = col;
}

//��һ����
void FiveChess::SetChess(int row,int col,char chessFlag)
{
	chessMap[row][col] = chessFlag;
	++ nCount;
	chessPro.push_back(pair<int,int>(row,col));
	SetCurrentPoint(row,col);
	UpdateLimit(); //�����������ƿ�
}

//��һ����
void FiveChess::RetractChess()
{
	if(nCount >=2)
	{
		int x,y;
		for(int i=0;i<2;++i)
		{
			x = (chessPro.end()-1)->first;
			y = (chessPro.end()-1)->second;
			chessPro.pop_back();

			chessMap[x][y] = SPACE;
			-- nCount;
		}
	}
}

//�ж�chessFlagɫ�������Ƿ�Ӯ��
/*
    JudgeWin(int chessFlag)���� chessFlag ˵��chessFlagɫ����Ӯ�ˣ�����0��ʾ
     	  ��δ�ֳ�ʤ��������3����ʾ���塣
    ���������������ϵ���ͬɫ����������������true������������жϡ�
*/
char FiveChess::JudgeWin(char chessFlag)
{
	Search(currentX,currentY,chessFlag);

	if(changlian || cheng5)
		return chessFlag;
	
	if(nCount<15*15) //����ȷ����Ӯ����ʱ���̻�δ����
	{
		return SPACE;
	}
	return PEACE_CHESS; //���ж������������δ���ؾ��Ǻ�����
}


//����1˵������Ǻ���ĳ������֣�����2���������֣�����3�����Ľ��֣�����0��ʾ���ǽ���
int FiveChess::IsKinjite(int row,int col)
{
	//���ݹ�������������γ�������ͬʱ�����γɽ��֣���ô��Ȼ�Ǻ���ʤ

	if(!haveKinjite)
		return 0;

	Search(row,col,BLACK_CHESS);
	
	if(changlian)
		return 1;
	if(cheng5)  // 
		return 0;
	if(lian3+tiao3>=2)
		return 2;
	if(huo4 + chong4 + tiaochong4 >= 2)
		return 3;

	return 0;
}

//�����������ƿ�
void FiveChess::UpdateLimit()
{
	int MM = 2;
	if(1 == nCount)
	{
		left = max(0,currentY-MM);
		right = min(SIZE-1,currentY+MM);
		top = max(0,currentX-MM);
		bottom = min(SIZE-1,currentX+MM);
	}
	else
	{
		if(currentX >= top+MM && currentX <= bottom-MM && 
			currentY >= left+MM && currentY <= right-MM )
			return ;
		else if(currentX >= top+MM && currentX <= bottom-MM)
		{
			if(currentY > right-MM)
				right = min(SIZE-1,currentY+MM);
			else if(currentY < left+MM)
				left = max(0,currentY-MM);
		}
		else if(currentY >= left+MM && currentY <=right-MM)
		{
			if(currentX > bottom-MM)
				bottom = min(SIZE-1,currentX+MM);
			else if(currentX < top+MM)
				top = max(0,currentX-MM);
		}
		else
		{
			if(currentX < top+MM && currentY > right-MM)
			{
				right = min(SIZE-1,currentY+MM);
				top = max(0,currentX-MM);
			}
			else if(currentX < top+MM && currentY < left+MM)
			{
				left = max(0,currentY-MM);
				top = max(0,currentX-MM);
			}
			else if(currentX > bottom-MM && currentY < left+MM)
			{
				left = max(0,currentY-MM);
				bottom = min(SIZE-1,currentX+MM);
			}
			else if(currentX > bottom-MM && currentY > right-MM)
			{
				right = min(SIZE-1,currentY+MM);
				bottom = min(SIZE-1,currentX+MM);
			}
		}
	}
}

/********�����͵�ͳ��*******************************************/
// �������ڽ��ֹ��򣬵�Ȼ���ؽ��ֲ�������
// �����ڽ��ֹ����£�����ͺ���������жϲ�ͬ�����涼��ֱ���

// ������chessMap[row][col] ���Ӻ�,�Ƿ��5������),����1��ʾ����������2��ʾ����,���򷵻�0 
int FiveChess::GetCheng5ChangLian(int row,int col,char chessFlag)
{
	int left,right,i,j;
	// �� 
	for(i=col-1, left=0; i>=0 && chessFlag == chessMap[row][i]; --i, ++left);
	for(i=col+1, right=0; i<SIZE && chessFlag == chessMap[row][i]; ++i, ++right); 
	
	if( 4 == left + right )
		return 1;
	else if( left + right >= 5 )
		return 2; 
	
	// ��
	for(i=row-1, left=0; i>=0 && chessFlag == chessMap[i][col]; --i, ++left); 
	for(i=row+1, right=0; i<SIZE && chessFlag == chessMap[i][col]; ++i, ++right);
	
	if( 4 == left + right )
		return 1;
	else if( left + right >= 5 )
		return 2; 
	
	// ' \ '
	for(i=row-1, j=col-1, left=0; i>=0 && j>=0 && chessFlag == chessMap[i][j]; --i, --j, ++left);
	for(i=row+1, j=col+1, right=0; i<SIZE && j<SIZE && chessFlag == chessMap[i][j]; ++i, ++j, ++right);
	
	if( 4 == left + right )
		return 1;
	else if( left + right >= 5 )
		return 2; 
	
	// ' / '
	for(i=row-1, j=col+1, left=0; i>=0 && j<SIZE && chessFlag == chessMap[i][j]; --i, ++j, ++left);
	for(i=row+1, j=col-1, right=0; i<SIZE && j>=0 && chessFlag == chessMap[i][j]; ++i, --j, ++right);
	
	if( 4 == left + right )
		return 1;
	else if( left + right >= 5 )
		return 2; 
	
	return 0; 
}

//  ������chessMap[row][col] ���Ӻ󣬷��ع��ɻ�4������ 
int FiveChess::GetHuo4(int row,int col,char chessFlag)
{
	int huo4=0,left,right,i,j;
	bool flag ;

	// �� 
	for(i=col-1, left=0; i>=0 && chessFlag == chessMap[row][i]; --i, ++left);
	for(i=col+1, right=0; i<SIZE && chessFlag == chessMap[row][i]; ++i, ++right); 
	
	if( 3 == left + right && col-left-1 >= 0 && SPACE == chessMap[row][col-left-1]
		&& col+right+1 < SIZE && SPACE == chessMap[row][col+right+1] )
	{	
		flag = true;
		if( haveKinjite && BLACK_CHESS == chessFlag )
		{
			if( col-left-2 >= 0 && chessFlag == chessMap[row][col-left-2] )
				flag = false;
			if( col+right+2 < SIZE && chessFlag == chessMap[row][col+right+2] )
				flag = false;
		}
		if( flag )
			++huo4;
	}

	// ��
	for(i=row-1, left=0; i>=0 && chessFlag == chessMap[i][col]; --i, ++left); 
	for(i=row+1, right=0; i<SIZE && chessFlag == chessMap[i][col]; ++i, ++right);
	
	if( 3 == left + right && row-left-1 >= 0 && SPACE == chessMap[row-left-1][col]
		&& row+right+1 < SIZE && SPACE == chessMap[row+right+1][col] )
	{
		flag = true;
		if( haveKinjite && BLACK_CHESS == chessFlag )
		{
			if( row-left-2 >= 0 && chessFlag == chessMap[row-left-2][col] )
				flag = false;
			if( row+right+2 < SIZE && chessFlag == chessMap[row+right+2][col] )
				flag = false;
		}
		if( flag )
			++huo4;
	}

	// ' \ '
	for(i=row-1, j=col-1, left=0; i>=0 && j>=0 && chessFlag == chessMap[i][j]; --i, --j, ++left);
	for(i=row+1, j=col+1, right=0; i<SIZE && j<SIZE && chessFlag == chessMap[i][j]; ++i, ++j, ++right);
	if( 3 == left + right && row-left-1 >= 0 && col-left-1 >= 0 && SPACE == chessMap[row-left-1][col-left-1]
		&& row+right+1 < SIZE && col+right+1 < SIZE && SPACE == chessMap[row+right+1][col+right+1])
	{
		flag = true;
		if( haveKinjite && BLACK_CHESS == chessFlag )
		{
			if( row-left-2 >= 0 && col-left-2 >= 0 && chessFlag == chessMap[row-left-2][col-left-2] )
				flag = false;
			if( row+right+2 < SIZE && col+right+2 < SIZE && chessFlag == chessMap[row+right+2][col+right+2] )
				flag = false;
		}
		if( flag )
			++huo4; 
	}

	// ' / '
	for(i=row-1, j=col+1, left=0; i>=0 && j<SIZE && chessFlag == chessMap[i][j]; --i, ++j, ++left);
	for(i=row+1, j=col-1, right=0; i<SIZE && j>=0 && chessFlag == chessMap[i][j]; ++i, --j, ++right);
	if( 3 == left + right && row-left-1 >= 0 && col+left+1 < SIZE && SPACE == chessMap[row-left-1][col+left+1]
		&& row+right+1 < SIZE && col-right-1 >= 0 && SPACE == chessMap[row+right+1][col-right-1] )
	{
		flag = true;
		if( haveKinjite && BLACK_CHESS == chessFlag )
		{
			if( row-left-2 >= 0 && col+left+2 < SIZE && chessFlag == chessMap[row-left-2][col+left+2] )
				flag = false;
			if( row+right+2 < SIZE && col-right-2 >= 0 && chessFlag == chessMap[row+right+2][col-right-2] )
				flag = false;
		}
		if( flag )
			++huo4;
	}

	return huo4;
}

	
// ������chessMap[row][col] ���Ӻ󣬷��ع��ɳ�3�ͳ�4(chong4,tiaochong4)������ 
pair<int,pair<int,int> > FiveChess::GetChong3Chong4(int row,int col,char chessFlag)
{
	int chong3=0,chong4=0,tiaochong4=0,i,j,num1,num2; // num1,num2 ��¼���߿ո������ 
	int left1,left2 ,right1,right2; // left1,righ1 ��¼�����ո�֮ǰ�� chessFlag ������.... 
	
	// ��
	num1 = num2 = 0;
	left1 = left2 = right1 = right2 = 0;

	for(i=col-1; i>=0; --i)
	{
		if( SPACE == chessMap[row][i] )
		{
			++num1;
			if( num1 >= 2 )
				break;
			if(i-1 >= 0 && SPACE == chessMap[row][i-1])
				break;
		}
		else if( chessFlag == chessMap[row][i] )
			num1 ? ++left2 : ++left1;
		else
			break;
	}
	 // ����������һ�� 
	for(i=col+1; i<SIZE; ++i)
	{
		if( SPACE == chessMap[row][i] )
		{
			++num2;
			if( num2 >= 2 )
				break;
			if(i+1 < SIZE && SPACE == chessMap[row][i+1])
				break;
		}
		else if( chessFlag == chessMap[row][i] )
			num2 ? ++right2 : ++right1;
		else
			break;
	}

	if( left1 + left2 + right1 >= 2 )
	{
		if( 0 == num1 && 2 == left1+right1 && num2 )
			++chong3;
		if( num1 && left1 < 2 && right1 < 2  && left2 )
			++chong3;
	}
	if( right1 + right2 + left1 >= 2 )
	{
		if( 0 == num2 && 2 == left1+right1 && num1)
			++chong3;
		if( num2 && left1 < 2 && right1 < 2 && right2 )
			++chong3;
	}
	if( left1 + left2 + right1 >= 3 )
	{
		if( 0 == num1 && 3 == left1+right1 && num2 )
			++chong4;
		if( num1 && left1 < 3 && right1 < 3 && left2 )
			++tiaochong4;
	}
	if( right1 + right2 + left1 >= 3 ) 
	{ 
		if( 0 == num2 && 3 == left1+right1 && num1 )
			++chong4;
		if( num2 && left1 < 3 && right1 < 3 && right2 )
			++tiaochong4;	
	}
	
	
	// �� 
	num1 = num2 = 0;
	left1 = left2 = right1 =right2 = 0;
	for(i=row-1; i>=0; --i)
	{
		if( SPACE == chessMap[i][col] )
		{
			++num1;
			if( num1 >= 2 )
				break;
			if(i-1 >= 0 && SPACE == chessMap[i-1][col])
				break;
		}
		else if( chessFlag == chessMap[i][col] )
			num1 ? ++left2 : ++left1;
		else
			break;
	}
	for(i=row+1; i<SIZE; ++i)
	{
		if( SPACE == chessMap[i][col])
		{
			++num2;
			if( num2 >= 2 )
				break;
			if(i+1 < SIZE && SPACE == chessMap[i+1][col])
				break;
		}
		else if( chessFlag == chessMap[i][col] )
			num2 ? ++right2 : ++right1;
		else
			break;
	}
	
	if( left1 + left2 + right1 >= 2 )
	{
		if( 0 == num1 && 2 == left1+right1 && num2 )
			++chong3;
		if( num1 && left1 < 2 && right1 < 2  && left2 )
			++chong3;
	}
	if( right1 + right2 + left1 >= 2 )
	{
		if( 0 == num2 && 2 == left1+right1 && num1)
			++chong3;
		if( num2 && left1 < 2 && right1 < 2 && right2 )
			++chong3;
	}
	if( left1 + left2 + right1 >= 3 )
	{
		if( 0 == num1 && 3 == left1+right1 && num2 )
			++chong4;
		if( num1 && left1 < 3 && right1 < 3 && left2 )
			++tiaochong4;
	}
	if( right1 + right2 + left1 >= 3 ) 
	{ 
		if( 0 == num2 && 3 == left1+right1 && num1 )
			++chong4;
		if( num2 && left1 < 3 && right1 < 3 && right2 )
			++tiaochong4;	
	}
		
	// ' \ '
	num1 = num2 = 0;
	left1 = left2 = right1 = right2 = 0;
	for(i=row-1,j=col-1; i>=0 && j>=0; --i, --j)
	{
		if( SPACE == chessMap[i][j] )
		{
			++num1;
			if( num1 >= 2 )
				break;
			if(i-1 >= 0 && j-1 >= 0 && SPACE == chessMap[i-1][j-1] )
				break;
		}
		else if( chessFlag == chessMap[i][j] )
			num1 ? ++left2 : ++left1;
		else
			break;
	}
	for(i=row+1,j=col+1; i<SIZE && j<SIZE; ++i, ++j)
	{
		if( SPACE == chessMap[i][j] )
		{
			++num2;
			if( num2 >= 2)
				break;
			if(i+1 < SIZE && j+1 < SIZE && SPACE == chessMap[i+1][j+1])
				break;
		}
		else if( chessFlag == chessMap[i][j] )
			num2 ? ++right2 : ++right1;
		else
			break;
	}
	
	if( left1 + left2 + right1 >= 2 )
	{
		if( 0 == num1 && 2 == left1+right1 && num2 )
			++chong3;
		if( num1 && left1 < 2 && right1 < 2  && left2 )
			++chong3;
	}
	if( right1 + right2 + left1 >= 2 )
	{
		if( 0 == num2 && 2 == left1+right1 && num1)
			++chong3;
		if( num2 && left1 < 2 && right1 < 2 && right2 )
			++chong3;
	}
	if( left1 + left2 + right1 >= 3 )
	{
		if( 0 == num1 && 3 == left1+right1 && num2 )
			++chong4;
		if( num1 && left1 < 3 && right1 < 3 && left2 )
			++tiaochong4;
	}
	if( right1 + right2 + left1 >= 3 ) 
	{ 
		if( 0 == num2 && 3 == left1+right1 && num1 )
			++chong4;
		if( num2 && left1 < 3 && right1 < 3 && right2 )
			++tiaochong4;	
	}
	
	// ' / '
	num1 = num2 = 0;
	left1 = left2 = right1 = right2 = 0;
	for(i=row-1,j=col+1; i>=0 && j<SIZE; --i, ++j)
	{
		if( SPACE == chessMap[i][j] )
		{
			++num1;
			if( num1 >= 2 )
				break;
			if(i-1 >= 0 && j+1 < SIZE && SPACE == chessMap[i-1][j+1])
				break;
		}
		else if( chessFlag == chessMap[i][j] )
			num1 ? ++left2 : ++left1;
		else
			break;
	} 
	for(i=row+1,j=col-1; i<SIZE && j>=0; ++i, --j)
	{
		if( SPACE == chessMap[i][j] )
		{
			++num2;
			if( num2 >= 2)
				break;
			if(i+1 < SIZE && j-1 >= 0 && SPACE == chessMap[i+1][j-1])
				break;
		}
		else if( chessFlag == chessMap[i][j] )
			num2 ? ++right2 : ++right1;
		else
			break;
	}
	
	if( left1 + left2 + right1 >= 2 )
	{
		if( 0 == num1 && 2 == left1+right1 && num2 )
			++chong3;
		if( num1 && left1 < 2 && right1 < 2  && left2 )
			++chong3;
	}
	if( right1 + right2 + left1 >= 2 )
	{
		if( 0 == num2 && 2 == left1+right1 && num1)
			++chong3;
		if( num2 && left1 < 2 && right1 < 2 && right2 )
			++chong3;
	}
	if( left1 + left2 + right1 >= 3 )
	{
		if( 0 == num1 && 3 == left1+right1 && num2 )
			++chong4;
		if( num1 && left1 < 3 && right1 < 3 && left2 )
			++tiaochong4;
	}
	if( right1 + right2 + left1 >= 3 ) 
	{ 
		if( 0 == num2 && 3 == left1+right1 && num1 )
			++chong4;
		if( num2 && left1 < 3 && right1 < 3 && right2 )
			++tiaochong4;	
	}

	return pair<int,pair<int,int> >(chong3,pair<int,int>(chong4,tiaochong4));
}


// ������chessMap[row][col] ���Ӻ󣬷��ع�����2����3������ 
// �ϸ��� "����"����Ϊ���ֹ�����Ҫ��"����"�Ļ�û��Ҫ�ǳ��ϸ�
pair<int,int> FiveChess::GetLian2Lian3(int row,int col,char chessFlag)
{
	int lian2=0,lian3=0,left,right,i,j,num1,num2;
	bool flag;
	
	// �� 
	for(i=col-1, left=0; i>=0 && chessFlag == chessMap[row][i]; --i, ++left);
	for(i=col+1, right=0; i<SIZE && chessFlag == chessMap[row][i]; ++i, ++right); 
	
	if( 1 == left + right )
	{
		for(i=col-left-1, num1=0; i>=0 && SPACE == chessMap[row][i]; --i, ++num1);
		for(i=col+right+1, num2=0; i<SIZE && SPACE == chessMap[row][i]; ++i, ++num2);
		if( num1 && num2 && num1 + num2 >= 4)
			++lian2;
	}

	if( 2 == left + right )
	{
		flag = true;
		if( col-left-2 >= 0 && chessFlag == chessMap[row][col-left-2] ) // �������������ǳ�����
			flag = false;
		if( col+right+2 < SIZE && chessFlag == chessMap[row][col+right+2] ) // �������������ǳ�����
			flag = false;

		if( flag )
		{
			for(i=col-left-1, num1=0; i>=0 && SPACE == chessMap[row][i]; --i, ++num1);
			for(i=col+right+1, num2=0; i<SIZE && SPACE == chessMap[row][i]; ++i, ++num2);
			if( num1 && num2 && num1 + num2 >= 3)
				++lian3;
		}	
	}
	
	// ��
	for(i=row-1, left=0; i>=0 && chessFlag == chessMap[i][col]; --i, ++left); 
	for(i=row+1, right=0; i<SIZE && chessFlag == chessMap[i][col]; ++i, ++right);
	
	if( 1 == left + right )
	{
		for(i=row-left-1, num1=0; i>=0 && SPACE == chessMap[i][col]; --i, ++num1);
		for(i=row+right+1, num2=0; i<SIZE && SPACE == chessMap[i][col]; ++i, ++num2);
		if( num1 && num2 && num1 + num2 >= 4)
			++lian2;	
	}
	if( 2 == left + right )
	{
		flag = true;
		if( row-left-2 >= 0 && chessFlag == chessMap[row-left-2][col] ) // �������������ǳ�����
			flag = false;
		if( row+right+2 < SIZE && chessFlag == chessMap[row+right+2][col] ) // �������������ǳ�����
			flag = false;

		if( flag )
		{
			for(i=row-left-1, num1=0; i>=0 && SPACE == chessMap[i][col]; --i, ++num1);
			for(i=row+right+1, num2=0; i<SIZE && SPACE == chessMap[i][col]; ++i, ++num2);
			if( num1 && num2 && num1 + num2 >= 3)
				++lian3;
		}
	}
	
	// ' \ '
	for(i=row-1, j=col-1, left=0; i>=0 && j>=0 && chessFlag == chessMap[i][j]; --i, --j, ++left);
	for(i=row+1, j=col+1, right=0; i<SIZE && j<SIZE && chessFlag == chessMap[i][j]; ++i, ++j, ++right);
	
	if( 1 == left + right )
	{
		for(i=row-left-1, j=col-left-1, num1=0; i>=0 && j>=0 && SPACE == chessMap[i][j]; --i, --j, ++num1);
		for(i=row+right+1, j=col+right+1, num2=0; i<SIZE && j<SIZE && SPACE == chessMap[i][j]; ++i, ++j, ++num2);
		if( num1 && num2 && num1 + num2 >= 4)
			++lian2;	
	}
	if( 2 == left + right )
	{
		flag = true;
		if( row-left-2 >= 0 && col-left-2 >= 0 && chessFlag == chessMap[row-left-2][col-left-2] ) // �������������ǳ�����
			flag = false;
		if( row+right+2 < SIZE && col+right+2 < SIZE && chessFlag == chessMap[row+right+2][col+right+2] ) // �������������ǳ�����
			flag = false;

		if( flag )
		{
			for(i=row-left-1, j=col-left-1, num1=0; i>=0 && j>=0 && SPACE == chessMap[i][j]; --i, --j, ++num1);
			for(i=row+right+1, j=col+right+1, num2=0; i<SIZE && j<SIZE && SPACE == chessMap[i][j]; ++i, ++j, ++num2);
			if( num1 && num2 && num1 + num2 >= 3)
				++lian3;
		}
	}
	
	// ' / '
	for(i=row-1, j=col+1, left=0; i>=0 && j<SIZE && chessFlag == chessMap[i][j]; --i, ++j, ++left);
	for(i=row+1, j=col-1, right=0; i<SIZE && j>=0 && chessFlag == chessMap[i][j]; ++i, --j, ++right);
	
	if( 1 == left + right )
	{
		for(i=row-left-1, j=col+left+1, num1=0; i>=0 && j<SIZE && SPACE == chessMap[i][j]; --i, ++j, ++num1);
		for(i=row+right+1, j=col-right-1, num2=0; i<SIZE && j>=0 && SPACE == chessMap[i][j]; ++i, --j, ++num2);
		if( num1 && num2 && num1 + num2 >= 4)
			++lian2;	
	}
	if( 2 == left + right )
	{
		flag = true;
		if( row-left-2 >= 0 && col+left+2 < SIZE && chessFlag == chessMap[row-left-2][col+left+2] ) // �������������ǳ�����
			flag = false;
		if( row+right+2 < SIZE && col-right-2 >= 0 && chessFlag == chessMap[row+right+2][col-right-2] ) // �������������ǳ�����
			flag = false;

		if( flag )
		{
			for(i=row-left-1, j=col+left+1, num1=0; i>=0 && j<SIZE && SPACE == chessMap[i][j]; --i, ++j, ++num1);
			for(i=row+right+1, j=col-right-1, num2=0; i<SIZE && j>=0 && SPACE == chessMap[i][j]; ++i, --j, ++num2);
			if( num1 && num2 && num1 + num2 >= 3)
				++lian3;
		}
	}
	
	return pair<int,int>(lian2,lian3);
}


// ������chessMap[row][col] ���Ӻ󣬷��ع�����2����3������ 
// �ϸ��� "����"����Ϊ���ֹ�����Ҫ�������Ļ�û��Ҫ�ǳ��ϸ�
pair<int,int> FiveChess::GetTiao2Tiao3(int row,int col,char chessFlag)
{
	int tiao2=0,tiao3=0,i,j,num1,num2;
	int left1,left2,right1,right2;  // ����� GetChong3Chong4() �еĺ��� ��ͬ 
	bool flag;

	// ��
	num1 = num2 = 0;
	left1 = left2 = right1 = right2 = 0;
	
	for(i=col-1; i>=0; --i)
	{
		if( SPACE == chessMap[row][i] )
		{
			++num1;
			if( num1 >= 2 )
				break;
			if(i-1 >= 0 && SPACE == chessMap[row][i-1])
				break;
		}
		else if( chessFlag == chessMap[row][i] )
			num1 ? ++left2 : ++left1;
		else
			break;
	}
	 // ����������һ�� 
	for(i=col+1; i<SIZE; ++i)
	{
		if( SPACE == chessMap[row][i] )
		{
			++num2;
			if( num2 >= 2 )
				break;
			if(i+1 < SIZE && SPACE == chessMap[row][i+1])
				break;
		}
		else if( chessFlag == chessMap[row][i] )
			num2 ? ++right2 : ++right1;
		else
			break;
	}
	
	if( 1 == left1+left2+right1 && 2 == num1 && col-left1-left2-2 >=0 && col+right1+1 < SIZE
		&& SPACE == chessMap[row][col-left1-left2-2] && SPACE == chessMap[row][col+right1+1] )
		++tiao2;
	if( 1 == right1+right2+left1 && 2 == num2 && col-left1-1 >= 0 && col+right1+right2+2 < SIZE
		&& SPACE == chessMap[row][col-left1-1] && SPACE == chessMap[row][col+right1+right2+2] )
		++tiao2;

	if( 2 == left1+left2+right1 && 2 == num1 && col-left1-left2-2 >=0 && col+right1+1 < SIZE
		&& SPACE == chessMap[row][col-left1-left2-2] && SPACE == chessMap[row][col+right1+1] )	
	{
		flag = true;
		if( haveKinjite && BLACK_CHESS == chessFlag )
		{
			if( col-left1-left2-3 >= 0 && chessFlag == chessMap[row][col-left1-left2-3] )
				flag = false;
			if( col+right1+2 < SIZE && chessFlag == chessMap[row][col+right1+2] )
				flag = false;	
		}
		if( flag )
			++tiao3;
	}
	if( 2 == right1+right2+left1 && 2 == num2 && col-left1-1 >= 0 && col+right1+right2+2 < SIZE
		&& SPACE == chessMap[row][col-left1-1] && SPACE == chessMap[row][col+right1+right2+2] )
	{
		flag = true;
		if( haveKinjite && BLACK_CHESS == chessFlag )
		{
			if( col-left1-2 >= 0 && chessFlag == chessMap[row][col-left1-2] )
				flag = false;
			if( col+right1+right2+3 < SIZE && chessFlag == chessMap[row][col+right1+right2+3] )
				flag = false;
		}
		if( flag )
			++tiao3;
	}
	
	// �� 
	num1 = num2 = 0;
	left1 = left2 = right1 = right2 = 0;
	for(i=row-1; i>=0; --i)
	{
		if( SPACE == chessMap[i][col] )
		{
			++num1;
			if( num1 >= 2 )
				break;
			if(i-1 >= 0 && SPACE == chessMap[i-1][col])
				break;
		}
		else if( chessFlag == chessMap[i][col] )
			num1 ? ++left2 : ++left1;
		else
			break;
	}
	for(i=row+1; i<SIZE; ++i)
	{
		if( SPACE == chessMap[i][col])
		{
			++num2;
			if( num2 >= 2 )
				break;
			if(i+1 < SIZE && SPACE == chessMap[i+1][col])
				break;
		}
		else if( chessFlag == chessMap[i][col] )
			num2 ? ++right2 : ++right1;
		else
			break;
	}
	
	if( 1 == left1+left2+right1 && 2 == num1 && row-left1-left2-2 >=0 && row+right1+1 < SIZE
		&& SPACE == chessMap[row-left1-left2-2][col] && SPACE == chessMap[row+right1+1][col] )
		++tiao2;
	if( 1 == right1+right2+left1 && 2 == num2 && row-left1-1 >= 0 && row+right1+right2+2 < SIZE
		&& SPACE == chessMap[row-left1-1][col] && SPACE == chessMap[row+right1+right2+2][col] )
		++tiao2;

	if( 2 == left1+left2+right1 && 2 == num1 && row-left1-left2-2 >=0 && row+right1+1 < SIZE
		&& SPACE == chessMap[row-left1-left2-2][col] && SPACE == chessMap[row+right1+1][col] )
	{
		flag = true;
		if( haveKinjite && BLACK_CHESS == chessFlag )
		{
			if( row-left1-left2-3 >= 0 && chessFlag == chessMap[row-left1-left2-3][col] )
				flag = false;
			if( row+right1+2 < SIZE && chessFlag == chessMap[row+right1+2][col] )
				flag = false;
		}
		if( flag )
			++tiao3;
	}
	if( 2 == right1+right2+left1 && 2 == num2 && row-left1-1 >= 0 && row+right1+right2+2 < SIZE
		&& SPACE == chessMap[row-left1-1][col] && SPACE == chessMap[row+right1+right2+2][col] )
		
	{
		flag = true;
		if( haveKinjite && BLACK_CHESS == chessFlag )
		{
			if( row-left1-2 >= 0 && chessFlag == chessMap[row-left1-2][col] )
				flag = false;
			if( row+right1+right2+3 < SIZE && chessFlag == chessMap[row+right1+right2+3][col] )
				flag = false;
		}
		if( flag )
			++tiao3;
	}
	 
	// ' \ '
	num1 = num2 = 0;
	left1 = left2 = right1 = right2 = 0;
	for(i=row-1,j=col-1; i>=0 && j>=0; --i, --j)
	{
		if( SPACE == chessMap[i][j] )
		{
			++num1;
			if( num1 >= 2 )
				break;
			if(i-1 >= 0 && j-1 >= 0 && SPACE == chessMap[i-1][j-1] )
				break;
		}
		else if( chessFlag == chessMap[i][j] )
			num1 ? ++left2 : ++left1;
		else
			break;
	}
	for(i=row+1,j=col+1; i<SIZE && j<SIZE; ++i, ++j)
	{
		if( SPACE == chessMap[i][j] )
		{
			++num2;
			if( num2 >= 2)
				break;
			if(i+1 < SIZE && j+1 < SIZE && SPACE == chessMap[i+1][j+1])
				break;
		}
		else if( chessFlag == chessMap[i][j] )
			num2 ? ++right2 : ++right1;
		else
			break;
	}
	
	if( 1 == left1+left2+right1 && 2 == num1 && row-left1-left2-2 >=0 && row+right1+1 < SIZE
		&& col-left1-left2-2 >= 0 && col+right1+1 < SIZE
		&& SPACE == chessMap[row-left1-left2-2][col-left1-left2-2] 
		&& SPACE == chessMap[row+right1+1][col+right1+1] )
		++tiao2;
	if( 1 == right1+right2+left1 && 2 == num2 && row-left1-1 >= 0 && row+right1+right2+2 < SIZE
		&& col-left1-1 >= 0 && col+right1+right2+2 < SIZE
		&& SPACE == chessMap[row-left1-1][col-left1-1] 
		&& SPACE == chessMap[row+right1+right2+2][col+right1+right2+2] )
		++tiao2;

	if( 2 == left1+left2+right1 && 2 == num1 && row-left1-left2-2 >=0 && row+right1+1 < SIZE
		&& col-left1-left2-2 >= 0 && col+right1+1 < SIZE
		&& SPACE == chessMap[row-left1-left2-2][col-left1-left2-2] 
		&& SPACE == chessMap[row+right1+1][col+right1+1] )
	{
		flag = true;
		if( haveKinjite && BLACK_CHESS == chessFlag )
		{
			if( row-left1-left2-3 >= 0 && col-left1-left2-3 >= 0 && chessFlag == chessMap[row-left1-left2-3][col-left1-left2-3] )
				flag = false;
			if( row+right1+2 < SIZE && col+right1+2 < SIZE && chessFlag == chessMap[row+right1+2][col+right1+2] )
				flag = false;
		}
		if( flag )
			++tiao3;
	}
	if( 2 == right1+right2+left1 && 2 == num2 && row-left1-1 >= 0 && row+right1+right2+2 < SIZE
		&& col-left1-1 >= 0 && col+right1+right2+2 < SIZE
		&& SPACE == chessMap[row-left1-1][col-left1-1] 
		&& SPACE == chessMap[row+right1+right2+2][col+right1+right2+2] )	
	{
		flag = true;
		if( haveKinjite && BLACK_CHESS == chessFlag )
		{
			if( row-left1-2 >= 0 && col-left1-2 >= 0 && chessFlag == chessMap[row-left1-2][col-left1-2] )
				flag = false;
			if( row+right1+right2+3 < SIZE && col+right1+right2+3 < SIZE && chessFlag == chessMap[row+right1+right2+3][col+right1+right2+3] )
				flag = false;
		}
		if( flag )
			++tiao3;
	}
	
	// ' / '
	num1 = num2 = 0;
	left1 = left2 = right1 = right2 = 0;
	for(i=row-1,j=col+1; i>=0 && j<SIZE; --i, ++j)
	{
		if( SPACE == chessMap[i][j] )
		{
			++num1;
			if( num1 >= 2 )
				break;
			if(i-1 >= 0 && j+1 < SIZE && SPACE == chessMap[i-1][j+1])
				break;
		}
		else if( chessFlag == chessMap[i][j] )
			num1 ? ++left2 : ++left1;
		else
			break;
	} 
	for(i=row+1,j=col-1; i<SIZE && j>=0; ++i, --j)
	{
		if( SPACE == chessMap[i][j] )
		{
			++num2;
			if( num2 >= 2)
				break;
			if(i+1 < SIZE && j-1 >= 0 && SPACE == chessMap[i+1][j-1])
				break;
		}
		else if( chessFlag == chessMap[i][j] )
			num2 ? ++right2 : ++right1;
		else
			break;
	}
	
	if( 1 == left1+left2+right1 && 2 == num1 && row-left1-left2-2 >=0 && col+left1+left2+2 < SIZE
		&& row+right1+1 < SIZE && col-right1-1 >= 0
		&& SPACE == chessMap[row-left1-left2-2][col+left1+left2+2] 
		&& SPACE == chessMap[row+right1+1][col-right1-1] )
		++tiao2;
	if( 1 == right1+right2+left1 && 2 == num2 && row-left1-1 >= 0 && col+left1+1 < SIZE
		&& row+right1+right2+2 < SIZE && col-right1-right2-2 >= 0
		&& SPACE == chessMap[row-left1-1][col+left1+1] 
		&& SPACE == chessMap[row+right1+right2+2][col-right1-right2-2] )
		++tiao2;

	if( 2 == left1+left2+right1 && 2 == num1 && row-left1-left2-2 >=0 && col+left1+left2+2 < SIZE
		&& row+right1+1 < SIZE && col-right1-1 >= 0
		&& SPACE == chessMap[row-left1-left2-2][col+left1+left2+2] 
		&& SPACE == chessMap[row+right1+1][col-right1-1] )
	{
		flag = true;
		if( haveKinjite && BLACK_CHESS == chessFlag )
		{
			if( row-left1-left2-3 >= 0 && col+left1+left2+3 < SIZE && chessFlag == chessMap[row-left1-left2-3][col+left1+left2+3] )
				flag = false;
			if( row+right1+2 < SIZE && col-right1-2 >= 0 && chessFlag == chessMap[row+right1+2][col-right1-2] )
				flag = false;
		}
		if( flag )
			++tiao3;
	}
	if( 2 == right1+right2+left1 && 2 == num2 && row-left1-1 >= 0 && col+left1+1 < SIZE
		&& row+right1+right2+2 < SIZE && col-right1-right2-2 >= 0
		&& SPACE == chessMap[row-left1-1][col+left1+1] 
		&& SPACE == chessMap[row+right1+right2+2][col-right1-right2-2] )
	{
		flag = true;
		if( haveKinjite && BLACK_CHESS == chessFlag )
		{
			if( row-left1-2 >= 0 && col+left1+2 < SIZE && chessFlag == chessMap[row-left1-2][col+left1+2] )
				flag = false;
			if( row+right1+right2+3 < SIZE && col-right1-right2-3 >= 0 && chessFlag == chessMap[row+right1+right2+3][col-right1-right2-3] )
				flag = false;
		}
		if( flag )
			++tiao3;
	}
	return pair<int,int>(tiao2,tiao3);
}

// С���͵��ж�
void FiveChess::OntherChessType(int row,int col,char chessFlag)
{
	chong2 = huo1 = chong1 = 0; 
	int i,j,num[4],border[4][2];
	
	memset(num,0,sizeof(num));
	memset(border,1,sizeof(border));
	
	// ��
	for(i=col-1; i>=0; --i)
	{
		if( chessFlag == chessMap[row][i] )
		{ 
			++num[0];
			continue; 
		} 
		if( SPACE == chessMap[row][i] )
		{
			border[0][0] = 0;
			break;
		}
		else
		{
			border[0][0] = 1;
			break;
		}
	}
	for(i=col+1; i<SIZE; ++i)
	{
		if( chessFlag == chessMap[row][i] )
		{
			++num[0];
			continue;
		}
		if( SPACE == chessMap[row][i] )
		{
			border[0][1] = 0;
			break;
		}
		else
		{
			border[0][1] = 1;
			break;
		}
	}
	
	// ��
	for(i=row-1; i>=0; --i)
	{
		if( chessFlag == chessMap[i][col] )
		{
			++num[1];
			continue;
		}	
		if( SPACE == chessMap[i][col] )
		{
			border[1][0] = 0;
			break;
		}
		else
		{
			border[1][0] = 1;
			break;
		}
	} 
	for(i=row+1; i<SIZE; ++i)
	{
		if( chessFlag == chessMap[i][col] )
		{
			++num[1];
			continue;
		}
		if( SPACE == chessMap[i][col] )
		{
			border[1][1] = 0;
			break;
		}
		else
		{
			border[1][1] = 1;
			break;
		}
	}
	
	// ' \ '
	for(i=row-1, j=col-1; i>=0 && j>=0; --i, --j)
	{
		if( chessFlag == chessMap[i][j] )
		{
			++num[2];
			continue;
		}
		if( SPACE == chessMap[i][j] )
		{
			border[2][0] = 0;
			break;
		}
		else
		{
			border[2][0] = 1;
			break;
		}
	}
	for(i=row+1, j=col+1; i<SIZE && j<SIZE; ++i, ++j)
	{
		if( chessFlag == chessMap[i][j] )
		{
			++num[2];
			continue;
		}
		if( SPACE == chessMap[i][j] )
		{
			border[2][1] = 0;
			break;
		}
		else
		{
			border[2][1] = 1;
			break;
		}
	}
	
	// ' / '
	for(i=row-1, j=col+1; i>=0 && j<SIZE; --i, ++j)
	{
		if( chessFlag == chessMap[i][j] )
		{
			++num[3];
			continue;
		}
		if( SPACE == chessMap[i][j] )
		{
			border[3][0] = 0;
			break;
		}
		else
		{
			border[3][0] = 1;
			break;
		}
	}
	for(i=row+1, j=col-1; i<SIZE && j>=0; ++i, --j)
	{
		if( chessFlag == chessMap[i][j] )
		{
			++num[3];
			continue;
		}
		if( SPACE == chessMap[i][j] )
		{
			border[3][1] = 0;
			break;
		}
		else
		{
			border[3][1] = 1;
			break;
		}
	}
	
	// �����ж����� 
	for(i=0; i<4; ++i)
	{
		if( 1 == num[i] && 1 == ( border[i][0] + border[i][1] ) ) 
			++chong2;
		else if( 0 == num[i] && 0 == ( border[i][0] + border[i][1] ) )
			++huo1;
		else if( 0 == num[i] && 1 == ( border[i][0] + border[i][1] ) )
			++chong1;
	} 
}

/***************************************************************/

//���� chessMap[row][col] ����ܹ���� chessFlag ɫ�����ܹ�������Щ����
void FiveChess::Search(int row,int col,char chessFlag)
{
	//��ʼ��
	changlian = cheng5 = huo4 = chong4 = tiaochong4 = lian3 = tiao3 = chong3 = lian2 = tiao2 = 0;
	int res;
	pair<int,int> pi;
	pair<int,pair<int,int> > pip;

	// ����������
	res = GetCheng5ChangLian(row,col,chessFlag);
	if( 1 == res )
		cheng5 = 1;
	else if( 2 == res )
		changlian = 1;

	// ����
	huo4 = GetHuo4(row,col,chessFlag);

	// ����������
	pip = GetChong3Chong4(row,col,chessFlag);
	chong3 = pip.first;
	chong4 = pip.second.first;
	tiaochong4 = pip.second.second;

	// ������ ���� ���� �� ����
	// ����� ���� ���� �� ���� ���� ���ﻹ�ǰ����� ������ �ֿ��ˣ�����ʱ��Ҫ��
	pi = GetLian2Lian3(row,col,chessFlag);
	lian2 = pi.first;
	lian3 = pi.second;

	pi = GetTiao2Tiao3(row,col,chessFlag);
	tiao2 = pi.first;
	tiao3 = pi.second;
}

// ͨ�������λ�üӷ���ʵ�� ���ضԷ��Ľ�������
// ԭ������ֵ���������Χ�ж��ٿո�ͶԷ������ӹҹ�
int FiveChess::AddScore(int row,int col,char chessFlag)
{
	int score = 0;
	int s = 0,y = 0,m = 0; 
	char other = (BLACK_CHESS == chessFlag ? WHITE_CHESS : BLACK_CHESS);

	for(int i= max(0,row-2); i<= min(SIZE-1,row+2); ++i)
	{
		for(int j= max(0,col-2); j<= min(SIZE-1,col+2); ++j)	
		{
			if(SPACE == chessMap[i][j])
				++s;
			else if( other == chessMap[i][j])
				++y;
			else
				++m;
		}
	}

	score = y*30 + s*10 + m*10;
	return score;
}
/*
     ������ƺ����Ļ�����ͬʱ�����˽�����ǽ��֡�
*/
/*
	������3�ͳ�3����4��һ�����͵ĸ������������е㲻�ɱ�������⣬���컯��
	����4�����˳�3��.....
*/
int FiveChess::GetScore(int row,int col,char chessFlag,char flag)
{
	int score = 10;
	
	if( chessFlag == flag ) // �� �Լ�����
	{
		if( BLACK_CHESS == chessFlag && IsKinjite(row,col) ) //���Լ��Ľ��ֵ� 
			return -1;
		Search(row,col,chessFlag); //
		if(changlian || cheng5)
			score += 5000000;
		if(huo4)
			score += 250000; 
		if(chong4+tiaochong4 >= 2)
			score += 250000;
		if((lian3+tiao3) && (chong4+tiaochong4))
			score += 200000;
		if(lian3+tiao3 >= 2)
			score += 8000;

		score += chong4 *230; 
		score += tiaochong4*110;
		score += lian3 * 320;
		score += tiao3 * 150; //tiao3�ض������һ��tiao2�ķ�
		score += lian2* 160;
		score += tiao2 * 150;
		
		if((chong4+tiaochong4+huo4) && (lian2+tiao2>=2) )
			score += 220;
		if((lian3+tiao3) && (lian2+tiao2>=2) )
			score += 200;
		if(chong3 && chong4)
			score += 100;
		if(chong3>=2 && tiaochong4)
			score += 100;
		if(chong3 >= 2)
			score += 100;
		if(chong3 && (lian2+tiao2))
			score += 50;
		
		score += chong3 * 40;
		score += chong1 * 20;
		score += huo1 * 10;
		score += chong2 * 20;
	}
	else    // �� �Է�����
	{
		if( BLACK_CHESS == chessFlag && IsKinjite(row,col) ) // �ǶԷ��Ľ��ֵ�
			return -1;
		Search(row,col,chessFlag); //
		if(changlian || cheng5)
			score += 1000000;
		if(huo4)
			score += 45000;
		if(chong4+tiaochong4 >= 2)
			score += 45000;
		if((lian3+tiao3) && (chong4+tiaochong4))
			score += 40000;
		if(lian3+tiao3 >= 2)
			score += 1900;

		score += chong4 *230; 
		score += tiaochong4*110;
		score += lian3 * 320;
		score += tiao3 * 150; //tiao3�ض������һ��tiao2�ķ�
		score += lian2* 160;
		score += tiao2 * 150;
		
		if((chong4+tiaochong4+huo4) && (lian2+tiao2>=2) )
			score += 220;
		if((lian3+tiao3) && (lian2+tiao2>=2) )
			score += 200;
		if(chong3 && chong4)
			score += 100;
		if(chong3>=2 && tiaochong4)
			score += 100;
		if(chong3 >= 2)
			score += 100;
		if(chong3 && (lian2+tiao2))
			score += 50;
		
		score += chong3 * 40;
		score += chong1 * 20;
		score += huo1 * 10;
		score += chong2 * 20;
	}

	if( score >= 1900 || isDefend )
		score += AddScore(row,col,chessFlag); //
	return score;
}

//�õ� chessMap[row][col] ������ chessFlag ɫ����ļ�ֵ
int FiveChess::GetScoreForComputer(int row,int col,char chessFlag)
{
	return GetScore(row,col,chessFlag,cComputer);
}

//�õ� chessMap[row][col] ������ chessFlag ɫ����ļ�ֵ
int FiveChess::GetScoreForPeople(int row,int col,char chessFlag)
{
	return GetScore(row,col,chessFlag,cPeople);
}


//�õ������Ӧ������ĵ�ǰ���λ��
void FiveChess::ComputerPlay()
{
	pair<int,int>pi = GetCurrentMaxPoint(cComputer);
	comX = pi.first;
	comY = pi.second;
	int tempX = comX,tempY = comY;

	// ���ڶԷ��� ��3����3�������Ƿ��ػ�4�㣬���ǳ�4�㣬�����Ҫ��һ���ж�
	// ������3����2���������ӵ㣬������3����3���������ӵ�
	// ���ｫ�� 4 �����һ���ķ�ֵ��45000��
	if( 2 == winPeople && ( !winComputer || winComputer>winPeople) )
	{
		if( GetHuo4(comX,comY,cPeople) ) //�˹��ɻ�4
		{
			int score = GetScoreForComputer(comX,comY,cPeople); //�õ�����
			score += GetScoreForComputer(comX,comY,cComputer);
			//////////////////////////////////////////////////////////////////////////
			int row = comX,col = comY;
			int i,j,left,right;
			bool flag;
			char chessFlag = cPeople;

			// �� 
			for(i=col-1, left=0; i>=0 && chessFlag == chessMap[row][i]; --i, ++left);
			for(i=col+1, right=0; i<SIZE && chessFlag == chessMap[row][i]; ++i, ++right); 
			
			if( 3 == left + right && col-left-1 >= 0 && SPACE == chessMap[row][col-left-1]
				&& col+right+1 < SIZE && SPACE == chessMap[row][col+right+1] )
			{	
				flag = true;
				if( haveKinjite && BLACK_CHESS == chessFlag )
				{
					if( col-left-2 >= 0 && chessFlag == chessMap[row][col-left-2] )
						flag = false;
					if( col+right+2 < SIZE && chessFlag == chessMap[row][col+right+2] )
						flag = false;
				}
				if( flag ) 
				{
					int s1 = 0,s2 = 0;
					if( left )
						s1 = GetScoreForComputer(row,col-left-1,cComputer)+45000;
					if( right )
						s2 = GetScoreForComputer(row,col+right+1,cComputer)+45000;

					if(s1>score)
					{
						tempX = row, tempY = col-left-1, score = s1;
					}
					if(s2>score)
					{
						tempX = row, tempY = col+right+1, score = s2;
					}
				}
			}

			// ��
			for(i=row-1, left=0; i>=0 && chessFlag == chessMap[i][col]; --i, ++left); 
			for(i=row+1, right=0; i<SIZE && chessFlag == chessMap[i][col]; ++i, ++right);
			
			if( 3 == left + right && row-left-1 >= 0 && SPACE == chessMap[row-left-1][col]
				&& row+right+1 < SIZE && SPACE == chessMap[row+right+1][col] )
			{
				flag = true;
				if( haveKinjite && BLACK_CHESS == chessFlag )
				{
					if( row-left-2 >= 0 && chessFlag == chessMap[row-left-2][col] )
						flag = false;
					if( row+right+2 < SIZE && chessFlag == chessMap[row+right+2][col] )
						flag = false;
				}
				if( flag )
				{
					int s1 = 0,s2 = 0;
					if( left )
						s1 = GetScoreForComputer(row-left-1,col,cComputer)+45000;
					if( right )
						s2 = GetScoreForComputer(row+right+1,col,cComputer)+45000;
	
					if(s1>score)
					{
						tempX = row-left-1, tempY = col, score = s1;
					}
					if(s2>score)
					{
						tempX = row+right+1, tempY = col, score = s2;
					}
				}
			}

			// ' \ '
			for(i=row-1, j=col-1, left=0; i>=0 && j>=0 && chessFlag == chessMap[i][j]; --i, --j, ++left);
			for(i=row+1, j=col+1, right=0; i<SIZE && j<SIZE && chessFlag == chessMap[i][j]; ++i, ++j, ++right);
			if( 3 == left + right && row-left-1 >= 0 && col-left-1 >= 0 && SPACE == chessMap[row-left-1][col-left-1]
				&& row+right+1 < SIZE && col+right+1 < SIZE && SPACE == chessMap[row+right+1][col+right+1])
			{
				flag = true;
				if( haveKinjite && BLACK_CHESS == chessFlag )
				{
					if( row-left-2 >= 0 && col-left-2 >= 0 && chessFlag == chessMap[row-left-2][col-left-2] )
						flag = false;
					if( row+right+2 < SIZE && col+right+2 < SIZE && chessFlag == chessMap[row+right+2][col+right+2] )
						flag = false;
				}
				if( flag )
				{
					int s1 = 0,s2 = 0;
					if( left )
						s1 = GetScoreForComputer(row-left-1,col-left-1,cComputer)+45000;
					if( right )
						s2 = GetScoreForComputer(row+right+1,col+right+1,cComputer)+45000;
					
					if(s1>score)
					{
						tempX = row-left-1, tempY = col-left-1, score = s1;
					}
					if(s2>score)
					{
						tempX = row+right+1, tempY = col+right+1, score = s2;
					}
				}
			}
			
			// ' / '
			for(i=row-1, j=col+1, left=0; i>=0 && j<SIZE && chessFlag == chessMap[i][j]; --i, ++j, ++left);
			for(i=row+1, j=col-1, right=0; i<SIZE && j>=0 && chessFlag == chessMap[i][j]; ++i, --j, ++right);
			if( 3 == left + right && row-left-1 >= 0 && col+left+1 < SIZE && SPACE == chessMap[row-left-1][col+left+1]
				&& row+right+1 < SIZE && col-right-1 >= 0 && SPACE == chessMap[row+right+1][col-right-1] )
			{
				flag = true;
				if( haveKinjite && BLACK_CHESS == chessFlag )
				{
					if( row-left-2 >= 0 && col+left+2 < SIZE && chessFlag == chessMap[row-left-2][col+left+2] )
						flag = false;
					if( row+right+2 < SIZE && col-right-2 >= 0 && chessFlag == chessMap[row+right+2][col-right-2] )
						flag = false;
				}
				if( flag )
				{
					int s1 = 0,s2 = 0;
					if( left )
						s1 = GetScoreForComputer(row-left-1,col+left+1,cComputer)+45000;
					if( right )
						s2 = GetScoreForComputer(row+right+1,col-right-1,cComputer)+45000;
				
					if(s1>score)
					{
						tempX = row-left-1, tempY = col+left+1, score = s1;
					}
					if(s2>score)
					{
						tempX = row+right+1, tempY = col-right-1, score = s2;
					}
				}
			}
			//////////////////////////////////////////////////////////////////////////
		}
	}
	comX = tempX;
	comY = tempY;
}


//�õ���Ӧ������ĵ�ǰ���λ��
void FiveChess::PeoplePlay()
{
	pair<int,int>pi = GetCurrentMaxPoint(cPeople);
	peoX = pi.first;
	peoY = pi.second;	
	int tempX = peoX,tempY = peoY;

	// ���ڶԷ��� ��3����3�������Ƿ��ػ�4�㣬���ǳ�4�㣬�����Ҫ��һ���ж�
	// ������3����2���������ӵ㣬������3����3���������ӵ�
	// ���ｫ�� 4 �����һ���ķ�ֵ��45000��
	if( 2 == winComputer && ( !winPeople || winPeople>winComputer) )
	{
		if( GetHuo4(peoX,peoY,cComputer) ) //��������ɻ�4
		{
			int score = GetScoreForPeople(peoX,peoY,cComputer); //�õ�����
			score += GetScoreForPeople(peoX,peoY,cPeople);
			//////////////////////////////////////////////////////////////////////////
			int row = peoX,col = peoY;
			int i,j,left,right;
			bool flag;
			char chessFlag = cComputer;

			// �� 
			for(i=col-1, left=0; i>=0 && chessFlag == chessMap[row][i]; --i, ++left);
			for(i=col+1, right=0; i<SIZE && chessFlag == chessMap[row][i]; ++i, ++right); 
			
			if( 3 == left + right && col-left-1 >= 0 && SPACE == chessMap[row][col-left-1]
				&& col+right+1 < SIZE && SPACE == chessMap[row][col+right+1] )
			{	
				flag = true;
				if( haveKinjite && BLACK_CHESS == chessFlag )
				{
					if( col-left-2 >= 0 && chessFlag == chessMap[row][col-left-2] )
						flag = false;
					if( col+right+2 < SIZE && chessFlag == chessMap[row][col+right+2] )
						flag = false;
				}
				if( flag ) 
				{
					int s1 = 0,s2 = 0;
					if( left )
						s1 = GetScoreForPeople(row,col-left-1,cPeople)+45000;
					if( right )
						s2 = GetScoreForPeople(row,col+right+1,cPeople)+45000;

					if(s1>score)
					{
						tempX = row, tempY = col-left-1, score = s1;
					}
					if(s2>score)
					{
						tempX = row, tempY = col+right+1, score = s2;
					}
				}
			}

			// ��
			for(i=row-1, left=0; i>=0 && chessFlag == chessMap[i][col]; --i, ++left); 
			for(i=row+1, right=0; i<SIZE && chessFlag == chessMap[i][col]; ++i, ++right);
			
			if( 3 == left + right && row-left-1 >= 0 && SPACE == chessMap[row-left-1][col]
				&& row+right+1 < SIZE && SPACE == chessMap[row+right+1][col] )
			{
				flag = true;
				if( haveKinjite && BLACK_CHESS == chessFlag )
				{
					if( row-left-2 >= 0 && chessFlag == chessMap[row-left-2][col] )
						flag = false;
					if( row+right+2 < SIZE && chessFlag == chessMap[row+right+2][col] )
						flag = false;
				}
				if( flag )
				{
					int s1 = 0,s2 = 0;
					if( left )
						s1 = GetScoreForPeople(row-left-1,col,cPeople)+45000;
					if( right )
						s2 = GetScoreForPeople(row+right+1,col,cPeople)+45000;

					if(s1>score)
					{
						tempX = row-left-1, tempY = col, score = s1;
					}
					if(s2>score)
					{
						tempX = row+right+1, tempY = col, score = s2;
					}
				}
			}

			// ' \ '
			for(i=row-1, j=col-1, left=0; i>=0 && j>=0 && chessFlag == chessMap[i][j]; --i, --j, ++left);
			for(i=row+1, j=col+1, right=0; i<SIZE && j<SIZE && chessFlag == chessMap[i][j]; ++i, ++j, ++right);
			if( 3 == left + right && row-left-1 >= 0 && col-left-1 >= 0 && SPACE == chessMap[row-left-1][col-left-1]
				&& row+right+1 < SIZE && col+right+1 < SIZE && SPACE == chessMap[row+right+1][col+right+1])
			{
				flag = true;
				if( haveKinjite && BLACK_CHESS == chessFlag )
				{
					if( row-left-2 >= 0 && col-left-2 >= 0 && chessFlag == chessMap[row-left-2][col-left-2] )
						flag = false;
					if( row+right+2 < SIZE && col+right+2 < SIZE && chessFlag == chessMap[row+right+2][col+right+2] )
						flag = false;
				}
				if( flag )
				{
					int s1 = 0,s2 = 0;
					if( left )
						s1 = GetScoreForPeople(row-left-1,col-left-1,cPeople)+45000;
					if( right )
						s2 = GetScoreForPeople(row+right+1,col+right+1,cPeople)+45000;

					if(s1>score)
					{
						tempX = row-left-1, tempY = col-left-1, score = s1;
					}
					if(s2>score)
					{
						tempX = row+right+1, tempY = col+right+1, score = s2;
					}
				}
			}
			
			// ' / '
			for(i=row-1, j=col+1, left=0; i>=0 && j<SIZE && chessFlag == chessMap[i][j]; --i, ++j, ++left);
			for(i=row+1, j=col-1, right=0; i<SIZE && j>=0 && chessFlag == chessMap[i][j]; ++i, --j, ++right);
			if( 3 == left + right && row-left-1 >= 0 && col+left+1 < SIZE && SPACE == chessMap[row-left-1][col+left+1]
				&& row+right+1 < SIZE && col-right-1 >= 0 && SPACE == chessMap[row+right+1][col-right-1] )
			{
				flag = true;
				if( haveKinjite && BLACK_CHESS == chessFlag )
				{
					if( row-left-2 >= 0 && col+left+2 < SIZE && chessFlag == chessMap[row-left-2][col+left+2] )
						flag = false;
					if( row+right+2 < SIZE && col-right-2 >= 0 && chessFlag == chessMap[row+right+2][col-right-2] )
						flag = false;
				}
				if( flag )
				{
					int s1 = 0,s2 = 0;
					if( left )
						s1 = GetScoreForPeople(row-left-1,col+left+1,cPeople)+45000;
					if( right )
						s2 = GetScoreForPeople(row+right+1,col-right-1,cPeople)+45000;

					if(s1>score)
					{
						tempX = row-left-1, tempY = col+left+1, score = s1;
					}
					if(s2>score)
					{
						tempX = row+right+1, tempY = col-right-1, score = s2;
					}
				}
			}
			//////////////////////////////////////////////////////////////////////////
		}
	}
	peoX = tempX;
	peoY = tempY;
}


//�õ������ص�ǰ��ֵ����
pair<int,int> FiveChess::GetCurrentMaxPoint(char chessFlag)
{
	int row,col;
	memset(computer,0,sizeof(computer));
	memset(people,0,sizeof(people));

	for(row=0;row<SIZE;++row)
	{
		for(col=0;col<SIZE;++col)
		{
			if(SPACE == chessMap[row][col])
			{
				if(chessFlag == cComputer)
				{
					people[row][col] = GetScoreForComputer(row,col,cPeople);
					computer[row][col] = GetScoreForComputer(row,col,cComputer);
				}
				else 
				{
					people[row][col] = GetScoreForPeople(row,col,cPeople);
					computer[row][col] = GetScoreForPeople(row,col,cComputer);
				}
			}
		}
	}

	// 
	int cx,cy,px,py;
	int maxComputer = -10,maxPeople = -10;
	int maxScore = -10;
	int xx,yy;

	for(row=0;row<SIZE;++row)
	{
		for(col=0;col<SIZE;++col)
		{
			if(chessMap[row][col] != SPACE)
				continue;
			
			if(BLACK_CHESS == chessFlag) // ��ֹ���ֵ�
			{
				if(cPeople == chessFlag && -1 == people[row][col])
					continue;
				if(cComputer == chessFlag && -1 == computer[row][col])
					continue;
			}

			if(people[row][col]>maxPeople)
			{
				maxPeople = people[row][col];
				px = row;
				py = col;
			}
			
			if(computer[row][col]>maxComputer)
			{
				maxComputer = computer[row][col];
				cx = row;
				cy = col;
			} 
			if(computer[row][col]+people[row][col]>maxScore)
			{
				maxScore = computer[row][col] + people[row][col];
				xx = row;
				yy = col;
			}
		}
	}

	winPeople = winComputer = mustWin = 0;
	if(chessFlag == cComputer)  ////վ�ڼ�����ĽǶ�
	{
		if(maxComputer >= 5000000)
			winComputer = 1;
		else if(maxComputer >= 200000)
			winComputer = 2;
		else if(maxComputer >= 8000)
			winComputer = 3;

		if(maxPeople >= 1000000)
			winPeople = 1;
		else if(maxPeople >= 40000)
			winPeople = 2;
		else if(maxPeople >= 1900)
			winPeople = 3;
	}
	else  
	{
		if(maxComputer >= 1000000)
			winComputer = 1;
		else if(maxComputer >= 40000)
			winComputer = 2;
		else if(maxComputer >= 1900)
			winComputer = 3;

		if(maxPeople >= 5000000)
			winPeople = 1;
		else if(maxPeople >= 200000)
			winPeople = 2;
		else if(maxPeople >= 8000)
			winPeople = 3;
	}

	if( 1 == winComputer || 1 == winPeople )
		mustWin = 1;
	else if( 2 == winComputer || 2 == winPeople )
		mustWin = 2;
	else if( 3 == winComputer || 3 == winPeople )
		mustWin = 3;

	//���������������ô maxComputer �� maxPeople ��Ӧ��Ϊ -10 
	if(-10 == maxComputer && -10 == maxPeople)
		return pair<int,int>(-1,-1);

	// ��� ��3 ,�����ǵ��ı�
	if( mustWin ) //���б�Ӯ���ʱ��ȡ����֮����߷֣����������ܵı���ʧ��
		return pair<int,int>(xx,yy);

	return maxComputer >= maxPeople ? pair<int,int>(cx,cy) : pair<int,int>(px,py);
}

//�������ģ��������ֻ��һ����
void FiveChess::InitGameTree(int row,int col,bool& flag,int depth,int& depthC,int& depthP,bool& stop,int depthRecord)
{
	if(stop || depth > DEPTH || depth > depthRecord+1) 
		return ;

	if(BLACK_CHESS == cComputer && IsKinjite(row,col)) // ��������˽���λ��,����Ӯ
	{
		depthP = depth;
		stop = true;
		return ;
	}
	chessMap[row][col] = cComputer;   //�������

	PeoplePlay();
	
	if(peoX != -1 && peoY != -1 ) //����δ��
	{
		if( 1 == winPeople) //���г���������ֹͣ����
		{
			depthP = depth;
			stop = true;
		}
		else if( BLACK_CHESS == cPeople && IsKinjite(peoX,peoY))//�����˽��ֵ㣬������Ӯ
		{
			flag = true;
			depthC = depth;
			stop = true; 
		}
		else
		{		
			int px = peoX,py = peoY;
			chessMap[px][py] = cPeople;  //����

		    ComputerPlay();
		
			if( comX != -1 && comY != -1 ) //����δ��
			{
				if( 1 == winComputer)  //������г���㣬��������
				{
					flag = true;
					depthC = depth;
					stop = true;
				}
				else
					InitGameTree(comX,comY,flag,depth+1,depthC,depthP,stop,depthRecord); //�ݹ�
			}

			chessMap[px][py] = SPACE; // �ָ�
		}
	}
	chessMap[row][col] = SPACE; // �ָ�
}

//�������ģ���ˣ����� InitGameTree()
void FiveChess::AgainGameTree(int row,int col,int depth,int& depthC,int& depthP,bool& stop,int& depthRecord)
{
	// �����depth����Ӧ���ʵ���Щ����ֹ����vc����
	if(stop || depth > 12 || depth >= depthRecord) // �����depthCFromIGT��������� PeopleAttack() 
		return ;
	
	if(BLACK_CHESS == cPeople && IsKinjite(row,col)) // �����˽���λ��,������Ӯ
	{
		depthC = depth;
		stop = true;
		return ;
	}
	chessMap[row][col] = cPeople;   //����
	
	ComputerPlay();
	
	if(comX != -1 && comY != -1 ) //����δ��
	{
		if( 1 == winComputer) //������г���������ֹͣ����
		{
			depthC = depth;
			stop = true;
		}
		else if( BLACK_CHESS == cComputer && IsKinjite(comX,comY))//��������˽��ֵ㣬����Ӯ
		{
			depthP = depth;
			stop = true;
		}
		else
		{
			//˵������vc��������ô���������Ӧ������
			if( winPeople && ( !winComputer || winPeople<winComputer ) ) 
			{
				++depthRecord;
			}

			int cx = comX,cy = comY;
			chessMap[cx][cy] = cComputer;  //�������
			
			PeoplePlay();
			
			if( peoX != -1 && peoY != -1 ) //����δ��
			{
				if( 1 == winPeople)  //���г���㣬��������
				{
					depthP = depth;
					stop = true;
				}
				else
					AgainGameTree(peoX,peoY,depth+1,depthC,depthP,stop,depthRecord); //�ݹ�
			}
			
			chessMap[cx][cy] = SPACE; // �ָ�
		}
	}
	chessMap[row][col] = SPACE; // �ָ�
}

// ģ���������й�������������ܷ��� depthMM ���ڻ�����
bool FiveChess::ComputerAttack(int r,int c,int depthMM)
{
	chessMap[r][c] = cPeople; //���������
	
	int depthRecord = depthMM;

	bool flag = false; //��Ǽ�����Ƿ��ܻ�����
	int row,col;
	for(row=top;row<=bottom;++row)
	{
		for(col=left;col<=right;++col)
		{
			if(chessMap[row][col] != SPACE)
				continue;
			if(BLACK_CHESS == cComputer && IsKinjite(row,col))  //�����ִ�ڣ������Ϊ���ֵ�
				continue;
			
			bool stop = false;
			int depthC = 999,depthP = 999;
			
			depthRecord = depthMM; //
			
			InitGameTree(row,col,flag,0,depthC,depthP,stop,depthRecord);

			if(depthP <= depthC) // ����Ӯ
				continue;
			
			// ���������Ӯ��������
			flag = true;
			col = right+1;
			row = bottom+1;
		}
	}
	
	chessMap[r][c] = SPACE; //�ָ�
	
	if(flag)
		return true;
	return false;
}

// ģ���˽��й����������ܷ��� depthCFromIGT ���ڻ��ܼ����
bool FiveChess::PeopleAttack(int r,int c,int depthCFromIGT)
{
	chessMap[r][c] = cComputer; //������������

	int depthRecord = depthCFromIGT; // depth from InitGameTree()

	bool flag = false; //������Ƿ��ܻ��ܼ����
	int row,col;
	for(row=top;row<=bottom;++row)
	{
		for(col=left;col<=right;++col)
		{
			if(chessMap[row][col] != SPACE)
				continue;
			if(BLACK_CHESS == cPeople && IsKinjite(row,col))  //��ִ�ڣ������Ϊ���ֵ�
				continue;
			
			bool stop = false;
			int depthC = 999,depthP = 999;
			
			depthRecord = depthCFromIGT; //


			AgainGameTree(row,col,0,depthC,depthP,stop,depthRecord); 

			if(depthC <= depthP) // �������Ӯ
				continue;

			// ������Ӯ��������
			flag = true;
			col = right+1;
			row = bottom+1;
		}
	}

	chessMap[r][c] = SPACE; //�ָ�

	if(flag)
		return true;
	return false;
}


/*
     AI �Ľ����Ժͷ�����������
	 ������ VCAttack() ���� VCF��VCT������һ�����֣�������Ӯ��
	 ����Ĵ��뼸�����˶���,��Ȼ�ÿ�����Ĵ��봴�����ֻ���
*/
void FiveChess::AI()
{
	isDefend = false;
	AIState = 0;

	ComputerPlay();
	int tempX = comX,tempY = comY; //

	if(LayOut()) // �տ�ʼʱ����
		return ;

	if(1 == mustWin)  //�г����
		return ;

	// AI �ȼ�Ϊ 2 ʱ�Ž��� VC ���� 
	if(2 == grade && VCAttack()) // ���Խ��� VC ����
		return ;

	ControlDepth(); // ���õݹ����

	int row,col,depthRecord = 999,maxScore = -999;
	
	// ���ҵ�һ����Ӯ�㣬�㽫����Ϊ true��������Ҫ������Ƿ����Ӯ����������磺
	// �����������������Ӻ󹹳�һ����4��һ����3��Ȼ�����ڵ���4��ʱ���γ��˻�4�������˾���Ӯ��
	// ���Բ��ܵ������������ֵȥ�ж�
	bool safeFlag = false; 

	for(row=top;row<=bottom;++row)
	{
		for(col=left;col<=right;++col)
		{
			if(chessMap[row][col] != SPACE)
				continue;
			if(BLACK_CHESS == cComputer && IsKinjite(row,col))  //�����ִ�ڣ������Ϊ���ֵ�
				continue;
		
			bool flag = false,stop = false;
			int depthC = 999,depthP = 999;

			InitGameTree(row,col,flag,0,depthC,depthP,stop,depthRecord); 
		
			//�����ǵ���������ôdepthC��depthP������һ���ǲ���ģ�
			//depthP<=depthC��˵����������·���¼����������Ӯ
			if(depthP <= depthC) 
				continue;

			// �������ڼ�����Ľ�����ͼ�У������������ģ���˽�������˭��Ӯ������Ӯ���򷵻�true
			// ע���������
			if( PeopleAttack(row,col,depthC) ) //// ����һ������������������
				continue;

			if(flag)  //������б�Ӯ�㣬�㽫��㱣������
			{
				// ��Ҫ���ǽ������,��Ҫԭ������Ϊ��������
				bool ff = false;
				if(BLACK_CHESS == cComputer && haveKinjite && depthC>=2)
				{
					chessMap[row][col] = cComputer;  //�����������
					for(int i=top;i<=bottom;++i)
					{
						for(int j=left;j<=right;++j)
						{
							if( IsKinjite(i,j) ) // �����˽��ֵ�
							{
								ff = true;
								i = bottom+1;
								j= right+1;
							}
						}
					}
					chessMap[row][col] = SPACE; //�ָ�
				}
				if(ff)
					continue;

				if(depthC < depthRecord)  //ѡȡ���С��
				{
					maxScore = GetScoreForComputer(row,col,cComputer);
					depthRecord = depthC;
					tempX = row;
					tempY = col;
				}
				else if(depthC == depthRecord) //���һ��ʱѡȡȨֵ���
				{
					int res = GetScoreForComputer(row,col,cComputer);
					if(res > maxScore)
					{
						maxScore = res;
						tempX = row;
						tempY = col;
					}
				}
				safeFlag = true; ///
			}
		}

	}

	comX = tempX;
	comY = tempY;

	if(!safeFlag) // ��Ҫ������Ƿ���Ӯ
	{ 
		PeoplePlay();
		bool flag = false;
		if( winPeople )  //����Ӯ����������赲
		{
			if(BLACK_CHESS != cComputer ||  !IsKinjite(peoX,peoY)) // ���Ǽ�����Ľ��ֵ�
			{
				comX = peoX;
				comY = peoY;
			}
			else
				flag = true;
		}
		if( flag ) // ��ô�����ܵ�������Ӯ����һ���ܾ���������ĵ�����
		{ 
			int depthWin=8; //
			maxScore = -999;
			
			for(row=top;row<=bottom;++row)
			{
				for(col=left;col<=right;++col)
				{
					if(chessMap[row][col] != SPACE)
						continue;
					if( BLACK_CHESS == cComputer && IsKinjite(row,col)) //������������Լ��Ľ��ֵ�
						continue;
					
					if(!PeopleAttack(row,col,depthWin)) // ������� depthWin �� ������Ӯ
					{
						// �Ǿ���һ���Լ�������Լ�ֵ���ĵ�
						int res = GetScoreForComputer(row,col,cComputer);
						if(res > maxScore)
						{
							maxScore = res;
							tempX = row;
							tempY = col;
						}
					}
				}
			}
			
			comX = tempX;
			comY = tempY;
		}
				
	}

}


// �жϾ��ƣ��Ӷ����� DEPTH
// �����жϵľ�������Է��ض���,���翴������Ӻ�ɹ��ɶ��ٻ�2����3����4��
// ���Է�������������ʱ����ʱ�������Ӧ����������
void FiveChess::ControlDepth()
{
	DEPTH = 12; //Ĭ�� 
	
	if(nCount < 6)
	{
		DEPTH = 4;
		return ;
	}

	return ; // �������ˣ����ظ�ë
/*
	int row,col;

	// people
	for(row=top;row<=bottom;++row)
	{
		for(col=left;col<=right;++col)
		{
			if(chessMap[row][col] != SPACE)
				continue;
			Search(row,col,cPeople);

			if(lian2+tiao2>=3)
				DEPTH = min(DEPTH,7);

			if(chong4)
				DEPTH = min(DEPTH,6);
			if(lian3+tiao3)
				DEPTH = min(DEPTH,6);
			
			if(lian2+tiao2 && lian3+tiao3)
				DEPTH = min(DEPTH,5);
			if(chong4 && chong3 && lian2+tiao2)
				DEPTH = min(DEPTH,5);
			if(chong4 && chong3>=2)
				DEPTH = min(DEPTH,5);
		}
	}

	// computer
	for(row=top;row<=bottom;++row)
	{
		for(col=left;col<=right;++col)
		{
			if(chessMap[row][col] != SPACE)
				continue;
			Search(row,col,cComputer);
			
			if(chong4 && chong3) 
				++DEPTH;
			if(chong4 && lian2+tiao2)
				++DEPTH;
			if(lian3+tiao3 && chong3)
				++DEPTH;
		}
	}

	DEPTH = min(DEPTH,10);
*/

}

// ����
//����Ҫ��������Ҫ��
/*
	��ʮ���ֿ�������������ʤ���ֺ������ذܿ��֣���������������ֶ��Եģ�
	��ʤ�����ǻ��¾ֺ����¾֣�
	�ذܿ��������Ǿֺ����Ǿ֡�
*/
bool FiveChess::LayOut()
{
	// ֻ����ǰ�������Ĳ���
	if(nCount >= 3)
		return false;

	if(BLACK_CHESS == cComputer)// �����ִ�ڣ�ֻ���ǵ�����
	{
		// �������һ�ֱ�Ȼ����Ԫ����ô������8��λ�ã�����λ�ò����ǡ�
		int res = 0;
		srand((unsigned)time(NULL));
		res = rand() % 2;

		// ����
		if( (6 == currentX || 8 == currentX) && 7 == currentY )
			comX = currentX, comY = ( res ? 6 : 8) ;
		else if( 7 == currentX && (6 == currentY || 8 == currentY) )
			comX = (res ? 6 : 8), comY = currentY;
		
		// ����
		else if( 6 == currentX && 8 == currentY )
			res ? ( comX = comY = 8 ) : ( comX = comY = 6 );
		else if( 6 == currentX && 6 == currentY )
			res ? ( comX = 8, comY = 6 ) : ( comX = 6, comY = 8 );
		else if( 8 == currentX && 6 == currentY )
			res ? ( comX = comY = 6 ) : ( comX = comY = 8 );
		else if( 8 == currentX && 8 == currentY )
			res ? ( comX = 8, comY = 6 ) : ( comX = 6, comY = 8 );

		// ������������������,���жϰ�������λ��
		else
		{
			if(currentX<=7 && currentY<=7)
				comX = comY = 8;
			else if(currentX>=7 && currentY>=7)
				comX = comY = 6;
			else if(currentX>=7 && currentY<=7)
				comX = 6, comY = 8;
			else
				comX = 8, comY = 6;
		}

	}
	else //�����ִ�ף�ֻ���ǵڶ���
	{
		// ������������
		int res = 0;
		srand((unsigned)time(NULL));
		res = rand() % 4;

		while(1)
		{
			if( 0 == res )
			{
				if( currentX-1 >= 0 )
				{
					comX = currentX-1, comY = currentY;
					break;
				}
				else 
					res = 1;
			}
			if( 1 == res )
			{
				if( currentX+1 < SIZE )
				{
					comX = currentX+1, comY = currentY;
					break;
				}
				else
					res = 2;
			}
			if( 2 == res )
			{
				if( currentY-1 >= 0 )
				{
					comX = currentX, comY = currentY-1;
					break;
				}
				else
					res = 3;
			}
			if( 3 == res )
			{
				if( currentY+1 < SIZE )
				{
					comX = currentX, comY = currentY+1;
					break;
				}
				else
					res = 0;
			}
		}

	}
	return true;
}

// VC ������
void FiveChess::VCAttackTree(int type,int row,int col,char cOneself,char cOpposite,int depth,bool& flag,int& ansDepth,int depthRecord)
{
	// ��ȿ���,���̫��Ļ������ӶȾ�̫���ˣ�depthRecord������¼�Ѿ��ѹ��ĵ��д��Ӯ�����С���
	if( depth>depthMM || depth>depthRecord )
		return ;

	if(cOneself == cComputer)
	{
		ComputerPlay();
		if( 1 == winComputer )
			flag = true;
	}
	else
	{
		PeoplePlay();
		if( 1 == winPeople )
			flag = true;
	}
	if( flag ) // flag Ϊ true ʱ����ʾ���Ա�Ӯ���ܱ�Ӯ�򷵻أ�����Ҫȥ�����ŵģ�����̫��
	{
		ansDepth = depth; //��¼���ٲ���Ӯ
		return ;
	}

	chessMap[row][col] = cOneself; // �Լ�����

	int oppositeX,oppositeY;
	int winOpposite = 0;
	int dd = 1; // ��ȵĵ���ֵ
	
	//////////////////////�Է�˼��/////////////////////////////////////////////
	if( cOpposite == cPeople ) 
	{
		PeoplePlay();
		oppositeX = peoX, oppositeY = peoY;
		winOpposite = winPeople;
	}
	else //
	{
		ComputerPlay();
		oppositeX = comX, oppositeY = comY;
		winOpposite = winComputer;
	}

	// ����Է��ڷ��ص�ͬʱ Ҳ�����˳�4���3����ô�Լ�����Ҫ���������ضԷ�����ʱӦ�ý���ȼ�1
	if( 1 == type ) // VCF ����
	{
		pair<int,pair<int,int> > ppi = GetChong3Chong4(oppositeX,oppositeY,cOpposite);
		if(ppi.second.first + ppi.second.second) //���ɳ�4������4
			dd = 2;

//		if(ppi.second.first + ppi.second.second >=2 ) //
//			winOpposite = 1;
	}
	else if( 2 == type ) // VCF �� VCT ���Ϲ���
	{
		pair<int,pair<int,int> > ppi = GetChong3Chong4(oppositeX,oppositeY,cOpposite);
		if(ppi.second.first + ppi.second.second) //���ɳ�4������4
			dd = 2;
		pair<int,int> pi1 = GetLian2Lian3(oppositeX,oppositeY,cOpposite);
		if(pi1.second) //������3
			dd = 2;
		pair<int,int> pi2  = GetTiao2Tiao3(oppositeX,oppositeY,cOpposite);
		if(pi2.second) //������3
			dd = 2;

//		if(ppi.second.first+ppi.second.second + pi1.second+pi2.second >=2 ) //
//			winOpposite = 1;
	}
	//////////////////////////////////////////////////////////////////////////

	if( (oppositeX != -1 && oppositeY != -1) && (winOpposite != 1) ) // ����δ�� �� �Է� δ����
	{
		if( cOpposite == BLACK_CHESS && IsKinjite(oppositeX,oppositeY) ) // �õ��ǶԷ��Ľ���
			flag = true;
		else
		{
			chessMap[oppositeX][oppositeY] = cOpposite; // �Է�����

			VCPoint vcPoint[100];// �����3����4����4��Щ��
			int cnt=0; 
			int i,j;

			for(i=0;i<SIZE;++i)
			{
				for(j=0;j<SIZE;++j)
				{
					if( chessMap[i][j] != SPACE )
						continue;
					if( BLACK_CHESS == cOneself && IsKinjite(i,j) ) // �Լ��Ľ��ֵ�
						continue;

					int res = IsCombForVC(i,j,cOneself); 
					//�ǹ������򱣴�,�������ͷ�2�֣�VCF����VCF��VCT���Ϲ���
					if( 1 == type && res<3 ) 
						continue;
					if( 2 == type && !res )
						continue;

					vcPoint[cnt++].init(i,j,res);
					if( cnt >= 100 )
					{
						i = j = SIZE;
					}
				}
			}
			
			sort(vcPoint,vcPoint+cnt);  //�����ȼ�����

			for(i=0;i<cnt;++i)
			{
				if( !flag )
				{
					VCAttackTree(type,vcPoint[i].row,vcPoint[i].col,cOneself,cOpposite,depth+dd,flag,ansDepth,depthRecord); //
				}
			}

			chessMap[oppositeX][oppositeY] = SPACE; // �ָ�
		}
	}

	chessMap[row][col] = SPACE; //�ָ�
}


// �ж��Ƿ��п��Խ��� VC �����ĵ�
// ���жϼ�����ܷ� VCF����ȡʤ�����ж����Ƿ�ͨ��VCF����ȡʤ��Ȼ�����жϼ�����Ƿ�ͨ��VCF��VCT����
//����ȡʤ��Ҳ�ж��˵�VCF��VCT���Ϲ���(��Ȼ��һ��).
// Ϊ����� ������ ���ϵĳ��Ը�����ȣ��ȴ�С����ȿ�ʼ���أ��������ܱ�֤����С�Ĳ���Ӯ�壬
// һ���ҵ���Ӯ�㣬��������
// Ŀǰ��Ȼ��bug���������ٶȺ�����֮��Ȩ��
bool FiveChess::VCAttack()
{
	// VC ��������4����4(��4������4)����3����3����3�������ȼ����ν���

	int row,col;
	VCPoint vcPoint[100]; //
	int cnt;
	int tempDepth; //��ȿ���

	VCPoint dePoint[100]; //���� VC ���أ��������ģ���˵Ĺ����б��湥����
	int num = 0; //

	int tempX,tempY,depthRecord=9999,maxScore = -9999;
	bool ff = false;

	// ���жϼ�����Ƿ�ɽ��� VCF ����,  �����򷵻�
	//////////////////////////////////////////////////////////////////////////
	cnt = 0;

	for(row=top;row<=bottom;++row)
	{
		for(col=left;col<=right;++col)
		{
			if( chessMap[row][col] != SPACE )
				continue;
			if( BLACK_CHESS == cComputer && IsKinjite(row,col) ) // ������Ľ��ֵ�
				continue;
			
			int res = IsCombForVC(row,col,cComputer);

			if( res<3 ) // VCF����
				continue;

			vcPoint[cnt++].init(row,col,res);	//�ǽ��������ȱ���
			
			if(cnt>=100) 
			{
				row = bottom+1;
				col = right+1;
			}
		}
	}

	sort(vcPoint,vcPoint+cnt); //

	for(tempDepth = VCDEPTH; tempDepth <= VCDEPTH_END; tempDepth += VCDEPTH_DIS)//��Ȳ��ϵ���
	{
		depthMM = tempDepth; //
		for(int i=0;i<cnt;++i)
		{
			bool flag = false;
			int ansDepth = 9999;
			VCAttackTree(1,vcPoint[i].row,vcPoint[i].col,cComputer,cPeople,0,flag,ansDepth,depthRecord);
			
			if(flag) // �ҵ�һ����Ӯ������
			{	
				ff = true; //
				if(ansDepth<depthRecord) //ѡȡ�����Ӯ��
				{
					depthRecord = ansDepth;
					tempX = vcPoint[i].row;
					tempY = vcPoint[i].col;
					maxScore = GetScoreForComputer(tempX,tempY,cComputer);
					maxScore += GetScoreForComputer(tempX,tempY,cPeople);
				}
				else if( ansDepth == depthRecord ) //���һ��ʱѡȡȨֵ���
				{
					int res = GetScoreForComputer(vcPoint[i].row,vcPoint[i].col,cComputer);
					res += GetScoreForComputer(vcPoint[i].row,vcPoint[i].col,cPeople);
					if( res > maxScore )
					{
						maxScore = res;
						tempX = vcPoint[i].row;
						tempY = vcPoint[i].col;
					}
				}
			}
		}
		if( ff )
		{
			comX = tempX;
			comY = tempY; 
			AIState = 1; //
			return true;
		}

	}

	// ���ж����Ƿ�ͨ�� VCF����ȡʤ�������ԣ������������� VCF��VCT�����Ϲ�������Ϊ������
	// ����Ҫ�����ж� ���Ƿ񻹿��� ͨ�� VCF��VCT�����Ϲ���ȡʤ�����ϵ�������������
	int peoIsVCF = false;
	//////////////////////////////////////////////////////////////////////////
	cnt = 0;

	for(row=top;row<=bottom;++row)
	{
		for(col=left;col<=right;++col)
		{
			if( chessMap[row][col] != SPACE )
				continue;
			if( BLACK_CHESS == cPeople && IsKinjite(row,col) ) // �˵Ľ��ֵ�
				continue;
			
			int res = IsCombForVC(row,col,cPeople);
			
			if( res<3 ) // VCF����
				continue;
			
			vcPoint[cnt++].init(row,col,res);	//�ǽ��������ȱ���
			
			if(cnt>=100) 
			{
				row = bottom+1;
				col = right+1;
			}
		}
	}
	
	sort(vcPoint,vcPoint+cnt); //

	
	for(tempDepth = VCDE_DEPTH; tempDepth <= VCDE_DEPTH_END; tempDepth += VCDE_DEPTH_DIS)//��Ȳ��ϵ���
	{
		depthMM = tempDepth; //
		for(int i=0;i<cnt;++i)
		{
			bool flag = false;
			int ansDepth = 9999;

			VCAttackTree(1,vcPoint[i].row,vcPoint[i].col,cPeople,cComputer,0,flag,ansDepth,depthRecord);
			
			if(flag) // �ҵ�һ����Ӯ������
			{
				ff = true; //
				i = cnt;
				tempDepth = VCDE_DEPTH_END+1;
			}
		}		
	}
	if( ff ) 
	{
		peoIsVCF = true;
		AIState = 2;
	}

	ff = false;
	
	//�жϼ�����ܷ�ͨ�� VCF��VCT���Ϲ���ȡʤ,  ǰ�����˲��ܽ��� VCF����
	//////////////////////////////////////////////////////////////////////////
	if(!peoIsVCF)
	{
		cnt = 0;

		for(row=top;row<=bottom;++row)
		{
			for(col=left;col<=right;++col)
			{
				if( chessMap[row][col] != SPACE )
					continue;
				if( BLACK_CHESS == cComputer && IsKinjite(row,col) ) // ������Ľ��ֵ�
					continue;
				
				int res = IsCombForVC(row,col,cComputer);
				
				if( !res ) // VCF��VCT���Ϲ���
					continue;
				
				vcPoint[cnt++].init(row,col,res);	//�ǽ��������ȱ���
				
				if(cnt>=100) 
				{
					row = bottom+1;
					col = right+1;
				}
			}
		}
		sort(vcPoint,vcPoint+cnt); //

		for(tempDepth = VCDEPTH; tempDepth <= VCDEPTH_END; tempDepth += VCDEPTH_DIS)//��Ȳ��ϵ���
		{
			depthMM = tempDepth; //
			for(int i=0;i<cnt;++i)
			{
				bool flag = false;
				int ansDepth = 9999;
				VCAttackTree(2,vcPoint[i].row,vcPoint[i].col,cComputer,cPeople,0,flag,ansDepth,depthRecord);
				
				if(flag) //�ҵ�һ����Ӯ������
				{
					ff = true; //
					if(ansDepth<depthRecord) //ѡȡ�����Ӯ��
					{
						depthRecord = ansDepth;
						tempX = vcPoint[i].row;
						tempY = vcPoint[i].col;
						maxScore = GetScoreForComputer(tempX,tempY,cComputer);
						maxScore += GetScoreForComputer(tempX,tempY,cPeople);
					}
					else if( ansDepth == depthRecord ) //���һ��ʱѡȡȨֵ���
					{
						int res = GetScoreForComputer(vcPoint[i].row,vcPoint[i].col,cComputer);
						res += GetScoreForComputer(vcPoint[i].row,vcPoint[i].col,cPeople);
						if( res > maxScore )
						{
							maxScore = res;
							tempX = vcPoint[i].row;
							tempY = vcPoint[i].col;
						}
					}

				}
			}
			if( ff )
			{
				comX = tempX;
				comY = tempY; 
				AIState = 3;
				return true;
			}
		}
	}

	//�ж����ܷ�ͨ�� VCF��VCT���Ϲ���ȡʤ��  VC ����
	//////////////////////////////////////////////////////////////////////////
	cnt = 0;
	depthRecord=9999;
	
	for(row=top;row<=bottom;++row)
	{
		for(col=left;col<=right;++col)
		{
			if( chessMap[row][col] != SPACE )
				continue;
			if( BLACK_CHESS == cPeople && IsKinjite(row,col) ) // �Լ��Ľ��ֵ�
				continue;
			
			int res = IsCombForVC(row,col,cPeople);
			
			if( !res ) // VCF��VCT���Ϲ���
				continue;
			
			vcPoint[cnt++].init(row,col,res);	//�ǽ��������ȱ���
			
			if(cnt>=100) 
			{
				row = bottom+1;
				col = right+1;
			}
		}
	}
	
	sort(vcPoint,vcPoint+cnt); //

	for(tempDepth = VCDE_DEPTH; tempDepth <= VCDE_DEPTH_END; tempDepth += VCDE_DEPTH_DIS)//��Ȳ��ϵ���
	{
		depthMM = tempDepth; //
		for(int i=0;i<cnt;++i)
		{
			bool flag = false;
			int ansDepth = 9999;
			isDefend = false;

			VCAttackTree(2,vcPoint[i].row,vcPoint[i].col,cPeople,cComputer,0,flag,ansDepth,depthRecord);
		
			if(flag) // �ҵ�һ����Ӯ������
			{
				isDefend = true; //���أ���Ҫ��Ϊ�� ���ڶԷ������ķ���
				ff = true; //		
				if(ansDepth<depthRecord) //ѡȡ�����Ӯ��
				{
					depthRecord = ansDepth;
					tempX = vcPoint[i].row;
					tempY = vcPoint[i].col;	
					maxScore = GetScoreForComputer(tempX,tempY,cPeople);
					maxScore += GetScoreForComputer(tempX,tempY,cComputer);
				}
				else if( ansDepth == depthRecord ) //���һ��ʱѡȡȨֵ���
				{
					int res = GetScoreForComputer(vcPoint[i].row,vcPoint[i].col,cPeople);
					res += GetScoreForComputer(vcPoint[i].row,vcPoint[i].col,cComputer);
					if( res > maxScore )
					{
						maxScore = res;
						tempX = vcPoint[i].row;
						tempY = vcPoint[i].col;
					}
				}
			}
		}
		if( ff ) 
		{
			// ���أ������ѡ�񹥻�λ�����ӣ���������Ϊ���˸õ��ʹ�˲������ VC ������
			//////////////////////////////////////////////////////////////////////////
			for(row=top;row<=bottom;++row)
			{
				for(col=left;col<=right;++col)
				{
					if(chessMap[row][col] != SPACE)
						continue;
					if(BLACK_CHESS == cComputer && IsKinjite(row,col))
						continue;
					
					int res1 = IsCombForVC(row,col,cComputer);
					int res2 = IsCombForVC(row,col,cPeople);

					if( !res1 && !res2)
						continue;
					else if( (3 == res1 || 4 == res1) && !res2 ) //һ������²�Ҫ���Լ���4������4
						continue;

					int res = GetScoreForComputer(row,col,cComputer);
					res += GetScoreForComputer(row,col,cPeople);

					dePoint[num++].init(row,col,res);
				}
			}

			sort(dePoint,dePoint+num); //��Ȩֵ����

			for(int p=0;p<num;++p)
			{
				bool noVC = true;
	
				chessMap[dePoint[p].row][dePoint[p].col] = cComputer; //����������������
				
				// ����������ģ�� �˵� VC ����
				cnt = 0;
				depthRecord=9999;

				for(row=top;row<=bottom;++row)
				{
					for(col=left;col<=right;++col)
					{
						if( chessMap[row][col] != SPACE )
							continue;
						if( BLACK_CHESS == cPeople && IsKinjite(row,col) ) // �˵Ľ��ֵ�
							continue;
						
						int res = IsCombForVC(row,col,cPeople);
						
						if( !res ) // VCF��VCT���Ϲ���
							continue;
						
						vcPoint[cnt++].init(row,col,res);	//�ǽ��������ȱ���
						
						if(cnt>=100) 
						{
							row = bottom+1;
							col = right+1;
						}
					}
				}
				
				sort(vcPoint,vcPoint+cnt); //
				
				
				for(tempDepth = VCDE_DEPTH; tempDepth <= VCDE_DEPTH_END; tempDepth += VCDE_DEPTH_DIS)//��Ȳ��ϵ���
				{
					depthMM = tempDepth; //
					for(int i=0;i<cnt;++i)
					{
						bool flag = false;
						int ansDepth = 9999;
						VCAttackTree(2,vcPoint[i].row,vcPoint[i].col,cPeople,cComputer,0,flag,ansDepth,depthRecord);
						
						if(flag) // �ҵ�����������
						{
							noVC = false;
							i = cnt;
							tempDepth = VCDE_DEPTH_END+1;
						}
					}
				}
				
				chessMap[dePoint[p].row][dePoint[p].col] = SPACE; //�ָ�
				
				if(noVC) //�����û�� VC ������
				{
					tempX = dePoint[p].row;
					tempY = dePoint[p].col;
					break;
				}
			}
			
			//////////////////////////////////////////////////////////////////////////
			comX = tempX;
			comY = tempY; 
			AIState = 4;
			return true;
		}
	}
	return false;
}

//�ж��Ƿ��� VC �����Ĺ����㣬���������ȼ�
int FiveChess::IsCombForVC(int row,int col,char chessFlag)
{
	Search(row,col,chessFlag);
	
	if(huo4)
		return 5;
	if(chong4)
		return 4;
	if(tiaochong4)
		return 3;
	if(lian3)
		return 2;
	if(tiao3)
		return 1;

	return 0;
}