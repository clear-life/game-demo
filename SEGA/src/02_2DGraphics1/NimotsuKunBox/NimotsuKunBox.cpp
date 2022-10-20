#include "GameLib/Framework.h"
using namespace GameLib;

#include <algorithm>
#include <fstream>
using namespace std;

// 函数原型
void readFile(char** buffer, int* size, const char* filename);
void mainLoop();

// 二维数组模板类
template<class T> 
class Array2D
{
private:
	T* mArray;
	int mSize0;
	int mSize1;

public:
	Array2D() : mArray(0) {}
	~Array2D()
	{
		delete[] mArray;
		mArray = 0;
	}

	void setSize(int size0, int size1)
	{
		mSize0 = size0;
		mSize1 = size1;
		mArray = new T[size0 * size1];
	}

	T& operator()(int index0, int index1)
	{
		return mArray[index1 * mSize0 + index0];
	}

	const T& operator()(int index0, int index1) const
	{
		return mArray[index1 * mSize0 + index0];
	}
};

// 状态类
class State
{
private:
	enum Object
	{
		OBJ_SPACE,
		OBJ_WALL,
		OBJ_BLOCK,
		OBJ_MAN,

		OBJ_UNKNOWN,
	};

	int mWidth;
	int mHeight;
	Array2D<Object> mObjects;
	Array2D<bool> mGoalFlags;

	void setSize(const char* stageData, int size);
	// 绘制网格
	static void drawCell(int x, int y, unsigned color);		

public:
	State(const char* stageData, int size);
	void update(char input);
	void draw() const;
	bool hasCleared() const;
};

State* gState = 0;

namespace GameLib
{
	void Framework::update()
	{
		mainLoop();
	}
}

// 主循环
void mainLoop()
{
	// 第一帧初始化
	if (!gState)
	{
		const char* filename = "stageData.txt";
		char* stageData;
		int fileSize;

		readFile(&stageData, &fileSize, filename);

		if (!stageData)
		{
			cout << "地图文件无法读取" << endl;
			return;
		}

		gState = new State(stageData, fileSize);

		delete[] stageData;
		stageData = 0;

		gState->draw();

		return;
	}

	// 主循环
	bool cleared = false;
	if (gState->hasCleared()) cleared = true;		// 通关检测

	cout << "a:左 s:右 w:上 z:下 请输入:" << endl;

	char input;				// 获取输入
	cin >> input;

	gState->update(input);	// 刷新
		
	gState->draw();			// 绘制

	if (cleared)
	{
		cout << "恭喜通关" << endl;

		delete gState;
		gState = 0;
	}
}

void readFile(char** buffer, int* size, const char* filename)
{
	ifstream in(filename, ifstream::binary);

	if (!in)
	{
		*buffer = 0;
		*size = 0;
	}
	else
	{
		in.seekg(0, ifstream::end);
		*size = static_cast<int>(in.tellg());
		*buffer = new char[*size];

		in.seekg(0, ifstream::beg);
		in.read(*buffer, *size);
	}
}

State::State(const char* stageData, int size)
{
	setSize(stageData, size);				// 确定宽度和高度

	mObjects.setSize(mWidth, mHeight);		// 申请空间
	mGoalFlags.setSize(mWidth, mHeight);

	for (int y = 0; y < mHeight; ++y)		// 初始化
		for (int x = 0; x < mWidth; ++x)
		{
			mObjects(x, y) = OBJ_WALL;
			mGoalFlags(x, y) = false;
		}

	int x = 0, y = 0;
	for (int i = 0; i < size; ++i)
	{
		Object t;
		bool goalFlag = false;

		switch (stageData[i])
		{
		case '#': t = OBJ_WALL; break;
		case ' ': t = OBJ_SPACE; break;
		case 'o': t = OBJ_BLOCK; break;
		case 'O': t = OBJ_BLOCK; goalFlag = true; break;
		case '.': t = OBJ_SPACE; goalFlag = true; break;
		case 'p': t = OBJ_MAN; break;
		case 'P': t = OBJ_MAN; goalFlag = true; break;
		case '\n': x = 0; ++y; t = OBJ_UNKNOWN; break;
		default: t = OBJ_UNKNOWN; break;
		}

		if (t != OBJ_UNKNOWN)
		{
			mObjects(x, y) = t;
			mGoalFlags(x, y) = goalFlag;
			++x;
		}
	}
}

void State::setSize(const char* stageData, int size)
{
	mWidth = mHeight = 0;

	int x = 0, y = 0;
	for (int i = 0; i < size; ++i)
	{
		switch (stageData[i])
		{
		case '#': case ' ': case 'o': case 'O':
		case '.': case 'p': case 'P':
			++x;
			break;
		case '\n':
			++y;
			mWidth = max(mWidth, x);
			mHeight = max(mHeight, y);
			x = 0;
			break;
		}
	}
}

void State::draw() const
{
	for (int y = 0; y < mHeight; ++y)
	{
		for (int x = 0; x < mWidth; ++x)
		{
			Object o = mObjects(x, y);
			bool goalFlag = mGoalFlags(x, y);

			unsigned color = 0;
			if (goalFlag)
			{
				switch (o)
				{
				case OBJ_SPACE: cout << '.'; color = 0x0000ff; break;
				case OBJ_WALL: cout << '#'; color = 0xffffff; break;
				case OBJ_BLOCK: cout << 'O'; color = 0xff00ff; break;
				case OBJ_MAN: cout << 'P'; color = 0x00ffff; break;
				}
			}
			else
			{
				switch (o) {
				case OBJ_SPACE: cout << ' '; color = 0x000000; break;
				case OBJ_WALL: cout << '#'; color = 0xffffff; break;
				case OBJ_BLOCK: cout << 'o'; color = 0xff0000; break;
				case OBJ_MAN: cout << 'p'; color = 0x00ff00; break;
				}
			}
			drawCell(x, y, color);
		}
		cout << endl;
	}
}

void State::drawCell(int x, int y, unsigned color)
{
	unsigned* vram = Framework::instance().videoMemory();
	unsigned windowWidth = Framework::instance().width();
	for (int i = 0; i < 16; ++i)
		for (int j = 0; j < 16; ++j)
			vram[(y * 16 + j) * windowWidth + x * 16 + i] = color;
}

void State::update(char input)
{
	int dx = 0, dy = 0;

	switch (input)
	{
	case 'a': dx = -1; break;
	case 's': dx = 1; break;
	case 'w': dy = -1; break;
	case 'z': dy = 1; break;
	}

	int w = mWidth, h = mHeight;

	Array2D<Object>& o = mObjects;

	int x = -1, y = -1;
	bool found = false;

	for (y = 0; y < h; ++y)
	{
		for (x = 0; x < w; ++x)
		{
			if (o(x, y) == OBJ_MAN)
			{
				found = true;
				break;
			}
		}
		if (found) break;
	}

	int tx = x + dx, ty = y + dy;
	if (tx < 0 || tx >= w || ty < 0 || ty >= h) return;

	if (o(tx, ty) == OBJ_SPACE)
	{
		o(tx, ty) = OBJ_MAN;
		o(x, y) = OBJ_SPACE;
	}
	else if (o(tx, ty) == OBJ_BLOCK)
	{
		int tx2 = tx + dx, ty2 = ty + dy;
		if (tx2 < 0 || tx2 >= w || ty2 < 0 || ty2 >= h) return;

		if (o(tx2, ty2) == OBJ_SPACE)
		{
			o(tx2, ty2) = OBJ_BLOCK;
			o(tx, ty) = OBJ_MAN;
			o(x, y) = OBJ_SPACE;
		}
	}
}

bool State::hasCleared() const
{
	for (int y = 0; y < mHeight; ++y)
		for (int x = 0; x < mWidth; ++x)
			if (mObjects(x, y) == OBJ_BLOCK && mGoalFlags(x, y) == false)
				return false;
	return true;
}