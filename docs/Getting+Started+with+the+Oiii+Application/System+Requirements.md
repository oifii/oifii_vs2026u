# Getting Started with the Oiii Application – System Requirements

This section outlines the prerequisites for building and running the **Oiii.exe** desktop application on Windows. Ensure your development and runtime environments meet the specifications below.

## Supported Platform 🎯

Oiii targets 32-bit Windows on x86 processors.

- **Operating System**: Windows 7 and later (32-bit)
- **Architecture**: I386 (x86)
- **Subsystem**: Win32 API
- **Machine Type**: `/machine:I386`

## Development Environment 🛠

To compile Oiii from source, install the following:

- **Microsoft Visual C++** (classic toolset with MFC)
- **Win32 SDK** (for headers and libraries)
- **MFC Libraries** (either shared or static, as configured)
- **NMake** (for building via the provided `OIII.mak`)
- **Environment Variables**:
- `INCLUDE` set to Win32 SDK and MFC include paths
- `LIB` set to Win32 SDK, MFC, and AccuSoft library paths

Configuration properties in `OIII.mak` specify MFC usage and build directories:

```makefile
# Use shared MFC libraries (Use_MFC 6)
!IF  "$(CFG)" == "OIII - Win32 Debug"
  # PROP BASE Use_MFC 6
  # PROP Use_Debug_Libraries 1
  OUTDIR=.\WinDebug
!ENDIF
```

## Dependencies & Libraries 📚

Oiii links against standard Windows DLLs and third-party imaging libraries:

| Library | Purpose | Source |
| --- | --- | --- |
| **kernel32.dll** | Core system services |  |
| **user32.dll** | Window and message management |  |
| **gdi32.dll** | Graphics Device Interface |  |
| **comdlg32.dll** | Common file dialogs |  |
| **advapi32.dll** | Advanced Windows APIs |  |
| **shell32.dll** | Shell and file operations |  |
| **ole32.dll** | OLE and COM support |  |
| **oleaut32.dll** | OLE Automation |  |
| **uuid.dll** | GUID generation |  |
| **odbc32.dll** | ODBC database connectivity |  |
| **odbccp32.dll** | ODBC setup support |  |
| **accuimr5.lib** | AccuSoft Pro Imaging Toolkit |  |
| **ow_iff.lib** | Oiii image‐format DLL interface |  |
| **ow_ipl.lib** | Oiii Pro Imaging library |  |
| **opengl32.lib** | OpenGL graphics rendering |  |
| **glu32.lib** | OpenGL Utility Library |  |


Ensure all library paths are accessible to the linker.

## Recommended Build Configurations ⚙️

Oiii provides two standard build configurations for Win32:

|  | Configuration | Output Folder | Description |
| --- | --- | --- | --- |
| **Debug | Win32** | `.\WinDebug` | Symbols enabled, no optimizations |
| **Release | Win32** | `.\WinRel` | Optimizations enabled, no debug symbols |


Switch configurations by defining the `CFG` macro when invoking NMake:

```bash
nmake /f OIII.mak CFG="OIII - Win32 Release"
```

## Example: Platform Check in Code

Guard Windows‐specific code with the `WIN32` macro:

```cpp
#ifdef WIN32
  // Initialize Win32‐only components
  LoadLibrary(_T("oifiilib.dll"));
#endif
```

This macro is defined automatically by the compiler for all Win32 builds.

---

By ensuring these requirements are met, you can successfully build and run the **Oiii** application on a supported Windows environment.