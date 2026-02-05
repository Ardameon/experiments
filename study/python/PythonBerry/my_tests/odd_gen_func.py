#!/usr/bin/python3

def odds_list(count, start = 1):
    """Create list of odd numbers of appropriate length"""
    # Take next odd number if start is not odd
    if start & 1 == 0:
        start += 1

    last_odd = count * 2 + start

    return [x for x in range(start, last_odd, 2)]

print(odds_list(10))
print(odds_list(10, 5))
print(odds_list(5, 1))
