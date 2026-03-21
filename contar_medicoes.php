<?php
header('Content-Type: application/json');

$servidor = "localhost";
$usuario  = "iot";
$senha    = "IoT2026#";
$banco    = "IoT";

$conexao = mysqli_connect($servidor, $usuario, $senha, $banco);

if (!$conexao) {
    echo json_encode(["erro" => "Conexão falhou"]);
    exit;
}

$sql = "SELECT COUNT(1) as total FROM Medicoes WHERE DATE(data_hora) = DATE(now() - interval 3 hour)";

$resultado = mysqli_query($conexao, $sql);

if ($resultado) {
    $linha = mysqli_fetch_assoc($resultado);
    
    echo json_encode([
        "status" => "sucesso",
        "total_registros" => (int)$linha['total']
    ], JSON_PRETTY_PRINT);
} else {
    echo json_encode(["erro" => mysqli_error($conexao)]);
}

mysqli_close($conexao);
?>
