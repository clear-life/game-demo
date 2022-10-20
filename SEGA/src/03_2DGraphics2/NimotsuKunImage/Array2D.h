#ifndef INCLUDED_ARRAY_2D_H
#define INCLUDED_ARRAY_2D_H

template<class T> 
class Array2D
{
private:
	/*
	mSize0 : ����
	mSize1 : ����
	*/
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
		if (mArray)
		{
			delete[] mArray;
			mArray = 0;
		}

		mSize0 = size0;	
		mSize1 = size1;
		mArray = new T[size0 * size1];
	}

	/*
	��ѧ�е� (x,y) �������е� (i,j) ��ʾ�෴�ĺ���
	��ѧ�� (x,y) ��ʾ������Ϊ x��������Ϊ y, ԭ�������½�(0,0)��
	������ (i,j����ʾ�� i ��(�൱��������)���� j ��(�൱�ں�����), ԭ�������ϽǴ�
	*/
	T& operator()(int x, int y)	
	{										 
		return mArray[y * mSize0 + x];
	}
	const T& operator()(int x, int y) const  
	{
		return mArray[y * mSize0 + x];
	}
};

#endif 