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
var Artists = [];
var ArtistAlbumn = {};
var AAMusic = {};

function isMusic(file) {
  var MusicExt = /\.mp3/g;
  return file.match(MusicExt) != null;
}

function getFromAlbumn(Path, Artist, Albumn) {
  fs.readdir(path.join(Path, Artist, Albumn), function(err, files) {
    var Key = Artist + ":" + Albumn;
    if (!(Key in AAMusic)) {
      AAMusic[Key] = [];
    }
    files.forEach(function(file) {
      if (isMusic(file)) {
        AAMusic[Key].push(file); 
      }
    });
  });
}

function getFromArtist(Path, Artist) {
  fs.readdir(path.join(Path, Artist), function(err, files) {
    if (!(Artist in Artists)) {
      Artists.push(Artist);
      ArtistAlbumn[Artist] = [];
    }
    files.forEach(function(file) {
      var stats = fs.lstatSync(path.join(Path, Artist, file));
      if (stats.isDirectory()) {
        ArtistAlbumn[Artist].push(file);
        getFromAlbumn(Path, Artist, file); 
      }
    });
  });
}

function getArtistMusics(Artist) {
  var Musics = [];
  ArtistAlbumn[Artist].forEach(function(Albumn) {
    var Key = Artist + ":" + Albumn;
    AAMusic[Key].forEach(function (Music) {
      Musics.push(Music + ":" + Albumn + ":" + Artist); 
    });
  });
  Musics.sort();
  return Musics;
}

function getAlbumnMusics(Artist, Albumn) {
  var Musics = [];
  var Key = Artist + ":" + Albumn;
  AAMusic[Key].forEach(function (Music) {
    Musics.push(Music + ":" + Albumn + ":" + Artist); 
  });
  Musics.sort();
  return Musics;
}

function getAlbumnList() {
  var Albumns = [];
  for (var Artist in ArtistAlbumn) {
    ArtistAlbumn[Artist].forEach(function (Albumn) {
      Albumns.push(Albumn + ":" + Artist); 
    });
  }
  Albumns.sort();
  return Albumns;
}

function getMusicList() {
  var Musics = [];
  for (var Tuple in AAMusic) {
    var VTuple = Tuple.split(":");
    AAMusic[Tuple].forEach(function (Music) {
      Musics.push(Music + ":" + VTuple[1] + ":" + VTuple[0]); 
    }); 
  }
  Musics.sort();
  return Musics;
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
app.use('archive', express.static(source));

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
    List: getArtistMusics(Artist), 
    Head: Artist
  }
  res.render('index.html', Status);
});

app.get('/albumns', function(req, res) {
  var Status = {
    Control: "albumns",
    Search: true,
    List: getAlbumnList(), 
    Head: "Álbuns"
  }
  res.render('index.html', Status);
});

app.get('/albumns/:name', function(req, res) {
  var AlbumnPair = req.params['name'].split(":");
  console.log("/albumns/" + AlbumnPair);
  var Status = {
    Control: "music",
    Search: false,
    List: getAlbumnMusics(AlbumnPair[1], AlbumnPair[0]), 
    Head: AlbumnPair[0]
  }
  res.render('index.html', Status);
});

app.get('/musics', function(req, res) {
  var Status = {
    Control: "musicList",
    Search: true,
    List: getMusicList(), 
    Head: "Músicas Disponíveis"
  }
  res.render('index.html', Status);
});

app.get('/queue', function(req, res) {
  var Status = {
    Control: "queue",
    Search: false,
    List: Queue.slice(1),
    Fullpath: Queue[0],
    Head: "Fila de Reprodução"
  }
  console.log(Queue[0]);
  res.render('index.html', Status);
});

app.get('/queue/next', function(req, res) {
  Queue.shift();
  res.redirect("/queue");
});

app.get('/add/:name', function(req, res) {
  var MusicTuple = req.params['name'];
  console.log(MusicTuple);
  Queue.push(MusicTuple);
  res.redirect("back");
});

app.get('/rem/:name', function(req, res) {
  var MusicTuple = req.params['name'];
  console.log("Remove: " + MusicTuple);
  var Idx = Queue.indexOf(MusicTuple);
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
