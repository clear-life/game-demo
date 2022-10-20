#include <iostream>

using namespace std;

// #墙 .终点 o箱子 p人
const char gStageData[] = "\
########\n\
# .. p #\n\
# oo   #\n\
#      #\n\
########";
const int gStateWidth = 8;
const int gStateHeight = 5;

enum Object
{
	OBJ_SPACE,			// 空格
	OBJ_WALL,			// 墙
	OBJ_GOAL,			// 目的地
	OBJ_BLOCK,			// 箱子
	OBJ_BLOCK_ON_GOAL,	// 到达目的地的箱子
	OBJ_MAN,			// 人
	OBJ_MAN_ON_GOAL,	// 到达目的地的人

	OBJ_UNKNOWN,		// 未知
};

// 函数声明
void initialize(Object* state, int w, int h, const char* stageData);	// 初始化
void draw(const Object* state, int w, int h);							// 画图
void update(Object* state, char input, int w, int h);					// 更新
bool checkClear(const Object* state, int w, int h);						// 检查

int main()
{
	Object* state = new Object[gStateHeight * gStateWidth];				// 当前状态

	initialize(state, gStateWidth, gStateHeight, gStageData);

	while (true)
	{
		draw(state, gStateWidth, gStateHeight);							// 画图

		if (checkClear(state, gStateWidth, gStateHeight))				// 判断是否结束
		{
			break;
		}

		cout << "a:left s:right w:up z:down." << endl;					// 提示信息

		char input;														// 获取输入
		cin >> input;

		update(state, input, gStateWidth, gStateHeight);				// 执行动作
	}

	cout << "Congratulation's! you win." << endl;

	delete[] state;
	state = 0;

	return 0;
}

// 初始化
void initialize(Object* state, int width, int /* height */, const char* stageData)
{
	const char* d = stageData;
	int x = 0;
	int y = 0;

	while (*d != '\0')
	{
		Object t;
		switch (*d)
		{
		case '#': t = OBJ_WALL; break;
		case ' ': t = OBJ_SPACE; break;
		case 'o': t = OBJ_BLOCK; break;
		case 'O': t = OBJ_BLOCK_ON_GOAL; break;
		case '.': t = OBJ_GOAL; break;
		case 'p': t = OBJ_MAN; break;
		case 'P': t = OBJ_MAN_ON_GOAL; break;
		case '\n': x = 0, ++y; t = OBJ_UNKNOWN; break;
		default: t = OBJ_UNKNOWN; break;
		}

		++d;

		if (t != OBJ_UNKNOWN)
		{
			state[y * width + x] = t;
			++x;
		}
	}
}

// 画图
void draw(const Object* state, int width, int height)
{
	const char font[] = { ' ', '#', '.', 'o', 'O', 'p', 'P' };		// 枚举值的含义
	for (int y = 0; y < height; ++y)
	{
		for (int x = 0; x < width; ++x)
		{
			Object st = state[y * width + x];
			cout << font[st];
		}
		cout << endl;
	}
}

// 执行动作
void update(Object* s, char input, int w, int h)
{
	int dx = 0;
	int dy = 0;

	switch (input)
	{
	case 'a':dx = -1; break;
	case 's':dx = 1; break;
	case 'w':dy = -1; break;
	case 'z':dy = 1; break;
	}

	int x = -1, y = -1;
	for (int i = 0; i < w * h; i++)
	{
		if (s[i] == OBJ_MAN || s[i] == OBJ_MAN_ON_GOAL)
		{
			x = i % w;
			y = i / w;
		}
	}

	int tx = x + dx;
	int ty = y + dy;
	if (tx < 0 || tx >= w || ty < 0 || ty >= h) return;

	int p = y * w + x;
	int tp = ty * w + tx;
	if (s[tp] == OBJ_SPACE || s[tp] == OBJ_GOAL)
	{
		s[tp] = s[tp] == OBJ_GOAL ? OBJ_MAN_ON_GOAL : OBJ_MAN;
		s[p] = s[p] == OBJ_MAN_ON_GOAL ? OBJ_GOAL : OBJ_SPACE;
	}
	else if (s[tp] == OBJ_BLOCK || s[tp] == OBJ_BLOCK_ON_GOAL)
	{
		int tx2 = tx + dx;
		int ty2 = ty + dy;
		if (tx2 < 0 || tx2 >= w || ty2 < 0 || ty2 >= h) return;

		int tp2 = ty2 * w + tx2;
		if (s[tp2] == OBJ_SPACE || s[tp2] == OBJ_GOAL)
		{
			s[tp2] = s[tp2] == OBJ_GOAL ? OBJ_BLOCK_ON_GOAL : OBJ_BLOCK;
			s[tp] = s[tp] == OBJ_BLOCK_ON_GOAL ? OBJ_MAN_ON_GOAL : OBJ_MAN;
			s[p] = s[p] == OBJ_MAN_ON_GOAL ? OBJ_GOAL : OBJ_SPACE;
		}
	}
}

// 判断是否结束
bool checkClear(const Object* s, int width, int height)
{
	for (int i = 0; i < width * height; ++i)
	{
		if (s[i] == OBJ_BLOCK) return false;
	}
	return true;
}