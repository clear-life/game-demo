#ifndef INCLUDED_STATE_H
#define INCLUDED_STATE_H

#include "Array2D.h"

class Image;

class State
{
private:
	// 地图上格子类别
	enum Object
	{
		OBJ_SPACE,	// 空气
		OBJ_WALL,	// 墙
		OBJ_BLOCK,	// 箱子
		OBJ_MAN,	// 人

		OBJ_UNKNOWN,
	};

	// 图片类别
	enum ImageID
	{
		IMAGE_ID_PLAYER,	// 人
		IMAGE_ID_WALL,		// 墙
		IMAGE_ID_BLOCK,		// 箱子
		IMAGE_ID_BLOCK_ON_GOAL,	// 在目的地的箱子
		IMAGE_ID_GOAL,		// 目的地
		IMAGE_ID_SPACE,		// 空气
	};
	/*
	mWidth: 地图宽度
	mHeight: 地图高度
	单位: 逻辑单位，无实际单位，即一个单位表示一个格子的边长
	*/
	int mWidth;
	int mHeight;

	Array2D<Object> mObjects;
	Array2D<bool> mGoalFlags;

	Image* mImage;

	void setSize(const char* stageData, int size);	// 输入：原始文件数据，数据大小，功能：设置地图宽度和高度
	void drawCell(int x, int y, ImageID) const;		// 绘制原始地图的 (x, y) 方格，绘制为 ImageID 对应的图片

public:
	State(const char* stageData, int size);			// 根据原始地图数据和大小，构造初始地图
	~State();
	void update(char input);						// 根据输入更新地图数	据
	void draw() const;								// 在窗口内绘制地图
	bool hasCleared() const;						// 判断游戏是否结束
};

#endif