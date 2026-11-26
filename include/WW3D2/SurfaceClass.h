// 0x00116600 stores the interface pointer and takes a COM reference.
// Get_Description 0x00116620 calls the Direct3D 9 surface's byte slot 0x30.
#pragma once

struct IDirect3DSurface9;

class SurfaceClass
{
public:
	struct SurfaceDescription
	{
		unsigned Format;	// Direct3D format token
		unsigned Width;
		unsigned Height;
	};

	SurfaceClass();
	explicit SurfaceClass(IDirect3DSurface9 *surface);
	SurfaceClass(const SurfaceClass &other);
	~SurfaceClass();
	SurfaceClass &operator=(const SurfaceClass &other);

	void Get_Description(SurfaceDescription &description);

private:
	IDirect3DSurface9 *m_surface;
};

typedef char SurfaceClassSize[(sizeof(SurfaceClass) == 4) ? 1 : -1];
