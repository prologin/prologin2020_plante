'use strict';

let TILE_SIZE = 75;
let WIDTH = TILE_SIZE * 20 + 400;
let HEIGHT = TILE_SIZE * 20;

function rect(width, height, color) {
  let res = new PIXI.Graphics();
  // res.lineStyle(1, 0xFFFFFF, 1);
  res.beginFill(color);
  res.drawRect(0, 0, width, height);
  res.endFill();
  return res;
}

function stats_box(title, infos, width) {
    let height = 60 + 40 * infos.length;
    let back = rect(width, height, 0x3D0E0E);

    let title_w = new PIXI.Text(
        title,
        {
            font: "Arial",
            fontSize: 32,
            fill: "white"
        }
    );

    title_w.x = width / 2;
    title_w.y = 30;
    title_w.anchor.set(0.5);
    back.addChild(title_w);

    for (let i = 0 ; i < infos.length ; i++) {
        let line = infos[i];

        let text_back = rect(width - 4, 38);
        text_back.x = 2;
        text_back.y = 62 + i * 40;
        back.addChild(text_back);

        let key = new PIXI.Text(
            line[0],
            {
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
            line[1],
            {
                font: "Arial",
                fontSize: 24,
                fill: "white"
            }
        );
        val.x = text_back.width - 10 - val.width / 2;
        val.y = 20;
        val.anchor.set(0.5);
        text_back.addChild(val);
    }

    return back;
}

function square(size, color) {
  return rect(size, size, color);
}

let app, context;

class Context {
    constructor(mode, container, turnSlider, map) {
        this.mode = mode;
        this.container = container;
        this.turnSlider = turnSlider;
        this.map = map;
    }
}

class Cell {
  constructor(str) {
    let split = str.split(",").map(x => parseInt(x));
    this.heat = split[0];
    this.light = split[1];
    this.water = split[2];
    if (this.heat === 0 && this.light === 0 && this.water === 0)
      var index = 5;
    else if (this.heat > 0 && this.water === 0)
      var index = 1;
    else if (this.heat > 0 && this.water !== 0)
      var index = 2;
    else if (this.heat === 0 && this.water !== 0)
      var index = 6;
    else if (this.water !== 0)
      var index = 4;
    else
      var index = 3;
    this.sprite = new PIXI.Sprite(PIXI.loader.resources["frame" + index].texture);
    this.sprite.width = TILE_SIZE;
    this.sprite.height = TILE_SIZE;
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
        this.elegance = plant.élégance;
        this.rayon_deplacement = plant.rayon_déplacement;
        this.rayon_collecte = plant.rayon_collecte;
        this.adulte = plant.adulte;
        this.enracinee = plant.enracinée;
        this.consommation = plant.consommation;
        this.jardinier = jardinier;
        this.sprite = new PIXI.Sprite(PIXI.loader.resources["plant_" + (this.jardinier === 0 ? "a" : "b")].texture);
        this.sprite.height = TILE_SIZE;
        this.sprite.width = TILE_SIZE;
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
        if (context.mode === 'preview')
        {
            this.load(context.map);
        }
        else
        {
            this.load(dump[0].carte);
            this.update_plants(dump[0]);
        }
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

        this.cell_details = null;
        this.plant_details = null;
    }

    selected_plant() {
        const all_plants = this.p1_plants.concat(this.p2_plants);

        for (let p in all_plants)
            if (all_plants[p].pos_x === map.selected_x && all_plants[p].pos_y === map.selected_y)
                return all_plants[p];

        return null;
    }

    update_cell_details() {
        if (!this.cells[this.selected_y] || !this.cells[this.selected_y][this.selected_x])
            return;

        let cell = this.cells[this.selected_y][this.selected_x];

        if (this.cell_details != null)
            this.sprite.removeChild(this.cell_details);

        this.cell_details = stats_box(
            "Terrain",
            [
                ["position", "(" + this.selected_x + ", " + this.selected_y + ")"],
                ["chaleur", cell.heat],
                ["lumière", cell.light],
                ["humidité", cell.water]
            ],
            380
        );

        this.cell_details.x = 20 * TILE_SIZE + 10;
        this.cell_details.y = 90;
        this.sprite.addChild(this.cell_details);
    }

    update_plant_details() {
        if (!this.cells[this.selected_y] || !this.cells[this.selected_y][this.selected_x])
            return;

        if (this.plant_details != null)
            this.sprite.removeChild(this.plant_details);

        if (this.selected_plant() == null)
            return;

        const plant = this.selected_plant();

        this.plant_details = stats_box(
            "Plante",
            [
                ["jardinier", plant.jardinier],
                ["age", plant.age],
                ["vie", plant.vie],
                ["vie max", plant.vie_max],
                ["force", plant.force],
                ["elegance", plant.elegance],
                ["rayon de deplacement", plant.rayon_deplacement],
                ["rayon de collecte", plant.rayon_collecte],
                ["adulte", plant.adulte ? "oui" : "non"],
                ["enracinee", plant.enracinee ? "oui" : "non"],
                ["consommation chaleur", plant.consommation[0]],
                ["consommation lumière", plant.consommation[1]],
                ["consommation eau", plant.consommation[2]],
            ],
            380
        );

        this.plant_details.x = 20 * TILE_SIZE + 10;
        this.plant_details.y = this.cell_details.height + this.cell_details.y + 10;
        this.sprite.addChild(this.plant_details);
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
            this.sprite.addChild(plant_obj.sprite);
        });
        dump.joueurs[1].plantes.forEach(plant => {
            let plant_obj = new Plant(plant, 1);
            this.p2_plants.push(plant_obj);
            this.sprite.addChild(plant_obj.sprite);
        });
    }

    load(map) {
        let lines = map.split("\n");
        let line_index = 0;
        for (; line_index < 20; line_index++) {
            let row = [];
            let line_split = lines[line_index].split(" ");
            for (const i in line_split) {
                let cell = new Cell(line_split[i]);
                cell.sprite.x = i * TILE_SIZE;
                cell.sprite.y = line_index * TILE_SIZE;
                this.sprite.addChild(cell.sprite);
                row.push(cell);
            }
            this.cells.push(row);
        }
    }
}


function start_viewer(container, turnSlider) {
    context = new Context('replay', container, turnSlider, null);
    start()
}

function start_preview(container, map) {
    context = new Context('preview', container, null, map.text());
    start()
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

    if (context.mode !== 'preview')
        PIXI.loader.add("dump", "dump");

    PIXI.loader.add("dog_blue", "/static/img/sprites/dog_blue.png");
    PIXI.loader.add("plant_a", "/static/img/sprites/plant_a.png");
    PIXI.loader.add("plant_b", "/static/img/sprites/plant_b.png");
    for (var i = 1; i <= 6; ++i)
        PIXI.loader.add("frame" + i, "/static/img/sprites/frame" + i + ".png");
    PIXI.loader.load(setup);
}


let dump, map;

let lastTurn = 0;

function setup(loader, resources) {
    if (context.mode !== 'preview') {
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
    }

    app.stage.interactive = true;
    app.stage.on("pointerdown", clickHandler);
    window.addEventListener("keydown", keyHandler);

    if (context.mode !== 'preview')
        app.ticker.add(delta => gameLoop(delta));
}

let counter = 0;
function gameLoop(delta)
{
    counter += delta;
    const currentTurn = Math.floor(counter / 60);
    if (currentTurn !== lastTurn)
    {
        map.update_plants(dump[currentTurn]);
        map.title_text.text = "Tour " + currentTurn;
        map.update_cell_details();
        map.update_plant_details();
        context.turnSlider.val(currentTurn).trigger('change');
    }
    lastTurn = currentTurn;
}
