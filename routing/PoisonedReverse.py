#!/usr/bin/env python3
INF = 16

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

def processData(num, message):
    routers = sorted(num)
    routersNum = len(routers)


def PoisonedReverse():
    last = '   '
    messageList = []
    while True:
        router = input()
        print("Test:", router)
        messageList.append(router)
        if last == '' and router == '':
            break
        last = router
    print("sdfs")
    for item in messageList:
        print(item)

PoisonedReverse()
    