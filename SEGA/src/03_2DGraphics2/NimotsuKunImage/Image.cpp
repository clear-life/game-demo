#include "Image.h"
#include "File.h"

#include "GameLib/Framework.h"

using namespace GameLib;

/*
.dds ͼƬ��ʽ:
[0,128): ��ʽ��Ϣ
	[0,4): �ļ�˵����
	[12,16): ͼƬ�߶�
	[16,20): ͼƬ���
	ͼƬ��λ: 32 bit ARGB(Direct �н� A8R8G8B8)
[128,) : ������Ϣ
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

void Image::draw(		// ��ͼƬ�� (srcX, srcY) Ϊ���Ͻǵ� width X height ��С����, ���Ƶ����ڵ� (dstX, dstY) Ϊ���Ͻǵĵط�
	int dstX, int dstY,
	int srcX, int srcY,
	int width, int height
) const
{
	unsigned* vram = Framework::instance().videoMemory();
	unsigned windowWidth = Framework::instance().width();	// ���ڿ��

	for(int y = 0; y < height; ++y)		// ���л���
		for (int x = 0; x < width; ++x)		
		{
			unsigned* dst = &vram[(y + dstY) * windowWidth + (x + dstX)];	// ����Ŀ�ĵأ�����(dstX, dstY��
			*dst = mData[(y + srcY) * mWidth + (x + srcX)];					// ����Դ: ͼƬ(srcX, srcY)�����ƴ�С width X height
		}
}