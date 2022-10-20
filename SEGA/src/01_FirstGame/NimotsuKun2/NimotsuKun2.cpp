#include <iostream>
#include <fstream>
#include <algorithm>

using namespace std;

// 读取文件
void readFile(char** buffer, int* size, const char* filename);

// 二维数组类
template< class T > class Array2D {
private:
	T* mArray;
	int mSize0;
	int mSize1;

public:
	Array2D() : mArray(0), mSize0(0), mSize1(0) {}
	~Array2D() {
		delete[] mArray;
		mArray = 0;
	}
	void setSize(int size0, int size1) {
		mSize0 = size0;
		mSize1 = size1;
		mArray = new T[size0 * size1];
	}
	T& operator()(int index0, int index1) {
		return mArray[index1 * mSize0 + index0];
	}
	const T& operator()(int index0, int index1) const {
		return mArray[index1 * mSize0 + index0];
	}
};

// 状态类
class State {
private:
	enum Object {
		OBJ_SPACE,		// 能走的空地, 包含目的地
		OBJ_WALL,		// 墙
		OBJ_BLOCK,		// 箱子
		OBJ_MAN,		// 人

		OBJ_UNKNOWN,	// 未知
	};
	int mWidth;			// 宽度
	int mHeight;		// 高度
	Array2D< Object > mObjects;	// 地图
	Array2D< bool > mGoalFlags;	// 是否为目的地

	void setSize(const char* stageData, int size);	// 获取地图大小

public:
	State(const char* stageData, int size);			// 构造函数
	void draw() const;								// 画图
	void update(char input);						// 执行动作
	bool hasCleared() const;						// 判断是否结束
};

int main(int argc, char** argv) {
	const char* filename = "stageData.txt";
	if (argc >= 2) {
		filename = argv[1];
	}
	char* stageData;
	int fileSize;

	// 读取初始地图
	readFile(&stageData, &fileSize, filename);
	if (!stageData) {
		cout << "stage file could not be read." << endl;
		return 1;
	}

	// 状态对象
	State* state = new State(stageData, fileSize);

	//主循环
	while (true) {
		// 画图
		state->draw();

		if (state->hasCleared()) {		//通关检测
			break;
		}

		cout << "a:left s:right w:up z:down. command?" << endl; //操作说明

		// 获取输入
		char input;
		cin >> input;

		// 执行动作
		state->update(input);
	}

	//打印通关祝贺的信息
	cout << "Congratulation's! you won." << endl;

	//析构
	delete[] stageData;
	stageData = 0;

	return 0;
}

void readFile(char** buffer, int* size, const char* filename) {
	ifstream in(filename);			// in 表示输入文件
	if (!in) {
		*buffer = 0;
		*size = 0;
	}
	else {
		in.seekg(0, ifstream::end);	// 设置指针为文件尾
		*size = static_cast<int>(in.tellg());	// 获取当前位置
		in.seekg(0, ifstream::beg);	// 设置指针为文件头
		*buffer = new char[*size];	// 申请 buffer 内存
		in.read(*buffer, *size);	// 读取整个文件到 buffer 中
	}
}

void State::setSize(const char* stageData, int size) {
	mWidth = mHeight = 0;
	//当前位置
	int x = 0;
	int y = 0;
	for (int i = 0; i < size; ++i) {
		switch (stageData[i]) {
		case '#': case ' ': case 'o': case 'O':
		case '.': case 'p': case 'P':
			++x;
			break;
		case '\n':
			++y;
			//更新最大值
			mWidth = max(mWidth, x);
			mHeight = max(mHeight, y);	// y 少计算了 1, 所以地图要多设置一行
			x = 0;
			break;
		}
	}
}

State::State(const char* stageData, int size) {
	setSize(stageData, size);				// 获取地图大小

	mObjects.setSize(mWidth, mHeight);		// 初始化地图
	mGoalFlags.setSize(mWidth, mHeight);	// 初始化目的地

	for (int y = 0; y < mHeight; ++y) {
		for (int x = 0; x < mWidth; ++x) {
			mObjects(x, y) = OBJ_WALL;		// 全部设为墙
			mGoalFlags(x, y) = false;		// 全为false
		}
	}
	int x = 0;
	int y = 0;
	for (int i = 0; i < size; ++i) {
		Object t;
		bool goalFlag = false;
		switch (stageData[i]) {
		case '#': t = OBJ_WALL; break;
		case ' ': t = OBJ_SPACE; break;
		case 'o': t = OBJ_BLOCK; break;
		case 'O': t = OBJ_BLOCK; goalFlag = true; break;
		case '.': t = OBJ_SPACE; goalFlag = true; break;
		case 'p': t = OBJ_MAN; break;
		case 'P': t = OBJ_MAN; goalFlag = true; break;
		case '\n': x = 0; ++y; t = OBJ_UNKNOWN; break; //换行处理
		default: t = OBJ_UNKNOWN; break;
		}
		if (t != OBJ_UNKNOWN) {				// 跳过未定义元素
			mObjects(x, y) = t;
			mGoalFlags(x, y) = goalFlag;	// 是否为目的地
			++x;
		}
	}
}

void State::draw() const {
	for (int y = 0; y < mHeight; ++y) {
		for (int x = 0; x < mWidth; ++x) {
			Object o = mObjects(x, y);
			bool goalFlag = mGoalFlags(x, y);
			if (goalFlag) {				// 是目的地
				switch (o) {
				case OBJ_SPACE: cout << '.'; break;
				case OBJ_WALL: cout << '#'; break;
				case OBJ_BLOCK: cout << 'O'; break;
				case OBJ_MAN: cout << 'P'; break;
				}
			}
			else {						// 不是目的地
				switch (o) {
				case OBJ_SPACE: cout << ' '; break;
				case OBJ_WALL: cout << '#'; break;
				case OBJ_BLOCK: cout << 'o'; break;
				case OBJ_MAN: cout << 'p'; break;
				}
			}
		}
		cout << endl;
	}
}

void State::update(char input) {
	// 偏移量
	int dx = 0;
	int dy = 0;
	switch (input) {
	case 'a': dx = -1; break;	// 左
	case 's': dx = 1; break;	// 右
	case 'w': dy = -1; break;	// 上
	case 'z': dy = 1; break;	// 下
	}

	int w = mWidth;
	int h = mHeight;
	Array2D< Object >& o = mObjects;

	// 人物坐标
	int x, y;
	x = y = -1; //危险值
	bool found = false;
	for (y = 0; y < mHeight; ++y) {
		for (x = 0; x < mWidth; ++x) {
			if (o(x, y) == OBJ_MAN) {
				found = true;
				break;
			}
		}
		if (found) {
			break;
		}
	}

	//移动后的坐标
	int tx = x + dx;
	int ty = y + dy;
	//边界情况判断
	if (tx < 0 || ty < 0 || tx >= w || ty >= h) {	//按键无效
		return;
	}
	//A.该方向上是空白或者终点。则小人移动
	if (o(tx, ty) == OBJ_SPACE) {
		o(tx, ty) = OBJ_MAN;
		o(x, y) = OBJ_SPACE;
	}
	//B.如果该方向上是箱子。并且该方向的下下个格子是空白或者终点，则允许移动
	else if (o(tx, ty) == OBJ_BLOCK) {
		//检测同方向上的下下个格子是否位于合理值范围
		int tx2 = tx + dx;
		int ty2 = ty + dy;
		if (tx2 < 0 || ty2 < 0 || tx2 >= w || ty2 >= h) { //按键无效
			return;
		}
		if (o(tx2, ty2) == OBJ_SPACE) {
			//按顺序替换
			o(tx2, ty2) = OBJ_BLOCK;
			o(tx, ty) = OBJ_MAN;
			o(x, y) = OBJ_SPACE;
		}
	}
}

bool State::hasCleared() const {
	for (int y = 0; y < mHeight; ++y) {
		for (int x = 0; x < mWidth; ++x) {
			if (mObjects(x, y) == OBJ_BLOCK) {
				if (mGoalFlags(x, y) == false) {
					return false;
				}
			}
		}
	}
	return true;
}