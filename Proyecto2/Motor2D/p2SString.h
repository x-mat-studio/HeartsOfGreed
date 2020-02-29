#ifndef __p2SString_H__
#define __p2SString_H__

#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <assert.h>
#include "p2Defs.h"

#define TMP_STRING_SIZE	4096

class P2SString
{
private:

	char* str;
	unsigned int size;

public:


	// Constructors
	P2SString()
	{
		Alloc(1);
		Clear();
	}


	P2SString(unsigned int size)
	{
		if (size > 0)
			Alloc(size);
		else
			Alloc(1);

		Clear();
	}


	P2SString(const P2SString& string)
	{
		Alloc(string.size);
		strcpy_s(str, size, string.str);
	}


	P2SString(const char* format, ...)
	{
		size = 0;


		if (format != NULL)
		{
			static char tmp[TMP_STRING_SIZE];
			static va_list  ap;

			// Construct the string from variable arguments
			va_start(ap, format);
			int res = vsprintf_s(tmp, TMP_STRING_SIZE, format, ap);
			va_end(ap);


			if (res > 0)
			{
				Alloc(res + 1);
				strcpy_s(str, size, tmp);
			}


		}


		if (size == 0)
		{
			Alloc(1);
			Clear();
		}


	}


	// Destructor
	virtual ~P2SString()
	{


		if (str != NULL)
			delete[] str;


	}


	const P2SString& create(const char* format, ...)
	{
		size = 0;


		if (format != NULL)
		{
			static char tmp[TMP_STRING_SIZE];
			static va_list  ap;

			// Construct the string from variable arguments
			va_start(ap, format);
			int res = vsprintf_s(tmp, TMP_STRING_SIZE, format, ap);
			va_end(ap);


			if (res > 0)
			{
				Alloc(res + 1);
				strcpy_s(str, size, tmp);
			}


		}


		if (size == 0)
		{
			Alloc(1);
			Clear();
		}


		return *this;
	}


	// Operators
	bool operator== (const P2SString& string) const
	{
		return strcmp(string.str, str) == 0;
	}


	bool operator== (const char* string) const
	{


		if (string != NULL)
			return strcmp(string, str) == 0;


		return false;
	}


	bool operator!= (const P2SString& string) const
	{
		return strcmp(string.str, str) != 0;
	}


	bool operator!= (const char* string) const
	{


		if (string != NULL)
			return strcmp(string, str) != 0;


		return true;
	}


	const P2SString& operator= (const P2SString& string)
	{


		if (string.Length() + 1 > size)
		{
			delete[] str;
			Alloc(string.Length() + 1);
		}
		else
			Clear();


		strcpy_s(str, size, string.str);

		return(*this);
	}


	const P2SString& operator= (const char* string)
	{
		P2SString t(string);
		(*this) = t;
		return *this;


		if (string != NULL)
		{


			if (strlen(string) + 1 > size)
			{
				delete[] str;
				Alloc(strlen(string) + 1);
			}
			else
				Clear();


			strcpy_s(str, size, string);
		}
		else
		{
			Clear();
		}


		return(*this);
	}


	const P2SString& operator+= (const P2SString& string)
	{
		unsigned int needSize = string.Length() + Length() + 1;


		if (needSize > size)
		{
			char* tmp = str;
			Alloc(needSize);
			strcpy_s(str, size, tmp);
			delete[] tmp;
		}


		strcat_s(str, size, string.str);

		return(*this);
	}


	const P2SString& operator+= (const char* string)
	{


		if (string != NULL)
		{
			unsigned int needSize = strlen(string) + Length() + 1;


			if (needSize > size)
			{
				char* tmp = str;
				Alloc(needSize);
				strcpy_s(str, size, tmp);
				delete[] tmp;
			}


			strcat_s(str, size, string);
		}


		return(*this);
	}


	// Utils
	unsigned int Length() const
	{
		return strlen(str);
	}


	void Clear()
	{
		str[0] = '\0';
	}


	const char* GetString() const
	{
		return str;
	}


	char* GetCharArray() const
	{
		return str;
	}


	unsigned int GetCapacity() const
	{
		return size;
	}


	bool Cut(unsigned int begin, unsigned int end = 0)
	{
		uint len = Length();


		if (end >= len || end == 0)
			end = len - 1;


		if (begin > len || end <= begin)
			return false;


		char* p1 = str + begin;
		char* p2 = str + end + 1;


		while (*p1++ = *p2++);


		return true;
	}


	void DeleteLast()
	{


		if (str == NULL)
			return;


		char* p1 = str + Length() - 1;
		char* p2 = str + Length();


		while (*p1++ = *p2++);


	}

	void Trim()
	{
		// cut right --
		char* end = str + size;


		while (*--end == ' ')* end = '\0';


		// cut left --
		char* start = str;


		while (*++start == ' ');


		uint s = strlen(start);


		for (uint i = 0; i < s + 1; ++i)
			str[i] = start[i];


	}


	uint Substitute(const char* src, const char* dst)
	{
		assert(src);
		assert(dst);

		uint instances = Find(src);


		if (instances > 0)
		{
			uint srcLen = strlen(src);
			uint dstLen = strlen(dst);
			uint diff = dstLen - srcLen;
			uint neededSize = 1 + strlen(str) + (diff * instances);


			if (size < neededSize)
			{
				char* tmp = str;
				Alloc(neededSize);
				strcpy_s(str, size, tmp);
				delete tmp;
			}


			for (uint i = 0; i < size - srcLen; ++i)
			{


				if (strncmp(src, &str[i], srcLen) == 0)
				{


					// make room
					for (uint j = strlen(str) + diff; j > i + diff; --j)
					{
						str[j] = str[j - diff];
					}


					// copy
					for (uint j = 0; j < dstLen; ++j)
					{
						str[i++] = dst[j];
					}


				}


			}


		}


		return instances;
	}


	uint Find(const char* string) const
	{
		uint ret = 0;


		if (string != NULL)
		{
			uint len = strlen(string);


			for (uint i = 0; i < size - len; ++i)
			{


				if (strncmp(string, &str[i], len) == 0)
				{
					i += len;
					++ret;
				}


			}


		}


		return ret;
	}


	/**
	* Paste a substring into buffer
	*/
	uint SubString(unsigned int start, unsigned int end, P2SString& buffer) const
	{


		if (str != NULL)
		{
			start = MIN(start, size);
			end = (end == 0) ? size : MIN(end, size);
			uint s = end - start;


			if (s > buffer.size)
			{
				char* tmp = buffer.str;
				buffer.Alloc(s);
				delete[] tmp;
			}


			strncpy_s(buffer.str, s, &str[start], s);
			buffer.str[s] = '\0';
			return(end - start);
		}
		else
			return 0;


	}


	void Insert(const char* string, unsigned int position = 0)
	{


		if (string != NULL)
		{
			//Make room----
			unsigned int need_size = strlen(string) + Length() + 1;


			if (need_size > size)
			{
				char* tmp = str;
				Alloc(need_size);
				strcpy_s(str, size, tmp);
				delete[] tmp;
			}


			//Copy into a buffer the amount of String that we need to move
			int nToMove = this->Length() - position;
			const char* toMove = &str[nToMove];
			P2SString buffer(toMove);

			//Remove the content we just copied into a buffer from the original
			this->Cut(nToMove);

			//Add the new string + what we removed
			this->operator+=(string);
			this->operator+=(buffer.GetString());
		}


	}


private:


	void Alloc(unsigned int requiered_memory)
	{
		size = requiered_memory;
		str = new char[size];
	}


};

#endif // __p2SString_H__