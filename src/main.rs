
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

mod component;

enum ArrowKey {
    Up,
    Down,
    Left,
    Right
}

#[derive(Default)]
struct ArrowKeyEvent(Vec<ArrowKey>);

struct CameraFollowerSystem;
impl<'a> System<'a> for CameraFollowerSystem {
    type SystemData = (Write<'a, component::CamPos>, ReadStorage<'a, component::Pos>, ReadStorage<'a, component::CameraFollower>);

    fn run(&mut self, data: Self::SystemData) {
        let (mut cam_pos, positions, camera_followers) = data;

        let mut found_one = false;
        for (pos, _cf) in (&positions, &camera_followers).join() {
            if found_one {
                println!("You have multiple entities with a position and camera follower. This is bad");
                break;
            }
            *cam_pos = component::CamPos(pos.0, pos.1);
            found_one = true;
        }
    }
}

struct KeyboardInputSystem;
impl<'a> System<'a> for KeyboardInputSystem {
    type SystemData = (Write<'a, ArrowKeyEvent>, WriteStorage<'a, component::Pos>, ReadStorage<'a, component::KeyboardController>);

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
    type SystemData = (Write<'a, component::ColorPos>, ReadStorage<'a, component::BlockColor>, ReadStorage<'a, component::Pos>);

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

struct MySys {
    sdl_context: sdl2::Sdl,
    canvas: sdl2::render::Canvas<sdl2::video::Window>,
}

impl MySys {
    fn new() -> MySys {
        let context = sdl2::init().unwrap();
        let vid_subsys = context.video().unwrap();
        let win = vid_subsys
            .window("rust-sdl2 demo: Game of Life",
                    SQUARE_SIZE*PLAYGROUND_WIDTH,
                    SQUARE_SIZE*PLAYGROUND_HEIGHT)
            .position_centered()
            .build()
            .unwrap();

        let mut canv = win.into_canvas()
            .target_texture()
            .present_vsync()
            .build().unwrap();

        let mut my_sys = MySys{
            sdl_context: context,
            canvas: canv,
        };

        println!("Using SDL_Renderer \"{}\"", my_sys.canvas.info().name);

        my_sys.canvas.set_draw_color(color::get(color::Type::White));
        // clears the my_sys.canvas with the color we set in `set_draw_color`.
        my_sys.canvas.clear();
        // However the my_sys.canvas has not been updated to the window yet, everything has been processed to
        // an internal buffer, but if we want our buffer to be displayed on the window, we need to call
        // `present`. We need to call this everytime we want to render a new frame on the window.
        my_sys.canvas.present();

        my_sys
    }

    fn clear(&mut self) {
        self.canvas.set_draw_color(Color::RGB(255, 255, 255));
        self.canvas.clear();
    }

    fn show(&mut self) {
        self.canvas.present();
    }
}

pub fn main() {
    let mut my_sys = MySys::new();

    let mut world = World::new();
    world.register::<component::Pos>();
    world.register::<component::BlockColor>();
    world.register::<component::KeyboardController>();
    world.register::<component::CameraFollower>();

    world
        .create_entity()
        .with(component::Pos(1, 4))
        .with(component::BlockColor{ id: color::Type::Black })
        .with(component::KeyboardController)
        .with(component::CameraFollower)
        .build();
    world
        .create_entity()
        .with(component::Pos(7, 4))
        .with(component::BlockColor{ id: color::Type::Green })
        .build();
    world
        .create_entity()
        .with(component::Pos(0, 0))
        .with(component::BlockColor{ id: color::Type::Brown })
        .build();
    world
        .create_entity()
        .with(component::Pos(1, 10))
        .with(component::BlockColor{ id: color::Type::Blue })
        .build();


    // struct DeltaTime(f32);
    // world.add_resource(DeltaTime(0.05));
    // let mut delta = world.write_resource::<DeltaTime>();

    world.add_resource(component::ColorPos{ vec: Vec::new() });
    world.add_resource(ArrowKeyEvent(Vec::new()));
    world.add_resource(component::CamPos(0, 0));

    let mut render_sys = RenderSystem;
    let mut keyboard_sys = KeyboardInputSystem;
    let mut cam_follow_sys = CameraFollowerSystem;

    // this struct manages textures. For lifetime reasons, the my_sys.canvas cannot directly create
    // textures, you have to create a `TextureCreator` instead.
    let texture_creator : TextureCreator<_> = my_sys.canvas.texture_creator();

    let mut game = ant_sim::AntGame::new();
    let mut event_pump = my_sys.sdl_context.event_pump().unwrap();

    let mut tex_vec = Vec::new();

    for n in 0..color::len() {
        tex_vec.push(texture_block(&mut my_sys.canvas, &texture_creator, color::enumify(n)));
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

        let color_pos = world.read_resource::<component::ColorPos>();
        let cam_pos = world.read_resource::<component::CamPos>();

        // rendering
        my_sys.clear();

        for (bc, pos) in color_pos.vec.clone() {
            let x_pos_unit = pos.0 - cam_pos.0;
            let y_pos_unit = pos.1 - cam_pos.1;

            let canvas_size = my_sys.canvas.output_size().unwrap();
            let x_pos_px = (x_pos_unit * SQUARE_SIZE as i32) + (canvas_size.0 as i32 / 2);
            let y_pos_px = (y_pos_unit * SQUARE_SIZE as i32) + (canvas_size.1 as i32 / 2);

            let rect = Rect::new(x_pos_px, y_pos_px, SQUARE_SIZE, SQUARE_SIZE);

            let _res = my_sys.canvas.copy(&tex_vec[color::indexify(bc.id)], None, rect);
        }

        bla = tex_vec as u32;

        my_sys.show();
    }
}
