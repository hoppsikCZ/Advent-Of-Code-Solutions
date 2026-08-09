let result = 0;
let value = 50;

function adjustValue(num: number) {
    let prevRes = value;
    value += num;
    if (prevRes != 0 && value < 0 && Math.abs(num) % 100 >= prevRes) result++;
    if (prevRes != 0 && prevRes + num % 100 > 99) result++;
    result += Math.floor(Math.abs((num) / 100))
    if (value == 0) result++;
    value %= 100;
    if (value < 0) value = 100 + value;

    // Puvodni reseni
    /*while (num != 0) {
        if (num > 0) {
            value++;
            num--;
        } else {
            value--;
            num++;
        }

        if (value > 99) value = 0;
        else if (value < 0) value = 99;

        if (value == 0 ) result++;
    }*/

    console.log(value + ' ' + num + ' ' + result)
}

const data = await Bun.file('input.txt').text();

if (!data) {
    console.log('Error parsing data.');
    process.exit(0);
}

const lines = data.split('\n');

for (let line of lines) {
    let turn = parseInt(line.slice(1));
    if (line[0] == 'R') {
        adjustValue(turn);
    } else {
        adjustValue(-turn);
    }
}

console.log(result);