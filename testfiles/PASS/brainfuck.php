<?php
declare(strict_types=1);
$data = "";
$ptr = 0;
$program = reads();
$program_length = strlen($program);
$pc = 0;

function setData(string $data, int $ptr, int $value) : string {
    $dataLen = strlen($data);
    // $data1 = ssubstr($data, 0, $ptr);
    // $ptrCont = $ptr + 1;
    // $data2 = ssubstr($data, $ptrCont, $dataLen);
    // $chr = chr($value);
    // $out = $data1 . $chr . $data2;
    return $out;
}


