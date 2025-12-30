// ----------------------------------------------------------------------------------------------------------------------------
//
// Version 1.01
//
// ----------------------------------------------------------------------------------------------------------------------------

#include <stdio.h>
#include <stdarg.h>
#include <string.h>

#pragma warning(disable : 4996)

// ----------------------------------------------------------------------------------------------------------------------------

class CString
{
protected:
	char *String;
	int StringLength;

public:
	CString();
	CString(const char *DefaultString);
	CString(const CString &DefaultString);

	~CString();

	void SetDefaults();

	operator char* ();

	CString& operator = (const char *NewString);
	CString& operator = (const CString &NewString);
	CString& operator += (const char *NewString);
	CString& operator += (const CString &NewString);

	friend CString operator + (const CString &String1, const char *String2);
	friend CString operator + (const char *String1, const CString &String2);
	friend CString operator + (const CString &String1, const CString &String2);

	void Set(const char *Format, ...);
	void Set(const CString &NewString);
	void Append(const char *Format, ...);
	void Append(const CString &Appendix);
	void Empty();

	int GetLength();
	bool IsEmpty();
};

// ----------------------------------------------------------------------------------------------------------------------------
