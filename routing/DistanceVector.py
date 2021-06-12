#!/usr/bin/env python3
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
    return num, messageList[:-1]

def DistanceVector():
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

print(getInput())
    