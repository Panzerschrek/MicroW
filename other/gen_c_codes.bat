dft.exe real_sounds/sten.wav -c 32 -o out/crowbar.bin -t0 0.00 -t1 0.3
bin2ccode.exe out/crowbar.bin -o out/crowbar.c
pause