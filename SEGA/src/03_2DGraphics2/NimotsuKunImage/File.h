#ifndef INCLUDED_FILE_H
#define INCLUDED_FILE_H

class File {
private:
	/*
	mSize: �ļ���С����λ�ֽ�
	mData: �ļ�����
	*/
	int mSize;	
	char* mData;

public:
	File(const char* filename);
	~File();
	int size() const;
	const char* data() const;
	unsigned getUnsigned(int position) const;	// ��ȡ�� position ~ position + 3 �� unsigned ֵ
};

#endif
