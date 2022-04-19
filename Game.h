#pragma once

#include "Tool.h"
#include <mutex>
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
	int* _bigNetAux[2];			//游戏主界面表示辅助数组
	int  _netWidth, _netHeight;	//主游戏界面的宽度、高度
	Tool _tool[2];					//当前游戏主界面正在下落的方块
	Tool _nextTool[2];				//下一个即将出现出现的下落方块
	int _iLocX[2], _iLocY[2];			//方块坐标，俄罗斯方块左上角位置
	int _score;
	int _difficulty;
	std::mutex _lock;

public:
	Game(int height, int width);
	~Game();
	void Start();					//开始游戏
	void Input(UINT nChar);			//键盘输入接收处理函数
	bool Go();						//运行一步
	void PauseOrContinue();			//暂停/继续键的控制函数
	int GetScore();
	int GetTickTime();
	int GetDifficulty();
private:
	void NextTool(int i);				//生成下一个方块
	bool CanMoveDown(int i);				//方块是否可以向下移动
	//计算参数中的一维数组中不为0的元素数
	int  CountNoneZero(int* matrix, int Height, int Width);
	//将俄罗斯方块标记加入到指定主界面数组中
	int *AddToolToAux(int* net, int iOffsetX, int iOffsetY,
		const Tool& _tool);
	int *AddToolToAux(int* net, int iOffsetX, int iOffsetY,
		const Tool& _tool,bool allowcopy);
	void MoveDown(int i);		//方块向下移动一格
	bool CanMoveLeft(int i);		//方块是否可以向左移动
	bool CanMoveRight(int i);	//方块是否可以向右移动
	void MoveLeft(int i);		//方块向左移动一格
	void MoveRight(int i);		//方块向右移动一格
	bool CanRoll(int i);			//方块是否可以变换形态
	void Roll(int i);			//变换方块形态
	void RemoveLines();		//移除行操作
	bool CanRemoveLine(int index);//是否可以移除一行
	void RemoveLine(int index);	//移除一行
	bool IsDead();				//游戏是否结束
	void GetRefreshIndex(int index[]);
};