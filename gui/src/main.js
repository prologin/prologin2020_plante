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
    this.a = split[0];
    this.b = split[1];
    this.c = split[2];
    this.sprite = square(TILE_SIZE, 0x008800);
  }
}

class Plant {
  constructor(plant) {
    console.log('plant', plant);
    this.pos_x = plant.pos.x;
    this.pos_y = plant.pos.y;
    this.vie_max = plant.vie_max;
    this.force = plant.force;
    this.elegance = plant.élégance;
    this.rayon_deplacement = plant.rayon_déplacement;
    this.rayon_collecte = plant.rayon_collecte;
    this.sprite = new PIXI.Sprite(PIXI.loader.resources["sprites/plant_y.png"].texture);
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
    dump.joueurs[0].plantes.forEach(plant => {
      let plant_obj = new Plant(plant);
      this.p1_plants.push(plant_obj);
      this.sprite.addChild(plant_obj.sprite);
    });
    dump.joueurs[1].plantes.forEach(plant => {
      let plant_obj = new Plant(plant);
      this.p2_plants.push(plant_obj);
      this.sprite.addChild(plant_obj.sprite);
    });
  }
}




PIXI.loader.add("dump", "dump.json");
PIXI.loader.add("sprites/dog_blue.png");
PIXI.loader.add("sprites/plant_y.png");
PIXI.loader.load(setup);

function setup(loader, resources) {

  let dog = new PIXI.Sprite(PIXI.loader.resources["sprites/dog_blue.png"].texture);
  dog.width = 75;
  dog.height = 75;


  console.log(resources["dump"].data);
  let map = new Map(resources["dump"].data);
  app.stage.addChild(map.sprite);

  app.stage.addChild(dog);

  console.log(map);
}


