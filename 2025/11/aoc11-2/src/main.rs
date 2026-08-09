use std::collections::HashMap;

struct ResultCount {
    neither: i64,
    dac: i64,
    fft: i64,
    both: i64,
}

fn main() {
    let input = read_file("input.txt");
    let lines: Vec<&str> = input.lines().collect();
    let racks = parse_racks(lines);
    let mut calculated: HashMap<String, ResultCount> = HashMap::new();
    print!(
        "{}",
        find_all_out_routes("svr", &racks, Vec::new(), false, false, &mut calculated, 0)
    )
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

fn find_all_out_routes(
    key: &str,
    map: &HashMap<&str, Vec<&str>>,
    visited: Vec<&str>, // Turns out the bug was somewhere else, so this var and all logic aruond it
    // wasn't needed
    dac: bool,
    fft: bool,
    calculated: &mut HashMap<String, ResultCount>,
    depth: i32,
) -> i64 {
    // println!("Doint new recursion {}", key);
    let mut result = 0;
    let routes = &map[key];
    for route in routes {
        // println!("Trying route {}", route);
        let new_dac = dac || *route == "dac";
        let new_fft = fft || *route == "fft";
        let mut new_visited = visited.clone();
        if *route == "out" {
            if new_dac && new_fft {
                result += 1;
                // println!("Found out!");
            } else {
                // println!("Conditions not met");
                continue;
            }
        } else if !visited.contains(route) {
            if !calculated.contains_key(*route) {
                calculated.insert(
                    route.to_string(),
                    ResultCount {
                        neither: -1,
                        dac: -1,
                        fft: -1,
                        both: -1,
                    },
                );
            }
            new_visited.push(key);
            if let Some(res) = calculated.get(*route) {
                if new_dac && new_fft {
                    if res.both != -1 {
                        result += res.both;
                        // println!("Adding cached result {}", res.both);
                        continue;
                    }
                } else if new_dac {
                    if res.dac != -1 {
                        // println!("Adding cached result {}", res.dac);
                        result += res.dac;
                        continue;
                    }
                } else if new_fft {
                    if res.fft != -1 {
                        // println!("Adding cached result {}", res.fft);
                        result += res.fft;
                        continue;
                    }
                } else if res.neither != -1 {
                    result += res.neither;
                    continue;
                }

                let local_result = find_all_out_routes(
                    *route,
                    map,
                    new_visited,
                    new_dac,
                    new_fft,
                    calculated,
                    depth + 1,
                );
                // println!("{}", local_result);
                if let Some(calc_res) = calculated.get_mut(*route) {
                    if new_dac && new_fft {
                        calc_res.both = local_result;
                        // println!("{} {} Caching {}", depth, *route, calc_res.both);
                    } else if new_dac {
                        // println!("{} {} Caching {}", depth, *route, calc_res.dac);
                        calc_res.dac = local_result;
                    } else if new_fft {
                        // println!("{} {} Caching {}", depth, *route, calc_res.fft);
                        calc_res.fft = local_result;
                    } else {
                        calc_res.neither = local_result;
                    }
                } else {
                    print!("Error writing results.")
                }

                // println!(
                //     "{} {} Adding recursion result {}",
                //     depth, *route, local_result
                // );
                result += local_result;
            } else {
                print!("Error finding route in calculated!");
            }
        }
    }
    result
}
