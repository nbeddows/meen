function (generatePythonUnitTestDeps rootPath testControllersPath testProgramsPath zlibPath)
if(DEFINED zlibBinDir)
    set(addZlibBinDir "import os\nos.add_dll_directory(os.path.abspath(\"${zlibPath}\"))\n")
endif()

if (NOT DEFINED MachEmuPackageTest)
   set(unitTestDeps "# absolute path to Python controller test modules\nsys.path.append(\"${testControllersPath}\")\nprogramsDir = \"${testProgramsPath}\"\n")
endif()

# command to prepend dll search path to machine test script
file(GENERATE OUTPUT "MachineTestDeps${buildType}.py" CONTENT
"# Copyright (c) 2021-2024 Nicolas Beddows <nicolas.beddows@gmail.com>\n\n\
# Permission is hereby granted, free of charge, to any person obtaining a copy\n\
# of this software and associated documentation files (the \"Software\"), to deal\n\
# in the Software without restriction, including without limitation the rights\n\
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell\n\
# copies of the Software, and to permit persons to whom the Software is\n\
# furnished to do so, subject to the following conditions:\n\n\
# The above copyright notice and this permission notice shall be included in all\n\
# copies or substantial portions of the Software.\n\n\
# THE SOFTWARE IS PROVIDED \"AS IS\", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR\n\
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,\n\
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE\n\
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER\n\
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,\n\
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE\n\
# SOFTWARE.\n\n\
${addZlibBinDir}
import sys\n\
# absolute path to the C++ controller test module\n\
sys.path.append(\"${rootPath}/${runtimeDir}\")\n\
${unitTestDeps}")
endfunction()