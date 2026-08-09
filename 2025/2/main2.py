file = open('input.txt')
input = file.read().split(',')

result = 0

for entry in input:
    entries = entry.split('-')
    for num in range(int(entries[0]), int(entries[1]) + 1):
        num_str = str(num)
        num_len = len(num_str)
        for i in range(1, int(num_len / 2) + 1):
            if num_len % i != 0:
                continue
            slices = [num_str[j:j + i] for j in range(0, num_len, i)]
            if len(set(slices)) == 1:
                result += num
                break
            
print(result)