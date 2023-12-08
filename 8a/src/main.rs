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
    let mut steps = 0;
    let mut current_location = "AAA";
    for instruction in instructions.iter().cycle() {
        if current_location == "ZZZ" {
            break;
        }
        let (left, right) = adjacency_list.get(current_location).unwrap();
        current_location = match instruction {
            Instruction::Left => left,
            Instruction::Right => right,
        };
        steps += 1;
    }
    println!("{steps}");
}
