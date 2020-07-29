'use strict';

let TILE_SIZE = 75

let app = new PIXI.Application({
    width: TILE_SIZE * 20,
    height: TILE_SIZE * 20,
    antialias: true,
    transparent: false,
    resolution: 1
  }
);
document.body.appendChild(app.view);

function square(size, color) {
  let res = new PIXI.Graphics();
  res.lineStyle(1, 0xFFFFFF, 1);
  res.beginFill(color);
  res.drawRect(0, 0, size, size);
  res.endFill();
  return res;
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
    this.sprite = new PIXI.Sprite(PIXI.loader.resources["sprites/frame" + index + ".png"].texture);
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
    this.sprite = new PIXI.Sprite(PIXI.loader.resources["sprites/plant_" + (this.jardinier == 0 ? "a" : "b") + ".png"].texture);
    this.sprite.height = TILE_SIZE;
    this.sprite.width = TILE_SIZE;
    this.sprite.x = this.pos_x * TILE_SIZE;
    this.sprite.y = this.pos_y * TILE_SIZE;
  }
}

class Map {
  constructor(dump) {
    this.cells = [];
    this.p1_plants = [];
    this.p2_plants = [];
    this.sprite = square(TILE_SIZE * 20, 0x000000);
    this.load(dump);
    this.update_plants(dump);
    this.selected_x = undefined;
    this.selected_y = undefined;
    this.select_square = square(TILE_SIZE, 0xAA0000);
    this.select_square.visible = false;
    this.select_square.alpha = 0.2;
    this.sprite.addChild(this.select_square);
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

  load(dump) {
    let lines = dump.carte.split("\n");
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


PIXI.loader.add("dump", "dump.txt");
PIXI.loader.add("sprites/dog_blue.png");
PIXI.loader.add("sprites/plant_a.png");
PIXI.loader.add("sprites/plant_b.png");
for (var i = 1; i <= 6; ++i)
    PIXI.loader.add("sprites/frame" + i + ".png");
PIXI.loader.load(setup);


let dog, dump, map, turnText;

let lastTurn = 0;

function setup(loader, resources) {

  dog = new PIXI.Sprite(PIXI.loader.resources["sprites/dog_blue.png"].texture);
  dog.width = 75;
  dog.height = 75;


  var json_str = "{\"dump\": [" + resources["dump"].data; 
  json_str = json_str.substring(0, json_str.length - 2) + "]}";
  dump = JSON.parse(json_str).dump;
  map = new Map(dump[0]);
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
  }
  lastTurn = currentTurn;
}


