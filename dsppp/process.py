import re
import xlsxwriter

START = 0
IN_TEST = 1 
MEASURE = 2
CYCLE_CPP = 3
CYCLE_C = 4
ERROR = 5

line_nb = 0
state = START 
dimensions = "?"

cpp = 0
c = 0 

stats = {}

with open("result.txt","r") as f:
    lines = f.readlines()
    for l in lines:
        if line_nb >= 3:
           if re.match('Error',l):
              state = ERROR
              continue 
           if state == ERROR:
              state = IN_TEST
              continue
           if state == START:
              if re.match(r'^[a-zA-Z]+.*$',l):
                 #print(l)
                 test_name = l.strip("\n")
                 state = IN_TEST
                 stats[test_name]=[]
                 continue
           if state == IN_TEST:
              if re.match(r'----',l):
                 state = MEASURE 
                 continue
              if re.match(r'^[a-zA-Z]+.*$',l):
                 state = IN_TEST
                 test_name = l.strip("\n")
                 stats[test_name]=[]
                 continue
           if state == MEASURE:
              dimensions = l.strip("\n")
              state = CYCLE_CPP 
              continue 
           if state == CYCLE_CPP:
              m = re.match(r'Cycle count = ([0-9]+)',l)
              if m:
                 cpp = m.group(1)
                 state = CYCLE_C 
              continue
           if state == CYCLE_C:
              if re.match(r'----',l):
                 state = MEASURE 
                 stats[test_name].append({"dim":dimensions,"cpp":cpp})
                 continue
              m = re.match(r'Cycle count = ([0-9]+)',l)
              if m:
                c = m.group(1)
                state = IN_TEST 
                stats[test_name].append({"dim":dimensions,"cpp":cpp,"c":c})
                continue
              else:
                stats[test_name].append({"dim":dimensions,"cpp":cpp})
                state = IN_TEST 
                continue





        line_nb = line_nb + 1 

dst="C:/Users/CHRFAV01/OneDrive - ARM/Documents/Presentations/CMSIS_Compute"

def pos(row,col):
    return(f"{chr(ord('A')+col)}{row}")

for s in stats:
    ns = re.sub(r'[ ]',"_",s) + ".xlsx"
    print(ns)
    workbook = xlsxwriter.Workbook(dst+"/"+ns)
    worksheet = workbook.add_worksheet("Results")
    line_nb = 0

    title = workbook.add_format({'bold': True,'font_size':24})
    sub_title = workbook.add_format({'bold': True,
                                     'font_size':14,
                                     'align':"center",
                                     'bg_color':"#CCCCCC"})
    percent = workbook.add_format({'num_format': '0.00%'})
    dimEven = workbook.add_format({'bold': True,'bg_color':"#CCCCCC"})
    dimOdd = workbook.add_format({'bold': True,'bg_color':"#EEEEEE"})

    worksheet.write(line_nb,0, s,title)
    line_nb = line_nb + 1

    worksheet.set_row(line_nb, 30)
    worksheet.set_column("D:D", 30)

    if len(stats[s])==2:
            worksheet.write(line_nb,0, 'dims',sub_title)
            worksheet.write(line_nb,1, 'cpp',sub_title)
            worksheet.write(line_nb, 2, 'CPP Improvement',sub_title)

    else:
            worksheet.write(line_nb,0, 'dims',sub_title)
            worksheet.write(line_nb,1, 'cpp',sub_title)
            worksheet.write(line_nb,2, 'c',sub_title)
            worksheet.write(line_nb, 3, 'CPP Improvement',sub_title)

    line_nb = line_nb + 1
    for x in stats[s]:
             if (line_nb % 2 == 0):
                    dim = dimOdd
             else:
                    dim = dimEven
             if "c" in x:
                 worksheet.write(line_nb,0, x["dim"],dim)
                 worksheet.write(line_nb,1, float(x["cpp"]))
                 worksheet.write(line_nb,2, float(x["c"]))
                 worksheet.write(line_nb, 3, f"=(C{line_nb+1}-B{line_nb+1})/C{line_nb+1}",percent)
             else:
                 worksheet.write(line_nb,0, x["dim"],dim)
                 worksheet.write(line_nb,1, float(x["cpp"]))
                 worksheet.write(line_nb, 2, f"=(C{line_nb+1}-B{line_nb+1})/C{line_nb+1}",percent)

             line_nb = line_nb + 1
    


    workbook.close()
         