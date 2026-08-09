fn main() {
    let mut result: i64 = 0;
    let input = read_file("input.txt");
    let lines: Vec<&str> = input.lines().collect();

    let (operations, lines) = extract_signs(lines);
    let columns = convert_to_num_columns(&lines);

    for (idx, column) in columns.iter().enumerate() {
        if operations[idx] {
            result += column.iter().product::<i64>();
        } else {
            result += column.iter().sum::<i64>();
        }
    }

    println!("{}", result)
}

fn convert_to_num_columns(input: &Vec<&str>) -> Vec<Vec<i64>> {
    let mut result: Vec<Vec<i64>> = vec![];

    for line in input {
        for (x, num) in line.split_whitespace().enumerate() {
            if result.len() <= x {
                result.push(vec![]);
            }

            result[x].push(num.parse().unwrap());
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