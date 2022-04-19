#pragma once

#include "Tool.h"
#include <mutex>
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
	int* _bigNetAux[2];			//��Ϸ�������ʾ��������
	int  _netWidth, _netHeight;	//����Ϸ����Ŀ�ȡ��߶�
	Tool _tool[2];					//��ǰ��Ϸ��������������ķ���
	Tool _nextTool[2];				//��һ���������ֳ��ֵ����䷽��
	int _iLocX[2], _iLocY[2];			//�������꣬����˹�������Ͻ�λ��
	int _score;
	int _difficulty;
	std::mutex _lock;

public:
	Game(int height, int width);
	~Game();
	void Start();					//��ʼ��Ϸ
	void Input(UINT nChar);			//����������մ�����
	bool Go();						//����һ��
	void PauseOrContinue();			//��ͣ/�������Ŀ��ƺ���
	int GetScore();
	int GetTickTime();
	int GetDifficulty();
private:
	void NextTool(int i);				//������һ������
	bool CanMoveDown(int i);				//�����Ƿ���������ƶ�
	//��������е�һά�����в�Ϊ0��Ԫ����
	int  CountNoneZero(int* matrix, int Height, int Width);
	//������˹�����Ǽ��뵽ָ��������������
	int *AddToolToAux(int* net, int iOffsetX, int iOffsetY,
		const Tool& _tool);
	int *AddToolToAux(int* net, int iOffsetX, int iOffsetY,
		const Tool& _tool,bool allowcopy);
	void MoveDown(int i);		//���������ƶ�һ��
	bool CanMoveLeft(int i);		//�����Ƿ���������ƶ�
	bool CanMoveRight(int i);	//�����Ƿ���������ƶ�
	void MoveLeft(int i);		//���������ƶ�һ��
	void MoveRight(int i);		//���������ƶ�һ��
	bool CanRoll(int i);			//�����Ƿ���Ա任��̬
	void Roll(int i);			//�任������̬
	void RemoveLines();		//�Ƴ��в���
	bool CanRemoveLine(int index);//�Ƿ�����Ƴ�һ��
	void RemoveLine(int index);	//�Ƴ�һ��
	bool IsDead();				//��Ϸ�Ƿ����
	void GetRefreshIndex(int index[]);
};