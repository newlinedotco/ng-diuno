var express = require('express'),
  connect   = require('connect'),
  app = express();

//CORS middleware
var allowCrossDomain = function(req, res, next) {
  // res.setHeader("Access-Control-Allow-Headers", "X-Requested-With,content-type");
  res.setHeader('Access-Control-Allow-Methods', 'GET, POST, OPTIONS, PUT, PATCH, DELETE');
  res.setHeader('Access-Control-Allow-Origin', '*');
  res.setHeader('Access-Control-Allow-Credentials', true);
  next();
}

// Configuration
app.configure(function(){
  app.use(connect.urlencoded());
  app.use(connect.json());
  app.use(express.methodOverride());
  
  //Error Handling
  app.use(express.logger());
  app.use(express.errorHandler({
    dumpExceptions: true, 
    showStack: true
  }));
  
  app.use(allowCrossDomain);
  app.use(express.static(__dirname + '/dist'));
  app.use(app.router);
});

app.get('/', function(req, res){
  res.redirect("/index.html");
});

app.get('/pins', function(req, res) {
  res.send({"pins": 1})
})

//Heroku
var port = process.env.PORT || 3000;
app.listen(port, function() {
  console.log("Listening on " + port);
});