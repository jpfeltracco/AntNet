
extern crate rand;
extern crate rayon;
extern crate specs;
extern crate sdl2;

use rayon::iter::ParallelIterator;

use specs::prelude::*;
// use specs::Read;

// use sdl2::rect::{Point, Rect};
use sdl2::rect::Rect;
use sdl2::pixels::Color;
use sdl2::event::Event;
use sdl2::mouse::MouseButton;
use sdl2::keyboard::Keycode;
use sdl2::video::{Window, WindowContext};
use sdl2::render::{Canvas, Texture, TextureCreator};

mod ant_sim;
use ant_sim::{SQUARE_SIZE, PLAYGROUND_WIDTH, PLAYGROUND_HEIGHT};

enum ArrowKey {
    Up,
    Down,
    Left,
    Right
}

#[derive(Default)]
struct ArrowKeyEvent(Vec<ArrowKey>);

// world.add_resource(ResizeEvents(Vec::new()));

// while let Some(event) = window.poll_event() {
    // match event {
        // Input::Resize(x, y) => world.write_resource::<ResizeEvents>().0.push((x, y)),
        // // ...
    // }
// }

mod color;

#[derive(Debug, Copy, Clone)]
struct Pos(i32, i32);
impl Component for Pos {
    // This uses `VecStorage`, because all entities have a position.
    type Storage = VecStorage<Self>;
}

#[derive(Debug, Clone, Copy)]
struct BlockColor {
    id: color::Type,
}
impl Component for BlockColor {
    type Storage = DenseVecStorage<Self>;
}

struct KeyboardController;
impl Component for KeyboardController {
    type Storage = DenseVecStorage<Self>;
}

struct KeyboardInputSystem;
impl<'a> System<'a> for KeyboardInputSystem {
    type SystemData = (Write<'a, ArrowKeyEvent>, WriteStorage<'a, Pos>, ReadStorage<'a, KeyboardController>);

    fn run(&mut self, data: Self::SystemData) {
        let (mut key_event, mut positions, keyboard_controller) = data;

        for e in &key_event.0 {
            for (pos, _kc) in (&mut positions, &keyboard_controller).join() {
                match e {
                    &ArrowKey::Up => pos.1 -= 1,
                    &ArrowKey::Down => pos.1 += 1,
                    &ArrowKey::Left => pos.0 -= 1,
                    &ArrowKey::Right => pos.0 += 1,
                }
            }
        }
        key_event.0.clear();
    }
}

struct RenderSystem;
impl<'a> System<'a> for RenderSystem {
    type SystemData = (Write<'a, ColorPos>, ReadStorage<'a, BlockColor>, ReadStorage<'a, Pos>);

    fn run(&mut self, data: Self::SystemData) {
        let (mut block_color, texture, pos) = data;
        block_color.vec.clear();

        for (tex, pos) in (&texture, &pos).join() {
            block_color.vec.push((tex.clone(), pos.clone()));
        }
    }
}

#[derive(Default, Debug, Clone)]
struct ColorPos {
    vec: Vec<(BlockColor, Pos)>,
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
    world.register::<Pos>();
    world.register::<BlockColor>();
    world.register::<KeyboardController>();

    world
        .create_entity()
        .with(Pos(1, 4))
        .with(BlockColor{ id: color::Type::Black })
        .with(KeyboardController)
        .build();
    world
        .create_entity()
        .with(Pos(7, 4))
        .with(BlockColor{ id: color::Type::Green })
        .build();
    world
        .create_entity()
        .with(Pos(0, 0))
        .with(BlockColor{ id: color::Type::Brown })
        .build();
    world
        .create_entity()
        .with(Pos(1, 10))
        .with(BlockColor{ id: color::Type::Blue })
        .build();


    // struct DeltaTime(f32);
    // world.add_resource(DeltaTime(0.05));
    // let mut delta = world.write_resource::<DeltaTime>();

    world.add_resource(ColorPos{ vec: Vec::new() });

    world.add_resource(ArrowKeyEvent(Vec::new()));
    // let mut color_pos = world.write_resource::<ColorPos>();


    let mut render_sys = RenderSystem;
    let mut keyboard_sys = KeyboardInputSystem;

    // this struct manages textures. For lifetime reasons, the canvas cannot directly create
    // textures, you have to create a `TextureCreator` instead.
    let texture_creator : TextureCreator<_> = canvas.texture_creator();

    // Create a "target" texture so that we can use our Renderer with it later
    let _grass_tex = texture_block(&mut canvas, &texture_creator, color::Type::Green);
    let dirt_tex = texture_block(&mut canvas, &texture_creator, color::Type::Brown);
    let _water_tex = texture_block(&mut canvas, &texture_creator, color::Type::Blue);

    let mut game = ant_sim::AntGame::new();

    let mut event_pump = sdl_context.event_pump().unwrap();
    // let mut frame : u32 = 0;
// while let Some(event) = window.poll_event() {
    // match event {
        // Input::Resize(x, y) => world.write_resource::<ResizeEvents>().0.push((x, y)),
        // // ...
    // }
// }
    'running: loop {
        // get the inputs here
        for event in event_pump.poll_iter() {
            match event {
                Event::Quit {..} | Event::KeyDown { keycode: Some(Keycode::Escape), .. } => {
                    break 'running
                },
                Event::KeyDown { keycode: Some(Keycode::Up), repeat: false, .. } => {
                    world.write_resource::<ArrowKeyEvent>().0.push(ArrowKey::Up);
                },
                Event::KeyDown { keycode: Some(Keycode::Down), repeat: false, .. } => {
                    world.write_resource::<ArrowKeyEvent>().0.push(ArrowKey::Down);
                },
                Event::KeyDown { keycode: Some(Keycode::Left), repeat: false, .. } => {
                    world.write_resource::<ArrowKeyEvent>().0.push(ArrowKey::Left);
                },
                Event::KeyDown { keycode: Some(Keycode::Right), repeat: false, .. } => {
                    world.write_resource::<ArrowKeyEvent>().0.push(ArrowKey::Right);
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

        // run systems
        render_sys.run_now(&world.res);
        keyboard_sys.run_now(&world.res);

        // get data out of systems
        let color_pos = world.read_resource::<ColorPos>();

        // rendering
        canvas.set_draw_color(Color::RGB(255, 255, 255));
        canvas.clear();

        // shouldn't be building textures every single time we need them, need a way to cache
        for (bc, pos) in color_pos.vec.clone() {
            let tex = texture_block(&mut canvas, &texture_creator, bc.id);
            canvas.copy(&tex, None,
                            Rect::new(((pos.0 % PLAYGROUND_WIDTH as i32) * SQUARE_SIZE as i32) as i32,
                                      ((pos.1 % PLAYGROUND_WIDTH as i32) * SQUARE_SIZE as i32) as i32,
                                      SQUARE_SIZE,
                                      SQUARE_SIZE)).unwrap();
        }

        canvas.present();
    }
}
