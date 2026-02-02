# Getting Started with the Oiii Application – Installing Prerequisites

Before you can build or run **Oiii.exe**, your development machine must have several tools and SDKs installed. This section walks you through each prerequisite, explains why it’s needed, and shows how to verify your setup.

---

## 🖥️ Microsoft Visual C++ Development Environment

You need a compatible Visual C++ IDE and toolchain to compile legacy `.dsp` projects and MFC-based Win32 applications.

- **Why?**
- Oiii uses MFC classes and leverages Microsoft’s precompiled-header system (`stdafx.h`).
- The build scripts reference the `_AFXDLL` macro for shared MFC support .
- **What to install:**
- Visual Studio (2017, 2019, or 2022) with the **Desktop development with C++** workload.
- In **Individual components**, include **MFC and Windows XP support**.
- **How to verify:**
- Open a “Developer Command Prompt for VS”.
- Run `cl.exe` and confirm you see a Microsoft compiler banner.
- Locate `afx.h` under your VS install (e.g., `…\VC\Tools\MSVC\<ver>\atlmfc\include\afx.h`).

---

## 📷 AccuSoft Pro Imaging Toolkit SDK

The **Pro Imaging Toolkit** provides core image-processing APIs (e.g., `iplCreateImage*`, `iplAllocateImage`) used throughout Oiii.

| Component | Files | Purpose |
| --- | --- | --- |
| **Header files** | `ipl.h`, `ipl_errs.h`, `accuipl.h` | Define IPLAPI functions for ROI, tiling, etc. |
| **Libraries** | `accuimr5.lib` | Links IPL image routines into Oiii.exe |
| **Runtime DLLs** | `accuimr5.dll` | Distributes IPL functionality to end users |


- **Default install path:**

`C:\AccuSoft\ProImaging\IPL\`

- **Build script reference:**

The Oiii linker flags include `accuimr5.lib` alongside OpenGL libs .

- **Verification:**

```bash
  dir "C:\AccuSoft\ProImaging\IPL\include\ipl.h"
  dir "C:\AccuSoft\ProImaging\IPL\lib\accuimr5.lib"
```

---

## 🔄 AccuSoft Image Format Library SDK

The **Image Format Library** (IFL) adds read/write support for custom formats via `ow_iff.dll` and related exports.

| Component | Files | Purpose |
| --- | --- | --- |
| **Headers** | `ow_iff.h`, `ow_ipl.h` | Declare format-DLL entry points |
| **Makefiles** | `ow_iff.mak` | Build scripts for Win32 Debug/Release |
| **Libraries** | `ow_iff.lib`, `ow_ipl.lib` | Link format handlers into Oifiilib |


- **Default install path:**

`C:\AccuSoft\ImageFormatLibrary\`

- **Build script reference:**

The `ow_iff.mak` links against `accuimr5.lib` and Win32 system libs .

- **Verification:**

```bash
  dir "C:\AccuSoft\ImageFormatLibrary\include\ow_iff.h"
  dir "C:\AccuSoft\ImageFormatLibrary\lib\ow_iff.lib"
```

---

## ⚙️ Configuring Environment Variables

To streamline builds, set environment variables for AccuSoft SDK locations:

```bash
# AccuSoft Pro Imaging Toolkit
set ACCUSOFT_IPL=C:\AccuSoft\ProImaging\IPL

# AccuSoft Image Format Library
set ACCUSOFT_IFL=C:\AccuSoft\ImageFormatLibrary

# Add to Visual Studio Include/Library paths:
#   - $(ACCUSOFT_IPL)\include
#   - $(ACCUSOFT_IFL)\include
#   - $(ACCUSOFT_IPL)\lib
#   - $(ACCUSOFT_IFL)\lib
```

❗ **Tip:** In Visual Studio, go to **Project → Properties → VC++ Directories** and add these to **Include Directories** and **Library Directories**.

---

## ✅ Post-Installation Checklist

- [ ] **cl.exe** and **link.exe** invoke successfully in a Developer Command Prompt.
- [ ] `afx.h` and other MFC headers exist on disk (MFC installed).
- [ ] `ipl.h` and `accuimr5.lib` are present under `%ACCUSOFT_IPL%` .
- [ ] `ow_iff.h` and `ow_iff.lib` exist under `%ACCUSOFT_IFL%` .

Once these prerequisites are in place, you’re ready to open the Oiii solution or legacy `.dsp` file, migrate it if needed, and build **Oiii.exe**.