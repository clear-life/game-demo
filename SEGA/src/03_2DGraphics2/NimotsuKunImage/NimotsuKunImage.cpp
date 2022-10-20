#include "GameLib/Framework.h"

using namespace GameLib;

#include "State.h"
#include "File.h"

void mainLoop();

State* gState = 0;		// 地图对象指针

namespace GameLib
{
	void Framework::update() {
		mainLoop();
	}
}

void mainLoop()
{
	if (Framework::instance().isEndRequested())		// requestEnd() 是否被调用过, 被调用过说明程序该结束了
	{
		if (gState)			// delete 指针
		{
			delete gState;
			gState = 0;
		}
		return;
	}

	if (!gState)	// 创建地图
	{
		File file("stageData.txt");
		if (!file.data())
		{
			cout << "无法读取地图文件" << endl;
			return;
		}

		gState = new State(file.data(), file.size());

		gState->draw();

		return;
	}

	bool cleared = false;
	if (gState->hasCleared()) cleared = true;

	cout << "a:左 s:右 w:上 z:下  请输入:" << endl;
	char input;
	cin >> input;

	if (input == 'q')		// 按下 'q' 后，调用 requestEnd() 函数表明程序要结束了
	{
		delete gState;
		gState = 0;
		Framework::instance().requestEnd();
		return;
	}

	gState->update(input);

	gState->draw();

	if (cleared)
	{
		cout << "恭喜通关" << endl;
		
		delete gState;
		gState = 0;
	}
}