use sdl2::pixels::Color;

#[derive(Debug, Clone, Copy)]
pub enum Type {
    Black,
    Green,
    Brown,
    Blue,
    White,
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
    }
}
