# Getting Started with the Oiii Application – Building Oiii.exe

This section guides you through compiling the Oiii desktop application from source. You will learn how to open the project, configure build settings, manage include/library paths, and produce the final `Oiii.exe` binary.

## Prerequisites

Before building, ensure you have:

- **Microsoft Visual C++** (compatible with the `.dsp`/`.vcxproj` format).
- **AccuSoft Pro Imaging Toolkit** and **Image Format Library** installed.
- All third-party libraries (PortAudio, libsndfile, FreeImage) built for Win32/Win64.
- The source tree checked out (e.g., `oifii/oifii_vs2026u`).

## Opening the Project 🛠️

Choose one of the following workflows:

### Visual C++ IDE

1. Launch Visual C++.
2. Open `OIII.dsp` (the legacy DSP‐style project).
3. The IDE will import settings into a temporary `.vcproj`.

### NMAKE (Command Line)

Run NMAKE against the provided makefile:

```bash
nmake /f OIII.mak CFG="OIII - Win32 Debug"
```

This uses the `OIII.mak` file to drive the build .

## Selecting a Build Configuration ⚙️

You must specify one of two configurations:

- **OIII – Win32 Debug**

Use for interactive development and step-through debugging.

- **OIII – Win32 Release**

Use for optimized binaries ready for deployment.

Invalid choices cause a clear error message at the start of the build .

## Configuring Include Paths

Ensure the compiler can find all headers via **AdditionalIncludeDirectories**:

| Component | Include Directory |
| --- | --- |
| Oiii core / MFC | `.\` |
| oifiilib (point/segment wrappers) | `.\oifiilib` |
| ow_iff (image‐format DLL) | `.\ow_iff` |
| ow_ipl (AccuSoft Image Toolkit) | `.\ow_ipl` |
| FreeImagePlus | `.\freeimage3180_vs2017\Wrapper\FreeImagePlus` |
| FreeImage | `.\freeimage3180_vs2017\Dist\x32` |
| PortAudio | `.\portaudio(x86)\include` |
| libsndfile | `.\libsndfile\include` |


Example snippet from the project file:

```xml
<AdditionalIncludeDirectories>
  .\oifiilib;.\ow_iff;.\ow_ipl;.\freeimage3180_vs2017\Wrapper\FreeImagePlus;.\freeimage3180_vs2017\Dist\x32;… 
</AdditionalIncludeDirectories>
```

## Configuring Library Dependencies

Linker settings must reference all required `.lib` files:

- **Core libraries**
- `oifiilib.lib` (scientific sets)
- `ow_iff.lib`, `ow_ipl.lib` (image‐format support)
- **AccuSoft / System**
- `opengl32.lib`, `glu32.lib`
- AccuSoft runtime (`accuimr5.lib`)
- **Third-party**
- `portaudio_x86.lib`
- `libsndfile-1.lib`
- FreeImagePlus/FreeImage libs

Snippet from the Debug link settings:

```xml
<AdditionalDependencies>
  .\ow_iff\Debug\ow_iff.lib;
  .\ow_ipl\Debug\ow_ipl.lib;
  .\oifiilib\Debug\oifiilib.lib;
  opengl32.lib;glu32.lib;portaudio_x86.lib;libsndfile-1.lib;…
</AdditionalDependencies>
```

## Building the Solution

After configuration:

1. **In IDE:** Press **Build → Build Solution**.
2. **In CLI:**

```bash
   nmake /f OIII.mak CFG="OIII - Win32 Debug"
```

1. The build system will:
2. Compile dozens of source files (`POINTSET.CPP`, `SEGMENT.CPP`, `GRAPHSET.CPP`, `GRAPHDOC.CPP`, `LOGDOC.CPP`, `OW2VIEW.CPP`, …)
3. Generate the precompiled header `OIII.pch`
4. Link everything into `Oiii.exe` for the chosen configuration .

## Build Output

On success, you will find:

| Artifact | Location |
| --- | --- |
| Oiii.exe | `.\WinDebug\Oiii.exe` |
| Oiii.pdb | `.\WinDebug\Oiii.pdb` |
| Oiii.tlb | `.\WinDebug\Oiii.tlb` |
| Oiii.bsc | `.\WinDebug\Oiii.bsc` |


For Release builds, replace `WinDebug` with `WinRel` .

## Troubleshooting

```card
{
    "title": "Invalid Configuration",
    "content": "Specify CFG as exactly \"OIII - Win32 Debug\" or \"OIII - Win32 Release\"."
}
```

Ensure all include directories and library paths match your local folder layout. Any mismatch will cause missing‐file or unresolved‐symbol errors.