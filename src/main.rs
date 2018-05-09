
extern crate rand;
extern crate rayon;
extern crate specs;
extern crate sdl2;


use rayon::iter::ParallelIterator;

use specs::prelude::*;

// use sdl2::rect::{Point, Rect};
use sdl2::rect::Rect;
use sdl2::pixels::Color;
use sdl2::event::Event;
use sdl2::mouse::MouseButton;
use sdl2::keyboard::Keycode;
use sdl2::video::{Window, WindowContext};
use sdl2::render::{Canvas, Texture, TextureCreator};


mod game_of_life;
use game_of_life::{SQUARE_SIZE, PLAYGROUND_WIDTH, PLAYGROUND_HEIGHT};

#[derive(Debug, Copy, Clone)]
struct Pos(f32, f32);
impl Component for Pos {
    // This uses `VecStorage`, because all entities have a position.
    type Storage = VecStorage<Self>;
}

#[derive(Debug)]
struct Vel(f32, f32);
impl Component for Vel {
    // This uses `DenseVecStorage`, because nearly all entities have a velocity.
    type Storage = DenseVecStorage<Self>;
}

#[derive(Debug, Clone, Copy)]
struct BlockColor {
    id: color::Type,
}
impl Component for BlockColor {
    type Storage = DenseVecStorage<Self>;
}

struct RenderSystem {
    texture_ids: Vec<(BlockColor, Pos)>,
}
impl<'a> System<'a> for RenderSystem {
    type SystemData = (ReadStorage<'a, BlockColor>, ReadStorage<'a, Pos>);

    fn run(&mut self, (mut texture, mut pos): Self::SystemData) {
        (&mut texture, &mut pos).par_join().for_each(|(tex, pos)| {
            let tup = (tex.clone(), pos.clone());
            self.texture_ids.push(tup);
        });
    }
}

impl RenderSystem {
    fn new() -> Self {
        return Self { texture_ids: Vec::new() }
    }

    fn get_textures(&self) -> Vec<(BlockColor, Pos)> {
        return self.texture_ids.clone();
    }
}

struct PhysicsSystem;
impl<'a> System<'a> for PhysicsSystem {
    type SystemData = (WriteStorage<'a, Pos>, ReadStorage<'a, Vel>);

    fn run(&mut self, (mut pos, vel): Self::SystemData) {
        (&mut pos, &vel).par_join().for_each(|(pos, vel)| {
            pos.0 += vel.0;
            pos.1 += vel.1;
        });
    }
}

mod color {
    use sdl2::pixels::Color;

    #[derive(Debug, Clone, Copy)]
    pub enum Type {
        Black,
        Green,
        Brown,
        Blue,
        White,
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

}

fn texture_block<'a>(canvas: &mut Canvas<Window>, texture_creator: &'a TextureCreator<WindowContext>, col: color::Type) -> Texture<'a> {
    let mut texture = texture_creator.create_texture_target(None, SQUARE_SIZE, SQUARE_SIZE).unwrap();
    
    canvas.with_texture_canvas(&mut texture, |texture_canvas| {
        texture_canvas.set_draw_color(color::get(col));
        texture_canvas.clear();
    }).unwrap();

    return texture;
}

pub fn main() {
    let sdl_context = sdl2::init().unwrap();
    let video_subsystem = sdl_context.video().unwrap();

    // the window is the representation of a window in your operating system,
    // however you can only manipulate properties of that window, like its size, whether it's
    // fullscreen, ... but you cannot change its content without using a Canvas or using the
    // `surface()` method.
    let window = video_subsystem
        .window("rust-sdl2 demo: Game of Life",
                SQUARE_SIZE*PLAYGROUND_WIDTH,
                SQUARE_SIZE*PLAYGROUND_HEIGHT)
        .position_centered()
        .build()
        .unwrap();

    // the canvas allows us to both manipulate the property of the window and to change its content
    // via hardware or software rendering. See CanvasBuilder for more info.
    let mut canvas = window.into_canvas()
        .target_texture()
        .present_vsync()
        .build().unwrap();

    println!("Using SDL_Renderer \"{}\"", canvas.info().name);
    canvas.set_draw_color(color::get(color::Type::White));
    // clears the canvas with the color we set in `set_draw_color`.
    canvas.clear();
    // However the canvas has not been updated to the window yet, everything has been processed to
    // an internal buffer, but if we want our buffer to be displayed on the window, we need to call
    // `present`. We need to call this everytime we want to render a new frame on the window.
    canvas.present();

    let mut world = World::new();
    world.register::<BlockColor>();

    world
        .create_entity()
        .with(Pos(1., 4.))
        .with(BlockColor{ id: color::Type::Black })
        .build();

    let mut render_sys = RenderSystem::new();
    let mut resources = specs::prelude::Resources::new();

    // this struct manages textures. For lifetime reasons, the canvas cannot directly create
    // textures, you have to create a `TextureCreator` instead.
    let texture_creator : TextureCreator<_> = canvas.texture_creator();

    // Create a "target" texture so that we can use our Renderer with it later
    let _grass_tex = texture_block(&mut canvas, &texture_creator, color::Type::Green);
    let dirt_tex = texture_block(&mut canvas, &texture_creator, color::Type::Brown);
    let water_tex = texture_block(&mut canvas, &texture_creator, color::Type::Blue);

    let mut game = game_of_life::GameOfLife::new();

    let mut event_pump = sdl_context.event_pump().unwrap();
    let mut frame : u32 = 0;
    'running: loop {
        // get the inputs here
        for event in event_pump.poll_iter() {
            match event {
                Event::Quit {..} | Event::KeyDown { keycode: Some(Keycode::Escape), .. } => {
                    break 'running
                },
                Event::KeyDown { keycode: Some(Keycode::Space), repeat: false, .. } => {
                    game.toggle_state();
                },
                Event::MouseButtonDown { x, y, mouse_btn: MouseButton::Left, .. } => {
                    let x = (x as u32) / SQUARE_SIZE;
                    let y = (y as u32) / SQUARE_SIZE;
                    match game.get_mut(x as i32, y as i32) {
                        Some(square) => {*square = !(*square);},
                        None => {panic!()}
                    };
                },
                _ => {}
            }
        }

        render_sys.run_now(&mut resources);

        let texts = render_sys.get_textures();
        for text_color in texts {
            
        }


        // update the game loop here
        // if frame >= 3 {
            // game.update();
            // frame = 0;
        // }

        // canvas.set_draw_color(Color::RGB(255, 255, 255));
        // canvas.clear();
        // for (i, unit) in (&game).into_iter().enumerate() {
            // let i = i as u32;
            // let square_texture = if frame >= 15 {
                // &dirt_tex
            // } else {
                // &water_tex
            // };
            // if *unit {
                // canvas.copy(square_texture,
                            // None,
                            // Rect::new(((i % PLAYGROUND_WIDTH) * SQUARE_SIZE) as i32,
                                      // ((i / PLAYGROUND_WIDTH) * SQUARE_SIZE) as i32,
                                      // SQUARE_SIZE,
                                      // SQUARE_SIZE)).unwrap();
            // }
        // }
        // canvas.present();
        // if let game_of_life::State::Playing = game.state() {
            // frame += 1;
        // };
    }
}
