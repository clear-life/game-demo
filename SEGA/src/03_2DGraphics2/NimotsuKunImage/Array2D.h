#ifndef INCLUDED_ARRAY_2D_H
#define INCLUDED_ARRAY_2D_H

template<class T> 
class Array2D
{
private:
	/*
	mSize0 : 列数
	mSize1 : 行数
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
	数学中的 (x,y) 与数组中的 (i,j) 表示相反的含义
	数学中 (x,y) 表示横坐标为 x，纵坐标为 y, 原点在左下角(0,0)处
	数组中 (i,j）表示第 i 行(相当于纵坐标)，第 j 列(相当于横坐标), 原点在左上角处
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