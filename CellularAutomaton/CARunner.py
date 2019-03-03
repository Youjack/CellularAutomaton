# -*- coding: utf-8 -*-

import time
import RuleBuilder
import MapBuilder
from coreCA import coreAutomaton

def Runner():
    print('- Cellular Automaton Runner -')
    d = int(input('Number of Dimensions = '))
    RuleName = input('Name of Rule = ')
    MapName = input('Name of Map = ')
    Rule = RuleBuilder.Input(d, RuleName)
    Map = MapBuilder.Input(d, MapName)
    
    nStep = int(input('Number of Steps = '))
    withAMP = int(input('Use Accelerator ? Yes = 1 or No = 0 : '))

    t = time.time()
    MapOut = coreAutomaton([
        d,
        Map['cMapExt'], Map['MapExt'], Map['lMapExt'],
        Map['Map'],
        Rule['RuleExt'], Rule['nStates'], Rule['nRule'],
        Rule['cRuleMap'], Rule['RuleOut'],
        nStep, withAMP ])
    t = time.time() - t
    MapBuilder.PrintMap(d, Map['cMapExt'], Map['MapExt'], Map['lMapExt'], MapOut)
    # the last term in pMapOut is TIME spent, which should be deleted
    print('used %d ms' % MapOut[-1])
    MapOut.pop()
    
    saveMap = input('Save Result ? Yes =1 or No =0 : ')
    if saveMap == '1':
        name = input('Map Name = ')
        MapBuilder.Output({
            'name' : name, 'd' : d,
            'cMapExt' : Map['cMapExt'],
            'MapExt' : Map['MapExt'],
            'lMapExt' : Map['lMapExt'],
            'Map' : MapOut })
        print('Saved Map %s !' % name)
