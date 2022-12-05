<?php
declare(strict_types=1);
function fce1 () : void {
  fce2();
}

function fce2 () : void {
  fce1();
}

