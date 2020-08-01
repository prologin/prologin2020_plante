'use strict';

let TILE_SIZE = 75

function rect(width, height, color) {
  let res = new PIXI.Graphics();
  res.lineStyle(1, 0xFFFFFF, 1);
  res.beginFill(color);
  res.drawRect(0, 0, width, height);
  res.endFill();
  return res;
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
    if (this.heat == 0 && this.light == 0 && this.water == 0)
      var index = 5;
    else if (this.heat > 0 && this.water == 0)
      var index = 1;
    else if (this.heat > 0 && this.water != 0)
      var index = 2;
    else if (this.heat == 0 && this.water != 0)
      var index = 6;
    else if (this.water != 0)
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
    this.vie_max = plant.vie_max;
    this.force = plant.force;
    this.elegance = plant.élégance;
    this.rayon_deplacement = plant.rayon_déplacement;
    this.rayon_collecte = plant.rayon_collecte;
    this.jardinier = jardinier;
    this.sprite = new PIXI.Sprite(PIXI.loader.resources["plant_" + (this.jardinier == 0 ? "a" : "b")].texture);
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
    if (context.mode == 'preview')
    {
      this.load(context.map);
    }
    else
    {
      this.load(dump[0].carte);
      this.update_plants(dump);
    }
    this.selected_x = undefined;
    this.selected_y = undefined;
    this.select_square = square(TILE_SIZE, 0xAA0000);
    this.select_square.visible = false;
    this.select_square.alpha = 0.2;
    this.sprite.addChild(this.select_square);

    this.details_area = rect(400, 500, 0x222222);
    this.details_area.x = 20 * TILE_SIZE;
    this.sprite.addChild(this.details_area);

    this.details_text = new PIXI.Text("", {font:"50px Arial", fill:"white"});
    this.details_text.x = this.details_area.width / 2;
    this.details_text.y = this.details_area.height / 2;
    this.details_text.anchor.set(0.5);
    this.details_area.addChild(this.details_text);

  }

  select_tile(x, y) {
    if (x < 20 && y < 20 && x >= 0 && y >= 0) {
      this.selected_x = x;
      this.selected_y = y;
      this.select_square.visible = true;
      this.select_square.x = TILE_SIZE * x;
      this.select_square.y = TILE_SIZE * y;
    } else {
      this.selected_x = undefined;
      this.selected_y = undefined;
      this.select_square.visible = false;
    }
  }

  render() {
    let txt = "";
    if (this.selected_x != undefined) {
      let cell = this.cells[this.selected_y][this.selected_x];
      txt += "case:\n";
      txt += `\tchaleur: ${cell.heat}\n`;
      txt += `\tlumière: ${cell.light}\n`;
      txt += `\teau: ${cell.water}\n`;

      function add_plant_details(p) {
        if (p.pos_x == map.selected_x && p.pos_y == map.selected_y) {
          txt += "\n\nplante:\n";
          txt += `\tvie: ${p.vie_max}\n`;
          txt += `\tforce: ${p.force}\n`;
          txt += `\telegance: ${p.elegance}\n`;
          txt += `\trayon déplacement: ${p.rayon_deplacement}\n`;
          txt += `\trayon collecte: ${p.rayon_collecte}\n`;
          txt += `\tjardinier: ${dump[0].joueurs[p.jardinier].name}\n`;
        }
      }

      for (var p in this.p1_plants) {
        add_plant_details(this.p1_plants[p]);
      }
      for (var p in this.p2_plants) {
        add_plant_details(this.p2_plants[p]);
      }
    }
    this.details_text.text = txt;
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

    PIXI.loader.add("dump", "dump");
    var json_str = "{\"dump\": [" + resources["dump"].data; 
    json_str = json_str.substring(0, json_str.length - 2) + "]}";
    dump = JSON.parse(json_str).dump;

    start()
}

function start_preview(container, map) {
    context = new Context('preview', container, null, map.text());
    start()
}

function start() {
    app = new PIXI.Application({
        width: TILE_SIZE * 20 + 400,
        height: TILE_SIZE * 20,
        antialias: true,
        transparent: true,
        resolution: 1
      }
    );
    context.container.append(app.view);

    PIXI.loader.add("dog_blue", "/static/img/sprites/dog_blue.png");
    PIXI.loader.add("plant_a", "/static/img/sprites/plant_a.png");
    PIXI.loader.add("plant_b", "/static/img/sprites/plant_b.png");
    for (var i = 1; i <= 6; ++i)
        PIXI.loader.add("frame" + i, "/static/img/sprites/frame" + i + ".png");
    PIXI.loader.load(setup);
}


let dog, dump, map, turnText;

let lastTurn = 0;

function setup(loader, resources) {

  dog = new PIXI.Sprite(PIXI.loader.resources["dog_blue"].texture);
  dog.width = 75;
  dog.height = 75;


  map = new Map(context);
  app.stage.addChild(map.sprite);

  app.stage.addChild(dog);

  turnText = new PIXI.Text("turn = " + lastTurn, {font:"50px Arial", fill:"red"});
  app.stage.addChild(turnText);

  var clickHandler = function(e){
    let mouse_x = app.renderer.plugins.interaction.mouse.global.x;
    let mouse_y = app.renderer.plugins.interaction.mouse.global.y;
    let index_x = Math.floor(mouse_x / TILE_SIZE);
    let index_y = Math.floor(mouse_y / TILE_SIZE);
    map.select_tile(index_x, index_y);
  }
  app.stage.interactive = true;
  app.stage.on("pointerdown", clickHandler);

  app.ticker.add(delta => gameLoop(delta));
}

let counter = 0;
function gameLoop(delta)
{
  counter += delta;
  dog.x += 1;
  const currentTurn = Math.floor(counter / 60);
  if (currentTurn != lastTurn)
  {
      console.log('currentTurn = ', currentTurn);
      map.update_plants(dump[currentTurn]);
      turnText.text = "turn = " + currentTurn;
      context.turnSlider.val(currentTurn).trigger('change');
  }
  lastTurn = currentTurn;
  map.render();
}
