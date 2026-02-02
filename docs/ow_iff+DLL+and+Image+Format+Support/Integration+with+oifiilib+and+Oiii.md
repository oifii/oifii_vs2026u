# ow_iff DLL and Image Format Support

## Overview

The **ow_iff** library is a Win32 DLL that wraps the AccuSoft Image Format Library into a simple API. It exposes image import/export, metadata inspection, and bitmap manipulation functions. Downstream code uses this DLL without needing to know AccuSoft details .

## DLL Exports

The header `ow_iff.h` defines the export macro and declares core functions.

```cpp
#ifdef OW_IFF_EXPORTS
  #define OW_IFF_API __declspec(dllexport)
#else
  #define OW_IFF_API __declspec(dllimport)
#endif

// Example exported functions
OW_IFF_API int ACCUAPI OW_IFF_IMGLOW_get_filetype(char FAR *filename);
OW_IFF_API HGLOBAL ACCUAPI OW_IFF_IMG_bitmap_info(
    int imghandle, int FAR *width, int FAR *height, int FAR *bits_per_pixel
);
OW_IFF_API int ACCUAPI OW_IFF_IMG_save_bitmap(
    int imghandle, char FAR *filename, int type
);
```

**Key Functions**

| Function | Purpose |
| --- | --- |
| OW_IFF_IMGLOW_get_filetype | Determine image format by filename |
| OW_IFF_IMG_bitmap_info | Retrieve width, height, bits/pixel |
| OW_IFF_IMG_dib_to_ddb | Convert DIB handle to device bitmap |
| OW_IFF_IMG_import_ascii | Load ASCII‐encoded images |
| OW_IFF_IMG_create_handle / OW_IFF_IMG_delete_bitmap | Allocate and free image handles |
| OW_IFF_IMG_promote_8 / OW_IFF_IMG_promote_24 | Convert bit depths |
| OW_IFF_IMG_save_bitmap | Write image to disk |


## Integration with oifiilib

The **oifiilib** C/C++ toolkit combines geometric and image routines into one library. To surface **ow_iff** capabilities, its main headers include `ow_iff.h` alongside point/segment/image classes. This lets client code call image‐load and bitmap‐save functions in the same compilation unit as spatial algorithms .

```cpp
// Example in oifiilib umbrella header
#include "oifiilib.h"
#include "ow_iff.h"
```

- **Benefit:** Geometry and image I/O appear under one namespace.
- **Pattern:** Precompiled header pulls in both `oifiilib` and `ow_iff` includes for fast builds.

## Linking in Oiii

The **Oiii** desktop application links directly against `ow_iff.lib`. Its project configuration shows:

```xml
<AdditionalIncludeDirectories>
    ...;\oifiilib;\ow_iff;\ow_ipl
</AdditionalIncludeDirectories>
<AdditionalDependencies>
    .\ow_iff\x64\Release\ow_iff.lib;
    .\ow_ipl\x64\Release\ow_ipl.lib;
    ...;\oifiilib\x64\Release\oifiilib.lib;
</AdditionalDependencies>
```

This ensures all image and IPL routines are available at link time without manual library loading .

## End-User Workflow

From the user’s perspective, **Oiii** seamlessly handles image files:

1. **Import:** User opens JPEG/PNG/TIFF; Oiii calls `OW_IFF_IMGLOW_get_filetype` and related import functions.
2. **Visualization:** Bitmap handles convert to DDB, then into pixel‐ and point‐set objects.
3. **Analysis:** Geometric routines from **oifiilib** operate on these sets.
4. **Export:** Modified data reassembled into an image handle, then saved via `OW_IFF_IMG_save_bitmap`.

### Flowchart of Integration

```mermaid
flowchart LR
  UI[Oiii UI] -->|calls| Core[Oiii Core]
  Core -->|uses| OIFIILIB[oifiilib API]
  OIFIILIB -->|includes & calls| OW_IFF_DLL[ow_iff.dll]
  OW_IFF_DLL --> AccuSoft[AccuSoft IFL/IPL]
```

## Code Snippet: Loading an Image

```cpp
#include "oifiilib.h"
#include "ow_iff.h"

// Load a bitmap and retrieve info
int handle = OW_IFF_IMG_create_handle(biHeaderPtr);
HGLOBAL raw = OW_IFF_IMG_bitmap_info(handle, &w, &h, &bpp);
// Convert into Oiii pixel set...
```

## Summary

- **ow_iff.dll** wraps AccuSoft complexity into plain C exports .
- **oifiilib** headers include `ow_iff.h` to unify image and geometry APIs.
- **Oiii.exe** links `ow_iff.lib` at build time, allowing transparent import/export of image formats .
- For users, the architecture provides one‐step image loading, analysis via point/segment/image sets, and saving, all hidden behind a consistent toolkit interface.