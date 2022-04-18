#include "pch.h"
#include "Game.h"

Game::Game(int height, int width) : _tool(0), _nextTool(0),Score(0),TickTime(500)
{
	_netWidth = width;
	_netHeight = height;
	_bigNet = _bigNetAux = NULL;
	_state = STOP;

	// ������Ϸ�����ʾ����
	_bigNet = new int[_netHeight * _netWidth];
	// ������Ϸ�����ʾ��������
	_bigNetAux = new int[_netHeight * _netWidth];
	// ��ʼ����Ϸ�����ʾ����Ϊ0
	for (int i = 0; i < _netHeight; i++)
		for (int j = 0; j < _netWidth; j++)
			_bigNet[i * _netWidth + j] = 0;
	// ��������֣���������˹������״�����ʹ��
	srand((unsigned int)time(0));
}

//��������
Game::~Game(void)
{
	delete[] _bigNet;
	delete[] _bigNetAux;
}

//��ʼ��Ϸ
void Game::Start()
{
	Score = 0;
	TickTime = 500;
	// ��ʼ����Ϸ״̬Ϊ����״̬
	_state = GO;
	// ��ʼ����Ϸ�����ʾ����Ϊ0
	for (int i = 0; i < _netHeight; i++)
		for (int j = 0; j < _netWidth; j++)
			_bigNet[i * _netWidth + j] = 0;
	//	�����������η���Ĳ���
	NextTool();
	NextTool();
}

//������һ�����飬�����µĻ�ľ
void Game::NextTool()
{
	//����4*4�������ڵ����Ͻ�����
	_iLocX = (_netWidth - 4) / 2; _iLocY = 0;
	_tool = _nextTool;
	//������һ������˹����
	_nextTool = Tool(rand() % 7 + 1);
}

//�����Ƿ���������ƶ�
bool Game::CanMoveDown()
{
	int cnt1 = 4, cnt2 = 0;
	cnt1 += CountNoneZero(_bigNet, _netHeight, _netWidth);
	// ���跢���任
	AddToolToAux(_bigNetAux, _iLocX, _iLocY + 1, _tool);
	// ͳ�Ʊ任�󷽿���
	cnt2 = CountNoneZero(_bigNetAux, _netHeight, _netWidth);
	return cnt2 == cnt1;
}

//���������в�Ϊ0��Ԫ����
int Game::CountNoneZero(int* matrix, int Height, int Width)
{
	int cnt = 0;
	for (int i = 0; i < Height; i++)
		for (int j = 0; j < Width; j++) {
			if (matrix[i * Width + j] != 0)
				++cnt;
		}
	return cnt;
}

//������˹�����Ǽ��뵽ָ��������������
void Game::AddToolToAux(int* net, int iOffsetX, int iOffsetY
	, const Tool& _tool)
{
	//��Ҫʱ�ȸ���������������ָ������
	if (net != _bigNet)
		for (int i = 0; i < _netHeight; i++)
			for (int j = 0; j < _netWidth; j++)
				net[i * _netWidth + j] = _bigNet[i * _netWidth + j];
	int iType = _tool.GetType();
	if (iType != 0) {
		//�ϲ�����˹��������
		for (int i = 0; i < 4; i++)
			for (int j = 0; j < 4; j++) {
				if (i + iOffsetY >= 0 &&
					i + iOffsetY < _netHeight &&
					j + iOffsetX >= 0 &&
					j + iOffsetX < _netWidth &&
					_tool.ElementAt(i, j) != 0)
					net[(i + iOffsetY) * _netWidth + j + iOffsetX] = iType;
			}
	}
}

//���������ƶ�һ��
void Game::MoveDown()
{
	++_iLocY;
}

//�����Ƿ���������ƶ�
bool Game::CanMoveLeft()
{
	int cnt1 = 4, cnt2 = 0;
	cnt1 += CountNoneZero(_bigNet, _netHeight, _netWidth);
	// ���跢���任
	AddToolToAux(_bigNetAux, _iLocX - 1, _iLocY, _tool);
	// ͳ�Ʊ任�󷽿���
	cnt2 = CountNoneZero(_bigNetAux, _netHeight, _netWidth);
	return cnt2 == cnt1;
}

//�����Ƿ���������ƶ�
bool Game::CanMoveRight()
{
	int cnt1 = 4, cnt2 = 0;
	cnt1 += CountNoneZero(_bigNet, _netHeight, _netWidth);
	// ���跢���任
	AddToolToAux(_bigNetAux, _iLocX + 1, _iLocY, _tool);
	// ͳ�Ʊ任�󷽿���
	cnt2 = CountNoneZero(_bigNetAux, _netHeight, _netWidth);
	return cnt2 == cnt1;
}

//���������ƶ�һ��
void Game::MoveLeft()
{
	--_iLocX;
}

//���������ƶ�һ��
void Game::MoveRight()
{
	++_iLocX;
}

//�����Ƿ���Ա任��̬
bool Game::CanRoll()
{
	int cnt1 = 4, cnt2 = 0;
	cnt1 += CountNoneZero(_bigNet, _netHeight, _netWidth);
	// ���跢���任
	Tool  toolRotated = _tool.Roll();
	AddToolToAux(_bigNetAux, _iLocX, _iLocY, toolRotated);
	// ͳ�Ʊ任�󷽿���
	cnt2 = CountNoneZero(_bigNetAux, _netHeight, _netWidth);
	return cnt2 == cnt1;
}

//�任������̬
void Game::Roll()
{
	//����ת�����˹�������
	_tool = _tool.Roll();
}

//����������մ�����
void Game::Input(UINT nChar)
{
	switch (nChar) {
	case VK_UP: if (CanRoll())     Roll(); break;
	case VK_DOWN: if (CanMoveDown()) MoveDown(); break;
	case VK_LEFT: if (CanMoveLeft()) MoveLeft(); break;
	case VK_RIGHT: if (CanMoveRight()) MoveRight(); break;
	}
}

//����һ��
bool Game::Go()
{
	if (CanMoveDown()) {	//�ж��Ƿ���������ƶ�һ��
		MoveDown();			//�����ƶ�һ��
		return true;
	}
	else {
		//������ӵ���Ϸ������������
		AddToolToAux(_bigNet, _iLocX, _iLocY, _tool);
		RemoveLines();		//�Ƴ�����
		NextTool();			//������м�����һ������
		if (IsDead()) {
			_state = STOP;	//������Ϸ״̬Ϊֹͣ
			return false;
		}
		return true;
	}
}

//�Ƴ��в���
void Game::RemoveLines()
{
	for (int i = _netHeight - 1; i >= 0; i--)
		while (CanRemoveLine(i)) {
			RemoveLine(i);
			Score++;
			if (Score > 30) {
				TickTime = 100;
			}
			else if (Score>20) {
				TickTime = 250;
			}
			else if (Score > 10) {
				TickTime = 400;
			}
			else {
				TickTime = 500;
			}
		}
			
}

//�Ƿ�����Ƴ�һ��
bool Game::CanRemoveLine(int index)
{
	int count = 0;
	for (int i = 0; i < _netWidth; i++)
		if (_bigNet[index * _netWidth + i] != 0)
			count++;
	//С��������������������ʱ���п�����
	return count == _netWidth;
}

//�Ƴ�һ��
void Game::RemoveLine(int index)
{
	for (int i = index; i > 0; i--) //ǰ������������һ��
		for (int j = 0; j < _netWidth; j++)
			_bigNet[i * _netWidth + j] = _bigNet[(i - 1) * _netWidth + j];
	for (int j = 0; j < _netWidth; j++)
		_bigNet[j] = 0; //��1����0
}

//��Ϸ�Ƿ����
bool Game::IsDead()
{
	int cnt1 = 4, cnt2 = 0;
	cnt1 += CountNoneZero(_bigNet, _netHeight, _netWidth);
	// ���跢���任
	AddToolToAux(_bigNetAux, _iLocX, _iLocY, _tool);
	// ͳ�Ƽ������˹�����С������
	cnt2 = CountNoneZero(_bigNetAux, _netHeight, _netWidth);
	//���Ⱦ������ص����������������Dead
	return cnt2 != cnt1;
}

//��ͣ/�������Ŀ��ƺ���
void Game::PauseOrContinue()
{
	if (_state == PAUSE)
		_state = GO;
	else if (_state == GO)
		_state = PAUSE;
}

int Game::GetScore()
{
	return Score;
}

int Game::GetTickTime()
{
	return TickTime;
}
