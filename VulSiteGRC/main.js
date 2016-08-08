var MariaClient = require('mariasql');
var Select = [];

var db = new MariaClient({
  host: 'localhost',
  user: 'root',
  password: 'root'
});

var logDbProps = function(query) {
  db.query(query, null, { useArray: true }, function(err, rows) {
    if (err) throw err;
    console.log(rows);
  });
}


var createDatabase = function(DbName) {
  db.query('create database ' + DbName);
  db.query('use ' + DbName);
  logDbProps('show databases');

  db.query('create table posts (' +
        'id int not null auto_increment primary key,' +
        'text varchar(511)' +
        ')');
  logDbProps('show tables');
  logDbProps('describe posts');
}

var dropDatabase = function(DbName) {
  db.query('drop database if exists ' + DbName);
  logDbProps('show databases');
}

var insertPost = function(post) {
  var Query = 'insert into posts (text) values ("' + post + '")';
  console.log("Query: " + Query);
  db.query(Query);
  db.query("select * from posts", null, { useArray: true }, function(err, rows) {
    if (err) throw err;
    Select = rows;
  });
}

dropDatabase('grc');

// --
// -------
// ---------
var express = require('express');
var path = require('path');
var app = express();

console.log('Dirname> ' + __dirname);
app.engine('html', require('ejs').renderFile);
app.set('views', path.join(__dirname, '/view'));

app.get('/', function(req, res) {
  console.log("Posts: " + Select.length)
  res.render('index.html', { posts: Select });
});

app.get('/add', function(req, res) {
  console.log('Inserting post: ' + req.query['post']);
  insertPost(req.query['post']);
  res.redirect('/');
});

app.listen(3000, function() {
  createDatabase('grc');
  console.log('Listen on port ' + 3000);
});
