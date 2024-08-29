#pragma once

class ICircle :public IUnknown
{
public:
	// ISum specific method declarations
	virtual HRESULT __stdcall PointOnCircle(double, double, int*,int*) = 0;// pure virtual
};
class ISineWave :public IUnknown
{
public:
	// ISubtract specific method declarations
	virtual HRESULT __stdcall PointOnSineWave(double, double,double, int*, int*) = 0;// pure virtual
};

// CLSID of SumSubtract Component  // {4C9D2DE7-BA49-4A2E-B248-F0FD9CA381B5}
const CLSID CLSID_ICircleSineWave ={ 0x4c9d2de7, 0xba49, 0x4a2e, 0xb2, 0x48, 0xf0, 0xfd, 0x9c, 0xa3, 0x81, 0xb5 };


// IID of ISum Interface  // {E896D427-9B84-4FE6-94F9-DCEA4C76146D}

const IID IID_ICircle = { 0xe896d427, 0x9b84, 0x4fe6, 0x94, 0xf9, 0xdc, 0xea, 0x4c, 0x76, 0x14, 0x6d };



// IID of ISubtract Interface  // {1438055B-C8E0-45B5-A389-02B0D1CC2378}
const IID IID_ISineWave = { 0x1438055b, 0xc8e0, 0x45b5, 0xa3, 0x89, 0x2, 0xb0, 0xd1, 0xcc, 0x23, 0x78 };

