'use strict';

angular.module('myApp', [
  'fsArdunio',
  'ngRoute'
])
.constant('HOSTED_URL', function() {
  var scriptTag = document.getElementById('appscript');
  var matches = scriptTag.src.match(/^http[s]?\:\/\/([^\/?#]+)(?:[#]?|$)/i);
  return matches[0];
})
.config(['$httpProvider', function($httpProvider) {
  $httpProvider.defaults.useXDomain = true;
  delete $httpProvider.defaults.headers.common['X-Requested-With'];
}])
.config(function($sceDelegateProvider, HOSTED_URL) {
  $sceDelegateProvider.resourceUrlWhitelist(['self', HOSTED_URL() + '**']);
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
