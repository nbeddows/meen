cd %1
set sdkName=machemu-v%2.%3.%4
echo Building %sdkName% sdk
if exist %sdkName% rmdir /S /Q %sdkName%
mkdir %sdkName%
cd %sdkName%
mkdir %7\%5\%6\
mkdir Base\module
mkdir Controller\module
mkdir Machine\module
mkdir SpaceInvaders\module
mkdir SpaceInvaders\msbuild
mkdir SpaceInvaders\source
mkdir SpaceInvaders\roms
mkdir SystemBus\module
copy ..\%5\machemu.lib %7\%5\
REM copy ..\%5\%6\Machine.pdb %7\%5\%6\
copy ..\..\Base\module\Base.ixx Base\module\
copy ..\..\Controller\module\ControllerFactory.ixx Controller\module\
copy ..\..\Controller\module\IController.ixx Controller\module\
copy ..\..\Machine\module\IMachine.ixx Machine\module\
copy ..\..\Machine\module\MachineFactory.ixx Machine\module\
copy ..\..\SpaceInvaders\module\SpaceInvaders.ixx SpaceInvaders\module\
copy ..\..\SpaceInvaders\msbuild\SpaceInvaders.vcxproj SpaceInvaders\msbuild\
copy ..\..\SpaceInvaders\source\main.cpp SpaceInvaders\source\
copy ..\..\SpaceInvaders\source\SpaceInvaders.cpp SpaceInvaders\source\
copy ..\..\SystemBus\module\SystemBus.ixx SystemBus\module\
echo Done

