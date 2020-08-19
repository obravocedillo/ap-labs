nestedExample = [1,4,[2,3,[1,2,[2]],2],1]
count = 0
def nestedArray(array):
    global count
    for element in array:
        if isinstance(element,list):
            nestedArray(element)
        else:
            count += 1
    return count

print(nestedArray(nestedExample))
