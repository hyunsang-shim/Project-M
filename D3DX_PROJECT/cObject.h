#pragma once
class cObject
{
public:
	cObject();
	virtual ~cObject();

protected:
	ULONG m_ulRefCount;
public:
	virtual void AddRef();
	virtual void Release();
};

