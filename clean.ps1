rm -f *.def
rm -f *.a
rm -f *.dll

if ($args[0] -eq "all") {
    rm -rf ./bin
    rm -rf openclblast
    rm -rf clblast.zip
    rm -rf opencl.zip
}
