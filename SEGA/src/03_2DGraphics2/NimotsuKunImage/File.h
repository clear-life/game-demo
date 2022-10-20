#ifndef INCLUDED_FILE_H
#define INCLUDED_FILE_H

class File {
private:
	/*
	mSize: 文件大小，单位字节
	mData: 文件内容
	*/
	int mSize;	
	char* mData;

public:
	File(const char* filename);
	~File();
	int size() const;
	const char* data() const;
	unsigned getUnsigned(int position) const;	// 获取从 position ~ position + 3 的 unsigned 值
};

#endif
