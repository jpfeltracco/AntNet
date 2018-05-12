use sdl2::pixels::Color;

#[derive(Debug, Clone, Copy)]
pub enum Type {
    Black = 0,
    Green = 1,
    Brown = 2,
    Blue = 3,
    White = 4,
    End = 5, // not a real texture, please don't use
}

impl Default for Type {
    fn default() -> Type {
        Type::Black
    }
}

pub fn get(color_type: Type) -> Color {
    match color_type {
        Type::Black => Color::RGB(0, 0, 0),
        Type::Green => Color::RGB(0, 255, 0),
        Type::Brown => Color::RGB(139, 69, 19),
        Type::Blue => Color::RGB(0, 0, 255),
        Type::White => Color::RGB(255, 255, 255),
        Type::End => Color::RGB(0, 0, 0),
    }
}

pub fn indexify(color_type: Type) -> usize {
    return color_type as usize;
}

// why oh why is a cast from enum to primative okay,
// but not primative back to enum???
pub fn enumify(index: usize) -> Type {
    match index {
        0 => Type::Black,
        1 => Type::Green,
        2 => Type::Brown,
        3 => Type::Blue,
        4 => Type::White,
        5 => Type::End,
        _ => Type::End,
    }
}

pub fn len() -> usize {
    return indexify(Type::End);
}
