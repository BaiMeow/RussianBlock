#pragma once
class Tool
{
private:
	int _data[4][4];
	int _type;
public:
	Tool(int type);
	~Tool();
	const int GetType() const;
	const int& ElementAt(int i, int j)const;
	Tool Roll();
};

