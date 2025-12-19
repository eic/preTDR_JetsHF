#!/bin/bash

cd ${1}

/eic/u/rongrong/bin/eic-shell  << EOF
./analysis ${2} ${3} ${4}
exit
EOF
