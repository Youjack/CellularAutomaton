# -*- coding: utf-8 -*-

import json

def ConvertBaseTo(base : '2~10', num) -> str:
    num = int(num)
    _num = ''
    while (num != 0):
        _num = str(num % base) + _num
        num = num // base
    return _num

def SumStates(num):
    num2 = ConvertBaseTo(2, num)
    s = 0
    for i in range(len(num2)):
        if (num2[i] == '1'): s += 1
    return s

if __name__ == '__main__':
    RuleOut = []
    # from dead to ...
    for i in range(256):
        s = SumStates(i)
        Out = 0
        if (s == 3): Out = 1
        RuleOut.append(Out)
    # from alive to ...
    for i in range(256):
        s = SumStates(i)
        Out = 0
        if (s == 2 or s == 3): Out = 1
        RuleOut.append(Out)
    
    Rule = {
        'name' : "LifeGameRule", 'd' : 2,
        'RuleExt' : 9, 'nStates' : 2, 'nRule' : 512,
        'cRuleMap' : [
            [0, 0],
            [-1, -1], [0, -1], [1, -1],
            [-1, 0], [1, 0],
            [-1, 1], [0, 1], [1, 1] ],
        'RuleOut' : RuleOut }
    
    jRule = json.dumps(Rule)
    file = open('LifeGameRule.CArule', 'w')
    file.writelines(jRule)
    file.close()
