#ifndef INCLUDED_FRAMEWORK_H
#define INCLUDED_FRAMEWORK_H

class Framework
{
public:
	static unsigned* videoMemory();		// ��ȡ֡�����ݵ�ַ
	static int width();					// ֡�Ŀ��
	static int height();				// ֡�ĸ߶�
	void update();						// ֡�ĵ���
	void requestEnd();					// �������
	bool isEndRequestd();				// �Ƿ����������
};

#endif