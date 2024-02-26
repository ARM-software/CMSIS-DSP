# Building and running examples {#dsppp_building}

## To build

First time:

```shell
cbuild -O cprj test.csolution.yml  --toolchain AC6 -c example.Release+VHT-Corstone-300 -p -r --update-rte

```

Other times:

```shell
cbuild -O cprj test.csolution.yml  --toolchain AC6 -c example.Release+VHT-Corstone-300
```

If you want to select another test, edit the file `example.cproject.yml` and uncomment the test.

## To run

If the tools have been installed with `vcpkg`:

```
FVP_Corstone_SSE-300_Ethos-U55.exe -f fvp_configs/VHT-Corstone-300.txt -a cpu0=cprj\out\example\VHT-Corstone-300\Release\example.axf
```

Otherwise, you'll need to use the path to your FVP installation.

