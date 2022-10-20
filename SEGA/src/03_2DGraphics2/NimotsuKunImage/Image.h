#ifndef INCLUDED_IMAGE_H
#define INCLUDED_IMAGE_H

class Image
{
private:
	/*
	mWidth: 图片宽度
	mHeight: 图片高度
	mData: 图片数据(一维数组)，单位 4 byte
	*/
	int mWidth;
	int mHeight;
	unsigned* mData;

public:
	Image(const char* filename);	// 加载 dds 图片文件
	~Image();
	int width() const;
	int height() const;
	void draw(	// 从图片的 (srcX, srcY) 为左上角的 width X height 大小矩形, 绘制到窗口的 (dstX, dstY) 为左上角的地方
		int dstX, int dstY,		
		int srcX, int srcY,
		int width, int height
	) const;
};

#endif