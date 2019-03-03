# -*- coding: utf-8 -*-

import json
from SmallFunctions import ReadNumList
from SmallFunctions import GenerateLinExt
from SmallFunctions import ConvertToLinCoo

# Temporary Solution
def PrintMap(d, cMapExt, MapExt, lMapExt, Map):
    if d == 1:
        for i in range(MapExt):
            if Map[i] == 0: print('+', end = '')
            else: print('O', end = '')
        print('')
    elif d == 2:
        for y in range(cMapExt[1]):
            for x in range(cMapExt[0]):
                if Map[ConvertToLinCoo(d, lMapExt, [x, y])] == 0: print('+', end = '')
                else: print('O', end = '')
            print('')
    else: pass

def Input(d, name):
    file = open('Maps&Rules\\%dD\\%s.CAmap' % (d, name), 'r')
    return json.loads(file.readline())

# output .CAmap File in JSON Format
def Output(Map):
    jMap = json.dumps(Map)
    file = open('Maps&Rules\\%dD\\%s.CAmap' % (Map['d'], Map['name']), 'w')
    file.writelines(jMap)
    file.close()

def Builder():
    print('- Map Builder -')

    name = input('Map Name = ')
    d = int(input('Number of Dimensions = '))
    # Ext = [ Ext(x), Ext(y), Ext(z), ... ]
    cMapExt = ReadNumList(input('Extents of Map = '))
    MapExt = 1
    for i in range(d): MapExt *= cMapExt[i]
    lMapExt = GenerateLinExt(d, cMapExt)

    NormSta = int(input('Normal State of Points = '))
    Map = []
    for i in range(MapExt): Map.append(NormSta)
    nPnt = int(input('Number of Points to Set = '))
    print('Points to Set :')
    for i in range(nPnt):
        # Coo = [ Coo(x), Coo(y), Coo(z), ... ]
        Coo = ReadNumList(input('[%d] Coordinate = ' % i))
        Sta = int(input('[%d] State = ' % i))
        Map[ConvertToLinCoo(d, lMapExt, Coo)] = Sta
    
    Output({
        'name' : name, 'd' : d,
        'cMapExt' : cMapExt, 'MapExt' : MapExt, 'lMapExt' : lMapExt,
        'Map' : Map })
    print('Created Map %s !' % name)
    PrintMap(d, cMapExt, MapExt, lMapExt, Map)
