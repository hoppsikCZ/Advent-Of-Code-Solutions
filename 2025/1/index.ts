let result = 0;
let value = 50;

function adjustValue(num: number) {
    value += num;
    value %= 100;
    if (value < 0) value = 100 + value;
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

    if (value === 0) {
        result++;
    }
}

console.log(result);