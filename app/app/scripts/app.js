'use strict';

angular.module('myApp', [
  'fsArduino',
  'ngRoute'
])
.constant('HOSTED_URL', function() {
  try {
    var scriptTag = document.getElementById('appscript');
    var matches = scriptTag.src.match(/^http[s]?\:\/\/([^\/?#]+)(?:[#]?|$)/i);
    return matches[0];
  } catch(e) {
    return 'http://localhost:9000';
  }
})
.config(['$httpProvider', function($httpProvider) {
  $httpProvider.defaults.useXDomain = true;
  delete $httpProvider.defaults.headers.common['X-Requested-With'];
}])
.config(function($sceDelegateProvider, HOSTED_URL) {
  console.log($sceDelegateProvider.resourceUrlWhitelist(['self', HOSTED_URL() + '/views', HOSTED_URL() + '**']));
})
.config(function ($routeProvider) {
  $routeProvider
    .when('/', {
      templateUrl: 'views/main.html',
      controller: 'MainCtrl'
    })
    .otherwise({
      redirectTo: '/'
    });
});