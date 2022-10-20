#ifndef INCLUDED_FRAMEWORK_H
#define INCLUDED_FRAMEWORK_H

class Framework
{
public:
	static unsigned* videoMemory();		// 获取帧的数据地址
	static int width();					// 帧的宽度
	static int height();				// 帧的高度
	void update();						// 帧的迭代
	void requestEnd();					// 请求结束
	bool isEndRequestd();				// 是否请求过结束
};

#endif