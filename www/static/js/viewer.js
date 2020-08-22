'use strict';

const TILE_SIZE = 75;
const WIDTH = TILE_SIZE * 20 + 400;
const HEIGHT = TILE_SIZE * 20;
const MAX_SPEED = 15;


const tile_map = {
    gravel: {
        ressources: [0, 0, 0],
        back: [29, 30, 31],
    },
    desert: {
        ressources: [1, 0, 0],
        back: [40, 41, 42, 43, 44, 45],
        in_border: [32, 34, 35, 36, 33, 37, 39, 38]
    },
    grass: {
        ressources: [0, 1, 0],
        back: [9, 10, 11, 12, 13, 14, 15, 16, 17],
        in_border: [1, 3, 4, 5, 2, 6, 8, 7]
    },
    ocean: {
        ressources: [0, 0, 1],
        back: [27, 28],
        out_border: [[19, 20, 21], 22, [84, 85, 86], 26, [24, 25], 23, [81, 82, 83]]
    },
    artic: {
        ressources: [0, 1, 1],
        back: [93, 94, 95, 96, 97, 98, 99],
        in_border: [107, 105, 104, 103, 106, 102, 100, 101]
    },
    lava: {
        ressources: [1, 1, 0],
        back: [62, 63],
        out_border: [57, 58, 92, 61, 60, 59, 91, 56]
    },
    oasis: {
        ressources: [1, 0, 1],
        back: [54, 55],
        out_border: [[47, 48], 49, [89, 90], 53, [51, 52], 50, [87, 88], 46]
    },
    jungle: {
        ressources: [1, 1, 1],
        back: [72, 73, 74, 75, 76, 77, 78, 79, 80],
        in_border: [64, 66, 67, 68, 65, 69, 71, 70]
    }
};

// ---
// --- Utilities
// ---

function norm(vec) {
    return vec.reduce((x, y) => x + y, 0);
}

function normalize(vec) {
    const div = norm(vec);
    return vec.map(x => x / div);
}

function dist(vec1, vec2) {
    console.assert(vec1.length === vec2.length);
    let dist = 0;

    for (let k = 0 ; k < vec1.length ; k++)
        dist += Math.abs(vec1[k] - vec2[k]);

    return dist;
}

// ---
// --- Animation
// ---

// Frames used to animate an actions is (1 + MAX_SPEED - speed)
let speed = 10;

function animation_duration() {
    return 1 + MAX_SPEED - speed;
}

function tile_kind(tile_ressources) {
    if (tile_ressources.reduce((a, b) => a + b, 0) == 0)
        return 'gravel';

    tile_ressources = normalize(tile_ressources);
    let best = null;
    let best_dist = Infinity;

    for (const [name, meta] of Object.entries(tile_map))
    {
        const new_dist = dist(tile_ressources, normalize(meta.ressources));

        if (new_dist < best_dist)
        {
            best = name;
            best_dist = new_dist;
        }
    }

    return best;
}

function rect(width, height, color) {
  let res = new PIXI.Graphics();
  res.beginFill(color);
  res.drawRect(0, 0, width, height);
  res.endFill();
  return res;
}

class StatsBox extends PIXI.Container {
    constructor(title, fields, y_offset) {
        super();
        this.visible = false;

        this.WIDTH = 380;
        this.HEIGHT = 60 + 40 * fields.length;
        this.x = 20 * TILE_SIZE + 10;
        this.y = y_offset;

        this.addChild(rect(this.WIDTH, this.HEIGHT, 0x1b1b43));

        let title_w = new PIXI.Text(
            title,
            {
               font: "Arial",
                fontSize: 32,
                fill: "white"
            }
        );

        title_w.x = this.WIDTH / 2;
        title_w.y = 30;
        title_w.anchor.set(0.5);
        this.addChild(title_w);

        this.lines = {};

        for (let i in fields) {
            let line = fields[i];
            let text_back = rect(this.WIDTH - 4, 38, 0x111111);
            text_back.x = 2;
            text_back.y = 62 + i * 40;
            this.addChild(text_back);

            let key = new PIXI.Text(
                line, {
                    font: "Arial",
                    fontSize: 24,
                    fontWeight: "bold",
                    fill: "white"
                }
            );
            key.y = 20;
            key.x = 10 + key.width / 2;
            key.anchor.set(0.5);
            text_back.addChild(key);

            let val = new PIXI.Text(
                "", {
                    font: "Arial",
                    fontSize: 24,
                    fill: "white"
                }
            );
            val.y = 20;
            val.anchor.set(0.5);
            text_back.addChild(val);

            this.lines[line] = val;
        }
    }

    update(values) {
        for (let [key, val] of Object.entries(values)) {
            this.lines[key].text = val;
            this.lines[key].x = this.WIDTH - this.lines[key].width / 2 - 10;
        }
    }
}

function square(size, color) {
  return rect(size, size, color);
}

let app, context;

class Context {
    constructor(mode, container, turnSlider, map, is_local) {
        this.mode = mode;
        this.container = container;
        this.turnSlider = turnSlider;
        this.map = map;
        this.is_local = is_local;
    }
}


let random = 0;

class Cell {
  constructor(str) {
    const ressources = str.split(",").map(x => parseInt(x));
    this.heat = ressources[0];
    this.light = ressources[1];
    this.water = ressources[2];


    const kind = tile_kind(ressources);
    let frame_id = tile_map[kind].back;

    if (typeof frame_id === "object")
    {
        random = (9661 + 9677 * random) % 11587;
        frame_id = frame_id[random % frame_id.length];
    }

    this.sprite = new PIXI.Sprite(PIXI.loader.resources[`frame_${frame_id}`].texture);
    this.sprite.width = TILE_SIZE;
    this.sprite.height = TILE_SIZE;
  }
}

class Dog {
    constructor(pos) {
        this.color = 0;
        this.frame = 0;
        this.sprite = new PIXI.Container();
        this.sprite.x = pos.x * TILE_SIZE;
        this.sprite.y = pos.y * TILE_SIZE;
        this.sprite.height = TILE_SIZE;
        this.sprite.width = TILE_SIZE;
    }

    update_sprite() {
        this.frame = (this.frame + 1) % 6;
        this.sprite.removeChildren();

        if (this.color != 0) {
            this.sprite.addChild(
                new PIXI.Sprite(PIXI.loader.resources[`dog_${this.frame}`].texture)
            );

            this.sprite.addChild(
                new PIXI.Sprite(PIXI.loader.resources[`colar_${this.color}`].texture)
            );
        }
    }
}

class Plant {
    constructor(plant, jardinier) {
        this.pos_x = plant.pos.x;
        this.pos_y = plant.pos.y;
        this.age = plant.age;
        this.vie = plant.vie;
        this.vie_max = plant.vie_max;
        this.force = plant.force;
        this.elegance = plant.elegance;
        this.rayon_deplacement = plant.rayon_deplacement;
        this.rayon_collecte = plant.rayon_collecte;
        this.adulte = plant.adulte;
        this.enracinee = plant.enracinee;
        this.consommation = plant.consommation;
        this.jardinier = jardinier;
        this.sprite = new PIXI.Container();
        this.sprite.height = TILE_SIZE;
        this.sprite.width = TILE_SIZE;
        this.update_sprite();
    }

    clone() {
        let clone = Object.assign({}, this);
        Object.setPrototypeOf(clone, Plant.prototype);
        clone.sprite = new PIXI.Container();
        clone.update_sprite();
        return clone;
    }

    update_sprite() {
        this.sprite.removeChildren();

        const face_id = Math.round(4 * this.vie / this.vie_max);
        this.sprite.addChild(new PIXI.Sprite(PIXI.loader.resources["face_" + face_id].texture));

        const hat_id = Math.min(2, Math.round(this.elegance / 50));
        const hat_texture = "hat_" + this.jardinier + "_" + hat_id;
        this.sprite.addChild(new PIXI.Sprite(PIXI.loader.resources[hat_texture].texture));

        const body_id = Math.min(2, Math.round(this.force / 50));
        const body_texture = "body_" + body_id;
        this.sprite.addChild(new PIXI.Sprite(PIXI.loader.resources[body_texture].texture));

        if (!this.enracinee)
            this.sprite.addChild(new PIXI.Sprite(PIXI.loader.resources["pot"].texture));

        this.sprite.x = this.pos_x * TILE_SIZE;
        this.sprite.y = this.pos_y * TILE_SIZE;
    }
}

class Map {
    constructor(context) {
        this.cells = [];
        this.p1_plants = [];
        this.p2_plants = [];
        this.sprite = square(TILE_SIZE * 20, 0x000000);
        this.grid = new PIXI.Container();
        this.sprite.addChild(this.grid);
        this.dogs = Array(20).fill(null).map(_ => new Array(20));

        for (let x = 0 ; x < 20 ; x++) {
            for (let y = 0 ; y < 20 ; y++) {
                this.dogs[x][y] = new Dog({x, y});
                this.sprite.addChild(this.dogs[x][y].sprite);
            }
        }

        if (context.mode === 'preview') {
            this.load(context.map);
        }
        else {
            this.load(dump[0].carte);
            this.update_plants(dump[0]);
        }

        this.seed = new PIXI.Sprite(PIXI.loader.resources["graine"].texture);
        this.seed.visible = false;
        this.sprite.addChild(this.seed);

        this.selected_x = undefined;
        this.selected_y = undefined;
        this.select_square = square(TILE_SIZE, 0xAA0000);
        this.select_square.visible = false;
        this.select_square.alpha = 0.2;
        this.sprite.addChild(this.select_square);

        this.title_area = rect(400, 80, 0x111111);
        this.title_area.x = 20 * TILE_SIZE;
        this.sprite.addChild(this.title_area);

        this.title_text = new PIXI.Text(
            "Tour 0",
            {
                font: "Arial",
                fontSize: 40,
                fill: "white",
                fontWeight: "bold"
            }
        );

        this.title_text.x = this.title_area.width / 2;
        this.title_text.y = this.title_area.height / 2;
        this.title_text.anchor.set(0.5);
        this.title_area.addChild(this.title_text);

        this.details_area = rect(800, HEIGHT - 80, 0x222222);
        this.details_area.x = 20 * TILE_SIZE;
        this.details_area.y = 80;
        this.sprite.addChild(this.details_area);

        this.cell_details = new StatsBox(
            "Terrain",
            ["position", "chaleur", "lumiere", "humidite"],
            160,
        );
        this.sprite.addChild(this.cell_details);

        this.plant_details = new StatsBox(
            "Plante",
            ["jardinier", "age", "vie", "vie max", "force", "elegance",
                "rayon de deplacement", "rayon de collecte", "adulte",
                "enracinee", "consommation de chaleur",
                "consommation de lumiere", "consommation d'eau"],
            this.cell_details.y + this.cell_details.height + 10,
        );
        this.sprite.addChild(this.plant_details);

        this.plant_portrait = rect(128, 128, 0x111111);
        this.plant_portrait.x = 20 * TILE_SIZE + 136;
        this.plant_portrait.y = 1150;
        this.plant_portrait.visible = false;
        this.sprite.addChild(this.plant_portrait);

        this.speed_info = new PIXI.Text(
            "", {
                font: "Arial",
                fontsize: 16,
                fill: "white",
            }
        )
        this.speed_info.x = 20 * TILE_SIZE + 200;
        this.speed_info.y = 20 * TILE_SIZE - 40;
        this.speed_info.anchor.set(0.5);
        this.update_speed_info();
        this.sprite.addChild(this.speed_info);

        let score_frames = [
            rect(185, 60, 0x9B870C),
            rect(185, 60, 0xa00f17),
        ];

        score_frames[0].x = 20 * TILE_SIZE + 10;
        score_frames[1].x = 20 * TILE_SIZE + 185 + 20;

        for (let i of [0, 1]) {
            score_frames[i].y = 90;
            this.sprite.addChild(score_frames[i]);
        }

        this.scores = score_frames.map(frame => {
            let score = new PIXI.Text(
                "0", {
                    font: "Arial",
                    fontsize: 16,
                    fill: "white",
                }
            );

            score.anchor.set(0.5);
            score.x = frame.x + frame.width / 2;
            score.y = frame.y + frame.height / 2;
            this.sprite.addChild(score);
            return score;
        });
    }

    clear_debug_chiens() {
        for (let x = 0 ; x < 20 ; x++)
            for (let y = 0 ; y < 20 ; y++)
                this.dogs[x][y].color = 0;
    }

    update_speed_info() {
        this.speed_info.text = "vitesse: " + (60 / animation_duration()).toFixed(1) + " action/s (+/-)";
    }

    update_scores(scores) {
        for (let i of [0, 1])
            this.scores[i].text = scores[i];
    }

    // TODO: implement this with a decent time complexity
    plant_at(x, y) {
        const all_plants = this.p1_plants.concat(this.p2_plants);

        for (let p in all_plants)
            if (all_plants[p].pos_x === x && all_plants[p].pos_y === y)
                return all_plants[p];

        return null;
    }

    selected_plant() {
        return this.plant_at(map.selected_x, map.selected_y);
    }

    del_plant(x, y) {
        const plant = this.plant_at(x, y);
        this.grid.removeChild(plant);
        this.p1_plants = this.p1_plants.filter(pp => pp.pos_x != x || pp.pos_y != y);
        this.p2_plants = this.p2_plants.filter(pp => pp.pos_x != x || pp.pos_y != y);
    }

    update_cell_details() {
        const cell = this.cells[this.selected_y][this.selected_x];
        this.cell_details.visible = true;
        this.cell_details.update({
            position: "(" + this.selected_x + ", " + this.selected_y + ")",
            chaleur: cell.heat,
            lumiere: cell.light,
            humidite: cell.water,
        });
    }

    update_plant_details() {
        this.plant_details.visible = this.selected_plant() !== null;
        this.plant_portrait.visible = this.plant_details.visible;

        if (!this.plant_details.visible)
            return;

        const plant = this.selected_plant();
        this.plant_details.update({
            "jardinier": plant.jardinier,
            "age": plant.age,
            "vie": plant.vie,
            "vie max": plant.vie_max,
            "force": plant.force,
            "elegance": plant.elegance,
            "rayon de deplacement": plant.rayon_deplacement,
            "rayon de collecte": plant.rayon_collecte,
            "adulte": plant.adulte ? "oui" : "non",
            "enracinee": plant.enracinee ? "oui" : "non",
            "consommation de chaleur": plant.consommation[0],
            "consommation de lumiere": plant.consommation[1],
            "consommation d'eau": plant.consommation[2],
        });

        let clone = plant.clone();
        clone.sprite.x = clone.sprite.y = 0;
        clone.sprite.width = 128;
        clone.sprite.height = 128;
        this.plant_portrait.removeChildren();
        this.plant_portrait.addChild(clone.sprite);
    }

    update_animations() {
        for (let x = 0 ; x < 20 ; x++)
            for (let y = 0 ; y < 20 ; y++)
                this.dogs[x][y].update_sprite();
    }

    select_tile(x, y) {
        if (x < 20 && y < 20 && x >= 0 && y >= 0) {
            this.selected_x = x;
            this.selected_y = y;
            this.select_square.visible = true;
            this.select_square.x = TILE_SIZE * x;
            this.select_square.y = TILE_SIZE * y;
            this.update_cell_details();
            this.update_plant_details();

        } else {
            this.selected_x = undefined;
            this.selected_y = undefined;
            this.select_square.visible = false;
        }
    }

    update_plants(dump) {
        this.p1_plants.forEach(plant => {
            plant.sprite.visible = false;
            app.stage.removeChild(plant.sprite);
        });
        this.p2_plants.forEach(plant => {
            plant.sprite.visible = false;
            app.stage.removeChild(plant.sprite);
        });

        this.p1_plants = [];
        this.p2_plants = [];
        dump.joueurs[0].plantes.forEach(plant => {
            let plant_obj = new Plant(plant, 0);
            this.p1_plants.push(plant_obj);
            this.grid.addChild(plant_obj.sprite);
        });
        dump.joueurs[1].plantes.forEach(plant => {
            let plant_obj = new Plant(plant, 1);
            this.p2_plants.push(plant_obj);
            this.grid.addChild(plant_obj.sprite);
        });
    }

    load(map) {
        let lines = map.split("\n");

        for (let line_index = 0; line_index < 20; line_index++) {
            let row = [];
            let line_split = lines[line_index].split(" ");
            for (const i in line_split) {
                let cell = new Cell(line_split[i]);
                cell.sprite.x = i * TILE_SIZE;
                cell.sprite.y = line_index * TILE_SIZE;
                this.grid.addChild(cell.sprite);
                row.push(cell);
            }
            this.cells.push(row);
        }
    }
}


function start_viewer(container, turnSlider, is_local) {
    context = new Context('replay', container, turnSlider, null, is_local);
    start()
}

function start_preview(container, map) {
    context = new Context('preview', container, null, map.text(), false);
    start()
}

var socket;
function connect_socket()
{
    socket = new WebSocket('ws://' + window.location.host);
    var backoff = 1;
    socket.onopen = () => {
        backoff = 1;
        console.log('connected to server');
        send(socket, 'hello');
    };
    socket.onerror = (e) => {
        console.warn(e);
        // check error code, don't reconnect if not needed
        backoff = Math.min(4, backoff + 1);
        setTimeout(connect_socket, 1000 * Math.pow(1.5, backoff));
    };
    socket.onmessage = (msg) => {
        msg = JSON.parse(msg.data);
        console.log('ws >', msg);
        if (msg.c === 'whatsup') {
            // TODO
        } else if (msg.c === 'end') {
            // TODO: show game end
        } else if (msg.c === 'turn') {
            dump = msg.state;
        }
    };
}

function send(socket, cmd, data) {
    let msg = $.extend({c: cmd}, data);
    console.log('ws <', msg);
    socket.send(JSON.stringify(msg));
}


function start() {
    app = new PIXI.Application({
        width: WIDTH,
        height: HEIGHT,
        antialias: true,
        transparent: true,
        resolution: 1
      }
    );
    context.container.append(app.view);

    if (context.mode !== 'preview' && !context.is_local)
        PIXI.loader.add("dump", "dump");
    if (context.is_local)
        connect_socket();

    PIXI.loader.add("graine", "/static/img/sprites/flowey/graine.png");
    PIXI.loader.add("pot", "/static/img/sprites/flowey/pot.png");

    // Read tiles
    for (let i = 1 ; i <= 107 ; i++) {
        const filename = ("" + i).padStart(2, "0");
        PIXI.loader.add(`frame_${i}`, `/static/img/sprites/bg/${filename}.png`);
    }

    // Read dog parts
    for (let i = 0 ; i < 6 ; i++)
        PIXI.loader.add(`dog_${i}`, `/static/img/sprites/dog/frames/${i}.png`);

    for (let i = 1 ; i <= 3 ; i++)
        PIXI.loader.add(`colar_${i}`, `/static/img/sprites/dog/colar/${i}.png`);

    // Read flower parts
    for (let i = 0 ; i <= 4 ; i++)
        PIXI.loader.add(`face_${i}`, `/static/img/sprites/flowey/face_${i}.png`);

    for (let player of [0, 1])
        for (let i = 0 ; i <= 2 ; i++)
            PIXI.loader.add(
                `hat_${player}_${i}`,
                `/static/img/sprites/flowey/fleur_${player + 1}_${i + 1}.png`
            );

    for (let i = 0 ; i <= 2 ; i++)
        PIXI.loader.add(
            `body_${i}`,
            `/static/img/sprites/flowey/pied_${i + 1}.png`
        );

    PIXI.loader.load(setup);
}


let dump, map;

let lastTurn = 0;

function setup(loader, resources) {
    if (context.mode !== 'preview' && !context.is_local) {
        let json_str = "{\"dump\": [" + resources["dump"].data;
        json_str = json_str.substring(0, json_str.length - 2) + "]}";
        dump = JSON.parse(json_str).dump;
    }

    map = new Map(context);
    app.stage.addChild(map.sprite);

    let $canvas = $("#replay canvas");

    function clickHandler(e) {
        let mouse_x = app.renderer.plugins.interaction.mouse.global.x;
        let mouse_y = app.renderer.plugins.interaction.mouse.global.y;
        let index_x = Math.floor(mouse_x / TILE_SIZE);
        let index_y = Math.floor(mouse_y / TILE_SIZE);
        map.select_tile(index_x, index_y);
    }

    function keyHandler(e) {
        if (e.key == "f") {
            if (document.fullscreen) {
                document.exitFullscreen();
                $("#replay_view canvas").css('width', '100%');
            }
            else {
                $("#replay_view")[0].requestFullscreen();
                $("#replay_view canvas").css('width', 'auto');
            }
        }
        else if (e.key == "+") {
            if (speed < MAX_SPEED) {
                speed += 1;
                map.update_speed_info();
            }
        }
        else if (e.key == "-") {
            if (speed > 0) {
                speed -= 1;
                map.update_speed_info();
            }
        }
    }

    app.stage.interactive = true;
    app.stage.on("pointerdown", clickHandler);
    window.addEventListener("keydown", keyHandler);

    if (context.mode !== 'preview')
        app.ticker.add(delta => gameLoop(delta));
}

function baffer(start, end, frame) {
    if (frame == 0)
        map.seed.visible = true;

    if (frame + 1 == animation_duration())
        map.seed.visible = false;

    const parabola = 4 * TILE_SIZE * Math.min(frame, animation_duration() - frame - 1) / animation_duration();
    map.seed.x = ((1 + frame) * (end.x - start.x) / animation_duration() + start.x) * TILE_SIZE;
    map.seed.y = ((1 + frame) * (end.y - start.y) / animation_duration() + start.y) * TILE_SIZE - parabola;
}

function death(pos, frame) {
    let plant = map.plant_at(pos.x, pos.y);

    if (plant == null)
        console.error("couldn't find plant at", pos);

    plant.sprite.alpha = 1 - (1 + frame) / animation_duration();

    if (frame + 1 == animation_duration())
        map.del_plant(pos.x, pos.y);
}

function depoter(start, end, frame) {
    let plant = map.plant_at(start.x, start.y);

    if (plant == null)
        console.error("couldn't find plant at", start);

    if (frame === 0) {
        plant.enracinee = true;
        plant.update_sprite();
    }

    plant.sprite.x = ((1 + frame) * (end.x - start.x) / animation_duration() + start.x) * TILE_SIZE;
    plant.sprite.y = ((1 + frame) * (end.y - start.y) / animation_duration() + start.y) * TILE_SIZE;

    if (frame + 1 == animation_duration()) {
        plant.pos_x = end.x;
        plant.pos_y = end.y;
    }
}

function afficher_debug_chien(pos, color, frame) {
    map.dogs[pos.x][pos.y].color = color;
}

var turn = 0;
var player = 0;
var action = 0;
var frame = 0;

function gameLoop(delta)
{
    while (action >= dump[turn]["joueurs"][player]["historique"].length) {
        if (turn > 0) {
            map.update_plants(dump[turn-1]); // sync previous turn
            map.update_scores(dump[turn-1].joueurs.map(p => p.score));
        }

        action = 0;
        player = 1 - player;
        turn += 1;

        map.clear_debug_chiens();
        map.title_text.text = "Tour " + turn / 2;
        context.turnSlider.val(turn).trigger('change');
    }

    const history = dump[turn]["joueurs"][player]["historique"];
    const curr_action = history[action];

    if (curr_action["action_type"] == "depoter")
        depoter(curr_action["position_depart"], curr_action["position_arrivee_"], frame);
    else if (curr_action["action_type"] == "death")
        death(curr_action["position"], frame);
    else if (curr_action["action_type"] == "baffer")
        baffer(curr_action["position_baffante"], curr_action["position_baffee"], frame);
    else if (curr_action["action_type"] == "afficher_debug_chien") {
        afficher_debug_chien(
            curr_action.position,
            curr_action.debug_chien,
            frame
        );

        // Actually update all consecutive debug dogs at once
        for (; action + 1 < history.length && history[action + 1].action_type == "afficher_debug_chien" ; action++)
            afficher_debug_chien(
                history[action + 1].position,
                history[action + 1].debug_chien,
                frame
            );
    }
    else
        console.warn("unsupported action:", curr_action["action_type"]);

    map.update_animations();

    if (map.selected_x && map.selected_y) {
        map.update_cell_details();
        map.update_plant_details();
    }

    frame += 1;

    if (frame >= animation_duration()) {
        frame = 0;
        action += 1;
    }
}
