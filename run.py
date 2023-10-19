#!/usr/bin/env python

import subprocess
for i in range(6, 17):
    p = subprocess.Popen(["./IonShield", str(i), "prod.mac"])
    p.wait()

