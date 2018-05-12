
extern crate rand;
extern crate rayon;
extern crate specs;
extern crate sdl2;

// use rayon::iter::ParallelIterator;

use specs::prelude::*;

use sdl2::rect::Rect;
use sdl2::pixels::Color;
use sdl2::event::Event;
use sdl2::mouse::MouseButton;
use sdl2::keyboard::Keycode;
use sdl2::video::{Window, WindowContext};
use sdl2::render::{Canvas, Texture, TextureCreator};

mod ant_sim;
use ant_sim::{SQUARE_SIZE, PLAYGROUND_WIDTH, PLAYGROUND_HEIGHT};
mod color;

enum ArrowKey {
    Up,
    Down,
    Left,
    Right
}

#[derive(Default)]
struct ArrowKeyEvent(Vec<ArrowKey>);

#[derive(Default, Debug, Clone)]
struct ColorPos {
    vec: Vec<(BlockColor, Pos)>,
}

#[derive(Debug, Default)]
struct CamPos(i32, i32);

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

struct CameraFollower;
impl Component for CameraFollower {
    type Storage = DenseVecStorage<Self>;
}

struct CameraFollowerSystem;
impl<'a> System<'a> for CameraFollowerSystem {
    type SystemData = (Write<'a, CamPos>, ReadStorage<'a, Pos>, ReadStorage<'a, CameraFollower>);

    fn run(&mut self, data: Self::SystemData) {
        let (mut cam_pos, positions, camera_followers) = data;

        let mut found_one = false;
        for (pos, _cf) in (&positions, &camera_followers).join() {
            if found_one {
                println!("You have multiple entities with a position and camera follower. This is bad");
                break;
            }
            *cam_pos = CamPos(pos.0, pos.1);
            found_one = true;
        }
    }
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
    world.register::<CameraFollower>();

    world
        .create_entity()
        .with(Pos(1, 4))
        .with(BlockColor{ id: color::Type::Black })
        .with(KeyboardController)
        .with(CameraFollower)
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
    
    world.add_resource(CamPos(0, 0));

    let mut render_sys = RenderSystem;
    let mut keyboard_sys = KeyboardInputSystem;
    let mut cam_follow_sys = CameraFollowerSystem;

    // this struct manages textures. For lifetime reasons, the canvas cannot directly create
    // textures, you have to create a `TextureCreator` instead.
    let texture_creator : TextureCreator<_> = canvas.texture_creator();

    let mut game = ant_sim::AntGame::new();
    let mut event_pump = sdl_context.event_pump().unwrap();

    let mut tex_vec = Vec::new();

    for n in 0..color::len() {
        tex_vec.push(texture_block(&mut canvas, &texture_creator, color::enumify(n)));
    }

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
        cam_follow_sys.run_now(&world.res);

        // get data out of systems
        let color_pos = world.read_resource::<ColorPos>();

        let cam_pos = world.read_resource::<CamPos>();

        println!("Cam Pos {:?}", *cam_pos);
        println!("Canvas size? {:?}", canvas.output_size());

        // rendering
        canvas.set_draw_color(Color::RGB(255, 255, 255));
        canvas.clear();

        for (bc, pos) in color_pos.vec.clone() {
            let x_pos_unit = pos.0 - cam_pos.0;
            let y_pos_unit = pos.1 - cam_pos.1;

            let canvas_size = canvas.output_size().unwrap();
            let x_pos_px = (x_pos_unit * SQUARE_SIZE as i32) + (canvas_size.0 as i32 / 2);
            let y_pos_px = (y_pos_unit * SQUARE_SIZE as i32) + (canvas_size.1 as i32 / 2);

            let rect = Rect::new(x_pos_px, y_pos_px, SQUARE_SIZE, SQUARE_SIZE);

            let _res = canvas.copy(&tex_vec[color::indexify(bc.id)], None, rect);
        }

        canvas.present();
    }
}
