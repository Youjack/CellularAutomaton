# -*- coding: utf-8 -*-

import json
from SmallFunctions import ReadNumList
from SmallFunctions import ConvertToLinCoo
from SmallFunctions import ConvertBaseTo

def Input(d, name):
    file = open('Maps&Rules\\%dD\\%s.CArule' % (d, name), 'r')
    return json.loads(file.readline())

# output .CArule File in JSON Format
def Output(Rule):
    jRule = json.dumps(Rule)
    file = open('Maps&Rules\\%dD\\%s.CArule' % (Rule['d'], Rule['name']), 'w')
    file.writelines(jRule)
    file.close()

def Builder():
    print('- Rule Builder -')

    name = input('Rule Name = ')
    d = int(input('Number of Dimensions = '))

    # Ext = [ Ext(x), Ext(y), Ext(z), ... ]
    RuleExt = int(input('Extents of Rules = '))
    print('Map of Rule : ')
    cRuleMap = []
    for i in range(RuleExt):
        # Coo = [ Coo(x), Coo(y), Coo(z), ... ]
        cRuleMap.append(ReadNumList(input('[%d] = ' % i)))

    nStates = int(input('Number of States (2~10) = '))
    nRule = nStates ** RuleExt
    print('Rules : ')
    RuleOut = []
    for i in range(nRule): 
        # Coo = [ Coo(x), Coo(y), Coo(z), ... ]
        RuleIn = ConvertBaseTo(nStates, i).zfill(RuleExt)
        RuleOut.append(int(input('Input = [%s], Output = ' % RuleIn)))
    
    Output({ 
        'name' : name, 'd' : d,
        'RuleExt' : RuleExt, 'nStates' : nStates, 'nRule' : nRule,
        'cRuleMap' : cRuleMap, 'RuleOut' : RuleOut })
    print('Created Rule %s !' % name)