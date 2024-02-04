$files=Get-Item *.jpg,*.png
# ユーザーからの入力を受け取る
$mipLevel="0"
$mipLevel = Read-Host "Please enter the mip level :"
$isOne = Read-Host "'isOne' if all mip levels are the same "1","0"otherwise :"

foreach($f in $files){
    if($isOne -eq "0")
    {
         $mipLevel = Read-Host "Please enter the mip level for $($f.Name):"
    }
    Start-Process -FilePath TextureConverter.exe -ArgumentList $f,"-ml",$mipLevel -Wait
}
pause