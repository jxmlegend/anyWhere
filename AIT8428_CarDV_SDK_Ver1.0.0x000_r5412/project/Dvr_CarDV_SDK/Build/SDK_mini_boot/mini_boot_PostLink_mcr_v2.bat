set PRJ_NAME=mini_boot
set AXF_DIR=.\%PRJ_NAME%_Data\MCR_V2
set AXF_NAME=%PRJ_NAME%.axf
set RAM_DIR=%AXF_DIR%
set PKG_DIR=..\..\tools\DownloadTool_mcrv2
set PKG_DIR_32MB=..\..\tools\DownloadTool_mcrv2_32MB
set PKG_NAME=MINIBOOT
set TOOL_DIR=..\..\..\..\core\tool

fromELF.exe -c -bin %AXF_DIR%\%AXF_NAME% -output %RAM_DIR%\ALL_DRAM
%TOOL_DIR%\FirmwarePackager .\mini_boot_LDS_mcr_v2.txt %RAM_DIR%\ %PKG_DIR%\%PKG_NAME% /q
copy %PKG_DIR%\%PKG_NAME% %PKG_DIR_32MB%\%PKG_NAME%
