ECHO OFF
ECHO "M0P"
python ..\processResult.py  -f ..\Output.pickle -e -r results_m0p.txt -html > m0p.html

ECHO "M23"
python ..\processResult.py  -f ..\Output.pickle -e -r results_m23.txt -html > m23.html

ECHO "M3"
python ..\processResult.py  -f ..\Output.pickle -e -r results_m3.txt  -html  > m3.html

ECHO "M4"
python ..\processResult.py  -f ..\Output.pickle -e -r results_m4.txt -html > m4.html

ECHO "M7"
python ..\processResult.py  -f ..\Output.pickle -e -r results_m7.txt -html > m7.html

ECHO "M7 unrolled"
python ..\processResult.py  -f ..\Output.pickle -e -r results_m7_unrolled.txt -html > m7_unrolled.html

ECHO "M33"
python ..\processResult.py  -f ..\Output.pickle -e -r results_m33.txt -html > m33.html

ECHO "CS300"
python ..\processResult.py  -f ..\Output.pickle -e -r results_cs300.txt     -html   > cs300.html

ECHO "CS300 U55"
python ..\processResult.py  -f ..\Output.pickle -e -r results_cs300_u55.txt   -html > cs300_u55.html

ECHO "CS310"
python ..\processResult.py  -f ..\Output.pickle -e -r results_cs310.txt  -html > cs310.html