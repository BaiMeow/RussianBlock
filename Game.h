#pragma once

#include "Tool.h"
enum GAME_STATE {
	PAUSE,//��ͣ
	GO,  //����
	STOP //ֹͣ
};

class Game
{
	friend class CRussianBlockDlg;
private:
	GAME_STATE _state;			//��Ϸ״̬
	int* _bigNet;				//��Ϸ�������ʾ����
	int* _bigNetAux;			//��Ϸ�������ʾ��������
	int  _netWidth, _netHeight;	//����Ϸ����Ŀ�ȡ��߶�
	Tool _tool;					//��ǰ��Ϸ��������������ķ���
	Tool _nextTool;				//��һ���������ֳ��ֵ����䷽��
	int _iLocX, _iLocY;			//�������꣬����˹�������Ͻ�λ��

public:
	Game(int height, int width);
	~Game();
	void Start();					//��ʼ��Ϸ
	void Input(UINT nChar);			//����������մ�����
	bool Go();						//����һ��
	void PauseOrContinue();			//��ͣ/�������Ŀ��ƺ���
	int GetScore();
	int GetTickTime();
private:
	void NextTool();				//������һ������
	bool CanMoveDown();				//�����Ƿ���������ƶ�
	//��������е�һά�����в�Ϊ0��Ԫ����
	int  CountNoneZero(int* matrix, int Height, int Width);
	//������˹�����Ǽ��뵽ָ��������������
	void AddToolToAux(int* net, int iOffsetX, int iOffsetY,
		const Tool& _tool);
	void MoveDown();		//���������ƶ�һ��
	bool CanMoveLeft();		//�����Ƿ���������ƶ�
	bool CanMoveRight();	//�����Ƿ���������ƶ�
	void MoveLeft();		//���������ƶ�һ��
	void MoveRight();		//���������ƶ�һ��
	bool CanRoll();			//�����Ƿ���Ա任��̬
	void Roll();			//�任������̬
	void RemoveLines();		//�Ƴ��в���
	bool CanRemoveLine(int index);//�Ƿ�����Ƴ�һ��
	void RemoveLine(int index);	//�Ƴ�һ��
	bool IsDead();				//��Ϸ�Ƿ����
	int Score;
	int TickTime;
};