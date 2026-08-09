use std::collections::HashMap;

fn main() {
    let input = read_file("input.txt");
    let lines: Vec<&str> = input.lines().collect();
    let racks = parse_racks(lines);
    print!("{}", find_all_out_routes("you", &racks))
}

fn read_file(file_path: &str) -> String {
    let contents = std::fs::read_to_string(file_path).expect("Failed to read the file");
    contents
}

fn parse_racks(lines: Vec<&str>) -> HashMap<&str, Vec<&str>> {
    let mut racks = HashMap::new();
    for line in &lines {
        let mut parts = line.split(' ');
        let key = parts.next().unwrap().trim_matches(':');
        let others = parts.collect();
        racks.insert(key, others);
    }
    racks
}

fn find_all_out_routes(key: &str, map: &HashMap<&str, Vec<&str>>) -> i32 {
    let routes = &map[key];
    let mut result = 0;
    for route in routes {
        if *route == "out" {
            result += 1;
        } else {
            result += find_all_out_routes(&route, &map);
        }
    }
    result
}
