#pragma once

typedef struct _BYTE_AUTOPTR
{
	unsigned char * m_buffer;
	size_t m_Size;
	_BYTE_AUTOPTR(size_t pAlloc) : m_buffer(new unsigned char[pAlloc]),m_Size(pAlloc) { }
	~_BYTE_AUTOPTR(void) { delete [] m_buffer; }

	void sprintf_s(const char * qFormat,...)
	{
		va_list args;
		va_start(args, qFormat);
		int r =  vsprintf_s((char *)m_buffer,m_Size,(char *)qFormat, args);
		va_end(args);
	}

	unsigned char * Buffer() const { return m_buffer; }
	void Buffer(unsigned char * val) { m_buffer = val; }
	size_t Size() const { return m_Size; }
	void Size(size_t val) { m_Size = val; }
}BYTE_AUTOPTR;
