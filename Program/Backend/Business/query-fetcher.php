<?php
    $host = '34.69.97.140';
    $port = '5432';
    $db   = 'g2-team-1-db';
    $user = 'g2_team_1_user';
    $pass = 'einszweidreivierfunf';

    $dsn = "pgsql:host=$host;port=$port;dbname=$db";

    $options = [
        PDO::ATTR_ERRMODE            => PDO::ERRMODE_EXCEPTION,
        PDO::ATTR_DEFAULT_FETCH_MODE => PDO::FETCH_ASSOC,
    ];

    try {
        $pdo = new PDO($dsn, $user, $pass, $options);
        $sql = "SELECT * FROM comedor";
        $stmt = $pdo->prepare($sql);
        $stmt->execute();
        $results = $stmt->fetchAll(PDO::FETCH_ASSOC);
        header('Content-Type: application/json');
        $response = ['status' => 'success', 'queryResult' => $results];
        echo json_encode($response);
    } catch (PDOException $e) {
        header('Content-Type: application/json');
        $response = ['status' => 'error', 'message' => $e->getMessage()];
        echo json_encode($response);
    }