Write-Host "Setting up NAVS. Please wait..."
$new_entry = 'C:\Program Files\CU Northwest\NAVS\NAVS_DESKTOP\'
$search_pattern = ';' + $new_entry.Replace("\","\\")
$old_path = [Environment]::GetEnvironmentVariable('path', 'machine');
$replace_string = ''
$without_entry_path = $old_path -replace $search_pattern, $replace_string
$new_path = $without_entry_path + ';' + $new_entry
[Environment]::SetEnvironmentVariable('path', $new_path,'Machine');
$env:Path += ";C:\Program Files\CU Northwest\NAVS\NAVS_DESKTOP\"
& 'C:\Program Files\CU Northwest\NAVS\NAVS_SUPPORT_SERVICE\NAVS_SUPPORT_SERVICE.exe' --startup=auto install
net start NAVSHelper