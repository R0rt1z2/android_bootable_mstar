To enable common UFBL features, we need to inject some code in platforms.
And from the past porting experience, there are common code for MTK platform which
was copy/pasted. Here I put the common code which probably would work on MTK based
products.

You need to enable FEATURE\_PLATFORM\_IMPL\_MTK in your product configuration
file(=project/{product}.mk=) in UFBL to include any piece of the below common
code, additionally, you need also to enable the feature macro for each feature
separately, which will describled below:

1. target\_device\_mtk\_impl.c
feature macro: FEATURE\_PLATFORM\_IMPL\_MTK\_TARGET\_DEVICE
implements the below strong version functions for secure boot:
   - amzn\_target\_device\_name
   - amzn\_target\_device\_type

2. unlock\_mtk\_impl.c implements the version-based unlock related functions
feature macro: FEATURE\_PLATFORM\_IMPL\_MTK\_UNLOCK
 - cmd\_oem\_relock which should be registered to "fastboot oem relock"
   - `fastboot oem relock` will update the unlock code, for the devices we
     released signed unlock code to factory PVT devices, we should let them
     use this command to update the unlock code after debugging so the old
     signed unlock code lose effectiveness.
   - `fastboot oem relock keep-version` will keep the unlock code, for
     engineer usage on engineering devices.
 - amzn\_fastboot\_publish\_unlock\_vars
   publish the "unlock\_code" and "unlock\_status", since we need to change these
   variables dynamically when the unlock version updated, and unlock status
   changed, we own both the fastboot variables' buffer.
 - amzn\_check\_unlock\_status
   Re-check the unlock status after flashing a unlock code signature, or relock,
   and update the unlock status buffer, so the fastboot unlock_status variable
   updated immediately.
 - amzn\_flash\_unlock\_signature
   This need to be called when perform `fastboot flash unlock <unlock\_signature\_file>`
   for example, inject it to cmd\_flash\_mmc\_img on eMMC tablets.
