#ifndef INCLUDED_STATE_H
#define INCLUDED_STATE_H

#include "Array2D.h"

class Image;

class State
{
private:
	// ��ͼ�ϸ������
	enum Object
	{
		OBJ_SPACE,	// ����
		OBJ_WALL,	// ǽ
		OBJ_BLOCK,	// ����
		OBJ_MAN,	// ��

		OBJ_UNKNOWN,
	};

	// ͼƬ���
	enum ImageID
	{
		IMAGE_ID_PLAYER,	// ��
		IMAGE_ID_WALL,		// ǽ
		IMAGE_ID_BLOCK,		// ����
		IMAGE_ID_BLOCK_ON_GOAL,	// ��Ŀ�ĵص�����
		IMAGE_ID_GOAL,		// Ŀ�ĵ�
		IMAGE_ID_SPACE,		// ����
	};
	/*
	mWidth: ��ͼ���
	mHeight: ��ͼ�߶�
	��λ: �߼���λ����ʵ�ʵ�λ����һ����λ��ʾһ�����ӵı߳�
	*/
	int mWidth;
	int mHeight;

	Array2D<Object> mObjects;
	Array2D<bool> mGoalFlags;

	Image* mImage;

	void setSize(const char* stageData, int size);	// ���룺ԭʼ�ļ����ݣ����ݴ�С�����ܣ����õ�ͼ��Ⱥ͸߶�
	void drawCell(int x, int y, ImageID) const;		// ����ԭʼ��ͼ�� (x, y) ���񣬻���Ϊ ImageID ��Ӧ��ͼƬ

public:
	State(const char* stageData, int size);			// ����ԭʼ��ͼ���ݺʹ�С�������ʼ��ͼ
	~State();
	void update(char input);						// ����������µ�ͼ��	��
	void draw() const;								// �ڴ����ڻ��Ƶ�ͼ
	bool hasCleared() const;						// �ж���Ϸ�Ƿ����
};

#endif