fn main() {
    let mut result: i64 = 0;
    let input = read_file("input.txt");
    let lines: Vec<&str> = input.lines().collect();

    let (operations, lines) = extract_signs(lines);
    let columns = parse_data(&lines);

    for (idx, column) in columns.iter().enumerate() {
        if operations[idx] {
            result += column.iter().product::<i64>();
        } else {
            result += column.iter().sum::<i64>();
        }
    }

    println!("{}", result)
}

fn parse_data(input: &Vec<&str>) -> Vec<Vec<i64>> {
    let mut result: Vec<Vec<i64>> = vec![];
    let mut temp_nums: Vec<String> = vec![];

    for line in input {
        for (x, num) in line.chars().enumerate() {
            if temp_nums.len() <= x {
                temp_nums.push("".to_string());
            }

            temp_nums[x].push(num);
        }
    }

    let mut groups = 0;
    for num_string in &temp_nums {
        if result.len() <= groups {
            result.push(vec![]);
        }
        if let Ok(num) = num_string.trim().parse() {
            result[groups].push(num);
        } else {
            groups += 1;
        }
    }

    result
}

fn read_file(file_path: &str) -> String {
    let contents = std::fs::read_to_string(file_path)
        .expect("Failed to read the file");
    contents
}

fn extract_signs(input: Vec<&str>) -> (Vec<bool>, Vec<&str>) {
    (input.last().unwrap().split_whitespace().map(|str| str == "*").collect(), input[..input.len() - 1].to_vec())
}