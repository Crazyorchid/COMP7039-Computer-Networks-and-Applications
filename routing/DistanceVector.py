#!/usr/bin/env python3
from copy import deepcopy
INF = 16
# 算法
def getInput():
    last = '   '
    num = []
    numFlag = True
    messageFlag = True
    messageList = []
    message = []
    while True:
        if numFlag:
            data = input()
            num.append(data)
            if data == '':
                numFlag = False
            continue
        router = input()
        if router == "":
            messageList.append(message)
            messageFlag = False
            message = []
            if last == '':
                break
        else:
            message.append(router)
        last = router
    return num[:-1], messageList[:-1]
def change(num):
    if num<INF:
        return str(num)
    elif num ==INF:
        return '-'
    else:
        return "INF"

def printInfo(tableDict,idx):
    for k,v in tableDict.items():
        _tmp = sorted([item for item in v.keys()])
        print("router {} at t={}".format(k,idx))
        print(" ".join(_tmp))
        for _k in _tmp:
            __tmp = [change(tableDict[k][_][_k]) for _ in _tmp]
            __tmp = [_k]+__tmp
            print(" ".join(__tmp))

def printSession(tableDict):
    routers = sorted(tableDict.keys())
    for item in routers:
        tmp = [i for i in routers if i!=item ]
        for i in tmp:
            data = INF
            idx = 0
            for _ in tmp:
                if tableDict[item][_][i] < data:
                    data = tableDict[item][_][i]
                    idx = _
            print("router {}: {} is {} routing through {}".format(item,i,data,idx))

def DistanceVector():
    routers, message = getInput()
    routers = sorted(routers)
    routersNum = len(routers)
    sessionsNum = len(message)
    tableDict = {}
    for item in routers:
        tableDict[item] = {}
        subs = [i for i in routers if i!=item]
        for _from in subs:
            tableDict[item][_from] = {}
            for _to in subs:
                tableDict[item][_from][_to] = INF
    total = 0
    for time in range(sessionsNum):
        _tableDict = deepcopy(tableDict)
        __tableDict = deepcopy(tableDict)
        newState = message[time]
        for item in newState:
            _from, _to, _num = item.split(' ')
            _num = int(_num)
            _tableDict[_from][_to][_to] = _num if _num != -1 else INF
            _tableDict[_to][_from][_from] = _num if _num != -1 else INF
        
        # Loop
        flag = False
        count = 0
        while not flag:
            count += 1
            flag = True

            for item in routers:
                subs = [i for i in routers if i!=item]
                for i in subs:
                    from_bias = _tableDict[item][i][i]
                    for j in subs:
                        if from_bias == INF:
                            tableDict[item][i][j] = INF
                            continue
                        if i==j:
                            to_bias = 0
                        else:
                            _subs = [t for t in routers if (t!= i)]
                            if count == 1:
                                to_bias = min([__tableDict[i][_][j] for _ in _subs])
                            else:
                                to_bias = min([_tableDict[i][_][j] for _ in _subs])
                        if tableDict[item][i][j]!=from_bias+to_bias:
                            flag = False
                            tableDict[item][i][j] = from_bias+to_bias
            if not flag:
                total += 1
                printInfo(tableDict,total-1)
            _tableDict = deepcopy(tableDict)
        printSession(tableDict)        
        



DistanceVector()



    