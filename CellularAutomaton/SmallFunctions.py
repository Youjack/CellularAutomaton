# -*- coding: utf-8 -*-

# From NumListReader
def ReadNumList(string):
    blank = ( ' ', ',', ';' )
    List = []
    s = ''
    for i in range(len(string)):
        if string[i] in blank:
            List.append(int(s))
            s = ''
        elif i == len(string)-1:
            List.append(int(s+string[i]))
        else:
            s += string[i]
    return List

# From LinCooConverter
def GenerateLinExt(d, cExt):
    lExt = [ 1, ]
    for i in range(d-1):
        p = 1
        for j in range(i+1): p *= cExt[j]
        lExt.append(p)
    return lExt
def ConvertToLinCoo(d, lExt, Coo : 'must be in Ext'):
    lCoo = 0
    for i in range(d):
        lCoo += Coo[i] * lExt[i]
    return lCoo

# From BaseConverter
def ConvertBaseFrom(base : '2~10', num) -> int:
    num = str(num)
    _num = 0
    l = len(num)
    for i in range(l):
        _num += int(num[i]) * base ** (l-i-1)
    return _num
def ConvertBaseTo(base : '2~10', num) -> str:
    num = int(num)
    _num = ''
    while (num != 0):
        _num = str(num % base) + _num
        num = num // base
    return _num