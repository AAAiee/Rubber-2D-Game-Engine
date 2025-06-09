@echo off

pushd ..
Vendor\Binaries\Premake\Windows\premake5.exe self-test
popd
pause