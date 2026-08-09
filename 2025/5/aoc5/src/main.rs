fn main() {
    let input = read_file("../input.txt");

    let lines: Vec<&str> = input.lines().collect();
    let lines2 = lines.clone();
    let allowed: Vec<(i64, i64)> = find_allowed(lines);
    let foods:Vec<i64> = find_food(lines2);
    let mut result = 0;

    for food in foods {
        for range in &allowed {
            if range.0 <= food && range.1 >= food {
                result += 1;
                break;
            }
        }
    }

    print!("{}", result)
}

fn read_file(file_path: &str) -> String {
    let contents = std::fs::read_to_string(file_path)
        .expect("Failed to read the file");
    contents
}

fn find_allowed(lines: Vec<&str>) -> Vec<(i64, i64)> {
    let mut allowed: Vec<(i64, i64)> = vec![];
    for line in lines {
        if line == "" {
            break;
        }

        let mut parts = line.split('-');
        let start: i64 = parts.next().unwrap().parse().unwrap();
        let end: i64 = parts.next().unwrap().parse().unwrap();

        allowed.push((start, end));
    }

    allowed
}

fn find_food(lines: Vec<&str>) -> Vec<i64> {
    let mut food: Vec<i64> = vec![];
    let start= lines.iter().position(|&x| x == "").unwrap();

    let length = lines.len();
    for num in &lines[start + 1..length] {
        food.push(num.parse().unwrap());
    }

    food
}