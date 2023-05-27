if (-not (Test-Path -Path "opencl.zip")) {
    echo "Downloading OpenCL..."
    curl --progress-bar -Lo opencl.zip "https://github.com/KhronosGroup/OpenCL-SDK/releases/download/v2023.04.17/OpenCL-SDK-v2023.04.17-Win-x64.zip"
}

if (-not (Test-Path -Path "clblast.zip")) {
    echo "Downloading CLBlast..."
    <# github / 7z #>
    # curl --progress-bar -Lo clblast.7z "https://github.com/CNugteren/CLBlast/releases/download/1.6.0/CLBlast-1.6.0-windows-x64.7z"
    <# appveyor / zip #>
    curl --progress-bar -Lo clblast.zip "https://ci.appveyor.com/api/buildjobs/nikwayllaa7nia4c/artifacts/CLBlast-1.6.0-Windows-x64.zip"
}

mkdir -f openclblast >$null
rm -rf openclblast/*
tar -xf opencl.zip -C openclblast

<# appveyor / zip #>
tar -xf clblast.zip -C openclblast
<# github / 7z #>
# if (-not (Test-Path -Path "7zr.exe")) {
#     echo "Downloading 7zr..."
#     curl --progress-bar -Lo 7zr.exe "https://www.7-zip.org/a/7zr.exe"
# }
# .\7zr.exe x -y .\clblast.7z -o"." -r
# mv -f clblast_release_dir/* openclblast/
# rm -f ./7zr.exe
# rm -rf clblast_release_dir

cp -rf openclblast/OpenCL-SDK-v2023.04.17-Win-x64/* openclblast
cp -rf openclblast_cmake/*.cmake openclblast/lib/cmake/CLBlast
rm -rf openclblast/OpenCL-SDK-v2023.04.17-Win-x64

cp openclblast/lib/clblast.dll .
cp openclblast/bin/OpenCL.dll .

gendef clblast.dll
dlltool -k -d clblast.def -l libclblast.a
gendef OpenCL.dll
dlltool -k -d OpenCL.def -l libOpenCL.a


mkdir -f bin >$null

gcc -I./openclblast/include .\device_query.c -L. -lclblast -lOpenCL -o bin/device_query.exe
gcc -I./openclblast/include .\saxpy_cpu.c -L. -lclblast -lOpenCL -o bin/saxpy_cpu.exe
gcc -I./openclblast/include .\saxpy_cl.c -L. -lclblast -lOpenCL -o bin/saxpy_cl.exe
cp -f clblast.dll bin/
