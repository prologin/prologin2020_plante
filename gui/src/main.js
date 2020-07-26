'use strict';

let app = new PIXI.Application({
    width: 512,
    height: 512,
    antialias: true,
    transparent: false,
    resolution: 1
  }
);


document.body.appendChild(app.view);

PIXI.loader
  .add("sprites/dog.gif")
  .load(setup);

function setup() {

  let dog = new PIXI.Sprite(PIXI.loader.resources["sprites/dog.gif"].texture);
  dog.width = 64;
  dog.height = 64;

  app.stage.addChild(dog);
}


class Cell {
  constructor(str) {
    let split = str.split(",").map(x => parseInt(x));
    this.a = split[0];
    this.b = split[1];
    this.c = split[2];
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
    this.load(file_content)
  }

  load(file_content) {
    let lines = file_content.split("\n");
    let line_index = 0;
    for (; line_index < 20; line_index++) {
      let row = [];
      let line_split = lines[line_index].split(" ")
      for (const i in line_split) {
        row.push(new Cell(line_split[i]));
      }
      this.cells.push(row);
    }
    let p1_n_plants = parseInt(lines[line_index++]);
    for (let i = 0; i < p1_n_plants; i++) {
      this.p1_plants.push(Plant.from_str(lines[line_index++]))
    }
    let p2_n_plants = parseInt(lines[line_index++]);
    for (let i = 0; i < p2_n_plants; i++) {
      this.p2_plants.push(Plant.from_str(lines[line_index++]))
    }
  }
}


let loader = new PIXI.loaders.Loader();
loader.add('map', "test.map");
loader.load(function(loader, resources) {
  let map_txt = resources['map'].data;
  let map = new Map(map_txt)
  console.log(map)
});

