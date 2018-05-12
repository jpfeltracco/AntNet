use specs::prelude::*;

use color;

#[derive(Default, Debug, Clone)]
pub struct ColorPos {
    pub vec: Vec<(BlockColor, Pos)>,
}

#[derive(Debug, Default)]
pub struct CamPos(pub i32, pub i32);

#[derive(Debug, Copy, Clone)]
pub struct Pos(pub i32, pub i32);
impl Component for Pos {
    // This uses `VecStorage`, because all entities have a position.
    type Storage = VecStorage<Self>;
}

#[derive(Debug, Clone, Copy)]
pub struct BlockColor {
    pub id: color::Type,
}
impl Component for BlockColor {
    type Storage = DenseVecStorage<Self>;
}

pub struct KeyboardController;
impl Component for KeyboardController {
    type Storage = DenseVecStorage<Self>;
}

pub struct CameraFollower;
impl Component for CameraFollower {
    type Storage = DenseVecStorage<Self>;
}
