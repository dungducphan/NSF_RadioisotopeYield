#!/usr/bin/env python

import subprocess
for i in range(5, 16):
    p = subprocess.Popen(["./IonShield", str(i), "prod.mac"])
    p.wait()

