use itertools::Itertools;
use std::collections::VecDeque;

const MULTIPLIER: usize = 2;

fn main() {
    let input = include_str!("input.txt");
    let columns = input.lines().next().unwrap().chars().count();
    let rows = input.lines().count();
    let mut empty_rows = Vec::new();
    let mut empty_columns = Vec::new();
    let lines: VecDeque<VecDeque<char>> =
        input.lines().map(|line| line.chars().collect()).collect();
    for row in 0..rows {
        let is_empty_row = !lines[row].iter().any(|c| *c == '#');
        if is_empty_row {
            empty_rows.push(row);
        }
    }
    for column in 0..columns {
        let is_empty_column = !lines.iter().map(|line| line[column]).any(|c| c == '#');
        if is_empty_column {
            empty_columns.push(column);
        }
    }
    let mut galaxies = Vec::new();
    for (row, line) in lines.iter().enumerate() {
        for (column, char) in line.iter().enumerate() {
            if *char == '#' {
                galaxies.push((row, column));
            }
        }
    }
    let mut total_distance = 0;
    for (galaxy_one, galaxy_two) in galaxies.into_iter().tuple_combinations::<(_, _)>() {
        let row_range = if galaxy_one.0 > galaxy_two.0 {
            galaxy_two.0..=galaxy_one.0
        } else {
            galaxy_one.0..=galaxy_two.0
        };
        let column_range = if galaxy_one.1 > galaxy_two.1 {
            galaxy_two.1..galaxy_one.1
        } else {
            galaxy_one.1..galaxy_two.1
        };
        let row_offset = empty_rows
            .iter()
            .filter(|row| row_range.contains(row))
            .count()
            * (MULTIPLIER - 1);
        let column_offset = empty_columns
            .iter()
            .filter(|col| column_range.contains(col))
            .count()
            * (MULTIPLIER - 1);
        let manhattan_distance = galaxy_two.0.abs_diff(galaxy_one.0)
            + row_offset
            + galaxy_two.1.abs_diff(galaxy_one.1)
            + column_offset;
        total_distance += manhattan_distance;
    }
    println!("{}", total_distance);
}
