file = open('input.txt')
input = file.read().split(',')

result = 0

for entry in input:
    entries = entry.split('-')
    for num in range(int(entries[0]), int(entries[1]) + 1):
        num = str(num)
        if len(num) % 2:
            continue
        
        length = int(len(num))
        half = int(length / 2)
        if num[0:half] == num[half:length]:
            result += int(num)
        
print(result)