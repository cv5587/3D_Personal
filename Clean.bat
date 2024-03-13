rmdir /s /q "ipch"
rmdir /s /q "Debug"
rmdir /s /q "Release"


rmdir /s /q .\Client\Default\ipch
rmdir /s /q .\Client\Default\Release


rmdir /s /q .\Client\Default\x64
rmdir /s /q .\Engine\Default\x64


del /f /q .\Client\Bin\Client.exe
del /f /q .\Client\Bin\Client.ilk
del /f /q .\Client\Bin\Client.pdb
del /f /q Framework.VC.db