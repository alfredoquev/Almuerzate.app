<?php
    $queryResult = [
        'campo1' => 'hola',
        'campo2' => true,
        'campo3' => 150,
        'campo4' => 'mundo'
    ];
    header('Content-Type: application/json');
    $response = ['status' => 'success', 'queryResult' => $queryResult];
    echo json_encode($response);