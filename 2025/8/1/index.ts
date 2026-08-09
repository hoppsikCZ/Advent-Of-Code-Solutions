class Vector3 {
    private x: number;
    private y: number;
    private z: number;

    public constructor (x: number, y: number, z: number) {
        this.x = x;
        this.y = y;
        this.z = z;
    }

    public getX(): number {
        return this.x;
    }

    public getY(): number {
        return this.y;
    }

    public getZ(): number {
        return this.z;
    }

    public distance(other: Vector3): number {
        return Math.sqrt(Math.pow(other.getX() - this.x, 2) + Math.pow(other.getY() - this.y, 2) + Math.pow(other.getZ() - this.z, 2));
    }
}

interface distanceTuple {
    a: number;
    b: number;
    distance: number;
}

function parseData(data: string): (Vector3[] | undefined) {
    let vecs = data.split('\n').map((line) => {
        const [x, y, z] = line.split(',').map(num => Number.parseInt(num));
        if (x && y && z) {
            return new Vector3(x, y, z);
        }

        return undefined
    });

    if (vecs.some(vec => !vec)) {
        return undefined;
    }
    
    return vecs as unknown as Vector3[];
}

const data = await Bun.file('input.txt').text();

if (!data) {
    console.log('Error parsing data.');
    process.exit(0);
}

let vecs = parseData(data);

if (!vecs) {
    process.exit(-1);
}

let pairs: distanceTuple[] = [];

for (let i = 0; i < vecs.length - 1; i++) {
    const a = vecs[i];
    for (let j = i + 1; j < vecs.length; j++) {
        if (i == j) continue;
        const b = vecs[j];
        if (!a || !b) continue;
         
        const distance = a.distance(b);
        const pair: distanceTuple = { a: i, b: j, distance };

        pairs.push(pair);
    }
}

pairs.sort((a, b) => a.distance - b.distance);

const firstThousand = pairs.slice(0, 1000);

//console.log(firstThousand)

let groups: number[][] = [];

firstThousand.forEach(tuple => {
    const idxA: number = groups.findIndex(group => group.includes(tuple.a));
    const idxB: number = groups.findIndex(group => group.includes(tuple.b));

    let a = groups[idxA];
    let b = groups[idxB];

    if (!a || !b) {
        if (!a && !b) {
            //console.log("Adding new " + tuple.a + " " + tuple.b);
            groups.push([tuple.a, tuple.b]);
            //console.log(groups);
        } else if (!a && b) {
            //console.log("Adding " + tuple.a + " to " + b);
            b.push(tuple.a);
            //console.log(groups);
        } else if (a && !b) {
            //console.log("Adding " + tuple.b + " to " + a);
            a.push(tuple.b);
            //console.log(groups);
        }
    } else if (a && b && a != b) {
            //console.log("Merging " + a + " and " + b)
            a.push(...b)
            groups.splice(idxB, 1);
            //console.log(groups)
    }
});

groups.sort((a, b) => b.length - a.length);

const first = groups[0];
const second = groups[1];
const third = groups[2];

if (first && second && third) {
    console.log(first.length * second.length * third.length);   
} else {
    console.log("Někde se stala chyba!");
}