f = open('resultados_note.txt')

lines = f.readlines()

names = list()

grasp_1 = list()
grasp_2 = list()
grasp_3 = list()
grasp_4 = list()
grasp_5 = list()

vns_1 = list()
vns_2 = list()
vns_3 = list()
vns_4 = list()
vns_5 = list()

i = 0
while (i < len(lines)):
    names.append(lines[i])
    
    grasp_1.append((float(lines[i+1].split(' ')[5]), float(lines[i+1].split(' ')[7])))
    vns_1.append((float(lines[i+1].split(' ')[14]), float(lines[i+1].split(' ')[16])))

    grasp_2.append((float(lines[i+2].split(' ')[5]), float(lines[i+2].split(' ')[7])))
    vns_2.append((float(lines[i+2].split(' ')[14]), float(lines[i+2].split(' ')[16])))

    grasp_3.append((float(lines[i+3].split(' ')[5]), float(lines[i+3].split(' ')[7])))
    vns_3.append((float(lines[i+3].split(' ')[14]), float(lines[i+3].split(' ')[16])))

    grasp_4.append((float(lines[i+4].split(' ')[5]), float(lines[i+4].split(' ')[7])))
    vns_4.append((float(lines[i+4].split(' ')[14]), float(lines[i+4].split(' ')[16])))

    grasp_5.append((float(lines[i+5].split(' ')[5]), float(lines[i+5].split(' ')[7])))
    vns_5.append((float(lines[i+5].split(' ')[14]), float(lines[i+5].split(' ')[16])))

    i += 6

f.close()

f = open('resultados_planilha.txt', 'w')

f.write('grasp - 1 ladrao - valores: \n')
for value in grasp_1:
    f.write(str(value[0]) + '\n')

f.write('grasp - 1 ladrao - tempos: \n')
for value in grasp_1:
    f.write(str(value[1]) + '\n')

f.write('grasp - 2 ladrao - valores: \n')
for value in grasp_2:
    f.write(str(value[0]) + '\n')

f.write('grasp - 2 ladrao - tempos: \n')
for value in grasp_2:
    f.write(str(value[1]) + '\n')

f.write('grasp - 3 ladrao - valores: \n')
for value in grasp_3:
    f.write(str(value[0]) + '\n')

f.write('grasp - 3 ladrao - tempos: \n')
for value in grasp_3:
    f.write(str(value[1]) + '\n')

f.write('grasp - 4 ladrao - valores: \n')
for value in grasp_4:
    f.write(str(value[0]) + '\n')

f.write('grasp - 4 ladrao - tempos: \n')
for value in grasp_4:
    f.write(str(value[1]) + '\n')

f.write('grasp - 5 ladrao - valores: \n')
for value in grasp_5:
    f.write(str(value[0]) + '\n')

f.write('grasp - 5 ladrao - tempos: \n')
for value in grasp_5:
    f.write(str(value[1]) + '\n')

f.write('vns - 1 ladrao - valores: \n')
for value in vns_1:
    f.write(str(value[0]) + '\n')

f.write('vns - 1 ladrao - tempos: \n')
for value in vns_1:
    f.write(str(value[1]) + '\n')

f.write('vns - 2 ladrao - valores: \n')
for value in vns_2:
    f.write(str(value[0]) + '\n')

f.write('vns - 2 ladrao - tempos: \n')
for value in vns_2:
    f.write(str(value[1]) + '\n')

f.write('vns - 3 ladrao - valores: \n')
for value in vns_3:
    f.write(str(value[0]) + '\n')

f.write('vns - 3 ladrao - tempos: \n')
for value in vns_3:
    f.write(str(value[1]) + '\n')

f.write('vns - 4 ladrao - valores: \n')
for value in vns_4:
    f.write(str(value[0]) + '\n')

f.write('vns - 4 ladrao - tempos: \n')
for value in vns_4:
    f.write(str(value[1]) + '\n')

f.write('vns - 5 ladrao - valores: \n')
for value in vns_5:
    f.write(str(value[0]) + '\n')

f.write('vns - 5 ladrao - tempos: \n')
for value in vns_5:
    f.write(str(value[1]) + '\n')