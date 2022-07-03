$samples = 3000
$baud = 230400
$port=[System.IO.Ports.SerialPort]::getportnames()
$uart= new-Object System.IO.Ports.SerialPort $port,$baud,None,8,one
$uart.Close()
$uart.Open()

for($i = 0; $i -lt $samples; $i++)
{
	$text = $uart.ReadLine()
	Add-Content GPS_Data.txt -Value $text -NoNewline
	Write-Host $text
}
pause