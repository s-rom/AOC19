$res = Measure-Command { .\x64\Release\AOC_19_C++.exe }
Write-Output "$($res.Milliseconds) ms"

