// ----------------------------------------------------------------------------------------------------------------------------

#include "string.h"

// ----------------------------------------------------------------------------------------------------------------------------

CString::CString()
{
	SetDefaults();
}

CString::CString(const char *NewString)
{
	SetDefaults();
	Set(NewString);
}

CString::CString(const CString &NewString)
{
	SetDefaults();
	Set(NewString);
}

CString::~CString()
{
	Empty();
}

void CString::SetDefaults()
{
	String = NULL;
	StringLength = 0;
}

CString::operator char* ()
{
	return String;
}

CString& CString::operator = (const char *NewString)
{
	if(String != NewString) Set(NewString);
	return *this;
}

CString& CString::operator = (const CString &NewString)
{
	if(this != &NewString) Set(NewString);
	return *this;
}

CString& CString::operator += (const char *NewString)
{
	Append(NewString);
	return *this;
}

CString& CString::operator += (const CString &NewString)
{
	Append(NewString);
	return *this;
}

CString operator + (const CString &String1, const char *String2)
{
	CString String = String1;
	String += String2;
	return String;
}

CString operator + (const char *String1, const CString &String2)
{
	CString String = String1;
	String += String2;
	return String;
}

CString operator + (const CString &String1, const CString &String2)
{
	CString String = String1;
	String += String2;
	return String;
}

void CString::Set(const char *Format, ...)
{
	Empty();

	if(Format != NULL)
	{
		va_list ArgList;

		va_start(ArgList, Format);

		StringLength = vsnprintf(NULL, 0, Format, ArgList);

		String = new char[StringLength + 1];

		vsprintf(String, Format, ArgList);

		String[StringLength] = 0;

		va_end(ArgList);
	}
}

void CString::Set(const CString &NewString)
{
	Empty();

	if(NewString.String != NULL)
	{
		StringLength = NewString.StringLength;

		String = new char[StringLength + 1];

		strcpy(String, NewString.String);

		String[StringLength] = 0;
	}
}

void CString::Append(const char *Format, ...)
{
	if(Format != NULL)
	{
		va_list ArgList;

		va_start(ArgList, Format);

		int AppendixLength = vsnprintf(NULL, 0, Format, ArgList);

		char *Appendix = new char[AppendixLength + 1];

		vsprintf(Appendix, Format, ArgList);

		Appendix[AppendixLength] = 0;

		char *OldString = String;

		StringLength += AppendixLength;

		String = new char[StringLength + 1];

		if(OldString == NULL)
		{
			strcpy(String, Appendix);
		}
		else
		{
			strcpy(String, OldString);
			strcat(String, Appendix);

			delete [] OldString;
		}

		delete [] Appendix;

		String[StringLength] = 0;

		va_end(ArgList);
	}
}

void CString::Append(const CString &Appendix)
{
	if(Appendix.String != NULL)
	{
		char *OldString = String;

		StringLength += Appendix.StringLength;

		String = new char[StringLength + 1];

		if(OldString == NULL)
		{
			strcpy(String, Appendix.String);
		}
		else
		{
			strcpy(String, OldString);
			strcat(String, Appendix.String);

			delete [] OldString;
		}

		String[StringLength] = 0;
	}
}

void CString::Empty()
{
	if(String != NULL)
	{
		delete [] String;
	}

	SetDefaults();
}

int CString::GetLength()
{
	return StringLength;
}

bool CString::IsEmpty()
{
	return StringLength == 0;
}

// ----------------------------------------------------------------------------------------------------------------------------
