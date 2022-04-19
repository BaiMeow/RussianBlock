#include "pch.h"
#include "Game.h"

Game::Game(int height, int width) : _nextTool(), _score(0),_tool(),_lock()
{
	_netWidth = width;
	_netHeight = height;
	_bigNet = _bigNetAux[0]= _bigNetAux[1] = NULL;
	_state = STOP;

	// 分配游戏界面表示数组
	_bigNet = new int[_netHeight * _netWidth];
	// 分配游戏界面表示辅助数组
	_bigNetAux[0] = new int[_netHeight * _netWidth];
	_bigNetAux[1] = new int[_netHeight * _netWidth];
	// 初始化游戏界面表示数组为0
	for (int i = 0; i < _netHeight; i++)
		for (int j = 0; j < _netWidth; j++)
			_bigNet[i * _netWidth + j] = 0;
	// 随机数播种，产生俄罗斯方块形状随机数使用
	srand((unsigned int)time(0));
}

//析构函数
Game::~Game(void)
{
	delete[] _bigNet;
	delete[] _bigNetAux[0];
	delete[] _bigNetAux[1];
}

//开始游戏
void Game::Start()
{
	_score = 0;
	_difficulty = 1;
	// 初始化游戏状态为运行状态
	_state = GO;
	// 初始化游戏界面表示数组为0
	for (int i = 0; i < _netHeight; i++)
		for (int j = 0; j < _netWidth; j++)
			_bigNet[i * _netWidth + j] = 0;
	//	连续调用两次方块的产生
	NextTool(1);
	NextTool(1);
	NextTool(0);
	NextTool(0);
}

//生成下一个方块，更换新的积木
void Game::NextTool(int i)
{
	//设置4*4方块所在的左上角坐标
	_iLocX[i] = (_netWidth-8)/ 3*(i+1)+i*4;
	_iLocY[i] = 0;
	_tool[i] = _nextTool[i];
	//更新下一个俄罗斯方块
	_nextTool[i] = Tool(rand() % 7 + 1);
}

//方块是否可以向下移动
bool Game::CanMoveDown(int i)
{
	int cnt1 = 8, cnt2 = 0;
	cnt1 += CountNoneZero(_bigNet, _netHeight, _netWidth);
	// 假设发生变换
	AddToolToAux(_bigNetAux[i], _iLocX[1 - i], _iLocY[1 - i], _tool[1-i]);
	AddToolToAux(_bigNetAux[i], _iLocX[i], _iLocY[i] + 1, _tool[i],false);
	// 统计变换后方块数
	cnt2 = CountNoneZero(_bigNetAux[i], _netHeight, _netWidth);
	return cnt2 == cnt1;
}

//计算数组中不为0的元素数
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

//将俄罗斯方块标记加入到指定主界面数组中
int *Game::AddToolToAux(int* net, int iOffsetX, int iOffsetY
	, const Tool& _tool,bool allowcopy)
{
	//必要时先复制主界面数组至指定数组
	if (allowcopy && net != _bigNet)
			for (int i = 0; i < _netHeight; i++)
				for (int j = 0; j < _netWidth; j++)
					net[i * _netWidth + j] = _bigNet[i * _netWidth + j];

	
	int iType = _tool.GetType();
	if (iType != 0) {
		//合并俄罗斯方块数据
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

//方块向下移动一格
void Game::MoveDown(int i)
{
	++_iLocY[i];
}

//方块是否可以向左移动
bool Game::CanMoveLeft(int i)
{
	if (i == 1&& _iLocY[i]<=4 &&_iLocX[i]-1 < _netWidth / 2 +1) {
		return false;
	}

	int cnt1 = 8, cnt2 = 0;
	cnt1 += CountNoneZero(_bigNet, _netHeight, _netWidth);
	// 假设发生变换
	AddToolToAux(_bigNetAux[i], _iLocX[i] - 1, _iLocY[i], _tool[i]);
	AddToolToAux(_bigNetAux[i], _iLocX[1 - i], _iLocY[1 - i], _tool[1 - i], false);
	// 统计变换后方块数
	cnt2 = CountNoneZero(_bigNetAux[i], _netHeight, _netWidth);
	return cnt2 == cnt1;
}

//方块是否可以向右移动
bool Game::CanMoveRight(int i)
{
	if (i == 0 &&_iLocY[i]<=4 && _iLocX[i] + 1 > _netWidth / 2 -3) {
		return false;
	}

	int cnt1 = 8, cnt2 = 0;
	cnt1 += CountNoneZero(_bigNet, _netHeight, _netWidth);
	// 假设发生变换
	AddToolToAux(_bigNetAux[i], _iLocX[i] + 1, _iLocY[i], _tool[i]);
	AddToolToAux(_bigNetAux[i], _iLocX[1 - i], _iLocY[1 - i], _tool[1 - i], false);
	// 统计变换后方块数
	cnt2 = CountNoneZero(_bigNetAux[i], _netHeight, _netWidth);
	return cnt2 == cnt1;
}

//方块向左移动一格
void Game::MoveLeft(int i)
{
	--_iLocX[i];
}

//方块向右移动一格
void Game::MoveRight(int i)
{
	++_iLocX[i];
}

//方块是否可以变换形态
bool Game::CanRoll(int i)
{
	int cnt1 = 8, cnt2 = 0;
	cnt1 += CountNoneZero(_bigNet, _netHeight, _netWidth);
	// 假设发生变换
	Tool  toolRotated = _tool[i].Roll();
	AddToolToAux(_bigNetAux[i], _iLocX[i], _iLocY[i], toolRotated);
	AddToolToAux(_bigNetAux[i], _iLocX[1 - i], _iLocY[1 - i], _tool[1 - i], false);
	// 统计变换后方块数
	cnt2 = CountNoneZero(_bigNetAux[i], _netHeight, _netWidth);
	return cnt2 == cnt1;
}

//变换方块形态
void Game::Roll(int i)
{
	//用旋转后俄罗斯方块代替
	_tool[i] = _tool[i].Roll();
}

//键盘输入接收处理函数
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

//运行一步
bool Game::Go()
{		
	std::lock_guard<std::mutex> lock(_lock);
	int index[2];
	GetRefreshIndex(index);

	for (int i = 0; i < 2; ++i) {
		if (CanMoveDown(index[i])) {	//判断是否可以向下移动一格
			MoveDown(index[i]);			//向下移动一格
		}
		else {
			//将方块加到游戏主界面数组中
			AddToolToAux(_bigNet, _iLocX[index[i]], _iLocY[index[i]], _tool[index[i]]);
			RemoveLines();		//移除满行
			NextTool(index[i]);			//向界面中加入下一个方块
			if (IsDead()) {
				_state = STOP;	//设置游戏状态为停止
				return false;
			}
		}
	}
	return true;

}

//移除行操作
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

//是否可以移除一行
bool Game::CanRemoveLine(int index)
{
	int count = 0;
	for (int i = 0; i < _netWidth; i++)
		if (_bigNet[index * _netWidth + i] != 0)
			count++;
	//小方块数量等于主界面宽度时本行可消除
	return count == _netWidth;
}

//移除一行
void Game::RemoveLine(int index)
{
	for (int i = index; i > 0; i--) //前面所有行下移一行
		for (int j = 0; j < _netWidth; j++)
			_bigNet[i * _netWidth + j] = _bigNet[(i - 1) * _netWidth + j];
	for (int j = 0; j < _netWidth; j++)
		_bigNet[j] = 0; //第1行清0
}

//游戏是否结束
bool Game::IsDead()
{	
	int cnt1 = 8, cnt2 = 0;
	cnt1 += CountNoneZero(_bigNet, _netHeight, _netWidth);
	// 假设发生变换
	AddToolToAux(_bigNetAux[0], _iLocX[0], _iLocY[0], _tool[0]);
	AddToolToAux(_bigNetAux[0], _iLocX[1], _iLocY[1], _tool[1],false);
	// 统计加入俄罗斯方块后小方块数
	cnt2 = CountNoneZero(_bigNetAux[0], _netHeight, _netWidth);
	//不等就是有重叠的情况发生，代表Dead
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

//暂停/继续键的控制函数
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
