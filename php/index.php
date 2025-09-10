<?php
$output = shell_exec('../build/main.exe');
$data = json_decode($output, true);
?>