/*
Not sure what I did wrong here, works with the examples :(
*/

use itertools::Itertools;

#[derive(Debug)]
struct Line<'a> {
    springs: &'a str,
    faulty: Vec<usize>,
}

fn main() {
    let input = include_str!("input.txt");
    let mut lines = Vec::new();
    for line in input.lines() {
        let (springs, faulty) = line.split_once(' ').unwrap();
        let faulty = faulty.split(',').map(|s| s.parse().unwrap()).collect();
        lines.push(Line { springs, faulty });
    }
    let mut total_arrangements = 0;
    for line in lines {
        let total_faulty: usize = line.faulty.iter().sum();
        let known_faulty = line.springs.bytes().filter(|b| *b == b'#').count();
        let unknown_faulty = total_faulty - known_faulty;
        if unknown_faulty == 0 {
            continue;
        }
        let unknown_locations: Vec<_> = line
            .springs
            .bytes()
            .enumerate()
            .filter_map(|(idx, b)| if b == b'?' { Some(idx) } else { None })
            .collect();
        for possible_arrangement in unknown_locations.into_iter().combinations(unknown_faulty) {
            if line.validate(&possible_arrangement) {
                let mut val: Vec<_> = line.springs.bytes().collect();
                for idx in possible_arrangement {
                    val[idx] = b'#';
                }
                println!("{:?}, {:?}", String::from_utf8(val).unwrap(), line.faulty);
                total_arrangements += 1;
            }
        }
    }
    println!("{total_arrangements}");
}

impl<'a> Line<'a> {
    fn validate(&self, possible_arrangement: &[usize]) -> bool {
        let mut possible_match = Vec::new();
        let mut temp = 0;
        for (idx, c) in self.springs.bytes().enumerate() {
            let is_broken = c == b'#' || possible_arrangement.contains(&idx);
            if is_broken {
                temp += 1;
            } else if temp > 0 {
                possible_match.push(temp);
                temp = 0;
            }
        }
        if temp > 0 {
            possible_match.push(temp);
        }
        self.faulty == possible_match
    }
}
