#!/usr/bin/python3

def josephus(items, k):
    result = []
    i = 0;

    while(len(items)):
        i = (i + k - 1) % len(items)
        result.append(items.pop(i))

    return result

def josephus_survivor(n, k):
    i = 0;

    items = [x for x in range(1, n + 1)]

    while len(items) > 1:
        i = (i + k - 1) % len(items)
        items.pop(i)

    return items[0]



def mix(s1, s2):
    d1 = {}
    d2 = {}
    dr = {}
    result_list = []

    l = [chr(x) for x in range(ord('a'), ord('z') + 1)]

    d1 = dict.fromkeys(l, 0)
    d2 = dict.fromkeys(l, 0)

    for c in s1:
        if c in l:
            d1[c] += 1

    for c in s2:
        if c in l:
            d2[c] += 1

    for c in l:
        if d1[c] == d2[c]:
            d = d1
            head = '=:'
        elif d1[c] > d2[c]:
            d = d1
            head = '1:'
        else:
            d = d2
            head = '2:'

        if d[c] > 1:
            if d[c] in dr:
                dr[d[c]].append(head + c * d[c])
            else:
                dr[d[c]] = [head + c * d[c]]

    for c in sorted(dr, reverse = True):
        result_list.extend(sorted(dr[c]))

    return '/'.join(result_list)

print(mix("Are they here", "yes, they are here"))
print(mix("looping is fun but dangerous", "less dangerous than coding"))
print(mix(" In many languages", " there's a pair of functions"))
print(mix("Lords of the Fallen", "gamekult"))
print(mix("codewars", "codewars"))
print(mix("A generation must confront the looming ", "codewarrs"))





