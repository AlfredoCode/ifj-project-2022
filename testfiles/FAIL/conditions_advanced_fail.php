<?php
declare(strict_types = 1);
if($x = 5 === 5){   // This cannot work
    $x = 5;
    $x = $y.$z.$o;
}
else{
    $a = $b;
    $x = $y.$z.$o;
}

?>