var express = require('express');
var ejs = require('ejs');
var fs = require('fs');
var path = require('path');
var app = express();

var PORT = 3000;

// ----
// --------
// -----------
// --------------- Searching for music
console.log(process.argv.slice(2));
var Artists = {};
var Albumns = {};
var AlbumnArtist = {};

function isMusic(file) {
  var MusicExt = /\.mp3/g;
  return file.match(MusicExt) != null;
}

function getFromAlbumn(Path, Artist, Albumn) {
  fs.readdir(path.join(Path, Artist, Albumn), function(err, files) {
    var Key = Artist + ":" + Albumn;
    if (!(Key in Albumns)) {
      Albumns[Key] = [];
    }
    files.forEach(function(file) {
      if (isMusic(file)) {
        Albumns[Key].push(file); 

        if (!(file in Artists[Artist])) {
          Artists[Artist].push(file);
        }
      }
    });
  });
}

function getFromArtist(Path, Artist) {
  fs.readdir(path.join(Path, Artist), function(err, files) {
    if (!(Artist in Artists)) {
      Artists[Artist] = [];
    }
    files.forEach(function(file) {
      var stats = fs.lstatSync(path.join(Path, Artist, file));
      if (stats.isDirectory()) {
        AlbumnArtist[file] = Artist;
        getFromAlbumn(Path, Artist, file); 
      } else {
        if (isMusic(file)) {
          Artists[Artist].push(file);
        }
      }
    });
  });
}

var source = process.argv[2];
fs.lstat(source, function(err, stats) {
  if (stats.isDirectory()) {
    fs.readdir(source, function(err, files) {
      files.forEach(function(file) {
        stats = fs.lstatSync(path.join(source, file)); 
        if (stats.isDirectory()) {
          getFromArtist(source, file); 
        }
      }); 
    }); 
  } 
});

// ----
// --------
// -----------
// --------------- Setting the application

var Queue = [];

app.engine("html", require('ejs').renderFile)

app.use(express.static(path.join(__dirname, '/public')));
app.set('views', path.join(__dirname, '/view'));

app.get('/', function(req, res) {
  res.redirect('/artists');
});

app.get('/artists', function(req, res) {
  var Status = {
    Control: "artists",
    Search: true,
    List: Artists, 
    Head: "Artistas"
  }
  res.render('index.html', Status);
});

app.get('/artists/:name', function(req, res) {
  var Artist = req.params['name'];
  console.log("/artists/" + Artist);
  var Status = {
    Control: "music",
    Search: false,
    List: Artists[Artist], 
    Head: Artist
  }
  res.render('index.html', Status);
});

app.get('/albumns', function(req, res) {
  var Status = {
    Control: "albumns",
    Search: true,
    List: Albumns, 
    Head: "Álbuns"
  }
  res.render('index.html', Status);
});

app.get('/albumns/:name', function(req, res) {
  var AlbumnPair = req.params['name'];
  console.log("/albumns/" + AlbumnPair);
  var Status = {
    Control: "music",
    Search: false,
    List: Albumns[AlbumnPair], 
    Head: AlbumnPair.split(":")[1]
  }
  res.render('index.html', Status);
});

app.get('/musics', function(req, res) {
  var All = []
  for (Key in Artists) {
    Artists[Key].forEach(function(file) {
      All.push(file + ":" + Key) 
    });
  }
  All.sort();
  var Status = {
    Control: "musicList",
    Search: true,
    List: All, 
    Head: "Músicas Disponíveis"
  }
  res.render('index.html', Status);
});

app.get('/queue', function(req, res) {
  var Status = {
    Control: "queue",
    Search: false,
    List: Queue,
    Head: "Fila de Reprodução"
  }
  res.render('index.html', Status);
});

app.get('/add/:name', function(req, res) {
  var MusicPair = req.params['name'];
  console.log(MusicPair);
  Queue.push(MusicPair);
  res.redirect("back");
});

app.get('/rem/:name', function(req, res) {
  var MusicPair = req.params['name'];
  console.log("Remove: " + MusicPair);
  var Idx = Queue.indexOf(MusicPair);
  if (Idx != -1) {
    Queue.splice(Idx, 1);
  }
  res.redirect("back");
});

app.get('/artists/query', function(req, res) {
  console.log("Request for: /artists/query");
  console.log(req.params);
  console.log(req.query);
  res.redirect('/')
});

app.listen(3000, function() {
  console.log("Listening on port: " + PORT);
});
