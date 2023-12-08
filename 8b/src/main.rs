use std::collections::HashMap;

enum Instruction {
    Left,
    Right,
}

impl TryFrom<u8> for Instruction {
    type Error = &'static str;

    fn try_from(value: u8) -> Result<Self, Self::Error> {
        match value {
            b'L' => Ok(Instruction::Left),
            b'R' => Ok(Instruction::Right),
            _ => Err("Unknown character"),
        }
    }
}

fn lcm(nums: &[usize]) -> usize {
    if nums.len() == 1 {
        return nums[0];
    }
    let a = nums[0];
    let b = lcm(&nums[1..]);
    a * b / gcd_of_two_numbers(a, b)
}

fn gcd_of_two_numbers(a: usize, b: usize) -> usize {
    if b == 0 {
        return a;
    }
    gcd_of_two_numbers(b, a % b)
}

fn main() {
    let input = std::fs::read_to_string("input.txt").unwrap();
    let mut lines = input.lines();
    let first_line = lines.next().unwrap();
    let instructions: Vec<Instruction> = first_line
        .bytes()
        .map(|ch| ch.try_into().unwrap())
        .collect();
    lines.next();
    let mut adjacency_list: HashMap<String, (String, String)> = HashMap::new();
    for line in lines {
        let current_location = line[0..3].to_string();
        let left = line[7..10].to_string();
        let right = line[12..15].to_string();
        adjacency_list.insert(current_location, (left, right));
    }
    let mut step_vec: Vec<usize> = Vec::new();
    let current_locations: Vec<_> = adjacency_list
        .keys()
        .filter(|loc| loc.ends_with('A'))
        .cloned()
        .collect();
    for mut current_location in current_locations {
        let mut steps = 0;
        for instruction in instructions.iter().cycle() {
            if current_location.ends_with('Z') {
                step_vec.push(steps);
                break;
            }
            let (left, right) = adjacency_list.get(&current_location).unwrap();
            current_location = match instruction {
                Instruction::Left => left,
                Instruction::Right => right,
            }
            .to_string();
            steps += 1;
        }
    }
    let steps = lcm(&step_vec);
    println!("{}", steps);
}
