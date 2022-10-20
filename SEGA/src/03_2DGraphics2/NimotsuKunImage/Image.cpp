#include "Image.h"
#include "File.h"

#include "GameLib/Framework.h"

using namespace GameLib;

/*
.dds 图片格式:
[0,128): 格式信息
	[0,4): 文件说明符
	[12,16): 图片高度
	[16,20): 图片宽度
	图片单位: 32 bit ARGB(Direct 中叫 A8R8G8B8)
[128,) : 数据信息
*/

Image::Image(const char* filename) : mWidth(0), mHeight(0), mData(0)
{
	File f(filename);
	mHeight = f.getUnsigned(12);
	mWidth = f.getUnsigned(16);
	mData = new unsigned[mWidth * mHeight];

	for (int i = 0; i < mWidth * mHeight; ++i)
		mData[i] = f.getUnsigned(128 + i * 4);
}

Image::~Image()
{
	delete[] mData;
	mData = 0;
}

int Image::width() const
{
	return mWidth;
}

int Image::height() const
{
	return mHeight;
}

void Image::draw(		// 从图片的 (srcX, srcY) 为左上角的 width X height 大小矩形, 绘制到窗口的 (dstX, dstY) 为左上角的地方
	int dstX, int dstY,
	int srcX, int srcY,
	int width, int height
) const
{
	unsigned* vram = Framework::instance().videoMemory();
	unsigned windowWidth = Framework::instance().width();	// 窗口宽度

	for(int y = 0; y < height; ++y)		// 逐行绘制
		for (int x = 0; x < width; ++x)		
		{
			unsigned* dst = &vram[(y + dstY) * windowWidth + (x + dstX)];	// 绘制目的地：窗口(dstX, dstY）
			*dst = mData[(y + srcY) * mWidth + (x + srcX)];					// 绘制源: 图片(srcX, srcY)，绘制大小 width X height
		}
}