'use strict';

let TILE_SIZE = 32

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
    this.a = split[0];
    this.b = split[1];
    this.c = split[2];
    this.sprite = square(TILE_SIZE, 0x008800);
  }
}

class Plant {
  constructor(pos_x, pos_y, vie_max, force, elegance, rayon_deplacement, rayon_collecte) {
    this.pos_x = pos_x;
    this.pos_y = pos_y;
    this.vie_max = vie_max;
    this.force = force;
    this.elegance = elegance;
    this.rayon_deplacement = rayon_deplacement;
    this.rayon_collecte = rayon_collecte;
    this.sprite = new PIXI.Sprite(PIXI.loader.resources["sprites/flower.gif"].texture);
    this.sprite.height = TILE_SIZE;
    this.sprite.width = TILE_SIZE;
    this.sprite.x = pos_x * TILE_SIZE;
    this.sprite.y = pos_y * TILE_SIZE;
  }

  static from_str(str) {
    let split = str.split(" ").map(x => parseInt(x));
    return new Plant(... split);
  }
}

class Map {
  constructor(file_content) {
    this.cells = [];
    this.p1_plants = [];
    this.p2_plants = [];
    this.sprite = square(TILE_SIZE * 20, 0x000000);
    this.load(file_content);
  }

  load(file_content) {
    let lines = file_content.split("\n");
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
    let p1_n_plants = parseInt(lines[line_index++]);
    for (let i = 0; i < p1_n_plants; i++) {
      let plant = Plant.from_str(lines[line_index++]);
      this.p1_plants.push(plant);
      this.sprite.addChild(plant.sprite);
    }
    let p2_n_plants = parseInt(lines[line_index++]);
    for (let i = 0; i < p2_n_plants; i++) {
      let plant = Plant.from_str(lines[line_index++]);
      this.p2_plants.push(plant);
      this.sprite.addChild(plant.sprite);
    }
  }
}




PIXI.loader.add('map', "test.map");
PIXI.loader.add("sprites/dog.gif");
PIXI.loader.add("sprites/flower.gif");
PIXI.loader.load(setup);

function setup(loader, resources) {

  let dog = new PIXI.Sprite(PIXI.loader.resources["sprites/dog.gif"].texture);
  dog.width = 64;
  dog.height = 64;


  let map_txt = resources['map'].data;
  let map = new Map(map_txt);
  app.stage.addChild(map.sprite);

  app.stage.addChild(dog);

  console.log(map);
}


