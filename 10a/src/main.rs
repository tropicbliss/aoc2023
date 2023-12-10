use ahash::{AHashMap, AHashSet};
use phf::phf_map;
use std::collections::VecDeque;

const INPUT: &str = include_str!("../target/release/input.txt");

#[derive(Clone, Copy, PartialEq, Debug)]
enum Direction {
    North,
    South,
    East,
    West,
}

enum Pipe {
    Direction([Direction; 2]),
    Starting,
    None,
}

static PIPES: phf::Map<u8, Pipe> = phf_map! {
    b'|' => Pipe::Direction([Direction::North, Direction::South]),
    b'-' => Pipe::Direction([Direction::East, Direction::West]),
    b'L' => Pipe::Direction([Direction::North, Direction::East]),
    b'J' => Pipe::Direction([Direction::North, Direction::West]),
    b'7' => Pipe::Direction([Direction::South, Direction::West]),
    b'F' => Pipe::Direction([Direction::South, Direction::East]),
    b'.' => Pipe::None,
    b'S' => Pipe::Starting
};

fn get_surrounding_pipes(
    row: usize,
    column: usize,
    max_rows: usize,
    max_columns: usize,
) -> Vec<(usize, usize, Direction)> {
    let mut result: Vec<(usize, usize, Direction)> = Vec::with_capacity(4);
    if row != 0 {
        result.push((row - 1, column, Direction::North));
    }
    if column != 0 {
        result.push((row, column - 1, Direction::West));
    }
    if row != max_rows - 1 {
        result.push((row + 1, column, Direction::South));
    }
    if column != max_columns - 1 {
        result.push((row, column + 1, Direction::East));
    }
    result
}

/// Only use this function before building out the adjancency list
fn get_directions(row: usize, column: usize) -> Option<[Direction; 2]> {
    let lines: Vec<_> = INPUT.lines().collect();
    let char = lines[row].as_bytes()[column];
    if let Pipe::Direction([dir1, dir2]) = PIPES.get(&char).expect("Unexpected character") {
        Some([*dir1, *dir2])
    } else {
        None
    }
}

fn get_node_from_current_direction(
    row: usize,
    column: usize,
    max_rows: usize,
    max_columns: usize,
    direction: &Direction,
) -> Option<(usize, usize)> {
    match direction {
        Direction::East => {
            if column != max_columns - 1 {
                Some((row, column + 1))
            } else {
                None
            }
        }
        Direction::North => {
            if row != 0 {
                Some((row - 1, column))
            } else {
                None
            }
        }
        Direction::South => {
            if row != max_rows - 1 {
                Some((row + 1, column))
            } else {
                None
            }
        }
        Direction::West => {
            if column != 0 {
                Some((row, column - 1))
            } else {
                None
            }
        }
    }
}

fn main() {
    let mut max_columns = 0;
    let max_rows = INPUT.lines().count();
    let mut adjacency_list = AHashMap::with_capacity(max_columns * max_rows);
    let mut starting_position = (0, 0);
    for (row, line) in INPUT.lines().enumerate() {
        if row == 0 {
            max_columns = line.len();
        }
        for (column, char) in line.bytes().enumerate() {
            match PIPES.get(&char).expect("Unexpected character") {
                Pipe::Direction(dirs) => {
                    let mut directions = Vec::with_capacity(2);
                    for dir in dirs {
                        if let Some(node) =
                            get_node_from_current_direction(row, column, max_rows, max_columns, dir)
                        {
                            directions.push(node);
                        }
                    }
                    adjacency_list.insert((row, column), directions);
                }
                Pipe::None => {}
                Pipe::Starting => {
                    let mut directions = Vec::with_capacity(4);
                    for (neighbouring_row, neighbouring_column, direction) in
                        get_surrounding_pipes(row, column, max_rows, max_columns)
                    {
                        if let Some(dir_array) =
                            get_directions(neighbouring_row, neighbouring_column)
                        {
                            let is_connected = match direction {
                                Direction::North => dir_array.contains(&Direction::South),
                                Direction::South => dir_array.contains(&Direction::North),
                                Direction::East => dir_array.contains(&Direction::West),
                                Direction::West => dir_array.contains(&Direction::East),
                            };
                            if is_connected {
                                if let Some(node) = get_node_from_current_direction(
                                    row,
                                    column,
                                    max_rows,
                                    max_columns,
                                    &direction,
                                ) {
                                    directions.push(node);
                                }
                            }
                        }
                    }
                    adjacency_list.insert((row, column), directions);
                    starting_position = (row, column);
                }
            };
        }
    }
    let mut max_depth = 0;
    let mut visited = AHashSet::new();
    let mut queue = VecDeque::new();
    queue.push_back((starting_position, 0));
    while let Some((node, depth)) = queue.pop_front() {
        max_depth = max_depth.max(depth);
        for adj_node in adjacency_list.get(&node).unwrap() {
            if !visited.contains(adj_node) {
                visited.insert(adj_node);
                queue.push_back((*adj_node, depth + 1));
            }
        }
    }
    println!("{max_depth}");
}
