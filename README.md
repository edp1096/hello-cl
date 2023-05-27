https://gist.github.com/yohanesgultom/b7e32f7649ac39e00ad65bcb83dfd72e

## Powershell scripts
* Before execute `ps1` script files, `ExecutionPolicy` should be set to `RemoteSigned` and unblock `ps1` files
```powershell
# Check
ExecutionPolicy
# Set as RemoteSigned
Set-ExecutionPolicy -Scope CurrentUser RemoteSigned

# Unblock ps1 files
Unblock-File *.ps1
```

## Build
```powershell
./build.ps1
```

## Clean
* Only *.def, *.dll, *.a
```powershell
./clean.ps1
```
* All clear
```powershell
./clean.ps1 all
```
