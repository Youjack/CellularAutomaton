# -*- coding: utf-8 -*-

import RuleBuilder
import MapBuilder
import CARunner

def Controller():
    toDo = int(input('Run a CA =0 or Create a Map =1 or Create a Rule =2 : '))
    if toDo == 0: CARunner.Runner()
    elif toDo == 1: MapBuilder.Builder()
    else: RuleBuilder.Builder()

if __name__ == "__main__":
    print('- Cellular Automaton -')
    Controller()
    input('Press Enter to Exit')
