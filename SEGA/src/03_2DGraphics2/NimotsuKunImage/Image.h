#ifndef INCLUDED_IMAGE_H
#define INCLUDED_IMAGE_H

class Image
{
private:
	/*
	mWidth: ͼƬ���
	mHeight: ͼƬ�߶�
	mData: ͼƬ����(һά����)����λ 4 byte
	*/
	int mWidth;
	int mHeight;
	unsigned* mData;

public:
	Image(const char* filename);	// ���� dds ͼƬ�ļ�
	~Image();
	int width() const;
	int height() const;
	void draw(	// ��ͼƬ�� (srcX, srcY) Ϊ���Ͻǵ� width X height ��С����, ���Ƶ����ڵ� (dstX, dstY) Ϊ���Ͻǵĵط�
		int dstX, int dstY,		
		int srcX, int srcY,
		int width, int height
	) const;
};

#endif