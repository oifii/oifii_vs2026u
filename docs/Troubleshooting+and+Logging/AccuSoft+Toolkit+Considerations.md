# Troubleshooting and Logging – AccuSoft Toolkit Considerations

This section guides developers through common pitfalls when integrating the AccuSoft Pro Imaging Toolkit (IPL) and Image Format Library into Oiii. It covers runtime configuration, version compatibility, error handling, and the correct use of DIB locking/unlocking to avoid undefined behavior.

## Installation and Runtime Accessibility 🛠️

Ensure the AccuSoft DLLs and libraries are discoverable at runtime:

- **DLL Locations**

Place the Pro Imaging Toolkit DLL (`ACCUPWR1.DLL` on Win3.x or `ACCUPNR1.DLL` on Windows NT) and the Image Format Library DLL in one of:

- Your application’s working directory
- A folder listed in the system `PATH`
- `%WINDIR%\System32`

- **Header and Library Matching**
- Include `ACCUIPL.H` before any IPL calls.
- Link against `ACCUIPL.LIB` compiled with the same compiler version used for your application.

- **Environment Variables**
- Verify `PATH` entries using a command prompt:

```bash
    echo %PATH%
    where ACCUPWR1.DLL
```

- Confirm the DLL version matches the header constants (`AI_REV_MAJOR`, `AI_REV_MINOR`) and runtime functions (`IPL_dll_rev_major()`, `IPL_dll_rev_minor()`).

## Licensing and SDK Compatibility ⚖️

When an image operation fails or a format cannot be opened:

- **Evaluation-Time Expiry**
- `AE_EVAL_DLL_TIMEOUT_HAS_EXPIRED` indicates an expired evaluation build.
- **Unsupported Formats**
- `PIXEL_DEPTH_UNSUPPORTED` means the target format’s bit depth isn’t supported.
- `NO_LZW_VERSION` or `DLL_NOT_LOADED` signal missing or mismatched format handlers.

- **Action Items**
- Check your toolkit’s serial number and support status.
- Verify that header and DLL version numbers align.
- Consult AccuSoft Technical Support if problems persist.

## DIB Locking and IP_IMAGE Management 🔒

Proper ordering of DIB lock/unlock and IP_IMAGE creation/destruction prevents crashes and memory corruption:

1. **Allocate and Lock a DIB**

```c
   HDIB   hDIB = DIB_alloc(width, height, bits_per_pixel);
   LPBMIH lpBMIH;
   if (hDIB < 0) { /* handle error */ }
   lpBMIH = DIB_lock(hDIB);
   if (!lpBMIH) { /* handle lock failure */ }
```

1. **Associate with an IP_IMAGE**

```c
   IP_IMAGE image = ip_image_alloc();
   if (image < 0) { /* handle alloc failure */ }
   ip_image_associate(lpBMIH, image);
```

1. **Process Image Data**
2. Call IPL routines (e.g., filters, transforms) using `image`
3. **Free the IP_IMAGE**

```c
   ip_image_free(image);
```

1. **Unlock and Free the DIB**

```c
   DIB_unlock(hDIB);
   DIB_free(hDIB);
```

## Error Handling and Logging 📋

> **⚠️ Warning:** Always call `ip_image_free` before `DIB_unlock`. Unlocking a DIB invalidates the `LPBMIH` pointer; freeing the associated IP_IMAGE handle afterwards causes undefined behavior.

Use the IPL error routines to capture and log errors:

- **Check for Errors**

```c
  if (!some_IPL_function(...)) {
      char  msg[256];
      int   code, pos;
      long  line;
      IPL_error_get(msg, &code, &line, &pos, sizeof(msg));
      log_error("AccuSoft Error %d at line %ld: %s (pos %d)", code, line, msg, pos);
      IPL_error_clear();
  }
```

- `IPL_error_check()` returns `FALSE` if an error is pending.
- `IPL_error_get()` retrieves the last error message, code, file position, and line.
- `IPL_error_clear()` resets the internal error state.

- **Best Practices**
- Wrap all IPL calls with error checks.
- Log both the error code and descriptive message.
- Clear errors before entering a new processing loop.

## Common Issues and Remedies

| Symptom | Likely Cause | Resolution |
| --- | --- | --- |
| DLL load fails (`DLL_NOT_LOADED`) | Mismatched or missing DLL in `PATH` | Ensure the correct DLL is present and `PATH` is updated. |
| Filter returns failure | Unsupported bit depth (`AE_WRONG_DIB_BIT_COUNT`) | Convert or promote image to supported depth. |
| Application crashes after unlock | IP_IMAGE freed after DIB unlock | Free IP_IMAGE before `DIB_unlock`. |
| Image format not recognized | Missing format handler library | Install matching Image Format Library DLLs. |
| Evaluation expiry errors | Using an expired evaluation build | Obtain a licensed release from AccuSoft. |


## Further Support

For unresolved issues:

- Review the AccuSoft README and TECHQ&A files in the installation directory.
- Contact AccuSoft Technical Support (phone, fax, or BBS).
- Reference your toolkit’s serial number and DLL revision information.

By following these guidelines, you can minimize integration issues with the AccuSoft Toolkit and maintain robust logging and error reporting in Oiii.