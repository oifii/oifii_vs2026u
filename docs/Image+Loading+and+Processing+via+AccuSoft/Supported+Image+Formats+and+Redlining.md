# Image Loading and Processing via AccuSoft – Supported Image Formats and Redlining

This section explains how Oiii integrates the AccuSoft Image Format Library and Redlining Toolkit to load, process, and annotate raster images. You will learn about the formats supported, the core classes and functions involved, and how redlining layers are applied without altering original data.

## Supported Image Formats 🖼️

Oiii relies on the AccuSoft Image Format Library (IFL) to detect and decode a wide range of raster formats. Version 5.0 of the library supports **36** file types, including but not limited to:

| Format Name | Extension(s) | Notes |
| --- | --- | --- |
| BMP | .bmp | Standard Windows bitmap |
| TIFF | .tif, .tiff | Multi-page, compressed/uncompressed |
| GIF | .gif | No LZW in this version |
| JPEG | .jpg, .jpeg | With EXIF rotation support |
| PCX | .pcx | Legacy Paintbrush format |
| Sun Raster | .ras | Sun Microsystems raster |
| XBM / XPM | .xbm, .xpm | X-Windows bitmaps |
| Photoshop PI﻿CT | .pct | Raster only |
| Kodak Photo CD | .pcd | Read-only |
| … and many more |  |  |


**Key features**

- **Format Detection:** `OW_IFF_IMGLOW_get_filetype` inspects file headers to classify supported formats.
- **On-the-fly Decompression:** `OW_IFF_IMG_decompress_bitmap` returns an image handle that the toolkit uses internally.
- **Conversion Utilities:** Palette promotion (`OW_IFF_IMG_promote_8` / `OW_IFF_IMG_promote_24`) ensures DIB compatibility.

## Integration with Oiii: COWPixelset & OW_IFF DLL

Oiii’s **COWPixelset** class encapsulates multiple image representations—AccuSoft handles, Intel IPL images, Windows DIBs, and an internal band-interleaf buffer.

### COWPixelset Responsibilities

- **Data Members**
- `int m_imghandle;` // AccuSoft IFL image handle
- `IplImage* m_pIplImage;` // IPL image buffer
- `BITMAPINFOHEADER* m_pDIB;` // Windows DIB pointer
- `char* m_pData;` // Custom band-interleaf buffer

- **Core Methods**
- `BOOL SetIFFHandle(int imghandle);`
- `int  GetIFFHandle();`
- `BOOL AllocDIB(int imghandle);`
- `void UpdateDIBFrom(int imghandle);`
- `BOOL IsIFFPresent();` / `IsDIBPresent()`

### Loading Workflow

When opening a supported image, Oiii performs:

```cpp
// Detect format
int iType = OW_IFF_IMGLOW_get_filetype(filename);
// Decompress to AccuSoft handle
int imghandle = OW_IFF_IMG_decompress_bitmap(filename);
if (imghandle < 0) { /* error handling */ }

// Store handle and build DIB
pPixelset->SetIFFHandle(imghandle);
pPixelset->AllocDIB(imghandle);
pPixelset->UpdateDIBFrom(imghandle);
```

```cpp
// After DIB is ready, Oiii may convert to IPL or custom buffer
if (pPixelset->IsDIBPresent() && !pPixelset->IsBIIPresent()) {
    pPixelset->AllocBII(pPixelset->GetDIBPointer());
    pPixelset->UpdateBIIFrom(pPixelset->GetDIBPointer());
}
```

This ensures that both visual display and downstream processing routines receive the correct image data .

## AccuSoft Redlining Toolkit (ART) ✏️

The AccuSoft Redlining Toolkit enables **annotation** and **redlining** without modifying original raster pixels. Its main characteristics:

- **Layer-Based Annotations:** ART creates a separate metafile layer above the image; the underlying raster remains unchanged.
- **High- and Low-Level API:**
- **High-Level:** A few functions suffice for typical annotation tasks.
- **Low-Level:** Detailed control over each drawing primitive.
- **Metafile Compatibility:** Annotations render to Windows metafiles; other applications can consume them.
- **Printing Flexibility:** Print annotations alone, the original image, or both combined.

**ART Feature Highlights**

- ➕ **Annotation Primitives** (lines, rectangles, text)
- ✂️ **Redlining & Redacting** tools
- 🔍 **Hotspot Regions** for interactive marking
- 📨 **Message Sending** for integrated workflows

## Applying Redlining in Oiii

Within Oiii, ART is used to overlay analytical and presentation graphics on loaded images:

1. **Create a Redlining Context:**

```cpp
   HART_CONTEXT hCtx = ART_CreateContext(hWnd);
```

1. **Draw Primitives:**

```cpp
   ART_SetColor(hCtx, RGB(255,0,0));         // Red pen
   ART_DrawRectangle(hCtx, x, y, width, height);
   ART_DrawText(hCtx, x, y, "Marker A");
```

1. **Render to Metafile Layer:**

```cpp
   HMETAFILE hMeta = ART_ExportMetafile(hCtx);
   DisplayMetaOnCanvas(hMeta);
   ART_DestroyContext(hCtx);
```

These annotations can then be toggled on/off for printing or live review, aiding both scientific analysis and report generation.

## Dependencies & Relationships

- **Header Files:**
- `ACCUIPL.H` (IFL & IPL definitions)
- `IMG_ERR.H` (error codes)
- **DLLs:**
- `ow_iff.dll` (wrapper around the AccuSoft IFL)
- `ipl.dll` (Intel Image Processing Library via OW_IPL API)
- `art.dll` (AccuSoft Redlining Toolkit)
- **Higher-Level Components:**
- **Oiii.exe** orchestrates UI, file dialogs, and passes data to `COWPixelset`.
- **oifiilib** exposes `COWPixelset` and related classes for point/segment/image processing.

---

This integration empowers Oiii to handle dozens of raster formats seamlessly and enrich images with non-destructive annotations, combining robust image processing with flexible redlining for scientific visualization and analysis.