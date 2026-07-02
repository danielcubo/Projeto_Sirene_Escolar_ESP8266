# Configura a porta COM correspondente ao seu Arduino/ESP
$port = New-Object System.IO.Ports.SerialPort COM3, 115200, None, 8, one
$port.Open()

# Loop infinito enviando a hora a cada 30 segundos
while ($true) {
    $agora = Get-Date
    $hora = [byte]$agora.Hour
    $minuto = [byte]$agora.Minute
    
    # Envia o cabeçalho 'H', o byte da hora e o byte do minuto
    $port.Write("H")
    $port.Write([byte[]]($hora), 0, 1)
    $port.Write([byte[]]($minuto), 0, 1)
    
    Start-Sleep -Seconds 30
}

$port.Close()