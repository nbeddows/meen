cd %1
set sdkName=machemu-v%2.%3.%4
echo Building %sdkName% sdk
if exist %sdkName% rmdir /S /Q %sdkName%
mkdir %sdkName%
cd %sdkName%
mkdir %7\%5\%6\
mkdir Base\module
mkdir Controller\module
mkdir Docs
mkdir Machine\module
mkdir SpaceInvaders\docs
mkdir SpaceInvaders\module
mkdir SpaceInvaders\msbuild
mkdir SpaceInvaders\source
mkdir SpaceInvaders\roms
copy ..\%5\machemu.lib %7\%5\
REM copy ..\%5\%6\Machine.pdb %7\%5\%6\
copy ..\..\Base\module\Base.ixx Base\module\
copy ..\..\Controller\module\IController.ixx Controller\module\
copy ..\..\Docs\Changelog.txt Docs\
copy ..\..\Machine\module\IMachine.ixx Machine\module\
copy ..\..\Machine\module\MachineFactory.ixx Machine\module\
copy ..\..\SpaceInvaders\docs\SpaceInvaders.txt SpaceInvaders\docs\
copy ..\..\SpaceInvaders\module\SpaceInvaders.ixx SpaceInvaders\module\
copy ..\..\SpaceInvaders\msbuild\SpaceInvaders.vcxproj SpaceInvaders\msbuild\
copy ..\..\SpaceInvaders\source\main.cpp SpaceInvaders\source\
copy ..\..\SpaceInvaders\source\SpaceInvaders.cpp SpaceInvaders\source\
copy ..\..\Welcome.txt .\
echo Done

