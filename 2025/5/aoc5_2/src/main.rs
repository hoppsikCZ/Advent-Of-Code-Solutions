fn main() {
    let input = read_file("../input.txt");

    let lines: Vec<&str> = input.lines().collect();
    let mut allowed: Vec<(i64, i64)> = find_allowed(lines);
    let mut result = 0;

    let mut control = 0;
    let mut i = 0;
    let allowed_lenght = allowed.len();

    while i < allowed_lenght - control {
        let range = allowed[i];
        let (min, max) = range;
        let mut changed = false;

        for (idx, (min_2, max_2)) in allowed[i + 1..].iter().copied().enumerate() {
            changed = false;
            let min_inside = min >= min_2 && min <= max_2;
            let max_inside = max >= min_2 && max <= max_2;
            let new_inside = min <= min_2 && max >= max_2;
            let old_inside = min_2 <= min && max_2 >= max;

            if min_inside || max_inside || new_inside || old_inside {
                //println!("Fused {:?} and {:?}", range, allowed[idx + i + 1]);
                if old_inside {
                    allowed[i] = (min_2, max_2);
                } else if min_inside {
                    allowed[i] = (min_2, max);
                } else if max_inside {
                    allowed[i] = (min, max_2);
                }

                allowed.remove(idx + i + 1);
                control += 1;
                changed = true;
                break;
            }
        }

        if !changed {
            i += 1;
        }

        //println!("Vals after loop: {} {} {:?}", i, control, allowed)
    }

    for range in &allowed {
        result += range.1 - range.0 + 1;
    }

    //println!("{:?}", allowed);
    println!("{}", result);
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