#include "pch.h"
#include "Game.h"

Game::Game(int height, int width) : _nextTool(), _score(0),_tool(),_lock()
{
	_netWidth = width;
	_netHeight = height;
	_bigNet = _bigNetAux[0]= _bigNetAux[1] = NULL;
	_state = STOP;

	// ������Ϸ�����ʾ����
	_bigNet = new int[_netHeight * _netWidth];
	// ������Ϸ�����ʾ��������
	_bigNetAux[0] = new int[_netHeight * _netWidth];
	_bigNetAux[1] = new int[_netHeight * _netWidth];
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
	delete[] _bigNetAux[0];
	delete[] _bigNetAux[1];
}

//��ʼ��Ϸ
void Game::Start()
{
	_score = 0;
	_difficulty = 1;
	// ��ʼ����Ϸ״̬Ϊ����״̬
	_state = GO;
	// ��ʼ����Ϸ�����ʾ����Ϊ0
	for (int i = 0; i < _netHeight; i++)
		for (int j = 0; j < _netWidth; j++)
			_bigNet[i * _netWidth + j] = 0;
	//	�����������η���Ĳ���
	NextTool(1);
	NextTool(1);
	NextTool(0);
	NextTool(0);
}

//������һ�����飬�����µĻ�ľ
void Game::NextTool(int i)
{
	//����4*4�������ڵ����Ͻ�����
	_iLocX[i] = (_netWidth-8)/ 3*(i+1)+i*4;
	_iLocY[i] = 0;
	_tool[i] = _nextTool[i];
	//������һ������˹����
	_nextTool[i] = Tool(rand() % 7 + 1);
}

//�����Ƿ���������ƶ�
bool Game::CanMoveDown(int i)
{
	int cnt1 = 8, cnt2 = 0;
	cnt1 += CountNoneZero(_bigNet, _netHeight, _netWidth);
	// ���跢���任
	AddToolToAux(_bigNetAux[i], _iLocX[1 - i], _iLocY[1 - i], _tool[1-i]);
	AddToolToAux(_bigNetAux[i], _iLocX[i], _iLocY[i] + 1, _tool[i],false);
	// ͳ�Ʊ任�󷽿���
	cnt2 = CountNoneZero(_bigNetAux[i], _netHeight, _netWidth);
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

int *Game::AddToolToAux(int* net, int iOffsetX, int iOffsetY, const Tool& _tool)
{
	return AddToolToAux(net, iOffsetX, iOffsetY, _tool, true);
}

//������˹�����Ǽ��뵽ָ��������������
int *Game::AddToolToAux(int* net, int iOffsetX, int iOffsetY
	, const Tool& _tool,bool allowcopy)
{
	//��Ҫʱ�ȸ���������������ָ������
	if (allowcopy && net != _bigNet)
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
	return net;
}

//���������ƶ�һ��
void Game::MoveDown(int i)
{
	++_iLocY[i];
}

//�����Ƿ���������ƶ�
bool Game::CanMoveLeft(int i)
{
	if (i == 1&& _iLocY[i]<=4 &&_iLocX[i]-1 < _netWidth / 2 +1) {
		return false;
	}

	int cnt1 = 8, cnt2 = 0;
	cnt1 += CountNoneZero(_bigNet, _netHeight, _netWidth);
	// ���跢���任
	AddToolToAux(_bigNetAux[i], _iLocX[i] - 1, _iLocY[i], _tool[i]);
	AddToolToAux(_bigNetAux[i], _iLocX[1 - i], _iLocY[1 - i], _tool[1 - i], false);
	// ͳ�Ʊ任�󷽿���
	cnt2 = CountNoneZero(_bigNetAux[i], _netHeight, _netWidth);
	return cnt2 == cnt1;
}

//�����Ƿ���������ƶ�
bool Game::CanMoveRight(int i)
{
	if (i == 0 &&_iLocY[i]<=4 && _iLocX[i] + 1 > _netWidth / 2 -3) {
		return false;
	}

	int cnt1 = 8, cnt2 = 0;
	cnt1 += CountNoneZero(_bigNet, _netHeight, _netWidth);
	// ���跢���任
	AddToolToAux(_bigNetAux[i], _iLocX[i] + 1, _iLocY[i], _tool[i]);
	AddToolToAux(_bigNetAux[i], _iLocX[1 - i], _iLocY[1 - i], _tool[1 - i], false);
	// ͳ�Ʊ任�󷽿���
	cnt2 = CountNoneZero(_bigNetAux[i], _netHeight, _netWidth);
	return cnt2 == cnt1;
}

//���������ƶ�һ��
void Game::MoveLeft(int i)
{
	--_iLocX[i];
}

//���������ƶ�һ��
void Game::MoveRight(int i)
{
	++_iLocX[i];
}

//�����Ƿ���Ա任��̬
bool Game::CanRoll(int i)
{
	int cnt1 = 8, cnt2 = 0;
	cnt1 += CountNoneZero(_bigNet, _netHeight, _netWidth);
	// ���跢���任
	Tool  toolRotated = _tool[i].Roll();
	AddToolToAux(_bigNetAux[i], _iLocX[i], _iLocY[i], toolRotated);
	AddToolToAux(_bigNetAux[i], _iLocX[1 - i], _iLocY[1 - i], _tool[1 - i], false);
	// ͳ�Ʊ任�󷽿���
	cnt2 = CountNoneZero(_bigNetAux[i], _netHeight, _netWidth);
	return cnt2 == cnt1;
}

//�任������̬
void Game::Roll(int i)
{
	//����ת�����˹�������
	_tool[i] = _tool[i].Roll();
}

//����������մ�����
void Game::Input(UINT nChar)
{
	std::lock_guard<std::mutex> lock(_lock);	
	
	switch (nChar) {
	case VK_UP: if (CanRoll(1))     Roll(1); break;
	case VK_DOWN: if (CanMoveDown(1)) MoveDown(1); break;
	case VK_LEFT: if (CanMoveLeft(1)) MoveLeft(1); break;
	case VK_RIGHT: if (CanMoveRight(1)) MoveRight(1); break;
	case 'W': if (CanRoll(0))	Roll(0); break;
	case 'S': if (CanMoveDown(0)) MoveDown(0); break;
	case 'A': if (CanMoveLeft(0)) MoveLeft(0); break;
	case 'D': if (CanMoveRight(0)) MoveRight(0); break;
	}
}

//����һ��
bool Game::Go()
{		
	std::lock_guard<std::mutex> lock(_lock);
	int index[2];
	GetRefreshIndex(index);

	for (int i = 0; i < 2; ++i) {
		if (CanMoveDown(index[i])) {	//�ж��Ƿ���������ƶ�һ��
			MoveDown(index[i]);			//�����ƶ�һ��
		}
		else {
			//������ӵ���Ϸ������������
			AddToolToAux(_bigNet, _iLocX[index[i]], _iLocY[index[i]], _tool[index[i]]);
			RemoveLines();		//�Ƴ�����
			NextTool(index[i]);			//������м�����һ������
			if (IsDead()) {
				_state = STOP;	//������Ϸ״̬Ϊֹͣ
				return false;
			}
		}
	}
	return true;

}

//�Ƴ��в���
void Game::RemoveLines()
{
	for (int i = _netHeight - 1; i >= 0; i--)
		while (CanRemoveLine(i)) {
			RemoveLine(i);
			_score++;
			if (_score > 30) {
				_difficulty = 4;
			}
			else if (_score>20) {
				_difficulty = 3;
			}
			else if (_score > 10) {
				_difficulty = 2;
			}
			else {
				_difficulty = 1;
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
	int cnt1 = 8, cnt2 = 0;
	cnt1 += CountNoneZero(_bigNet, _netHeight, _netWidth);
	// ���跢���任
	AddToolToAux(_bigNetAux[0], _iLocX[0], _iLocY[0], _tool[0]);
	AddToolToAux(_bigNetAux[0], _iLocX[1], _iLocY[1], _tool[1],false);
	// ͳ�Ƽ������˹�����С������
	cnt2 = CountNoneZero(_bigNetAux[0], _netHeight, _netWidth);
	//���Ⱦ������ص����������������Dead
	if (cnt2 != cnt1) {
		return true;
	}
	return false;
}

void Game::GetRefreshIndex(int index[])
{	
	for (int i = 0; i < _netWidth * _netHeight; ++i) 
		_bigNetAux[0][i] = 0;

	AddToolToAux(_bigNetAux[0], _iLocX[0], _iLocY[0]+1, _tool[0], false);
	AddToolToAux(_bigNetAux[0], _iLocX[1], _iLocY[1], _tool[1], false);
	if (CountNoneZero(_bigNetAux[0], _netHeight, _netWidth) != 8) {
		index[0] = 1;
		index[1] = 0;
	}
	else {
		index[0] = 0;
		index[1] = 1;
	}
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
	return _score;
}

int Game::GetTickTime()
{
	switch (_difficulty)
	{
	case 1:
		return 500;
		break;
	case 2:
		return 400;
		break;
	case 3:
		return 300;
		break;
	case 4:
		return 200;
		break;
	default:
		return 500;
		break;
	}
}

int Game::GetDifficulty()
{
	return _difficulty;
}
