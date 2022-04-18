#pragma once

#include "Tool.h"
enum GAME_STATE {
	PAUSE,//暂停
	GO,  //工作
	STOP //停止
};

class Game
{
	friend class CRussianBlockDlg;
private:
	GAME_STATE _state;			//游戏状态
	int* _bigNet;				//游戏主界面表示数组
	int* _bigNetAux;			//游戏主界面表示辅助数组
	int  _netWidth, _netHeight;	//主游戏界面的宽度、高度
	Tool _tool;					//当前游戏主界面正在下落的方块
	Tool _nextTool;				//下一个即将出现出现的下落方块
	int _iLocX, _iLocY;			//方块坐标，俄罗斯方块左上角位置

public:
	Game(int height, int width);
	~Game();
	void Start();					//开始游戏
	void Input(UINT nChar);			//键盘输入接收处理函数
	bool Go();						//运行一步
	void PauseOrContinue();			//暂停/继续键的控制函数
	int GetScore();
	int GetTickTime();
private:
	void NextTool();				//生成下一个方块
	bool CanMoveDown();				//方块是否可以向下移动
	//计算参数中的一维数组中不为0的元素数
	int  CountNoneZero(int* matrix, int Height, int Width);
	//将俄罗斯方块标记加入到指定主界面数组中
	void AddToolToAux(int* net, int iOffsetX, int iOffsetY,
		const Tool& _tool);
	void MoveDown();		//方块向下移动一格
	bool CanMoveLeft();		//方块是否可以向左移动
	bool CanMoveRight();	//方块是否可以向右移动
	void MoveLeft();		//方块向左移动一格
	void MoveRight();		//方块向右移动一格
	bool CanRoll();			//方块是否可以变换形态
	void Roll();			//变换方块形态
	void RemoveLines();		//移除行操作
	bool CanRemoveLine(int index);//是否可以移除一行
	void RemoveLine(int index);	//移除一行
	bool IsDead();				//游戏是否结束
	int Score;
	int TickTime;
};