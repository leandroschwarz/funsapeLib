$baud = 57600
$port= "COM3"
$uart= new-Object System.IO.Ports.SerialPort $port,$baud,None,8,one
$uart.Close()
$uart.Open()

while($uart.IsOpen)
{
	foreach($line in Get-Content .\GPS_Data.txt) 
	{	
		if(!$uart.IsOpen) { break }
		Write-Host $line
		#$uart.WriteLine($line)
		$uart.Write($line)
		$uart.Write("`r`n")
		Start-Sleep -Milliseconds 100
	}
}
Write-Host $port "Disconected"
Pause
