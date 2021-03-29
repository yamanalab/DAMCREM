#/bin/bash

find . -name "*~" | xargs --max-args=1 -I{} bash -c "echo {}; rm {}"

