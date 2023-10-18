#!/usr/bin/env python

import subprocess
for i in range(1, 51):
    p = subprocess.Popen(["./IonShield", str(i), "prod.mac"])
    p.wait()

