#include "pch.h"
#include "Game.h"

Game::Game(int height, int width) : _tool(0), _nextTool(0),Score(0),TickTime(500)
{
	_netWidth = width;
	_netHeight = height;
	_bigNet = _bigNetAux = NULL;
	_state = STOP;

	// 分配游戏界面表示数组
	_bigNet = new int[_netHeight * _netWidth];
	// 分配游戏界面表示辅助数组
	_bigNetAux = new int[_netHeight * _netWidth];
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
	delete[] _bigNetAux;
}

//开始游戏
void Game::Start()
{
	Score = 0;
	TickTime = 500;
	// 初始化游戏状态为运行状态
	_state = GO;
	// 初始化游戏界面表示数组为0
	for (int i = 0; i < _netHeight; i++)
		for (int j = 0; j < _netWidth; j++)
			_bigNet[i * _netWidth + j] = 0;
	//	连续调用两次方块的产生
	NextTool();
	NextTool();
}

//生成下一个方块，更换新的积木
void Game::NextTool()
{
	//设置4*4方块所在的左上角坐标
	_iLocX = (_netWidth - 4) / 2; _iLocY = 0;
	_tool = _nextTool;
	//更新下一个俄罗斯方块
	_nextTool = Tool(rand() % 7 + 1);
}

//方块是否可以向下移动
bool Game::CanMoveDown()
{
	int cnt1 = 4, cnt2 = 0;
	cnt1 += CountNoneZero(_bigNet, _netHeight, _netWidth);
	// 假设发生变换
	AddToolToAux(_bigNetAux, _iLocX, _iLocY + 1, _tool);
	// 统计变换后方块数
	cnt2 = CountNoneZero(_bigNetAux, _netHeight, _netWidth);
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

//将俄罗斯方块标记加入到指定主界面数组中
void Game::AddToolToAux(int* net, int iOffsetX, int iOffsetY
	, const Tool& _tool)
{
	//必要时先复制主界面数组至指定数组
	if (net != _bigNet)
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
}

//方块向下移动一格
void Game::MoveDown()
{
	++_iLocY;
}

//方块是否可以向左移动
bool Game::CanMoveLeft()
{
	int cnt1 = 4, cnt2 = 0;
	cnt1 += CountNoneZero(_bigNet, _netHeight, _netWidth);
	// 假设发生变换
	AddToolToAux(_bigNetAux, _iLocX - 1, _iLocY, _tool);
	// 统计变换后方块数
	cnt2 = CountNoneZero(_bigNetAux, _netHeight, _netWidth);
	return cnt2 == cnt1;
}

//方块是否可以向右移动
bool Game::CanMoveRight()
{
	int cnt1 = 4, cnt2 = 0;
	cnt1 += CountNoneZero(_bigNet, _netHeight, _netWidth);
	// 假设发生变换
	AddToolToAux(_bigNetAux, _iLocX + 1, _iLocY, _tool);
	// 统计变换后方块数
	cnt2 = CountNoneZero(_bigNetAux, _netHeight, _netWidth);
	return cnt2 == cnt1;
}

//方块向左移动一格
void Game::MoveLeft()
{
	--_iLocX;
}

//方块向右移动一格
void Game::MoveRight()
{
	++_iLocX;
}

//方块是否可以变换形态
bool Game::CanRoll()
{
	int cnt1 = 4, cnt2 = 0;
	cnt1 += CountNoneZero(_bigNet, _netHeight, _netWidth);
	// 假设发生变换
	Tool  toolRotated = _tool.Roll();
	AddToolToAux(_bigNetAux, _iLocX, _iLocY, toolRotated);
	// 统计变换后方块数
	cnt2 = CountNoneZero(_bigNetAux, _netHeight, _netWidth);
	return cnt2 == cnt1;
}

//变换方块形态
void Game::Roll()
{
	//用旋转后俄罗斯方块代替
	_tool = _tool.Roll();
}

//键盘输入接收处理函数
void Game::Input(UINT nChar)
{
	switch (nChar) {
	case VK_UP: if (CanRoll())     Roll(); break;
	case VK_DOWN: if (CanMoveDown()) MoveDown(); break;
	case VK_LEFT: if (CanMoveLeft()) MoveLeft(); break;
	case VK_RIGHT: if (CanMoveRight()) MoveRight(); break;
	}
}

//运行一步
bool Game::Go()
{
	if (CanMoveDown()) {	//判断是否可以向下移动一格
		MoveDown();			//向下移动一格
		return true;
	}
	else {
		//将方块加到游戏主界面数组中
		AddToolToAux(_bigNet, _iLocX, _iLocY, _tool);
		RemoveLines();		//移除满行
		NextTool();			//向界面中加入下一个方块
		if (IsDead()) {
			_state = STOP;	//设置游戏状态为停止
			return false;
		}
		return true;
	}
}

//移除行操作
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
	int cnt1 = 4, cnt2 = 0;
	cnt1 += CountNoneZero(_bigNet, _netHeight, _netWidth);
	// 假设发生变换
	AddToolToAux(_bigNetAux, _iLocX, _iLocY, _tool);
	// 统计加入俄罗斯方块后小方块数
	cnt2 = CountNoneZero(_bigNetAux, _netHeight, _netWidth);
	//不等就是有重叠的情况发生，代表Dead
	return cnt2 != cnt1;
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
	return Score;
}

int Game::GetTickTime()
{
	return TickTime;
}
